#include <gtest/gtest.h>
#include "Database.h"
#include <curl/curl.h>

using namespace std;

TEST(dbQuery, CheckQuery) {

        // should result in curl error bc not connected to DB
        string url = "";
        string api_key = "";
        Database *db = new Database(url,api_key);
        int resCount;
        vector<vector<string>> queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        vector<vector<string>> expected;
        EXPECT_EQ(queryRes, expected); // expected queryRes to be empty bc not connected to DB

        // should connect to DB and check query result
        db = new Database();
        queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        expected = {{ "\"location\"" }};
        EXPECT_EQ(queryRes, expected); 

        // query with 2 filters
        queryRes = db->query("Listing", "lid", "skill1_req", "eq", "drawing", "field", "eq", "arts", true, resCount);
        expected = {{"4"}};
        EXPECT_EQ(queryRes, expected); 

        // query with 3 filters
        queryRes = db->query("Listing", "lid", "skill1_req", "eq", "drawing", "skill2_req", "eq", "painting", "skill3_req", "eq", "sculpting", true, resCount);
        expected = {{"4","5"}};
        EXPECT_EQ(queryRes, expected); 

        delete db;

}

TEST(dbInsert, CheckInsert) {
        Database *db = new Database();
        string insertRes = db->insert("Employer", "{\"eid\": \"5\", \"company_name\": \"Generic Consulting\", \"size\": \"medium\"}");
        cout << "insertRes: " << insertRes << endl;
        string expected_insert =  R"([{"eid":5,"company_name":"Generic Consulting","size":"medium"}])";
        string expected_already_exists = "{\"code\":\"23505\",\"details\":\"Key (eid)=(5) already exists.\",\"hint\":null,\"message\":\"duplicate key value violates unique constraint \\\"Employer_pkey\\\"\"}";
        EXPECT_TRUE(insertRes==expected_insert || insertRes==expected_already_exists);

        delete db;
}

TEST(dbUpdate, CheckUpdate) {
        Database *db = new Database();
        string insertRes = db->insert("Employer", "{\"eid\": \"6\", \"company_name\": \"Small Consulting\", \"size\": \"medium\"}");
        string data = "{\"size\": \"small\"}";
        string updateRes = db->update("Employer", data, "eid", "eq", "6");
        cout << "updateRes: " << updateRes << endl;
        string expected_update =  R"([{"eid":6,"company_name":"Small Consulting","size":"small"}])";
        EXPECT_EQ(updateRes, expected_update);
        delete db;
}