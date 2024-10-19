// Copyright 2024 LionHackerz

/* Dallas Scott - ds4015
 * Unit tests for Matcher class */
#include <gtest/gtest.h>
#include "Database.h"
#include "Matcher.h"
#include <curl/curl.h>
#include <vector>

/* This tests the gatherRelevantDimensions() function in
	Matcher.cpp*/
TEST(GetDim, augmentsAndWeights)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 1;

	std::vector<std::vector<std::string>> testResults(0);
	std::vector<std::string> aug_weights;
	aug_weights.push_back("100");
	aug_weights.push_back("100");
	aug_weights.push_back("50");
	aug_weights.push_back("50");

	std::vector<std::string> aug_on;
	aug_on.push_back("\"skill1\"");
	aug_on.push_back("\"pay\"");
	aug_on.push_back("\"remote\"");
	aug_on.push_back("\"workspace\"");

	testResults.push_back(aug_on);
	testResults.push_back(aug_weights);

	EXPECT_EQ(testResults, m->gatherRelevantDimensions(uid));
}

/* This tests the filterJobs() function in
	Matcher.cpp */
TEST(FilterListings, discardTooManyNull)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> candidates;
	candidates.push_back(1);
	candidates.push_back(2);
	candidates.push_back(3);
	candidates.push_back(4);
	candidates.push_back(5);
	candidates.push_back(6);

	std::vector<std::vector<std::string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	std::vector<int> filter = m->filterJobs();

	EXPECT_EQ(candidates, filter);
}

/* This tests the main scoring function match() in
	Matcher.cpp*/
TEST(FilterListings, calculateScores)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> scores;
	scores.push_back(0);
	scores.push_back(0);
	scores.push_back(0);
	scores.push_back(800);
	scores.push_back(800);
	scores.push_back(900);

	std::vector<std::vector<std::string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	std::vector<int> filter = m->filterJobs();
	std::vector<int> sc = m->match(uid);

	EXPECT_EQ(scores, sc);
}

/* This tests the filterMatches() function, whether or
   not matches not meeting a certain threshold are
   excluded from candidates */
TEST(FilterListings, elimLowScores)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> candidates;
	candidates.push_back(4);
	candidates.push_back(5);
	candidates.push_back(6);
	std::vector<int> scores;
	scores.push_back(800);
	scores.push_back(800);
	scores.push_back(900);

	std::vector<std::vector<int>> testResults;
	testResults.push_back(candidates);
	testResults.push_back(scores);

	m->gatherRelevantDimensions(uid);
	m->filterJobs();
	m->match(uid);
	std::vector<std::vector<int>> filteredMatches = m->filterMatches();

	EXPECT_EQ(testResults, filteredMatches);
}

/* This tests the sortMatches() function, in Matcher.cpp */
TEST(Sort, sortMatches)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> candidates;
	candidates.push_back(6);
	candidates.push_back(4);
	candidates.push_back(5);
	std::vector<int> scores;
	scores.push_back(900);
	scores.push_back(800);
	scores.push_back(800);

	std::vector<std::vector<int>> testResults;
	testResults.push_back(candidates);
	testResults.push_back(scores);

	m->gatherRelevantDimensions(uid);
	m->filterJobs();
	m->match(uid);
	m->filterMatches();
	std::vector<std::vector<int>> sortedMatches = m->sortMatches();

	EXPECT_EQ(testResults, sortedMatches);
}

/* This tests the main display function in Matcher.cpp, which
 * calls on Listing.cpp to print individual listings. */
TEST(Display, displayMatches)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::string display = m->displayMatches(uid);

	std::string matchList = R"(There are a total of 3 matches out of 6 total listings for User 5

Listing 6:  Match score 900   ====================================

	Posted by: "City of San Francisco"

	Created on: "2024-10-18T12:04:51.757878+00:00"

	Field:  "Public Works"

	Position: "Sculptor"

	Job Description: "The city is looking for a skilled artisan familiar with sculpting and manipulating stone to work on a public project.  "

	Skills required: "carving", "sculpting", "stonework", "statue making", 

	Pay: 275000

	Remote Option Available: "false"

	Personality Types: "Artist"

	Location: "San Francisco"


		Matched Words: "sculpting", "stonework", 


Listing 4:  Match score 800   ================================

	Posted by: "Art School"

	Created on: "2024-10-13T16:23:25.000676+00:00"

	Field:  "arts"

	Position: "professor of art"

	Job Description: "We are looking for a skilled artist who also has the capacity for teaching."

	Skills required: "drawing", "painting", "sculpting", "teaching", 

	Pay: 75000

	Flexibility: "true"

	Modern Workspace: "true"

	Personality Types: "INFP"

	Location: "New York"


		Matched Words: art, "sculpting", "drawing", 


Listing 5:  Match score 800   ================================

	Posted by: "Art School"

	Created on: "2024-10-13T16:30:19.209194+00:00"

	Field:  "Art History"

	Position: "Exhibit Curator"

	Job Description: "The MoMA is looking for an experienced art historian to curator a new exhibit on a specific modern artist.  Experience with previous curation a plus."

	Skills required: "drawing", "painting", "sculpting", "teaching", 

	Pay: 75000

	Flexibility: "true"

	Modern Workspace: "true"

	Personality Types: "INFP"

	Location: "New York"


		Matched Words: art, "sculpting", "drawing", 


)";

	std::cout << display << std::endl;

	EXPECT_EQ(matchList, display);
}

/* This tests the getCandidates() function, a basic
 * helper in Matcher.cpp */
TEST(GetValues, retrieveCandidates)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> testCandidates;	
	testCandidates.push_back(1);
	testCandidates.push_back(2);
	testCandidates.push_back(3);
	testCandidates.push_back(4);
	testCandidates.push_back(5);
	testCandidates.push_back(6);

	m->gatherRelevantDimensions(uid);
	m->filterJobs();

	EXPECT_EQ(testCandidates, m->getCandidates());
}

/* This tests the getMatchedWords() function, a basic
 * helper in Matcher.cpp */
TEST(GetValues, retrieveMatchedWords)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::string w1 = "\"sculpting\"";
	std::string w2 = "\"stonework\"";

	std::vector<std::string> testMatchedWords;
	testMatchedWords.push_back(w1);
	testMatchedWords.push_back(w2);

	m->gatherRelevantDimensions(uid);
	m->filterJobs();
	m->match(uid);
	m->filterMatches();
	m->sortMatches();
	std::vector<std::string> gmw = m->getMatchedWords(0);

	EXPECT_EQ(testMatchedWords, gmw);
}