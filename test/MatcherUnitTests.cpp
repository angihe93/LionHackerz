#include <gtest/gtest.h>
#include "Database.h"
#include "Matcher.h"
#include <curl/curl.h>
#include <vector>

using namespace std;

TEST(RelDim, augmentsAndWeights) {
	Database *db = new Database();
	Matcher *m = new Matcher(*db);

	int uid = 1;

	vector<vector<string>> testResults(2);
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

	testResults[0] = aug_on;
	testResults[1] = aug_weights;

	EXPECT_EQ(testResults, m->gatherRelevantDimensions(uid));	
}