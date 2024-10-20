// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Listing.h"
#include <curl/curl.h>
#include <vector>

/* tests Listing::getListing() function in Listing.cpp */
TEST(ListingGet, checkGetListing) {
        Database *db = new Database();
        Listing *l = new Listing(*db);
        std::string getRes = l->getListing(1);
        std::cout << "getRes: " << getRes << std::endl;
        std::string expected = "\tPosted by: \"Google\"\n\n\tCreated on: \"2024-10-12T22:19:23.396145+00:00\"\n\n\tField:  \"Computer science\"\n\n\tPosition: \"Software Engineer\"\n\n\tJob Description: \"Looking for a code monkey.\"\n\n\tSkills required: \"C++\", \"Python\", \n\n\tDiverse Workforce: \"true\"\n\n\tRemote Option Available: \"true\"\n\n";
        EXPECT_EQ(getRes, expected);

        getRes = l->getListing(2);
        std::cout << "getRes: " << getRes << std::endl;
        expected = "\tPosted by: \"Microsoft\"\n\n\tCreated on: \"2024-10-12T22:21:39.955097+00:00\"\n\n\tField:  \"Software Development\"\n\n\tPosition: \"Lead Engineer\"\n\n\tJob Description: \"Microsoft is currently looking for an experienced candidate to lead our engineering team.\"\n\n\tSkills required: \"Leadership\", \"Communication\", \"Programming languages\", \n\n\tPay: 175000\n\n\tFlexibility: \"true\"\n\n\tModern Workspace: \"true\"\n\n\tGender Parity: \"true\"\n\n\tDiverse Workforce: \"true\"\n\n\tRemote Option Available: \"true\"\n\n\tPersonality Types: \"INTJ\"\n\n";
        EXPECT_EQ(getRes, expected);

        delete db;
        delete l;
}

/* tests Listing::changeField(),changePosition(),changeJobDescription(), changeFlex() functions in Listing.cpp */
TEST(ListingChange, checkChangeListing) {
        Database *db = new Database();
        Listing *l = new Listing(*db);
        
        // change field for invalid lid
        std::string changeRes = l->changeField(-1, "blank");
        std::string expected = "Error: The listing ID you provided does not exist in the database.";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change field for valid lid
        changeRes = l->changeField(1, "Computer science");
        expected = R"([{"lid":1,"created_at":"2024-10-12T22:19:23.396145+00:00","field":"Computer science","position":"Software Engineer","job_description":"Looking for a code monkey.","skill1_req":"C++","skill2_req":"Python","skill3_req":null,"skill4_req":null,"skill5_req":null,"pay":null,"job_flexibility":null,"modern_building":null,"mixed_gender":null,"diverse_workforce":true,"remote_available":true,"personality_types":null,"location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change position for invalid lid
        changeRes = l->changePosition(-1, "blank");
        expected = "Error: The listing ID you provided does not exist in the database.";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change position for valid lid
        changeRes = l->changePosition(1, "Software Engineer");
        expected = R"([{"lid":1,"created_at":"2024-10-12T22:19:23.396145+00:00","field":"Computer science","position":"Software Engineer","job_description":"Looking for a code monkey.","skill1_req":"C++","skill2_req":"Python","skill3_req":null,"skill4_req":null,"skill5_req":null,"pay":null,"job_flexibility":null,"modern_building":null,"mixed_gender":null,"diverse_workforce":true,"remote_available":true,"personality_types":null,"location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // changeJobDescription
        changeRes = l->changeJobDescription(2, "Microsoft is currently looking for an experienced candidate to lead our engineering team.");
        expected = R"([{"lid":2,"created_at":"2024-10-12T22:21:39.955097+00:00","field":"Software Development","position":"Lead Engineer","job_description":"Microsoft is currently looking for an experienced candidate to lead our engineering team.","skill1_req":"Leadership","skill2_req":"Communication","skill3_req":"Programming languages","skill4_req":null,"skill5_req":null,"pay":175000,"job_flexibility":true,"modern_building":true,"mixed_gender":true,"diverse_workforce":true,"remote_available":true,"personality_types":"INTJ","location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // changeFlex - Success
        int resCount = 0;
        std::vector<std::vector<std::string>> curVal = db->query("Listing_TEST", "job_flexibility", "lid", "eq", "1", false, resCount);
        std::string val_before_change = curVal[0][0];
        changeRes = l->changeFlex(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);

        // changeFlex - Invalid listing ID
        changeRes = l->changeFlex(15, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        // changeModernWorkspace - Success
        resCount = 0;
        curVal = db->query("Listing_TEST", "modern_building", "lid", "eq", "1", false, resCount);
        val_before_change = curVal[0][0];
        changeRes = l->changeModernWorkspace(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);

        // changeModernWorkspace - Invalid listing ID
        changeRes = l->changeModernWorkspace(15, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        delete db;
        delete l;
}

