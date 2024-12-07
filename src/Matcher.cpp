// Copyright 2024 LionHackerz
/* Dallas Scott - ds4015 */
/* Matcher Algorithm */

#include "Listing.h"
#include "Matcher.h"
#include <curl/curl.h>
#include <wn.h>
#include <crow.h>
#include <cpp_redis/cpp_redis>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#define BINSEARCH_THRESHOLD 5

/* Matcher class for pairing job seekers with employers */

Matcher::Matcher(Database &db)
{
    std::cout << "[Matcher::Matcher] Constructor called." << std::endl;
    this->db = &db;
    std::cout << "[Matcher::Matcher] Database reference set." << std::endl;
}

std::vector<std::vector<std::string>> Matcher::gatherRelevantDimensions(int uid)
{
    std::cout << "[Matcher::gatherRelevantDimensions] Function called with uid: " << uid << std::endl;
    int resCount = 0;

    /* query database for user augments */
    std::cout << "[Matcher::gatherRelevantDimensions] Querying database for user augments." << std::endl;
    std::vector<std::vector<std::string>> lists = this->db->query(
        "Has_Augment", "dim_id,weight_mod", "id", "eq",
        std::to_string(uid), false, resCount);

    std::cout << "[Matcher::gatherRelevantDimensions] Retrieved " << lists.size() << " lists from database." << std::endl;

    // debug for MatcherUserDimensionIntegrationTests seg fault
    // std::cout << "in gatherRelevantDimensions lists.empty(): " << lists.empty() << std::endl;
    if (lists.empty())
    {
        std::cout << "[Matcher::gatherRelevantDimensions] No relevant dimensions found for uid: " << uid << std::endl;
        return lists;
    }

    /* sort query by increasing dim_id */
    std::cout << "[Matcher::gatherRelevantDimensions] Sorting dimensions by dim_id." << std::endl;
    std::vector<int> indices;

    for (int i = 0; i < lists[0].size(); i++)
    {
        indices.push_back(i);
    }

    std::cout << "[Matcher::gatherRelevantDimensions] Sorting " << indices.size() << " indices." << std::endl;
    sort(indices.begin(), indices.end(), [&](int a, int b)
         { 
             int dimA = stoi(lists[0][a]);
             int dimB = stoi(lists[0][b]);
             std::cout << "[Matcher::gatherRelevantDimensions] Comparing dim_id: " << dimA << " < " << dimB << " = " << (dimA < dimB) << std::endl;
             return dimA < dimB; });

    std::cout << "[Matcher::gatherRelevantDimensions] Sorting completed." << std::endl;
    std::vector<std::string> sortedDimID(lists[0].size());
    std::vector<std::string> sortedWeights(lists[0].size());

    for (int i = 0; i < indices.size(); ++i)
    {
        sortedDimID[i] = lists[0][indices[i]];
        sortedWeights[i] = lists[1][indices[i]];
        std::cout << "[Matcher::gatherRelevantDimensions] Sorted dim_id[" << i << "]: " << sortedDimID[i]
                  << ", weight_mod[" << i << "]: " << sortedWeights[i] << std::endl;
    }

    lists[0] = sortedDimID;
    lists[1] = sortedWeights;

    /* get dim names from dim_ids */
    std::cout << "[Matcher::gatherRelevantDimensions] Retrieving dimension names." << std::endl;
    std::vector<std::string> dimNames;
    for (auto &l : lists[0])
    {
        int resCount2 = 0;
        std::cout << "[Matcher::gatherRelevantDimensions] Querying Dimension name for dim_id: " << l << std::endl;
        std::vector<std::vector<std::string>> dimName = db->query("Dimension", "name", "dim_id", "eq", l, false, resCount2);
        if (!dimName.empty() && !dimName[0].empty())
        {
            dimNames.push_back(dimName[0][0]);
            std::cout << "[Matcher::gatherRelevantDimensions] Retrieved dimName: " << dimName[0][0] << std::endl;
        }
        else
        {
            dimNames.push_back("Unknown");
            std::cout << "[Matcher::gatherRelevantDimensions] Dimension name not found for dim_id: " << l << std::endl;
        }
    }

    resCount = 0;
    /* store min pay requirement */
    std::cout << "[Matcher::gatherRelevantDimensions] Querying pay preference for uid: " << uid << std::endl;
    std::vector<std::vector<std::string>> payPref = db->query("Has_Dimension", "pay", "id", "eq", std::to_string(uid), false, resCount);
    if (!payPref.empty() && !payPref[0].empty())
    {
        payReq = stoi(payPref[0][0]);
        std::cout << "[Matcher::gatherRelevantDimensions] Pay requirement set to: " << payReq << std::endl;
    }
    else
    {
        payReq = 0;
        std::cout << "[Matcher::gatherRelevantDimensions] No pay requirement found. Defaulting to: " << payReq << std::endl;
    }

    /* get weights for each augment */
    std::cout << "[Matcher::gatherRelevantDimensions] Retrieving weights for augments." << std::endl;
    std::vector<int> weights;
    for (auto &l : lists[1])
        weights.push_back(stoi(l));

    std::cout << "[Matcher::gatherRelevantDimensions] Retrieved " << weights.size() << " weights." << std::endl;

    std::vector<std::vector<std::string>> results;
    results.push_back(dimNames);
    results.push_back(lists[1]);

    dimensions = dimNames;
    augments = weights;

    std::cout << "[Matcher::gatherRelevantDimensions] Function completed successfully." << std::endl;
    return results;
}

