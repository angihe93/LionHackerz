/* Dallas Scott - ds4015 */
/* Matcher Prototype */

#include <iostream>
#include <list>
#include <string>
#include <curl/curl.h>
#include "Matcher.h"

using namespace std;

/* Matcher class for pairing job seekers with employers */

Matcher::Matcher(Database &db)
{
    this->db = &db;
}

vector<vector<string> > Matcher::gatherRelevantDimensions(int uid)
{
    int resCount = 0;
    vector<vector<string> > lists = this->db->query("Has_Augment", "dim_id,weight_mod", "id", "eq",
                                                    std::to_string(uid), false, resCount);


    vector<string> dimNames;
    for (auto &l : lists[0])
    {
        int resCount2 = 0;
        vector<vector<string> > dimName = db->query("Dimension", "name", "dim_id", "eq", l, false, resCount2);
        dimNames.push_back(dimName[0][0]);
    }

    vector<int> weights;
    for (auto &l : lists[1])
        weights.push_back(stoi(l));

    vector<vector<string> > results;
    results.push_back(dimNames);
    results.push_back(lists[1]);

    dimensions = dimNames;
    augments = weights;

    return results;
}

vector<int> Matcher::filterJobs()
{
    int resCount = 0;

    vector<vector<string> > lists = this->db->query("Listing", "", "", "", "", false, resCount);

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
        if ((float)nonNullCount[l] / (float)prefDimCount > 0.25)
            candidates.push_back(stoi(lid));
        l++;        
    }

    return candidates;
}

void Matcher::match(int uid)
{

    /* Completed:  Score calculation on non-skill/non-interest based values */
    /* TODO:       Score calculation on skills/interests
                   String comparison for location/field/skills/interests to determine match */

    int resCount = 0;

    vector<vector<string> > userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                       to_string(uid), false, resCount);

    for (auto &c : candidates) 
    {
        int candidateScore = 0;

        int listCount = 0;

        /* score on location match + augment if applicable */
        vector<vector<string> > listingVals = this->db->query("Listing", "location", "lid", "eq",
                                                              to_string(c), false, listCount);

        auto locU = userVals[1][0];
        auto locE = listingVals[0][0];

        int uAug = 0;

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
        listingVals = this->db->query("Listing", "field", "lid", "eq", to_string(c),
                                      false, listCount);
        auto fieldU = userVals[2].begin();
        auto fieldE = listingVals[0].begin();
        if (*fieldU == *fieldE)
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

        /* score on pay + augment if applicable */
        listingVals = this->db->query("Listing", "pay", "lid", "eq", to_string(c), false, listCount);
        auto payU = userVals[3].begin();
        auto payE = listingVals[0].begin();
        if (*payU >= *payE)
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
        listingVals = this->db->query("Listing", "mixed_gender", "lid", "eq", to_string(c),
                                      false, listCount);
        auto gendU = userVals[4].begin();
        auto gendE = listingVals[0].begin();
        if (*gendU == *gendE)
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
        listingVals = this->db->query("Listing", "diverse_workforce", "lid", "eq",
                                      to_string(c), false, listCount);
        auto divU = userVals[5].begin();
        auto divE = listingVals[0].begin();
        if (*divU == *divE)
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
        listingVals = this->db->query("Listing", "personality_types", "lid", "eq",
                                      to_string(c), false, listCount);
        auto mbtiU = userVals[6].begin();
        auto mbtiE = listingVals[0].begin();
        if (*mbtiU == *mbtiE)
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
        listingVals = this->db->query("Listing", "job_flexibility", "lid", "eq",
                                      to_string(c), false, listCount);
        auto flexU = userVals[7].begin();
        auto flexE = listingVals[0].begin();
        if (*flexU == *flexE)
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
        listingVals = this->db->query("Listing", "remote_available", "lid", "eq",
                                      to_string(c), false, listCount);
        auto remU = userVals[8].begin();
        auto remE = listingVals[0].begin();
        if (*remU == *remE)
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
        listingVals = this->db->query("Listing", "modern_building", "lid", "eq",
                                      to_string(c), false, listCount);
        auto modernU = userVals[9].begin();
        auto modernE = listingVals[0].begin();
        if (*modernU == *modernE)
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
        return 9;

    if (d == "\"skill2\"")
        return 10;

    if (d == "\"skill3\"")
        return 11;

    if (d == "\"skill4\"")
        return 12;

    if (d == "\"skill5\"")
        return 13;

    if (d == "\"gender\"")
        return 17;

    if (d == "\"diversity\"")
        return 18;

    if (d == "\"remote\"")
        return 19;

    if (d == "\"workspace\"")
        return 16;

    if (d == "\"pay\"")
        return 14;

    if (d == "\"location\"")
        return 21;

    if (d == "\"flexibility\"")
        return 15;

    if (d == "\"mbti\"")
        return 20;

    return -1;
}