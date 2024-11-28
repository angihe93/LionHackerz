// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include <curl/curl.h>

/* tests Database::query() function in Database.cpp */
TEST(dbQuery, CheckQuery)
{

        // should result in curl error bc not connected to DB
        std::string url = "";
        std::string api_key = "";
        Database *db = new Database(url, api_key);
        int resCount;
        std::vector<std::vector<std::string>> queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        std::vector<std::vector<std::string>> expected;
        EXPECT_EQ(queryRes, expected); // expected queryRes to be empty bc not connected to DB

        // should connect to DB and check query result
        db = new Database();
        queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
        expected = {{"\"location\""}};
        EXPECT_EQ(queryRes, expected);

        // query with 2 filters
        queryRes = db->query("Listing", "lid", "skill1_req", "eq", "drawing", "field", "eq", "arts", true, resCount);
        expected = {{"4"}};
        EXPECT_EQ(queryRes, expected);

        // query with 3 filters
        queryRes = db->query("Listing", "lid", "skill1_req", "eq", "drawing", "skill2_req", "eq", "painting", "skill3_req", "eq", "sculpting", true, resCount);
        expected = {{"5", "4"}};
        EXPECT_EQ(queryRes, expected);

        delete db;
}

/* tests Database::insert() function in Database.cpp */
TEST(dbInsert, CheckInsert)
{
        Database *db = new Database();
        std::string insertRes = db->insert("Employer", "{\"eid\": \"5\", \"company_name\": \"Generic Consulting\", \"size\": \"medium\"}");
        std::cout << "insertRes: " << insertRes << std::endl;
        std::string expected_insert = R"([{"eid":5,"company_name":"Generic Consulting","size":"medium"}])";
        std::string expected_already_exists = "{\"code\":\"23505\",\"details\":\"Key (eid)=(5) already exists.\",\"hint\":null,\"message\":\"duplicate key value violates unique constraint \\\"Employer_pkey\\\"\"}";
        EXPECT_TRUE(insertRes == expected_insert || insertRes == expected_already_exists);

        delete db;
}

/* tests Database::update() function in Database.cpp */
TEST(dbUpdate, CheckUpdate)
{
        Database *db = new Database();
        std::string insertRes = db->insert("Employer", "{\"eid\": \"6\", \"company_name\": \"Small Consulting\", \"size\": \"medium\"}");
        std::string data = "{\"size\": \"small\"}";
        std::string updateRes = db->update("Employer", data, "eid", "eq", "6");
        std::cout << "updateRes: " << updateRes << std::endl;
        std::string expected_update = R"([{"eid":6,"company_name":"Small Consulting","size":"small"}])";
        EXPECT_EQ(updateRes, expected_update);
        delete db;
}

/* tests Database::deleteRecord() function in Database.cpp */
TEST(dbDelete, CheckDeleteRecord)
{
        Database *db = new Database();

        // Define table and record details
        std::string table = "Employer";
        std::string column = "eid";
        std::string op = "eq"; // 'eq' stands for 'equals'

        // Unique eid to prevent conflicts
        std::string eid = "100";

        // Step 1: Insert the record to ensure it exists
        std::string insertJson = R"({"eid": "100", "company_name": "Test Consulting", "size": "small"})";
        std::string insertRes = db->insert(table, insertJson);
        std::cout << "insertRes: " << insertRes << std::endl;

        // Expected responses for insert
        std::string expected_insert = R"([{"eid":100,"company_name":"Test Consulting","size":"small"}])";
        std::string expected_insert_already_exists = "{\"code\":\"23505\",\"details\":\"Key (eid)=(100) already exists.\",\"hint\":null,\"message\":\"duplicate key value violates unique constraint \\\"Employer_pkey\\\"\"}";

        // Verify
        EXPECT_TRUE(insertRes == expected_insert || insertRes == expected_insert_already_exists);

        // Step 2: Delete
        std::string deleteRes = db->deleteRecord(table, column, op, eid);
        std::cout << "deleteRes: " << deleteRes << std::endl;

        // Expected status code for delete
        std::string expected_delete_statusCode = "200"; // Based on your output

        // Verify
        EXPECT_EQ(deleteRes, expected_delete_statusCode);

        // Step 3: Attempt to delete the same record again
        std::string deleteRes2 = db->deleteRecord(table, column, op, eid);
        std::cout << "deleteRes2: " << deleteRes2 << std::endl;

        EXPECT_EQ(deleteRes2, expected_delete_statusCode);

        // Step 4: Query to confirm deletion
        int resCount = 0;
        std::vector<std::vector<std::string>> queryRes = db->query(table, "eid", column, op, eid, false, resCount);
        std::cout << "Query Results Size: " << queryRes.size() << std::endl;

        // Expect empty
        EXPECT_TRUE(queryRes.empty());

        // Clean up
        delete db;
}
