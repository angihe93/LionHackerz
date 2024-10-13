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

std::list<std::string> *Matcher::gatherRelevantDimensions(int uid)
{
    int resCount = 0;
    std::list<std::string> *lists = this->db->query("Has_Augment", "dim_id,weight_mod", "id", "eq",
                                                    std::to_string(uid), true, resCount);

    std::list<std::string> dimNames;

    for (auto it = lists[0].begin(); it != lists[0].end(); ++it)
    {
        int resCount2 = 0;
        std::list<std::string> *dimName = db->query("Dimension", "name", "dim_id", "eq", *it, false, resCount2);
        for (auto it2 = dimName[0].begin(); it2 != dimName[0].end(); it2++)
            dimNames.push_back(*it2);
    }

    list<int> weights;
    for (auto it = lists[1].begin(); it != lists[1].end(); ++it)
        weights.push_back(stoi(*it));

    std::list<std::string> *results = new std::list<std::string>[2];
    results[0] = dimNames;
    results[1] = lists[1];

    dimensions = dimNames;
    augments = weights;

    return results;
}

std::list<int> Matcher::filterJobs()
{
    int resCount = 0;

    std::list<std::string> *lists = this->db->query("Listing", "", "", "", "", true, resCount);

    std::cout << resCount << std::endl;
    int nonNullCount[resCount];

    for (int i = 0; i < resCount; i++)
        nonNullCount[i] = 0;

    int prefDimCount = 0;

    /* for each dimension the user prefers */
    for (auto it = dimensions.begin(); it != dimensions.end(); it++)
    {
        prefDimCount++;
        int listNo = matchDimensions(*it) - 1;
        std::cout << *it << " listNo: " << listNo << std::endl;

        int l = 0;
        /* get that dimension for all listings */
        for (auto dit = lists[listNo].begin(); dit != lists[listNo].end(); dit++)
        {
            /* for each listing, increment tally if the matching dimension is not null */
            std::cout << "l = " << l << std::endl;
            if (*dit != "\"null\"")
                nonNullCount[l]++;
            l++;
        }
    }

    /* with these tallies for each listing, discard listings where less than 25% of the
        user's preferred dimensions are not present in the listing - all others go into candidates */
    int l = 0;
    for (auto lid_it = lists[0].begin(); lid_it != lists[0].end(); lid_it++)
    {
        if ((float)nonNullCount[l] / (float)prefDimCount > 0.25)
            candidates.push_back(stoi(*lid_it));
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

    std::list<std::string> *userVals = this->db->query("Has_Dimension", "", "id", "eq",
                                                       to_string(uid), false, resCount);

    for (auto c_it = candidates.begin(); c_it != candidates.end(); c_it++)
    {

        int candidateScore = 0;

        int listCount = 0;

        /* score on location match + augment if applicable */
        std::list<std::string> *listingVals = this->db->query("Listing", "location", "lid", "eq",
                                                              to_string(*c_it), false, listCount);
        auto locU = userVals[1].begin();
        auto locE = listingVals[0].begin();

        auto uDim = dimensions.begin();
        auto uAug = augments.begin();
        if (*locU == *locE)
        {
            candidateScore += 150;

            while (*uDim != "location" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "location")
                    candidateScore += *uAug;
            }
        }

        /* score on field + augment if applicable */
        listingVals = this->db->query("Listing", "field", "lid", "eq", to_string(*c_it),
                                      false, listCount);
        auto fieldU = userVals[2].begin();
        auto fieldE = listingVals[0].begin();
        if (*fieldU == *fieldE)
        {
            candidateScore += 200;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "field" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "field")
                    candidateScore += *uAug;
            }
        }

        /* score on pay + augment if applicable */
        listingVals = this->db->query("Listing", "pay", "lid", "eq", to_string(*c_it), false, listCount);
        auto payU = userVals[3].begin();
        auto payE = listingVals[0].begin();
        if (*payU >= *payE)
        {
            candidateScore += 50;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "pay" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "pay")
                    candidateScore += *uAug;
            }
        }

        /* score on gender + augment if applicable */
        listingVals = this->db->query("Listing", "mixed_gender", "lid", "eq", to_string(*c_it),
                                      false, listCount);
        auto gendU = userVals[4].begin();
        auto gendE = listingVals[0].begin();
        if (*gendU == *gendE)
        {
            candidateScore += 25;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "gender" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "gender")
                    candidateScore += *uAug;
            }
        }

        /* score on diversity + augment if applicable */
        listingVals = this->db->query("Listing", "diverse_workforce", "lid", "eq",
                                      to_string(*c_it), false, listCount);
        auto divU = userVals[5].begin();
        auto divE = listingVals[0].begin();
        if (*divU == *divE)
        {
            candidateScore += 25;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "diversity" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "diversity")
                    candidateScore += *uAug;
            }
        }

        /* score on mbti + augment if applicable */
        listingVals = this->db->query("Listing", "personality_types", "lid", "eq",
                                      to_string(*c_it), false, listCount);
        auto mbtiU = userVals[6].begin();
        auto mbtiE = listingVals[0].begin();
        if (*mbtiU == *mbtiE)
        {
            candidateScore += 25;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "mbti" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "mbti")
                    candidateScore += *uAug;
            }
        }

        /* score on flexibility + augment if applicable */
        listingVals = this->db->query("Listing", "job_flexibility", "lid", "eq",
                                      to_string(*c_it), false, listCount);
        auto flexU = userVals[7].begin();
        auto flexE = listingVals[0].begin();
        if (*flexU == *flexE)
        {
            candidateScore += 50;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "flexibility" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "flexibility")
                    candidateScore += *uAug;
            }
        }

        /* score on remote + augment if applicable */
        listingVals = this->db->query("Listing", "remote_available", "lid", "eq",
                                      to_string(*c_it), false, listCount);
        auto remU = userVals[8].begin();
        auto remE = listingVals[0].begin();
        if (*remU == *remE)
        {
            candidateScore += 50;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "remote" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "remote")
                    candidateScore += *uAug;
            }
        }

        /* score on workspace + augment if applicable */
        listingVals = this->db->query("Listing", "modern_building", "lid", "eq",
                                      to_string(*c_it), false, listCount);
        auto modernU = userVals[9].begin();
        auto modernE = listingVals[0].begin();
        if (*modernU == *modernE)
        {
            candidateScore += 25;

            uDim = dimensions.begin();
            uAug = augments.begin();
            while (*uDim != "workspace" && uDim != dimensions.end())
            {
                uDim++;
                uAug++;
                if (*uDim == "workspace")
                    candidateScore += *uAug;
            }
        }

        scores.push_back(candidateScore);
    }
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

void Matcher::iterateList(std::list<std::string> l)
{
    std::cout << "(";
    for (auto it = l.begin(); it != l.end(); it++)
    {
        std::cout << *it;
        if (it != std::prev(l.end()))
            std::cout << ", ";
    }
    std::cout << ")" << std::endl;
}

void Matcher::iterateList(std::list<int> l)
{
    std::cout << "(";
    for (auto it = l.begin(); it != l.end(); it++)
    {
        std::cout << *it;
        if (it != std::prev(l.end()))
            std::cout << ", ";
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