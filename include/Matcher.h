/* Dallas Scott - ds4015 */
/* Matcher Prototype */
#ifndef MATCHER_H
#define MATCHER_H

#include <iostream>
#include <list>
#include <string>
#include <curl/curl.h>
#include "Database.h"

using namespace std;

/* Matcher class for pairing job seekers with employers */

class Matcher
{
public:
    Matcher(Database &db);

    /* If user has preferences for certain match criteria,
     * select those to apply augments to those dimensions.
     * stores the dimensions in list<int> dimensions by dim_id
     * and the corresponding augment values in list<int> augments.
     *
     *     @param uid: the given user id */
    std::list<std::string> *gatherRelevantDimensions(int uid);

    /* After populating dimensions with gatherRelevantDimensions(),
     * select all job listings and filter them by those which have
     * at least 75% of the user's preferred dimensions filled in.
     *
     * This eliminates listings where too many values were not
     * entered by the employer and hence what is most important
     * to the job seeker is not present in the listing.
     *
     *  Returns the list of filtered listings 'candidates' */
    std::list<int> filterJobs();

    /* After filtering jobs with filterJobs() to get candidates, the
     * match() function will calculate scores for each listing based on
     * the dimensions in the listing that match the dimensions in the
     * user's profile + additional augments for positive matches where
     * the user has specified.  These are stored in list<int> scores.
     *
     *      @param uid: the given user id to match on  */
    void match(int uid);

    void filterMatches(/*list of candidates*/);

    void sortMatches(/*revised list of candidates*/);

    void displayResults(/*sorted list of candidates*/);

private:
    Database *db;
    list<string> dimensions;
    list<int> augments;
    list<int> candidates;
    list<int> scores;
    /* helper functions */
    void iterateList(std::list<std::string> l); /* prints list */
    void iterateList(std::list<int> l);         /* prints list */
    int matchDimensions(std::string d);         /* match dims b/w Dimension and Listing tables */
};
#endif