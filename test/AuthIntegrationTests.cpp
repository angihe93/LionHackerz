// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Auth.h"
#include <curl/curl.h>
#include <vector>

// use MockDatabase for testing

/* tests all methods in Auth.cpp together */
TEST(AuthIntegration, checkAuthIntegration) {

    Database *db = new MockDatabase();
    Auth *a = new Auth(*db);

    // call genAPIKey before creating an api user, should fail
    std::string role = "admin";
    int uid = 9999;
    std::string apiKey = a->genAPIKey(role, uid);
    EXPECT_NE(apiKey.find("Error:"), std::string::npos);

    // create user
    int resCount = 0;
    std::vector<std::vector<std::string>> queryRes = db->query("API_User", "id", "", "", "", false, resCount);
    int largestUid = std::stoi(queryRes[0][resCount-1]);
    std::string email = "test" + std::to_string(largestUid) + "@email.com";
    std::string password = "test" + std::to_string(largestUid);
    uid = a->createAPIUser(email, password);
    EXPECT_GT(uid, 0);

    // call genAPIKey after creating a user, should succeed
    apiKey = a->genAPIKey(role, uid);
    EXPECT_EQ(apiKey.find("Error:"), std::string::npos);

    // call create user again should return same user id
    int uid2 = a->createAPIUser(email, password);
    EXPECT_EQ(uid, uid2);

    delete db;
    delete a;
}