std::vector<int> Matcher::filterJobs(bool test)
{
    std::cout << "[Matcher::filterJobs] Function called with test: " << test << std::endl;
    int resCount = 0;
    std::vector<std::vector<std::string>> lists;

    if (!test)
    {
        std::cout << "[Matcher::filterJobs] Fetching listings from 'Listing_AI'." << std::endl;
        lists = this->db->query("Listing_AI", "", "", "", "", false, resCount);
    }
    else
    {
        std::cout << "[Matcher::filterJobs] Fetching listings from 'Listing'." << std::endl;
        lists = this->db->query("Listing", "", "", "", "", false, resCount);
        all_listings = lists;
    }

    {
        std::cout << "[Matcher::filterJobs] Acquiring mutex lock." << std::endl;
        std::lock_guard<std::mutex> lock(mutex_);

        if (!test)
        {
            std::cout << "[Matcher::filterJobs] Checking for listings in Redis cache." << std::endl;
            auto redis_future = redis_client.get("listings_cache");
            redis_client.sync_commit();

            auto redis_reply = redis_future.get();

            if (redis_reply.is_string())
            {
                try
                {
                    std::cout << "[Matcher::filterJobs] Loading listings from Redis cache." << std::endl;
                    // Deserialize cached JSON into std::vector<std::vector<std::string>>
                    auto json_listings = nlohmann::json::parse(redis_reply.as_string());
                    std::vector<std::vector<std::string>> raw_listings;

                    for (const auto &row : json_listings)
                    {
                        raw_listings.emplace_back(row.get<std::vector<std::string>>());
                    }

                    all_listings = raw_listings;

                    std::cout << "[Matcher::filterJobs] Successfully loaded listings from cache." << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "[Matcher::filterJobs] Error processing cached data: " << e.what() << std::endl;
                }
            }
            else
            {
                std::cout << "[Matcher::filterJobs] No valid cache found. Fetching from database..." << std::endl;
                lists = this->db->query("Listing_AI", "", "", "", "", false, resCount);

                /* store them in redis cache for future requests */
                try
                {
                    std::cout << "[Matcher::filterJobs] Caching listings to Redis." << std::endl;
                    nlohmann::json json_cache = nlohmann::json::array();
                    for (const auto &row : lists)
                    {
                        json_cache.push_back(row);
                    }

                    redis_client.set("listings_cache", json_cache.dump(), [](cpp_redis::reply &reply)
                                     {
                        if (reply.is_error()) {
                            std::cerr << "Error caching listings: " << reply.error() << std::endl;
                        } });

                    redis_client.expire("listings_cache", 600); // store for 10 minutes
                    redis_client.sync_commit();

                    std::cout << "[Matcher::filterJobs] Cached listings successfully." << std::endl;
                    all_listings = lists;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "[Matcher::filterJobs] Error serializing or caching listings: " << e.what() << std::endl;
                }
            }
        }
    }

    std::vector<int> nonNullCount(resCount, 0);

    int prefDimCount = 0;

    /* for each dimension the user prefers */
    std::cout << "[Matcher::filterJobs] Iterating over preferred dimensions." << std::endl;
    for (std::string &d : dimensions)
    {
        if (d != "\"interest1\"" && d != "\"interest2\"" &&
            d != "\"interest3\"" && d != "\"interest4\"" &&
            d != "\"interest5\"")
        {
            prefDimCount++;
            std::cout << "[Matcher::filterJobs] Processing dimension: " << d << std::endl;
            int listNo = matchDimensions(d);
            std::cout << "[Matcher::filterJobs] Dimension " << d << " corresponds to listNo: " << listNo << std::endl;
            int l = 0;

            /* get that dimension for all listings */
            for (auto &v : lists[listNo])
            {
                /* for each listing, tally++ if matching dimension not null */
                if (v != "\"null\"")
                {
                    nonNullCount[l]++;
                    std::cout << "[Matcher::filterJobs] Listing " << l << " has non-null value for dimension " << d << std::endl;
                }

                if (!test)
                {
                    /* exclude listings with pay not per user preference */
                    if (d == "\"pay\"")
                    {
                        if (payReq - payReq * 0.25 > stoi(v))
                        {
                            nonNullCount[l] = -10;
                            std::cout << "[Matcher::filterJobs] Listing " << l << " excluded based on pay requirement." << std::endl;
                        }
                    }
                }
                l++;
            }
        }
    }

    {
        std::cout << "[Matcher::filterJobs] Acquiring mutex lock for candidates." << std::endl;
        std::lock_guard<std::mutex> lock(mutex_);
        candidates.clear();

        /* with these tallies for each listing, discard listings where less
           than 25% of the user's preferred dimensions are not present in
           the listing - all others go into candidates */
        int l = 0;
        std::cout << "[Matcher::filterJobs] Filtering candidates based on nonNullCount." << std::endl;
        for (auto &lid : lists[0])
        {
            if (static_cast<float>(nonNullCount[l]) / static_cast<float>(prefDimCount) > 0.25)
            {
                candidates.push_back(stoi(lid));
                std::cout << "[Matcher::filterJobs] Listing ID " << lid << " added to candidates." << std::endl;
            }
            else
            {
                std::cout << "[Matcher::filterJobs] Listing ID " << lid << " excluded from candidates." << std::endl;
            }
            l++;
        }
        sort(candidates.begin(), candidates.end());
        std::cout << "[Matcher::filterJobs] Candidates filtering completed. Total candidates: " << candidates.size() << std::endl;
        return candidates;
    }
}

