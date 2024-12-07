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
    this->db = &db;
}

std::vector<std::vector<std::string>> Matcher::gatherRelevantDimensions(int uid)
{
    int resCount = 0;

    /* query database for user augments */
    std::vector<std::vector<std::string>> lists = this->db->query(
        "Has_Augment", "dim_id,weight_mod", "id", "eq",
        std::to_string(uid), false, resCount);

    // debug for MatcherUserDimensionIntegrationTests seg fault
    // std::cout << "in gatherRelevantDimensions lists.empty(): " << lists.empty() << std::endl;
    if (lists.empty())
    {
        return lists;
    }

    /* sort query by increasing dim_id */
    std::vector<int> indices;

    for (int i = 0; i < lists[0].size(); i++)
        indices.push_back(i);

    sort(indices.begin(), indices.end(), [&](int a, int b)
         { return stoi(lists[0][a]) < stoi(lists[0][b]); });

    std::vector<std::string> sortedDimID(lists[0].size());
    std::vector<std::string> sortedWeights(lists[0].size());

    for (int i = 0; i < indices.size(); ++i)
    {
        sortedDimID[i] = lists[0][indices[i]];
        sortedWeights[i] = lists[1][indices[i]];
    }

    lists[0] = sortedDimID;
    lists[1] = sortedWeights;

    /* get dim names from dim_ids */
    std::vector<std::string> dimNames;
    for (auto &l : lists[0])
    {
        int resCount2 = 0;
        std::vector<std::vector<std::string>> dimName = db->query("Dimension", "name", "dim_id", "eq", l, false, resCount2);
        dimNames.push_back(dimName[0][0]);
    }

    resCount = 0;
    /* store min pay requirement */
    std::vector<std::vector<std::string>> payPref = db->query("Has_Dimension", "pay", "id", "eq", std::to_string(uid), false, resCount);
    if (!payPref.empty())
    {
        payReq = stoi(payPref[0][0]);
    }

    /* get weights for each augment */
    std::vector<int> weights;
    for (auto &l : lists[1])
        weights.push_back(stoi(l));

    std::vector<std::vector<std::string>> results;
    results.push_back(dimNames);
    results.push_back(lists[1]);

    dimensions = dimNames;
    augments = weights;

    return results;
}

