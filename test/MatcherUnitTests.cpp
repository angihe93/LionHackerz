#include <gtest/gtest.h>
#include "Database.h"
#include "Matcher.h"
#include <curl/curl.h>
#include <vector>

using namespace std;

TEST(RelDim, augmentsAndWeights)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 1;

	vector<vector<string>> testResults(0);
	vector<string> aug_weights;
	aug_weights.push_back("50");
	aug_weights.push_back("50");
	aug_weights.push_back("100");
	aug_weights.push_back("100");

	vector<string> aug_on;
	aug_on.push_back("\"remote\"");
	aug_on.push_back("\"workspace\"");
	aug_on.push_back("\"pay\"");
	aug_on.push_back("\"skill1\"");

	testResults.push_back(aug_on);
	testResults.push_back(aug_weights);

	EXPECT_EQ(testResults, m->gatherRelevantDimensions(uid));
}

TEST(FilterListings, discardTooManyNull)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	vector<int> candidates;
	candidates.push_back(1);
	candidates.push_back(2);
	candidates.push_back(3);
	candidates.push_back(4);
	candidates.push_back(5);
	candidates.push_back(6);

	vector<vector<string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	vector<int> filter = m->filterJobs();

	EXPECT_EQ(candidates, filter);
}

TEST(FilterListings, calculateScores)
{
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 5;

	vector<int> scores;
	scores.push_back(0);
	scores.push_back(0);
	scores.push_back(0);
	scores.push_back(800);
	scores.push_back(800);
	scores.push_back(900);

	vector<vector<string>> dimensions;
	dimensions.push_back(m->gatherRelevantDimensions(uid)[0]);
	vector<int> filter = m->filterJobs();
	vector<int> sc = m->match(uid);

	EXPECT_EQ(scores, sc);
}