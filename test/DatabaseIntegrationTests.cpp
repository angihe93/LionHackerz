// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include <curl/curl.h>

/* tests all methods in Database.cpp together */
TEST(dbIntegration, CheckDbIntegration) {

    // should result in curl error bc not connected to DB
    std::string url = "";
    std::string api_key = "";
    Database *db = new Database(url,api_key);
    int resCount;
    std::vector<std::vector<std::string>> queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
    std::vector<std::vector<std::string>> expected;
    EXPECT_EQ(queryRes, expected); // expected queryRes to be empty bc not connected to DB

    // should connect to DB and check query result
    db = new MockDatabase();
    queryRes = db->query("Dimension", "name", "dim_id", "eq", "1", true, resCount);
    expected = {{ "\"location\"" }};
    EXPECT_EQ(queryRes, expected);

    // query a non-existent dimension
    queryRes = db->query("Dimension", "name", "", "", "", true, resCount);
    std::string next_id = std::to_string(resCount+1);
    queryRes = db->query("Dimension", "name", "dim_id", "eq", next_id, true, resCount);
    EXPECT_EQ(resCount, 0);

    // insert a new dimension
    std::string insertRes = db->insert("Dimension", "{\"dim_id\": \"" + next_id + "\", \"name\": \"dimension name\", \"def_weight\": \"50\"}");
    std::cout << "insertRes: " << insertRes << std::endl;
    EXPECT_EQ(insertRes.find("error:"), std::string::npos);

    // query the new dimension
    queryRes = db->query("Dimension", "name", "dim_id", "eq", next_id, true, resCount);
    EXPECT_EQ(resCount, 1);

    // update the new dimension
    std::string data = "{\"def_weight\": \"70\"}";
    std::string updateRes = db->update("Dimension", data, "dim_id", "eq", next_id);
    std::cout << "updateRes: " << updateRes << std::endl;
    std::string expected_update =  R"([{"dim_id":)" + next_id + R"(,"name":"dimension name","def_weight":70}])";
    EXPECT_EQ(updateRes, expected_update);

    delete db;

}