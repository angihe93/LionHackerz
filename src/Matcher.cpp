/* Dallas Scott - ds4015 */
/* Matcher Prototype */

#include <iostream>
#include <list>
#include <string>
#include <curl/curl.h>
#include "Matcher.h"
#include <wn.h>

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

    vector<vector<string>> lists = this->db->query("Listing", "", "", "", "", true, resCount);

    this->all_listings = lists;

    int nonNullCount[resCount];

    for (int i = 0; i < resCount; i++)
        nonNullCount[i] = 0;

    int prefDimCount = 0;

    /* for each dimension the user prefers */
    for (auto &d : dimensions)
    {
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

    /* with these tallies for each listing, discard listings where less than 25% of the
        user's preferred dimensions are not present in the listing - all others go into candidates */
    int l = 0;
    for (auto &lid : lists[0])
    {
        std::cout << (float)nonNullCount[l] / (float)prefDimCount << std::endl;
        if ((float)nonNullCount[l] / (float)prefDimCount > 0.25)
            candidates.push_back(stoi(lid));
        l++;
    }

    return candidates;
}

void Matcher::match(int uid)
{
    iterateList(candidates);

    wninit(); // Initialize WordNet

    /* Completed:  Score calculation on non-skill/non-interest based values 
                   Score calculation on skills and field with WordNet string/synonym comparison 
       TODO:       Score calculation on interests
                   String comparison for interests/job description to determine match */

    int resCount = 0;

    vector<vector<string>> userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                      to_string(uid), false, resCount);

    for (auto &c : candidates)
    {
        int candidateScore = 0;

        int listCount = 0;

        int uAug = 0;

        /* score on location match + augment if applicable */
        auto locU = userVals[1][0];
        auto locE = all_listings[20][c - 1];

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
        auto fieldE = all_listings[5][c - 1];
        if (wordMatchFound(fieldU, fieldE))
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
        vector<vector<string>> userSkills = db->query("Has_Skill", "name", "id", "eq", to_string(uid), true, resCount);

        vector<string> listingSkills;

        listingSkills.resize(5);

        for (int i = 8; i < 13; i++)
            if (all_listings[i][c-1] != "\"null\"")
                listingSkills.push_back(all_listings[i][c - 1]);
        bool matchFound = false;
        for (string &wU : userSkills[0])
        {
            for (string &wE : listingSkills)
            {
                if (wordMatchFound(wU, wE))
                {
                    candidateScore += 100;

                    for (auto &d : dimensions)
                    {
                        if (d == "skill1" || d == "skill2" || d == "skill3" || d == "skill4" || d == "skill5")
                            candidateScore += augments[uAug];
                        uAug++;
                    }
                    uAug = 0;
                    matchFound = true;
                    break;
                }
            }
            if (matchFound)
                break;
        }
        matchFound = false;

        /* score on pay + augment if applicable */
        auto payU = userVals[3][0];
        auto payE = all_listings[13][c - 1];

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
        auto gendE = all_listings[16][c - 1];

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
        auto divE = all_listings[17][c - 1];
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
        auto mbtiE = all_listings[19][c - 1];
        if (mbtiU == mbtiE)
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
        auto flexE = all_listings[14][c - 1];

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
        auto remE = all_listings[18][c - 1];
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
        auto modernE = all_listings[15][c - 1];

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

        std::cout << "end candidate" << std::endl;
        std::cout << candidateScore << std::endl;
        scores.push_back(candidateScore);
    }
    iterateList(scores);
}

void Matcher::filterMatches(/*list of candidates*/)
{
    /* for each candidate:
         if score in corresponding index
         in 'scores' above match threshold,
         keep; else, remove from list.
    */
}

void Matcher::sortMatches(/*revised list of candidates*/)
{
    /* for each candidate remaining:
            sort by match score in ascending order */
}

void Matcher::displayResults(/*sorted list of candidates*/)
{
    /* for each sorted candidate:
            print job listing*/
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

bool Matcher::wordMatchFound(string fieldU, string fieldE)
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
    for (string &w : fieldVecU)
    {
        for (char &c : w)
            c = std::tolower(static_cast<unsigned char>(c));

        std::cout << w << std::endl;

        SynsetPtr synset = findtheinfo_ds(const_cast<char *>(w.c_str()), NOUN, SYNS, ALLSENSES);

        while (synset != NULL)
        {
            for (int i = 0; i < synset->wcount; ++i)
            {
                for (string &wE : fieldVecE)
                {
                    for (char &c : wE)
                        c = std::tolower(static_cast<unsigned char>(c));

                    std::cout << wE << " - " << synset->words[i] << std::endl;
                    if (wE == synset->words[i])
                        return true;
                }
            }
            synset = synset->nextss;
        }
    }
}
