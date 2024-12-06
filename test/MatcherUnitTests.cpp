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

	delete db;
	delete m;
}

/* This tests the filterJobs() function in
	Matcher.cpp */
TEST(FilterListings, discardTooManyNull)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<int> candidates;
	for (int i = 1; i < 7; i++)
	{
		if (i != 3)
			candidates.push_back(i);
	}

	std::vector<std::vector<std::string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	std::vector<int> filter = m->filterJobs(true);

	EXPECT_EQ(candidates, filter);

	delete db;
	delete m;
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
	scores.push_back(200);
	scores.push_back(995);
	scores.push_back(995);
	scores.push_back(1045);

	std::vector<std::vector<std::string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	std::vector<int> filter = m->filterJobs(true);
	std::vector<int> sc = m->match(uid);

	EXPECT_EQ(scores, sc);

	delete db;
	delete m;
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
	candidates.push_back(2);
	candidates.push_back(4);
	candidates.push_back(5);
	candidates.push_back(6);
	std::vector<int> scores;
	scores.push_back(200);
	scores.push_back(995);
	scores.push_back(995);
	scores.push_back(1045);

	std::vector<std::vector<int>> testResults;
	testResults.push_back(candidates);
	testResults.push_back(scores);

	m->gatherRelevantDimensions(uid);
	m->filterJobs(true);
	m->match(uid);
	std::vector<std::vector<int>> filteredMatches = m->filterMatches();

	EXPECT_EQ(testResults, filteredMatches);

	delete db;
	delete m;
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
	candidates.push_back(2);
	std::vector<int> scores;
	scores.push_back(1045);
	scores.push_back(995);
	scores.push_back(995);
	scores.push_back(200);

	std::vector<std::vector<int>> testResults;
	testResults.push_back(candidates);
	testResults.push_back(scores);

	m->gatherRelevantDimensions(uid);
	m->filterJobs(true);
	m->match(uid);
	m->filterMatches();
	std::vector<std::vector<int>> sortedMatches = m->sortMatches();

	EXPECT_EQ(testResults, sortedMatches);

	delete db;
	delete m;
}

/* This tests the main display function in Matcher.cpp, which
 * calls on Listing.cpp to print individual listings. */
TEST(Display, displayMatches)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::vector<JobMatch> display = m->displayMatches(uid, true);

	display[0].print();

	std::vector<JobMatch> matchList;

	struct JobMatch match1;

	match1.listingId = 6;
	match1.company = "\"TechForge\"";
	match1.description = "\"Develop and maintain software applications\"";
	match1.field = "\"Information Technology\"";
	match1.position = "\"Software Developer\"";
	match1.location = "\"Austin\"";
	match1.pay = 80000;
	match1.time_created = "\"2024-11-02T03:33:40.506159+00:00\"";
	match1.skill1 = "\"JavaScript\"";
	match1.skill2 = "\"React\"";
	match1.skill3 = "\"Node.js\"";
	match1.skill4 = "\"Problem-solving\"";
	match1.skill5 = "\"Teamwork\"";
	match1.score = 1045;
	match1.remote = "\"true\"";
	match1.gender = "\"true\"";
	match1.diversity = "\"true\"";
	match1.personality = "\"INTP\"";
	match1.modern = "\"true\"";
	match1.flex = "\"true\"";

	match1.print();

	matchList.push_back(match1);

	EXPECT_EQ(match1, display[0]);

	delete db;
	delete m;
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
	testCandidates.push_back(4);
	testCandidates.push_back(5);
	testCandidates.push_back(6);

	m->gatherRelevantDimensions(uid);
	m->filterJobs(true);

	EXPECT_EQ(testCandidates, m->getCandidates());

	delete db;
	delete m;
}

/* This tests the getMatchedWords() function, a basic
 * helper in Matcher.cpp */
TEST(GetValues, retrieveMatchedWords)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	std::string w1 = "sculpting";
	std::string w2 = "stonework";
	std::vector<std::string>
		testMatchedWords;
	testMatchedWords.push_back(w1);
	testMatchedWords.push_back(w2);

	m->gatherRelevantDimensions(uid);
	m->filterJobs(true);
	m->match(uid);
	m->filterMatches();
	m->sortMatches();
	std::vector<std::string> gmw = m->getMatchedWords(0);

	EXPECT_EQ(testMatchedWords[0], gmw[0]);
	EXPECT_EQ(testMatchedWords[1], gmw[1]);

	delete db;
	delete m;
}

/* This tests the matchDimensions() function, a basic
 * helper in Matcher.cpp */
// its a private method called by filterJobs:
/* After populating dimensions with gatherRelevantDimensions(),
     * select all job listings and filter them by those which have
     * at least 75% of the user's preferred dimensions filled in.
     *
     * This eliminates listings where too many values were not
     * entered by the employer and hence what is most important
     * to the job seeker is not present in the listing.
     *
     *  Returns the list of filtered listings 'candidates' */
//     std::vector<int> filterJobs(bool test);
// TEST(FilterListings, checkAddidtionalmatchDimensions)
// {
// 	Database *db = new Database();
// 	Matcher *m = new Matcher(*db);

// 	std::string d1 = "\"field\"";
// 	std::string d2 = "\"skill4\"";
// 	std::string d3 = "\"skill5\"";
// 	std::string d4 = "\"gender\"";
// 	std::string d5 = "\"diversity\"";
// 	std::string d6 = "\"remote\"";
// 	std::string d7 = "\"workspace\"";
// 	std::string d8 = "\"location\"";
// 	std::string d9 = "\"flexibility\"";
// 	std::string d10 = "\"invalid dimension\"";

// 	EXPECT_EQ(5, m->matchDimensions(d1));
// 	EXPECT_EQ(11, m->matchDimensions(d2));
// 	EXPECT_EQ(12, m->matchDimensions(d3));
// 	EXPECT_EQ(16, m->matchDimensions(d4));
// 	EXPECT_EQ(17, m->matchDimensions(d5));
// 	EXPECT_EQ(18, m->matchDimensions(d6));
// 	EXPECT_EQ(15, m->matchDimensions(d7));
// 	EXPECT_EQ(20, m->matchDimensions(d8));
// 	EXPECT_EQ(14, m->matchDimensions(d9));
// 	EXPECT_EQ(-1, m->matchDimensions(d10));

// 	delete db;
// 	delete m;
// }