std::vector<int> Matcher::filterJobs(bool test)
{
    int resCount = 0;
    std::vector<std::vector<std::string>> lists;

    if (!test)
    {
        lists = this->db->query("Listing_AI", "", "", "", "", false, resCount);
    }
    else
    {
        lists = this->db->query("Listing", "", "", "", "", false, resCount);
        all_listings = lists;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!test)
        {
            /* check for listings in redis cache */
            auto redis_future = redis_client.get("listings_cache");
            redis_client.sync_commit();

            auto redis_reply = redis_future.get();

            if (redis_reply.is_string())
            {
                try
                {
                    // Deserialize cached JSON into std::vector<std::vector<std::string>>
                    auto json_listings = nlohmann::json::parse(redis_reply.as_string());
                    std::vector<std::vector<std::string>> raw_listings;

                    for (const auto &row : json_listings)
                    {
                        raw_listings.emplace_back(row.get<std::vector<std::string>>());
                    }

                    all_listings = raw_listings;

                    std::cout << "Successfully loaded listings from cache" << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error processing cached data: " << e.what() << std::endl;
                }
            }
            else
            {
                /* no cache, get listings from db */
                std::cout << "No valid cache found. Fetching from database..." << std::endl;
                lists = this->db->query("Listing_AI", "", "", "", "", false, resCount);

                /* store them in redis cache for future requests */
                try
                {
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

                    std::cout << "Cached listings successfully" << std::endl;
                    all_listings = lists;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error serializing or caching listings: " << e.what() << std::endl;
                }
            }
        }
    }

    std::vector<int> nonNullCount(resCount, 0);

    int prefDimCount = 0;

    /* for each dimension the user prefers */
    for (std::string &d : dimensions)
    {
        if (d != "\"interest1\"" && d != "\"interest2\"" &&
            d != "\"interest3\"" && d != "\"interest4\"" &&
            d != "\"interest5\"")
        {
            prefDimCount++;
            int listNo = matchDimensions(d);
            int l = 0;

            /* get that dimension for all listings */
            for (auto &v : lists[listNo])
            {
                /* for each listing, tally++ if matching dimension not null */
                if (v != "\"null\"")
                    nonNullCount[l]++;

                if (!test)
                {
                    /* exclude listings with pay not per user preference */
                    if (d == "\"pay\"")
                    {
                        if (payReq - payReq * 0.25 > stoi(v))
                            nonNullCount[l] = -10;
                    }
                }
                l++;
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        candidates.clear();

        /* with these tallies for each listing, discard listings where less
           than 25% of the user's preferred dimensions are not present in
           the listing - all others go into candidates */
        int l = 0;
        for (auto &lid : lists[0])
        {
            if (static_cast<float>(nonNullCount[l]) / static_cast<float>(prefDimCount) > 0.25)
                candidates.push_back(stoi(lid));
            l++;
        }
        sort(candidates.begin(), candidates.end());
        return candidates;
    }
}

std::vector<int> Matcher::match(int uid)
{
    if (!OpenDB)
    {
        printf("Initializing WordNet...\n");
        if (wninit() == 0)
        {
            printf("WordNet initialized successfully.\n");
        }
        else
        {
            printf("Failed to initialize WordNet.\n");
        }
    }
    else
    {
        printf("WordNet is already initialized.\n");
    }

    int resCount = 0;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        matchedWords.resize(candidates.size());
    }

    std::vector<std::vector<std::string>> userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                                     std::to_string(uid), false, resCount);

    int cNum = 0;

    int upd_prog_interval = ceil(candidates.size() / 45);
    int progress = 30;

    scores.clear();

    /* for each candidate, calculate the match score based on all dimensions */
    for (auto &c : candidates)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        int lid = c;
        int cInd = 0;
        std::cout << "Listing #" << c << std::endl;
        for (auto it = all_listings[0].begin(); it != all_listings[0].end(); it++)
        {
            if (stoi(*it) == lid)
                break;
            cInd++;
        }

        int candidateScore = 0;

        int listCount = 0;

        int uAug = 0;

        /* score on location match + augment if applicable */
        auto locU = userVals[1][0];
        auto locE = all_listings[20][cInd];

        if (locU == locE)
        {
            candidateScore += 75;

            for (auto &d : dimensions)
            {
                if (d == "\"location\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
            //     std::cout << "\tScore on location: " << candidateScore << std::endl;
        }

        /* score on field + augment if applicable */
        auto fieldU = userVals[2][0];
        auto fieldE = all_listings[5][cInd];
        int fieldMatches = wordMatchFound(fieldU, fieldE, cNum);
        if (fieldMatches || fieldU == fieldE)
        {
            candidateScore += (300 + 5 * fieldMatches);

            for (auto &d : dimensions)
            {
                if (d == "\"field\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
            //  std::cout << "\tScore on field: " << candidateScore << std::endl;
        }

        /* score on skills + augment if applicable */
        std::vector<std::vector<std::string>> userSkills = db->query("Has_Skill", "name", "id", "eq", std::to_string(uid), false, resCount);

        std::vector<std::string> listingSkills;

        std::vector<std::string> skillAugments;
        std::vector<std::string> skillAugmentsAlreadyApplied;

        for (int i = 6; i < 13; i++)
            if (all_listings[i][cInd] != "\"null\"")
                listingSkills.push_back(all_listings[i][cInd]);

        for (std::string &wU : userSkills[0])
        {
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
                if (wU == wE || skillMatches)
                {
                    if (wU == wE)
                    {
                        bool alreadyStored = false;
                        for (std::string &mw : matchedWords[cNum])
                        {
                            if (mw == wE)
                            {
                                alreadyStored = true;
                            }
                        }
                        if (!alreadyStored)
                            matchedWords[cNum].push_back(wE);
                    }

                    candidateScore += (150 + 5 * skillMatches);

                    for (auto &d : dimensions)
                    {
                        if (find(skillAugments.begin(), skillAugments.end(),
                                 d) != skillAugments.end())
                        {
                            candidateScore += augments[uAug];
                            auto it = find(skillAugments.begin(), skillAugments.end(), d);
                            skillAugmentsAlreadyApplied.push_back(*it);
                            skillAugments.clear();
                        }
                        uAug++;
                    }
                    uAug = 0;
                    break;
                }
            }
        }

        /* score on interests + augment if applicable */
        std::vector<std::vector<std::string>> userInterests =
            db->query("Has_Interest", "name", "uid", "eq", std::to_string(uid),
                      false, resCount);

        std::vector<std::string> listingInterests;

        for (int i = 6; i < 13; i++)
            if (all_listings[i][cInd] != "\"null\"")
                listingInterests.push_back(all_listings[i][cInd]);

        std::vector<std::string> interestAugments;
        std::vector<std::string> interestAugmentsAlreadyApplied;

        for (std::string &wU : userInterests[0])
        {
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
                if (wU == wE || interestMatches)
                {
                    if (wU == wE)
                    {
                        bool alreadyStored = false;
                        for (std::string &mw : matchedWords[cNum])
                            if (mw == wE)
                                alreadyStored = true;
                        if (!alreadyStored)
                            matchedWords[cNum].push_back(wE);
                    }

                    candidateScore += (150 + 5 * interestMatches);

                    for (auto &d : dimensions)
                    {
                        if (find(interestAugments.begin(),
                                 interestAugments.end(), d) !=
                            interestAugments.end())
                        {
                            candidateScore += augments[uAug];
                            auto it = find(interestAugments.begin(), interestAugments.end(), d);
                            interestAugmentsAlreadyApplied.push_back(*it);
                            interestAugments.clear();
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
            payUser = stoi(payU);
        if (payE != "\"null\"")
            payListing = stoi(payE);
        if (payListing >= payUser && payListing != 0)
        {
            candidateScore += 100;

            for (auto &d : dimensions)
            {
                if (d == "\"pay\"")
                {
                    candidateScore += augments[uAug];
                }
                uAug++;
            }
            uAug = 0;
        }

        /* score on gender + augment if applicable */
        auto gendU = userVals[4][0];
        auto gendE = all_listings[16][cInd];

        if (gendU == gendE && gendU != "\"null\"")
        {
            candidateScore += 15;

            for (auto &d : dimensions)
            {
                if (d == "\"gender\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on diversity + augment if applicable */
        auto divU = userVals[5][0];
        auto divE = all_listings[17][cInd];
        if (divU == divE && divU != "\"null\"")
        {
            candidateScore += 15;

            for (auto &d : dimensions)
            {
                if (d == "\"diversity\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on mbti + augment if applicable */
        auto mbtiU = userVals[6][0];
        auto mbtiE = all_listings[19][cInd];
        if (wordMatchFound(mbtiU, mbtiE, cNum))
        {
            candidateScore += 15;

            for (auto &d : dimensions)
            {
                if (d == "\"mbti\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on flexibility + augment if applicable */
        auto flexU = userVals[7][0];
        auto flexE = all_listings[14][cInd];

        if (flexU == flexE && flexU != "\"null\"")
        {
            candidateScore += 15;

            for (auto &d : dimensions)
            {
                if (d == "\"flexibility\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on remote + augment if applicable */
        auto remU = userVals[8][0];
        auto remE = all_listings[18][cInd];
        if (remU == remE && remU != "\"null\"")
        {
            candidateScore += 25;

            for (auto &d : dimensions)
            {
                if (d == "\"remote\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on workspace + augment if applicable */
        auto modernU = userVals[9][0];
        auto modernE = all_listings[15][cInd];

        if (modernU == modernE && modernU != "\"null\"")
        {
            candidateScore += 15;

            for (auto &d : dimensions)
            {
                if (d == "\"workspace\"")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }
        std::cout << "score: " << candidateScore << std::endl;
        scores.push_back(candidateScore);

        if (cNum % upd_prog_interval == 0)
        {
            progress++;
            if (progress < 75)
            {
                redis_client.set("progress:" + std::to_string(uid),
                                 "{\"status\": \"processing\", \"progress\": " +
                                     std::to_string(progress) + "}");
                redis_client.commit();
            }
        }
        cNum++;
    }

    return scores;
}

std::vector<std::vector<int>> Matcher::filterMatches()
{
    std::lock_guard<std::mutex> lock(mutex_);

    int count = 0;
    std::vector<int> filteredCandidates;
    std::vector<int> filteredScores;
    std::vector<int> ind;
    std::vector<std::vector<std::string>> filteredMatchedWords;
    for (int &c : candidates)
    {
        if (scores[count] > 190)
        {
            filteredCandidates.push_back(c);
            filteredScores.push_back(scores[count]);
        }
        else
        {
            ind.push_back(count);
        }
        count++;
    }

    int offset = 0;
    for (auto &i : ind)
        matchedWords.erase(matchedWords.begin() + i - offset++);

    candidates = filteredCandidates;
    scores = filteredScores;

    std::vector<std::vector<int>> result;

    result.push_back(candidates);
    result.push_back(scores);

    return result;
}

std::vector<std::vector<int>> Matcher::sortMatches()
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<int> indices;

    for (int i = 0; i < scores.size(); i++)
        indices.push_back(i);

    sort(indices.begin(), indices.end(), [&](int a, int b)
         { return scores[a] > scores[b]; });

    std::vector<int> newCandidates(scores.size());
    std::vector<int> newScores(scores.size());
    std::vector<std::vector<std::string>> newMW(matchedWords.size());

    for (int i = 0; i < indices.size(); ++i)
    {
        newCandidates[i] = candidates[indices[i]];
        newScores[i] = scores[indices[i]];
        newMW[i] = matchedWords[indices[i]];
    }

    candidates = newCandidates;
    scores = newScores;
    matchedWords = newMW;

    std::vector<std::vector<int>> results;
    results.push_back(candidates);
    results.push_back(scores);

    return results;
}

std::vector<JobMatch> Matcher::displayMatches(int uid, bool test)
{
    gatherRelevantDimensions(uid);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 10}");
    redis_client.commit();

    if (test)
        filterJobs(true);
    else
        filterJobs(false);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 20}");
    redis_client.commit();

    match(uid);
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 75}");
    redis_client.commit();

    filterMatches();
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 80}");
    redis_client.commit();

    sortMatches();
    redis_client.set("progress:" + std::to_string(uid),
                     "{\"status\": \"processing\", \"progress\": 85}");
    redis_client.commit();

    struct JobMatch matchRes;

    int count = 0;
    Listing *l = new Listing(*db);
    std::vector<JobMatch> allMatches;

    int progress = 85;
    int store_interv = candidates.size() / 15;

    /* store matches in struct JobMatch */
    for (int i = 0; i < candidates.size(); i++)
    {
        if (i % store_interv == 0 && progress < 99)
        {
            progress++;
            redis_client.set("progress:" + std::to_string(uid),
                             "{\"status\": \"processing\", \"progress\": " +
                                 std::to_string(progress) + "}");
            redis_client.commit();
        }

        matchRes.listingId = candidates[i];
        matchRes.score = scores[count];

        std::vector<std::string> listingResult = l->getListing(candidates[i], test);

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
        // matchRes.pay = std::stoi(listingResult[10]);
        matchRes.flex = listingResult[11];
        matchRes.modern = listingResult[12];
        matchRes.gender = listingResult[13];
        matchRes.diversity = listingResult[14];
        matchRes.remote = listingResult[15];
        matchRes.personality = listingResult[16];
        matchRes.location = listingResult[17];

        allMatches.push_back(matchRes);

        std::string matchW;

        if (!matchedWords[0].empty())
        {
            for (std::string &mw : matchedWords[count])
                matchW = matchW + ", ";
        }

        matchRes.matchedWords = matchW;

        count++;
    }

    return allMatches;
}

std::vector<int> Matcher::getCandidates()
{
    return candidates;
}

int Matcher::matchDimensions(std::string d)
{
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

    return -1;
}

std::vector<std::string> Matcher::getMatchedWords(int lid)
{
    return matchedWords[lid];
}

int Matcher::wordMatchFound(std::string fieldU, std::string fieldE, int c)
{
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
    /* user synonms vs. listing */
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
                    }
                    ++matchCount;
                }
            }

            // Move to the next synset
            synset = synset->nextss;
        }
    }
    return matchCount;
}

std::vector<std::string> Matcher::tokenize(const std::string &input)
{
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token)
    {
        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
        token.erase(std::remove(token.begin(), token.end(), ','), token.end());
        tokens.push_back(token);
    }
    return tokens;
}