// Copyright 2024 LionHackerz

/* Dallas Scott - ds4015 */
/* Matcher Prototype */
#ifndef MATCHER_H
#define MATCHER_H

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <variant>
#include <curl/curl.h>
#include <wn.h>
#include "Global.h"
#include <string>
#include <regex>
#include <cpp_redis/cpp_redis>
#include "Database.h"
#include <nlohmann/json.hpp>

/* struct for holding a listing match */
struct JobMatch {
    int listingId;
    int pay;
    int score;
    std::string company;
    std::string time_created;
    std::string field;
    std::string position;
    std::string description;
    std::string skill1;
    std::string skill2;
    std::string skill3;
    std::string skill4;
    std::string skill5;
    std::string flex;
    std::string modern;
    std::string gender;
    std::string diversity;
    std::string remote;
    std::string personality;
    std::string location;
    std::string matchedWords;

nlohmann::json to_json() const {
            return {
                {"company", company},
                {"time_created", time_created},
                {"field", field},
                {"position", position},
                {"description", description},
                {"skill1", skill1},
                {"skill2", skill2},
                {"skill3", skill3},
                {"skill4", skill4},
                {"skill5", skill5},
                {"pay", pay},
                {"flex", flex},
                {"modern", modern},
                {"gender", gender},
                {"diversity", diversity},
                {"remote", remote},
                {"personality", personality},
                {"location", location}
            };
    }

};

/* Matcher class for pairing job seekers with employers */

using JobListingMapVariantType = std::variant<int, std::string, bool, std::vector<std::string>>;

class Matcher
{
public:
    Matcher(Database &db);

    /* This is the main call function for the Matcher class.  When
     * accessing the route to display matches in the browser, this
     * function will call all of the below functions in order to get
     * a set of matches and returns the results as a string to be
     * passed to the browser.  
     * 
     * There is no need to call any of the other functions in this
     * class if you are just looking to return the listings.  At
     * present, this will give a list of listing IDs and their
     * corresponding match scores in descending order when called.
     * When the Listing class is created, more details can be
     * included. */
    std::vector<JobMatch> displayMatches(int uid);

    /* If user has preferences for certain match criteria,
     * select those to apply augments to those dimensions.
     * stores the dimensions in list<int> dimensions by dim_id
     * and the corresponding augment values in list<int> augments.
     *
     *     @param uid: the given user id */
    std::vector<std::vector<std::string> > gatherRelevantDimensions(int uid);

    /* After populating dimensions with gatherRelevantDimensions(),
     * select all job listings and filter them by those which have
     * at least 75% of the user's preferred dimensions filled in.
     *
     * This eliminates listings where too many values were not
     * entered by the employer and hence what is most important
     * to the job seeker is not present in the listing.
     *
     *  Returns the list of filtered listings 'candidates' */
    std::vector<int> filterJobs();

    /* After filtering jobs with filterJobs() to get candidates, the
     * match() function will calculate scores for each listing based on
     * the dimensions in the listing that match the dimensions in the
     * user's profile + additional augments for positive matches where
     * the user has specified.  These are stored in list<int> scores.
     *
     *      @param uid: the given user id to match on  */
    std::vector<int> match(int uid);

    /* After populating matchs scores for each candidate with match(),
     * this function filters once again, this time by match score.
     * Any candidate that scores below 100 is remoted from the
     * match list. 
    */
    std::vector<std::vector<int>> filterMatches();

    /* With filtering finished, this function sorts the candidates,
     * their scores, and their matched words in descending order
     * (candidate with the largest score first).  The relative indices
     * of all 3 vectors are preserved after the sort (i.e., score in
     * index 1 corresponds to candidate in index 1). */
    std::vector<std::vector<int>> sortMatches();

    /* Retrieve the list of candidates */
    std::vector<int> getCandidates();

    /* Retrieve the list of matched words for listing lid*/
    std::vector<std::string> getMatchedWords(int lid);

    /* Helper functions for generating API's returned JSON objects*/
    // TODO: deal with arrays of things later
    std::map<std::string, std::variant<std::string, std::vector<std::map<std::string, JobListingMapVariantType>>>> matchResponse(int uid);

private:
    Database *db;
    std::vector<std::vector<std::string>> all_listings;  /* all job listings in db */
    std::vector<std::string> dimensions;  /* list of dimensions */
    std::vector<int> augments; /* user augments for certain dimenions */
    std::vector<int> candidates;  /* job listing candidates by listing id */
    std::vector<int> scores; /* scores for candidates (same indices as candidates) */
    std::vector<std::vector<std::string>> matchedWords; /* words that were matched for 
                                                            successful matches*/
    /* helper functions */
    void iterateList(std::vector<std::string> l); /* prints list */
    void iterateList(std::vector<int> l);         /* prints list */
    int binSearch(SynsetPtr s, int left, int right, char val); /* binary search for index */
    bool wordMatchFound(std::string fieldU, std::string fieldE, int c);
                                                /* matches words using WordNet synonyms */
    int matchDimensions(std::string d);         /* match dims b/w Dimension and Listing tables */
};
#endif