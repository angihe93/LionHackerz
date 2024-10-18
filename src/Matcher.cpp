/* Dallas Scott - ds4015 */
/* Matcher Prototype */

#include <iostream>
#include <list>
#include <string>
#include <curl/curl.h>
#include "Matcher.h"
#include "Listing.h"
#include <wn.h>
#include <sstream>

using namespace std;

/* Matcher class for pairing job seekers with employers */

Matcher::Matcher(Database &db)
{
    this->db = &db;
}

vector<vector<string>> Matcher::gatherRelevantDimensions(int uid)
{
    int resCount = 0;
    vector<vector<string>> lists = this->db->query("Has_Augment", "dim_id,weight_mod", "id", "eq",
                                                   std::to_string(uid), false, resCount);

    vector<string> dimNames;
    for (auto &l : lists[0])
    {
        int resCount2 = 0;
        vector<vector<string>> dimName = db->query("Dimension", "name", "dim_id", "eq", l, false, resCount2);
        dimNames.push_back(dimName[0][0]);
    }

    vector<int> weights;
    for (auto &l : lists[1])
        weights.push_back(stoi(l));

    vector<vector<string>> results;
    results.push_back(dimNames);
    results.push_back(lists[1]);

    dimensions = dimNames;
    augments = weights;

    return results;
}

vector<int> Matcher::filterJobs()
{
    int resCount = 0;

    vector<vector<string>> lists = this->db->query("Listing", "", "", "", "", false, resCount);

    this->all_listings = lists;

    int nonNullCount[resCount];

    for (int i = 0; i < resCount; i++)
        nonNullCount[i] = 0;

    int prefDimCount = 0;

    /* for each dimension the user prefers */
    for (string &d : dimensions)
    {
        if (d != "\"interest1\"" && d != "\"interest2\"" && d != "\"interest3\"" && d != "\"interest4\"" &&
            d != "\"interest5\"")
        {
            std::cout << d << std::endl;
            prefDimCount++;
            int listNo = matchDimensions(d) - 1;
            int l = 0;

            /* get that dimension for all listings */
            for (auto &v : lists[listNo])
            {
                /* for each listing, increment tally if the matching dimension is not null */
                if (v != "\"null\"")
                    nonNullCount[l]++;
                l++;
            }
        }
    }

    /* with these tallies for each listing, discard listings where less than 25% of the
        user's preferred dimensions are not present in the listing - all others go into candidates */
    int l = 0;
    for (auto &lid : lists[0])
    {
        if ((float)nonNullCount[l] / (float)prefDimCount > 0.25)
            candidates.push_back(stoi(lid));
        l++;
    }
    sort(candidates.begin(), candidates.end());
    return candidates;
}

