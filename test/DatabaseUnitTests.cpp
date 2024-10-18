#include <gtest/gtest.h>
#include "Database.h"
#include <curl/curl.h>

using namespace std;

TEST(dbRequest, CheckRequest) {
        std::string url = "";
        std::string api_key = "";
        Database *db = new Database(url,api_key);
        int resCount;
        std::vector<std::vector<std::string>> queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        vector<vector<string>> expected;
        EXPECT_EQ(queryRes, expected); // expected queryRes to be empty bc not connected to DB

        db = new Database();
        queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        expected = {{ "\"location\"" }};
        EXPECT_EQ(queryRes, expected); 


        delete db;

}