std::vector<int> Matcher::match(int uid)
{
    std::cout << "[Matcher::match] Function called with uid: " << uid << std::endl;
    if (!OpenDB)
    {
        std::cout << "[Matcher::match] Initializing WordNet..." << std::endl;
        if (wninit() == 0)
        {
            std::cout << "[Matcher::match] WordNet initialized successfully." << std::endl;
        }
        else
        {
            std::cout << "[Matcher::match] Failed to initialize WordNet." << std::endl;
        }
    }
    else
    {
        std::cout << "[Matcher::match] WordNet is already initialized." << std::endl;
    }

    int resCount = 0;

    {
        std::cout << "[Matcher::match] Acquiring mutex lock for matchedWords." << std::endl;
        std::lock_guard<std::mutex> lock(mutex_);
        matchedWords.resize(candidates.size());
        std::cout << "[Matcher::match] matchedWords resized to " << matchedWords.size() << std::endl;
    }

    std::cout << "[Matcher::match] Querying user values." << std::endl;
    std::vector<std::vector<std::string>> userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                                     std::to_string(uid), false, resCount);
    std::cout << "[Matcher::match] UserVals size: " << userVals.size() << std::endl;
    int cNum = 0;

    int upd_prog_interval = ceil(candidates.size() / 45.0);
    int progress = 30;

    scores.clear();

    /* for each candidate, calculate the match score based on all dimensions */
    std::cout << "[Matcher::match] Starting to calculate match scores for candidates." << std::endl;
    for (auto &c : candidates)
    {
        std::cout << "[Matcher::match] Processing Candidate #" << c << std::endl;

        std::lock_guard<std::mutex> lock(mutex_);
        int lid = c;
        int cInd = 0;
        std::cout << "[Matcher::match] Listing #" << c << std::endl;
        for (auto it = all_listings[0].begin(); it != all_listings[0].end(); it++)
        {
            if (stoi(*it) == lid)
                break;
            cInd++;
        }

        std::cout << "[Matcher::match] Listing index found: " << cInd << std::endl;

        int candidateScore = 0;

        int listCount = 0;

        int uAug = 0;

        /* score on location match + augment if applicable */
        auto locU = userVals[1][0];
        auto locE = all_listings[20][cInd];

        std::cout << "[Matcher::match] Comparing location: User (" << locU << ") vs Listing (" << locE << ")" << std::endl;
        if (locU == locE)
        {
            candidateScore += 75;
            std::cout << "[Matcher::match] Location matches. Score += 75." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"location\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for location: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on field + augment if applicable */
        auto fieldU = userVals[2][0];
        auto fieldE = all_listings[5][cInd];
        int fieldMatches = wordMatchFound(fieldU, fieldE, cNum);
        std::cout << "[Matcher::match] Field comparison: User (" << fieldU << ") vs Listing (" << fieldE << "), Matches: " << fieldMatches << std::endl;
        if (fieldMatches || fieldU == fieldE)
        {
            candidateScore += (300 + 5 * fieldMatches);
            std::cout << "[Matcher::match] Field matches. Score += " << (300 + 5 * fieldMatches) << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"field\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for field: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on skills + augment if applicable */
        std::cout << "[Matcher::match] Querying user skills." << std::endl;
        std::vector<std::vector<std::string>> userSkills = db->query("Has_Skill", "name", "id", "eq", std::to_string(uid), false, resCount);
        std::cout << "[Matcher::match] UserSkills size: " << userSkills.size() << std::endl;
        std::vector<std::string> listingSkills;

        std::vector<std::string> skillAugments;
        std::vector<std::string> skillAugmentsAlreadyApplied;

        for (int i = 6; i < 13; i++)
        {
            if (all_listings[i][cInd] != "\"null\"")
            {
                listingSkills.push_back(all_listings[i][cInd]);
                std::cout << "[Matcher::match] Listing has skill at index " << i << ": " << all_listings[i][cInd] << std::endl;
            }
        }

        for (std::string &wU : userSkills[0])
        {
            // Assign skill augments
            if (find(skillAugmentsAlreadyApplied.begin(),
                     skillAugmentsAlreadyApplied.end(),
                     "\"skill1\"") == skillAugmentsAlreadyApplied.end())
                skillAugments.push_back("\"skill1\"");
            if (find(skillAugmentsAlreadyApplied.begin(),
                     skillAugmentsAlreadyApplied.end(),
                     "\"skill2\"") == skillAugmentsAlreadyApplied.end())
                skillAugments.push_back("\"skill2\"");
            if (find(skillAugmentsAlreadyApplied.begin(),
                     skillAugmentsAlreadyApplied.end(),
                     "\"skill3\"") == skillAugmentsAlreadyApplied.end())
                skillAugments.push_back("\"skill3\"");
            if (find(skillAugmentsAlreadyApplied.begin(),
                     skillAugmentsAlreadyApplied.end(),
                     "\"skill4\"") == skillAugmentsAlreadyApplied.end())
                skillAugments.push_back("\"skill4\"");
            if (find(skillAugmentsAlreadyApplied.begin(),
                     skillAugmentsAlreadyApplied.end(),
                     "\"skill5\"") == skillAugmentsAlreadyApplied.end())
                skillAugments.push_back("\"skill5\"");

            for (std::string &wE : listingSkills)
            {
                int skillMatches = wordMatchFound(wU, wE, cNum);
                std::cout << "[Matcher::match] Comparing skill: User (" << wU << ") vs Listing (" << wE << "), Matches: " << skillMatches << std::endl;
                if (wU == wE || skillMatches)
                {
                    if (wU == wE)
                    {
                        bool alreadyStored = std::find(matchedWords[cNum].begin(), matchedWords[cNum].end(), wE) != matchedWords[cNum].end();
                        if (!alreadyStored)
                            matchedWords[cNum].push_back(wE);
                        std::cout << "[Matcher::match] Exact skill match found: " << wE << std::endl;
                    }

                    candidateScore += (150 + 5 * skillMatches);
                    std::cout << "[Matcher::match] Skill matches. Score += " << (150 + 5 * skillMatches) << std::endl;

                    for (auto &d : dimensions)
                    {
                        if (find(skillAugments.begin(), skillAugments.end(),
                                 d) != skillAugments.end())
                        {
                            candidateScore += augments[uAug];
                            auto it = find(skillAugments.begin(), skillAugments.end(), d);
                            skillAugmentsAlreadyApplied.push_back(*it);
                            std::cout << "[Matcher::match] Augmenting score for " << d << ": +" << augments[uAug] << std::endl;
                            skillAugments.erase(it);
                        }
                        uAug++;
                    }
                    uAug = 0;
                    break;
                }
            }
        }

        /* score on interests + augment if applicable */
        std::cout << "[Matcher::match] Querying user interests." << std::endl;
        std::vector<std::vector<std::string>> userInterests =
            db->query("Has_Interest", "name", "uid", "eq", std::to_string(uid),
                      false, resCount);

        std::vector<std::string> listingInterests;

        for (int i = 6; i < 13; i++)
        {
            if (all_listings[i][cInd] != "\"null\"")
            {
                listingInterests.push_back(all_listings[i][cInd]);
                std::cout << "[Matcher::match] Listing has interest at index " << i << ": " << all_listings[i][cInd] << std::endl;
            }
        }

        std::vector<std::string> interestAugments;
        std::vector<std::string> interestAugmentsAlreadyApplied;

        for (std::string &wU : userInterests[0])
        {
            // Assign interest augments
            if (find(interestAugmentsAlreadyApplied.begin(),
                     interestAugmentsAlreadyApplied.end(), "\"interest1\"") ==
                interestAugmentsAlreadyApplied.end())
                interestAugments.push_back("\"interest1\"");
            if (find(interestAugmentsAlreadyApplied.begin(),
                     interestAugmentsAlreadyApplied.end(), "\"interest2\"") ==
                interestAugmentsAlreadyApplied.end())
                interestAugments.push_back("\"interest2\"");
            if (find(interestAugmentsAlreadyApplied.begin(),
                     interestAugmentsAlreadyApplied.end(), "\"interest3\"") ==
                interestAugmentsAlreadyApplied.end())
                interestAugments.push_back("\"interest3\"");
            if (find(interestAugmentsAlreadyApplied.begin(),
                     interestAugmentsAlreadyApplied.end(), "\"interest4\"") ==
                interestAugmentsAlreadyApplied.end())
                interestAugments.push_back("\"interest4\"");
            if (find(interestAugmentsAlreadyApplied.begin(),
                     interestAugmentsAlreadyApplied.end(), "\"interest5\"") ==
                interestAugmentsAlreadyApplied.end())
                interestAugments.push_back("\"interest5\"");

            for (std::string &wE : listingInterests)
            {
                int interestMatches = wordMatchFound(wU, wE, cNum);
                std::cout << "[Matcher::match] Comparing interest: User (" << wU << ") vs Listing (" << wE << "), Matches: " << interestMatches << std::endl;
                if (wU == wE || interestMatches)
                {
                    if (wU == wE)
                    {
                        bool alreadyStored = std::find(matchedWords[cNum].begin(), matchedWords[cNum].end(), wE) != matchedWords[cNum].end();
                        if (!alreadyStored)
                            matchedWords[cNum].push_back(wE);
                        std::cout << "[Matcher::match] Exact interest match found: " << wE << std::endl;
                    }

                    candidateScore += (150 + 5 * interestMatches);
                    std::cout << "[Matcher::match] Interest matches. Score += " << (150 + 5 * interestMatches) << std::endl;

                    for (auto &d : dimensions)
                    {
                        if (find(interestAugments.begin(),
                                 interestAugments.end(), d) !=
                            interestAugments.end())
                        {
                            candidateScore += augments[uAug];
                            auto it = find(interestAugments.begin(), interestAugments.end(), d);
                            interestAugmentsAlreadyApplied.push_back(*it);
                            std::cout << "[Matcher::match] Augmenting score for " << d << ": +" << augments[uAug] << std::endl;
                            interestAugments.erase(it);
                        }
                        uAug++;
                    }
                    uAug = 0;
                    break;
                }
            }
        }

        /* score on pay + augment if applicable */
        auto payU = userVals[3][0];
        auto payE = all_listings[13][cInd];
        int payUser = 0;
        int payListing = 0;

        if (payU != "\"null\"")
        {
            payUser = stoi(payU);
            std::cout << "[Matcher::match] User pay preference: " << payUser << std::endl;
        }
        if (payE != "\"null\"")
        {
            payListing = stoi(payE);
            std::cout << "[Matcher::match] Listing pay: " << payListing << std::endl;
        }
        if (payListing >= payUser && payListing != 0)
        {
            candidateScore += 100;
            std::cout << "[Matcher::match] Pay matches user requirement. Score += 100." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"pay\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for pay: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on gender + augment if applicable */
        auto gendU = userVals[4][0];
        auto gendE = all_listings[16][cInd];

        std::cout << "[Matcher::match] Comparing gender: User (" << gendU << ") vs Listing (" << gendE << ")" << std::endl;
        if (gendU == gendE && gendU != "\"null\"")
        {
            candidateScore += 15;
            std::cout << "[Matcher::match] Gender matches. Score += 15." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"gender\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for gender: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on diversity + augment if applicable */
        auto divU = userVals[5][0];
        auto divE = all_listings[17][cInd];
        std::cout << "[Matcher::match] Comparing diversity: User (" << divU << ") vs Listing (" << divE << ")" << std::endl;
        if (divU == divE && divU != "\"null\"")
        {
            candidateScore += 15;
            std::cout << "[Matcher::match] Diversity matches. Score += 15." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"diversity\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for diversity: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on mbti + augment if applicable */
        auto mbtiU = userVals[6][0];
        auto mbtiE = all_listings[19][cInd];
        std::cout << "[Matcher::match] Comparing MBTI: User (" << mbtiU << ") vs Listing (" << mbtiE << ")" << std::endl;
        if (wordMatchFound(mbtiU, mbtiE, cNum))
        {
            candidateScore += 15;
            std::cout << "[Matcher::match] MBTI matches. Score += 15." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"mbti\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for MBTI: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on flexibility + augment if applicable */
        auto flexU = userVals[7][0];
        auto flexE = all_listings[14][cInd];
        std::cout << "[Matcher::match] Comparing flexibility: User (" << flexU << ") vs Listing (" << flexE << ")" << std::endl;
        if (flexU == flexE && flexU != "\"null\"")
        {
            candidateScore += 15;
            std::cout << "[Matcher::match] Flexibility matches. Score += 15." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"flexibility\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for flexibility: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on remote + augment if applicable */
        auto remU = userVals[8][0];
        auto remE = all_listings[18][cInd];
        std::cout << "[Matcher::match] Comparing remote: User (" << remU << ") vs Listing (" << remE << ")" << std::endl;
        if (remU == remE && remU != "\"null\"")
        {
            candidateScore += 25;
            std::cout << "[Matcher::match] Remote matches. Score += 25." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"remote\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for remote: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on workspace + augment if applicable */
        auto modernU = userVals[9][0];
        auto modernE = all_listings[15][cInd];
        std::cout << "[Matcher::match] Comparing workspace: User (" << modernU << ") vs Listing (" << modernE << ")" << std::endl;
        if (modernU == modernE && modernU != "\"null\"")
        {
            candidateScore += 15;
            std::cout << "[Matcher::match] Workspace matches. Score += 15." << std::endl;

            for (auto &d : dimensions)
            {
                if (d == "\"workspace\"")
                {
                    candidateScore += augments[uAug];
                    std::cout << "[Matcher::match] Augmenting score for workspace: +" << augments[uAug] << std::endl;
                }
                uAug++;
            }
            uAug = 0;
        }
        std::cout << "[Matcher::match] Final score for candidate #" << c << ": " << candidateScore << std::endl;
        scores.push_back(candidateScore);

        if (cNum % upd_prog_interval == 0)
        {
            progress++;
            if (progress < 75)
            {
                std::cout << "[Matcher::match] Updating progress to " << progress << "%" << std::endl;
                redis_client.set("progress:" + std::to_string(uid),
                                 "{\"status\": \"processing\", \"progress\": " +
                                     std::to_string(progress) + "}");
                redis_client.commit();
            }
        }
        cNum++;
    }

    std::cout << "[Matcher::match] Function completed. Total scores calculated: " << scores.size() << std::endl;
    return scores;
}

std::vector<std::vector<int>> Matcher::filterMatches()
{
    std::cout << "[Matcher::filterMatches] Function called." << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);

    int count = 0;
    std::vector<int> filteredCandidates;
    std::vector<int> filteredScores;
    std::vector<int> ind;
    std::vector<std::vector<std::string>> filteredMatchedWords;
    std::cout << "[Matcher::filterMatches] Filtering matches with scores > 190." << std::endl;
    for (int &c : candidates)
    {
        if (scores[count] > 190)
        {
            filteredCandidates.push_back(c);
            filteredScores.push_back(scores[count]);
            std::cout << "[Matcher::filterMatches] Candidate " << c << " with score " << scores[count] << " added." << std::endl;
        }
        else
        {
            ind.push_back(count);
            std::cout << "[Matcher::filterMatches] Candidate " << c << " with score " << scores[count] << " excluded." << std::endl;
        }
        count++;
    }

    int offset = 0;
    for (auto &i : ind)
    {
        matchedWords.erase(matchedWords.begin() + i - offset++);
        std::cout << "[Matcher::filterMatches] Erased matchedWords at index " << i - offset + 1 << std::endl;
    }

    candidates = filteredCandidates;
    scores = filteredScores;

    std::vector<std::vector<int>> result;

    result.push_back(candidates);
    result.push_back(scores);

    std::cout << "[Matcher::filterMatches] Function completed. Filtered candidates: " << candidates.size() << std::endl;
    return result;
}

std::vector<std::vector<int>> Matcher::sortMatches()
{
    std::cout << "[Matcher::sortMatches] Function called." << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<int> indices;

    for (int i = 0; i < scores.size(); i++)
        indices.push_back(i);

    std::cout << "[Matcher::sortMatches] Sorting matches based on scores." << std::endl;
    sort(indices.begin(), indices.end(), [&](int a, int b)
         { 
             std::cout << "[Matcher::sortMatches] Comparing scores: " << scores[a] << " > " << scores[b] << " = " << (scores[a] > scores[b]) << std::endl;
             return scores[a] > scores[b]; });

    std::vector<int> newCandidates(scores.size());
    std::vector<int> newScores(scores.size());
    std::vector<std::vector<std::string>> newMW(matchedWords.size());

    for (int i = 0; i < indices.size(); ++i)
    {
        newCandidates[i] = candidates[indices[i]];
        newScores[i] = scores[indices[i]];
        newMW[i] = matchedWords[indices[i]];
        std::cout << "[Matcher::sortMatches] Sorted Candidate " << i << ": ID " << newCandidates[i]
                  << " with Score " << newScores[i] << std::endl;
    }

    candidates = newCandidates;
    scores = newScores;
    matchedWords = newMW;

    std::vector<std::vector<int>> results;
    results.push_back(candidates);
    results.push_back(scores);

    std::cout << "[Matcher::sortMatches] Function completed. Candidates sorted." << std::endl;
    return results;
}

std::vector<JobMatch> Matcher::displayMatches(int uid, bool test)
{
    std::cout << "[Matcher::displayMatches] Function called with uid: " << uid << ", test: " << test << std::endl;
    gatherRelevantDimensions(uid);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 10}");
    redis_client.commit();
    std::cout << "[Matcher::displayMatches] Progress updated to 10%." << std::endl;

    if (test)
        filterJobs(true);
    else
        filterJobs(false);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 20}");
    redis_client.commit();
    std::cout << "[Matcher::displayMatches] Progress updated to 20%." << std::endl;

    match(uid);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 75}");
    redis_client.commit();
    std::cout << "[Matcher::displayMatches] Progress updated to 75%." << std::endl;

    filterMatches();
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 80}");
    redis_client.commit();
    std::cout << "[Matcher::displayMatches] Progress updated to 80%." << std::endl;

    sortMatches();
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 85}");
    redis_client.commit();
    std::cout << "[Matcher::displayMatches] Progress updated to 85%." << std::endl;

    struct JobMatch matchRes;

    int count = 0;
    Listing *l = new Listing(*db);
    std::vector<JobMatch> allMatches;

    int progress = 85;
    int store_interv = candidates.size() / 15;
    std::cout << "[Matcher::displayMatches] Starting to store matches in JobMatch structs." << std::endl;

    /* store matches in struct JobMatch */
    for (int i = 0; i < candidates.size(); i++)
    {
        if (i % store_interv == 0 && progress < 99)
        {
            progress++;
            std::cout << "[Matcher::displayMatches] Updating progress to " << progress << "%" << std::endl;
            redis_client.set("progress:" + std::to_string(uid),
                             "{\"status\": \"processing\", \"progress\": " +
                                 std::to_string(progress) + "}");
            redis_client.commit();
        }

        matchRes.listingId = candidates[i];
        matchRes.score = scores[count];
        std::cout << "[Matcher::displayMatches] Processing Listing ID: " << matchRes.listingId << ", Score: " << matchRes.score << std::endl;

        std::vector<std::string> listingResult = l->getListing(candidates[i], false);
        std::cout << "[Matcher::displayMatches] Retrieved listing details for ID: " << candidates[i] << std::endl;

        matchRes.company = listingResult[0];
        matchRes.time_created = listingResult[1];
        matchRes.field = listingResult[2];
        matchRes.position = listingResult[3];
        matchRes.description = listingResult[4];
        matchRes.skill1 = listingResult[5];
        matchRes.skill2 = listingResult[6];
        matchRes.skill3 = listingResult[7];
        matchRes.skill4 = listingResult[8];
        matchRes.skill5 = listingResult[9];
        matchRes.pay = stoi(listingResult[10]);
        matchRes.flex = listingResult[11];
        matchRes.modern = listingResult[12];
        matchRes.gender = listingResult[13];
        matchRes.diversity = listingResult[14];
        matchRes.remote = listingResult[15];
        matchRes.personality = listingResult[16];
        matchRes.location = listingResult[17];

        allMatches.push_back(matchRes);
        std::cout << "[Matcher::displayMatches] Added JobMatch for Listing ID: " << matchRes.listingId << std::endl;

        std::string matchW;

        if (!matchedWords[0].empty())
        {
            for (std::string &mw : matchedWords[count])
                matchW = matchW + ", " + mw;
        }

        matchRes.matchedWords = matchW;
        std::cout << "[Matcher::displayMatches] Matched Words: " << matchRes.matchedWords << std::endl;

        count++;
    }

    std::cout << "[Matcher::displayMatches] Function completed. Total matches: " << allMatches.size() << std::endl;
    return allMatches;
}

void Matcher::iterateList(std::vector<std::string> l)
{
    std::cout << "[Matcher::iterateList] Iterating over string list." << std::endl;
    int count = 0;
    int size = l.size();
    std::cout << "(";
    for (auto &v : l)
    {
        std::cout << v;
        if (count != size - 1)
            std::cout << ", ";
        count++;
    }
    std::cout << ")" << std::endl;
}

std::vector<int> Matcher::getCandidates()
{
    std::cout << "[Matcher::getCandidates] Returning candidates." << std::endl;
    return candidates;
}

void Matcher::iterateList(std::vector<int> l)
{
    std::cout << "[Matcher::iterateList] Iterating over integer list." << std::endl;
    int count = 0;
    int size = l.size();
    std::cout << "(";
    for (auto &v : l)
    {
        std::cout << v;
        if (count != size - 1)
            std::cout << ", ";
        count++;
    }
    std::cout << ")" << std::endl;
}

int Matcher::matchDimensions(std::string d)
{
    std::cout << "[Matcher::matchDimensions] Matching dimension: " << d << std::endl;
    if (d == "\"field\"")
        return 5;

    if (d == "\"skill1\"")
        return 8;

    if (d == "\"skill2\"")
        return 9;

    if (d == "\"skill3\"")
        return 10;

    if (d == "\"skill4\"")
        return 11;

    if (d == "\"skill5\"")
        return 12;

    if (d == "\"gender\"")
        return 16;

    if (d == "\"diversity\"")
        return 17;

    if (d == "\"remote\"")
        return 18;

    if (d == "\"workspace\"")
        return 15;

    if (d == "\"pay\"")
        return 13;

    if (d == "\"location\"")
        return 20;

    if (d == "\"flexibility\"")
        return 14;

    if (d == "\"mbti\"")
        return 19;

    std::cout << "[Matcher::matchDimensions] No match found for dimension: " << d << std::endl;
    return -1;
}

std::vector<std::string> Matcher::getMatchedWords(int lid)
{
    std::cout << "[Matcher::getMatchedWords] Getting matched words for listing ID: " << lid << std::endl;
    return matchedWords[lid];
}

int Matcher::wordMatchFound(std::string fieldU, std::string fieldE, int c)
{
    std::cout << "[Matcher::wordMatchFound] Comparing User Field: " << fieldU << " with Listing Field: " << fieldE << std::endl;
    std::vector<std::string> fieldVecU;
    std::vector<std::string> fieldVecE;
    /* tokenize strings */
    fieldVecU = tokenize(fieldU);
    fieldVecE = tokenize(fieldE);

    std::unordered_set<std::string> fieldSetE(fieldVecE.begin(), fieldVecE.end());
    std::unordered_set<std::string> normalizedFieldSetE;
    for (const std::string &word : fieldSetE)
    {
        std::string normalizedWord = word;
        std::transform(normalizedWord.begin(), normalizedWord.end(), normalizedWord.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        normalizedFieldSetE.insert(normalizedWord);
    }
    /* compare words */
    int matchCount = 0;
    /* user synonyms vs. listing */
    for (std::string &w : fieldVecU)
    {
        std::transform(w.begin(), w.end(), w.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        if (fieldSetE.find(w) != fieldSetE.end())
        {
            bool alreadyStored = std::find(matchedWords[c].begin(), matchedWords[c].end(), w) != matchedWords[c].end();
            if (!alreadyStored)
            {
                matchedWords[c].push_back(w);
                std::cout << "[Matcher::wordMatchFound] Exact word match found: " << w << std::endl;
            }
            ++matchCount;
        }

        // Check for matches in synsets
        char *wb = morphword(const_cast<char *>(w.c_str()), NOUN);
        SynsetPtr synset = (wb != NULL) ? findtheinfo_ds(wb, NOUN, SYNS, ALLSENSES)
                                        : findtheinfo_ds(const_cast<char *>(w.c_str()), NOUN, SYNS, ALLSENSES);

        while (synset != NULL)
        {
            // Create an unordered_set for synset words
            std::unordered_set<std::string> synsetWords;
            for (int i = 0; i < synset->wcount; ++i)
            {
                std::string synsetWord(synset->words[i]);
                std::transform(synsetWord.begin(), synsetWord.end(), synsetWord.begin(),
                               [](unsigned char c)
                               { return std::tolower(c); });
                synsetWords.insert(synsetWord);
            }

            // Check for matches in fieldSetE
            for (const std::string &wE : fieldSetE)
            {
                if (synsetWords.find(wE) != synsetWords.end())
                {
                    bool alreadyStored = std::find(matchedWords[c].begin(), matchedWords[c].end(), wE) != matchedWords[c].end();
                    if (!alreadyStored)
                    {
                        matchedWords[c].push_back(wE);
                        std::cout << "[Matcher::wordMatchFound] Synset word match found: " << wE << std::endl;
                    }
                    ++matchCount;
                }
            }

            // Move to the next synset
            synset = synset->nextss;
        }
    }
    std::cout << "[Matcher::wordMatchFound] Total matches found: " << matchCount << std::endl;
    return matchCount;
}

std::vector<std::string> Matcher::tokenize(const std::string &input)
{
    std::cout << "[Matcher::tokenize] Tokenizing input: " << input << std::endl;
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token)
    {
        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
        token.erase(std::remove(token.begin(), token.end(), ','), token.end());
        tokens.push_back(token);
        std::cout << "[Matcher::tokenize] Token found: " << token << std::endl;
    }
    return tokens;
}
