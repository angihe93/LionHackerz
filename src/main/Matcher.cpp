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
    std::list<std::string> *lists = this->db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", std::to_string(uid), true, resCount);

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

    /* for each job listing:
            if too many relevancies missing from description,
            skip; else, add to list 'cadidates' */
}

void Matcher::match(/*job*/)
{
    /* for each listing in 'candidates':
            calculate match score with
            defaults and augments, add to list 'scores' */
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