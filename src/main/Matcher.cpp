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

void Matcher::gatherRelevantDimensions(int uid)
{

    // std::list<std::string> *lists = this->db->query1("Has_Augment", "dim_id", "id", "eq", std::to_string(uid));
}

void Matcher::filterJobs()
{
    /* for each job listing:
            if relevancy missing from description,
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
