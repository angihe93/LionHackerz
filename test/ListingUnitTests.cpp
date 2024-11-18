// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Listing.h"
#include <curl/curl.h>
#include <vector>
#include <map>

// use MockDatabase for testing

/* tests Listing::getListing() function in Listing.cpp */
TEST(ListingGet, checkGetListing) {
        Database *db = new MockDatabase();
        Listing *l = new Listing(*db);
        std::string getRes = l->getListing(1);
        std::cout << "getRes: " << getRes << std::endl;
        std::string expected = "\tPosted by: \"Google\"\n\n\tCreated on: \"2024-10-12T22:19:23.396145+00:00\"\n\n\tField:  \"Computer science\"\n\n\tPosition: \"Software Engineer\"\n\n\tJob Description: \"Looking for a code monkey.\"\n\n\tSkills required: \"C++\", \"Python\", \n\n\tFlexibility: \"true\"\n\n\tModern Workspace: \"true\"\n\n\tGender Parity: \"true\"\n\n\tDiverse Workforce: \"true\"\n\n\tRemote Option Available: \"false\"\n\n";
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
        Database *db = new MockDatabase();
        Listing *l = new Listing(*db);
        
        // change field for invalid lid
        std::string changeRes = l->changeField(-1, "blank");
        std::string expected = "Error: The listing ID you provided does not exist in the database.";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change field for valid lid
        changeRes = l->changeField(1, "Computer science");
        expected = R"([{"lid":1,"created_at":"2024-10-12T22:19:23.396145+00:00","field":"Computer science","position":"Software Engineer","job_description":"Looking for a code monkey.","skill1_req":"C++","skill2_req":"Python","skill3_req":null,"skill4_req":null,"skill5_req":null,"pay":null,"job_flexibility":true,"modern_building":true,"mixed_gender":true,"diverse_workforce":true,"remote_available":false,"personality_types":null,"location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // change position for invalid lid
        changeRes = l->changePosition(-1, "blank");
        expected = "Error: The listing ID you provided does not exist in the database.";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // // change position for valid lid
        changeRes = l->changePosition(1, "Software Engineer");
        expected = R"([{"lid":1,"created_at":"2024-10-12T22:19:23.396145+00:00","field":"Computer science","position":"Software Engineer","job_description":"Looking for a code monkey.","skill1_req":"C++","skill2_req":"Python","skill3_req":null,"skill4_req":null,"skill5_req":null,"pay":null,"job_flexibility":true,"modern_building":true,"mixed_gender":true,"diverse_workforce":true,"remote_available":false,"personality_types":null,"location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // changeJobDescription
        changeRes = l->changeJobDescription(2, "Microsoft is currently looking for an experienced candidate to lead our engineering team.");
        expected = R"([{"lid":2,"created_at":"2024-10-12T22:21:39.955097+00:00","field":"Software Development","position":"Lead Engineer","job_description":"Microsoft is currently looking for an experienced candidate to lead our engineering team.","skill1_req":"Leadership","skill2_req":"Communication","skill3_req":"Programming languages","skill4_req":null,"skill5_req":null,"pay":175000,"job_flexibility":true,"modern_building":true,"mixed_gender":true,"diverse_workforce":true,"remote_available":true,"personality_types":"INTJ","location":null}])";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // changeJobDescription - Invalid Listing ID
        changeRes = l->changeJobDescription(9999, "Microsoft is currently looking for an experienced candidate to lead our engineering team.");
        expected = "Error: The listing ID you provided does not exist in the database.";
        // cout << "changeRes: " << changeRes << endl;
        EXPECT_EQ(changeRes, expected);

        // changeFlex - Success
        int resCount = 0;
        std::vector<std::vector<std::string>> curVal = db->query("Listing", "job_flexibility", "lid", "eq", "1", false, resCount);
        std::string val_before_change = curVal[0][0];
        std::cout << "changeFlex val_before_change: " << val_before_change << std::endl;
        changeRes = l->changeFlex(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);
        // revert change if success
        if (changeRes == expected) 
                l->changeFlex(1, resCount);

        // changeFlex - Invalid listing ID
        changeRes = l->changeFlex(9999, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        // changeModernWorkspace - Success
        resCount = 0;
        curVal = db->query("Listing", "modern_building", "lid", "eq", "1", false, resCount);
        val_before_change = curVal[0][0];
        std::cout << "changeModernWorkspace val_before_change: " << val_before_change << std::endl;
        changeRes = l->changeModernWorkspace(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);
        // revert change if success
        if (changeRes == expected) 
                l->changeModernWorkspace(1, resCount);

        // changeModernWorkspace - Invalid listing ID
        changeRes = l->changeModernWorkspace(9999, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        // changeGender - Success
        resCount = 0;
        curVal = db->query("Listing", "mixed_gender", "lid", "eq", "1", false, resCount);
        val_before_change = curVal[0][0];
        std::cout << "changeGender val_before_change: " << val_before_change << std::endl;
        changeRes = l->changeGender(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);
        // revert change if success
        if (changeRes == expected) 
                l->changeGender(1, resCount);

        // changeGender - Invalid listing ID
        changeRes = l->changeGender(9999, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        // changeDiversity - Success
        resCount = 0;
        curVal = db->query("Listing", "diverse_workforce", "lid", "eq", "1", false, resCount);
        val_before_change = curVal[0][0];
        std::cout << "changeDiversity val_before_change: " << val_before_change << std::endl;
        changeRes = l->changeDiversity(1, resCount);
        if (val_before_change == "\"null\"" || val_before_change == "\"false\"")
                expected = "\"true\"";
        else if (val_before_change == "\"true\"")
                expected = "\"false\"";
        EXPECT_EQ(changeRes, expected);
        // revert change if success
        if (changeRes == expected) 
                l->changeDiversity(1, resCount);

        // changeDiversity - Invalid listing ID
        changeRes = l->changeDiversity(9999, resCount);
        expected = "Error: The listing ID you provided does not exist in the database.";
        EXPECT_EQ(changeRes, expected);

        delete db;
        delete l;
}

/* tests Listing::insertListing() function in Listing.cpp */
TEST(ListingInsert, checkInsertListing) {
        Database *db = new MockDatabase();
        Listing *l = new Listing(*db);
        std::map<std::string, std::string> basicInfo = {{"field", "HealthTech"}, {"position", "Healthcare Data Analyst"}, {"job_description", "Analyze and interpret healthcare data to aid in decision-making"}, {"location", "Boston"}};
        std::map<std::string, std::string> skillsPersonality = {{"skill1_req", "Data Analysis"}, {"skill2_req", "SQL"}, {"skill3_req", "Healthcare Industry Knowledge"}, {"skill4_req", "Problem-solving"}, {"skill5_req", "Communication"}, {"personality_types", "INTJ"}};
        std::map<std::string, bool> boolFields = {{"job_flexibility", false}, {"modern_building", true}, {"mixed_gender", true}, {"diverse_workforce", true}, {"remote_available", false}};
        int8_t pay = 75000;
        int insertRes = l->insertListing(basicInfo, skillsPersonality, pay, boolFields);
}