void Matcher::match(int uid)
{

    wninit(); // Initialize WordNet

    int resCount = 0;

    matchedWords.resize(candidates.size());

    vector<vector<string>> userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                      to_string(uid), false, resCount);

    int cNum = 0;

    /* for each candidate, calculate the match score based on all dimensions */
    for (auto &c : candidates)
    {
        int lid = c;
        int cInd = 0;
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
            candidateScore += 150;

            for (auto &d : dimensions)
            {
                if (d == "location")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on field + augment if applicable */
        auto fieldU = userVals[2][0];
        auto fieldE = all_listings[5][cInd];
        if (wordMatchFound(fieldU, fieldE, cNum))
        {
            candidateScore += 200;

            for (auto &d : dimensions)
            {
                if (d == "field")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on skills + augment if applicable */
        vector<vector<string>> userSkills = db->query("Has_Skill", "name", "id", "eq", to_string(uid), false, resCount);

        vector<string> listingSkills;

        listingSkills.resize(7);

        for (int i = 6; i < 13; i++)
            if (all_listings[i][cInd] != "\"null\"")
                listingSkills.push_back(all_listings[i][cInd]);

        for (string &wU : userSkills[0])
        {
            for (string &wE : listingSkills)
            {
                if (wordMatchFound(wU, wE, cNum))
                {
                    candidateScore += 100;

                    for (auto &d : dimensions)
                    {
                        if (d == "skill1" || d == "skill2" || d == "skill3" || d == "skill4" ||
                            d == "skill5")
                            candidateScore += augments[uAug];
                        uAug++;
                    }
                    uAug = 0;
                    break;
                }
            }
        }

        /* score on interests + augment if applicable */
        vector<vector<string>> userInterests = db->query("Has_Interest", "name", "uid", "eq",
                                                         to_string(uid), false, resCount);

        vector<string> listingInterests;
        listingInterests.resize(7);

        for (int i = 6; i < 13; i++)
            if (all_listings[i][cInd] != "\"null\"")
                listingInterests.push_back(all_listings[i][cInd]);

        for (string &wU : userInterests[0])
        {
            for (string &wE : listingInterests)
            {
                if (wordMatchFound(wU, wE, cNum))
                {
                    candidateScore += 100;

                    for (auto &d : dimensions)
                    {
                        if (d == "interest1" || d == "interest2" || d == "interest3" ||
                            d == "interest4" || d == "interest5")
                            candidateScore += augments[uAug];
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

        if (payU >= payE)
        {
            candidateScore += 50;

            for (auto &d : dimensions)
            {
                if (d == "pay")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on gender + augment if applicable */
        auto gendU = userVals[4][0];
        auto gendE = all_listings[16][cInd];

        if (gendU == gendE)
        {
            candidateScore += 25;

            for (auto &d : dimensions)
            {
                if (d == "gender")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on diversity + augment if applicable */
        auto divU = userVals[5][0];
        auto divE = all_listings[17][cInd];
        if (divU == divE)
        {
            candidateScore += 25;

            for (auto &d : dimensions)
            {
                if (d == "diversity")
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
            candidateScore += 25;

            for (auto &d : dimensions)
            {
                if (d == "mbti")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on flexibility + augment if applicable */
        auto flexU = userVals[7][0];
        auto flexE = all_listings[14][cInd];

        if (flexU == flexE)
        {
            candidateScore += 50;

            for (auto &d : dimensions)
            {
                if (d == "flexibility")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on remote + augment if applicable */
        auto remU = userVals[8][0];
        auto remE = all_listings[18][cInd];
        if (remU == remE)
        {
            candidateScore += 50;

            for (auto &d : dimensions)
            {
                if (d == "remote")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }

        /* score on workspace + augment if applicable */
        auto modernU = userVals[9][0];
        auto modernE = all_listings[15][cInd];

        if (modernU == modernE)
        {
            candidateScore += 25;

            for (auto &d : dimensions)
            {
                if (d == "workspace")
                    candidateScore += augments[uAug];
                uAug++;
            }
            uAug = 0;
        }
        scores.push_back(candidateScore);
        cNum++;
    }
}

void Matcher::filterMatches()
{
    int count = 0;
    vector<int> erasures;
    for (int &c : candidates)
    {
        if (scores[count] <= 100)
            erasures.push_back(count);
        count++;
    }
    for (int &ind : erasures)
    {
        scores.erase(scores.begin() + ind);
        candidates.erase(candidates.begin() + ind);
        matchedWords[ind].clear();
        matchedWords.erase(matchedWords.begin() + ind);
    }
}

void Matcher::sortMatches()
{
    vector<int> indices;

    for (int i = 0; i < scores.size(); i++)
        indices.push_back(i);

    sort(indices.begin(), indices.end(), [&](int a, int b)
         { return scores[a] > scores[b]; });

    vector<int> newCandidates(scores.size());
    vector<int> newScores(scores.size());
    vector<vector<string>> newMW(matchedWords.size());

    for (int i = 0; i < indices.size(); ++i)
    {
        newCandidates[i] = candidates[indices[i]];
        newScores[i] = scores[indices[i]];
        newMW[i] = matchedWords[indices[i]];
    }

    candidates = newCandidates;
    scores = newScores;
    matchedWords = newMW;
}

string Matcher::displayMatches(int uid)
{
    gatherRelevantDimensions(uid);
    filterJobs();
    match(uid);
    filterMatches();
    sortMatches();

    ostringstream oss;
    oss << "There are a total of " << candidates.size() << " matches out of " << all_listings[0].size()
        << " total listings for User " << uid << std::endl
        << std::endl;

    int count = 0;
    Listing *l = new Listing(*db);

    for (int &c : candidates)
    {
        oss << "Listing " << c << ":  Match score " << scores[count] << "   ";

        int bars = scores[count] / 25;

        for (int i = 0; i < bars; i++)
            oss << "=";

        oss << std::endl
            << std::endl;

        oss << l->getListing(c) << std::endl;

        oss << "\t\tMatched Words: ";

        for (string &mw : matchedWords[count])
            oss << mw << ", ";

        oss << std::endl
            << std::endl
            << std::endl;
        count++;
    }

    return oss.str();
}

void Matcher::iterateList(vector<string> l)
{
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

vector<int> Matcher::getCandidates()
{
    return candidates;
}

void Matcher::iterateList(vector<int> l)
{
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

vector<string> Matcher::getMatchedWords(int lid)
{
    return matchedWords[lid];
}

bool Matcher::wordMatchFound(string fieldU, string fieldE, int c)
{
    /* tokenize strings */
    fieldU.erase(remove(fieldU.begin(), fieldU.end(), '\"'), fieldU.end());
    fieldE.erase(remove(fieldE.begin(), fieldE.end(), '\"'), fieldE.end());
    vector<string> fieldVecU;
    vector<string> fieldVecE;
    int delim = 0;
    int pos = 0;
    while ((delim = fieldU.find(' ', pos)) != std::string::npos)
    {
        fieldVecU.push_back(fieldU.substr(pos, delim - pos));
        pos = delim + 1;
    }
    if (pos < fieldU.size())
        fieldVecU.push_back(fieldU.substr(pos));
    delim = 0;
    pos = 0;
    while ((delim = fieldE.find(' ', pos)) != std::string::npos)
    {
        fieldVecE.push_back(fieldE.substr(pos, delim - pos));
        pos = delim + 1;
    }
    if (pos < fieldE.size())
        fieldVecE.push_back(fieldE.substr(pos));

    /* compare words */

    /* user synonms vs. listing */
    for (string &w : fieldVecU)
    {
        for (char &c : w)
            c = std::tolower(static_cast<unsigned char>(c));

        SynsetPtr synset = findtheinfo_ds(const_cast<char *>(w.c_str()), NOUN, SYNS, ALLSENSES);

        while (synset != NULL)
        {
            for (int i = 0; i < synset->wcount; ++i)
            {
                for (string &wE : fieldVecE)
                {
                    for (char &c : wE)
                        c = std::tolower(static_cast<unsigned char>(c));

                    if (wE == synset->words[i])
                    {
                        bool alreadyStored = false;
                        for (string &mw : matchedWords[c])
                            if (mw == wE)
                                alreadyStored = true;
                        if (!alreadyStored)
                            matchedWords[c].push_back(wE);
                        return true;
                    }
                }
            }
            synset = synset->nextss;
        }
    }

    /* listing synonyms vs. user */
    for (string &wE : fieldVecE)
    {
        for (char &c : wE)
            c = std::tolower(static_cast<unsigned char>(c));

        SynsetPtr synset2 = findtheinfo_ds(const_cast<char *>(wE.c_str()), NOUN, SYNS, ALLSENSES);

        while (synset2 != NULL)
        {
            for (int i = 0; i < synset2->wcount; ++i)
            {
                for (string &wU : fieldVecU)
                {
                    for (char &c : wU)
                        c = std::tolower(static_cast<unsigned char>(c));

                    if (wU == synset2->words[i])
                    {
                        bool alreadyStored = false;
                        for (string &mw : matchedWords[c])
                            if (mw == wU)
                                alreadyStored = true;
                        if (!alreadyStored)
                            matchedWords[c].push_back(wU);
                        return true;
                    }
                }
            }
            synset2 = synset2->nextss;
        }
    }
    return false;
}
