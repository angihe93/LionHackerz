// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Auth.h"
#include <curl/curl.h>
#include <vector>

// use MockDatabase for testing

/* tests Auth::decodeBasicAuth() function in Auth.cpp */
TEST(AuthDecodeBasicAuth, checkDecodeBasicAuth) {

        Database *db = new MockDatabase();
        Auth *a = new Auth(*db);

        std::string basicAuth = "Basic ZHVtbXlAZW1haWwuY29tOmR1bW15";
        std::pair<std::string, std::string> decoded = a->decodeBasicAuth(basicAuth);
        EXPECT_EQ(decoded.first, "dummy@email.com");
        EXPECT_EQ(decoded.second, "dummy");

        basicAuth = "wrong auth";
        decoded = a->decodeBasicAuth(basicAuth);
        EXPECT_EQ(decoded.first, "");
        EXPECT_EQ(decoded.second, "");

        delete db;
        delete a;
}

/* tests Auth::createAPIUser() function in Auth.cpp */
TEST(AuthCreateAPIUser, checkCreateAPIUser) {

    Database *db = new MockDatabase();
    Auth *a = new Auth(*db);

    // create user should fail
    std::string email = "";
    std::string password = "";
    int uid = a->createAPIUser(email, password);
    EXPECT_EQ(uid, -1);

    // create user should succeed
    // get the largest uid, the created uid should be this + 1
    // or if user with email already exists, return its uid
    int resCount = 0;
    std::vector<std::vector<std::string>> queryRes = db->query("API_User", "id", "", "", "", false, resCount);
    int largestUid = std::stoi(queryRes[0][resCount-1]);
    email = "test@email.com";
    password = "test";
    uid = a->createAPIUser(email, password);
    EXPECT_GT(uid, 0);

    delete db;
    delete a;

}

/* tests Auth::genAPIKey() function in Auth.cpp */
TEST(AuthGenAPIKey, checkGenAPIKey) {

        Database *db = new MockDatabase();
        Auth *a = new Auth(*db);

        // gen api should succeed
        std::string role = "admin";
        int uid = 1;
        std::string apiKey = a->genAPIKey(role, uid);
        EXPECT_EQ(apiKey.find("Error:"), std::string::npos);

        // gen api should fail
        role = "admin";
        uid = 9999;
        apiKey = a->genAPIKey(role, uid);
        EXPECT_NE(apiKey.find("Error:"), std::string::npos);

        // gen api should fail
        role = "wrong role";
        uid = 1;
        apiKey = a->genAPIKey(role, uid);
        EXPECT_NE(apiKey.find("Error:"), std::string::npos);

        delete db;
        delete a;
}