// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Listing.h"
#include <curl/curl.h>
#include <vector>
#include <map>

// use MockDatabase for testing

/* tests Listing::getListing() function in Listing.cpp */
TEST(ListingGet, checkGetListing)
{
    Database *db = new MockDatabase();
    Listing *l = new Listing(*db);

    // Test 1: Listing ID 1
    std::vector<std::string> res = l->getListing(1, true);
    std::string getRes = res[0];
    std::cout << "getRes: " << getRes << std::endl;

    // Extract and test the "Skills required" field
    std::size_t skillsStart = getRes.find("Skills required: ");
    ASSERT_NE(skillsStart, std::string::npos); // Ensure the field exists
    skillsStart += std::string("Skills required: ").length();
    std::size_t skillsEnd = getRes.find('\n', skillsStart);
    std::string skillsField = getRes.substr(skillsStart, skillsEnd - skillsStart);

    // Verify that the "Skills required" field is correct
    EXPECT_EQ(skillsField, "\"\", \"\", \"\", \"\", \"\"");

    // Test 2: Listing ID 2
    getRes = (l->getListing(2, true))[0];
    std::cout << "getRes: " << getRes << std::endl;

    // Extract and test the "Pay" field
    std::size_t payStart = getRes.find("Pay: ");
    ASSERT_NE(payStart, std::string::npos); // Ensure the field exists
    payStart += std::string("Pay: ").length();
    std::size_t payEnd = getRes.find('\n', payStart);
    std::string payField = getRes.substr(payStart, payEnd - payStart);

    // Verify that the "Pay" field is correct
    EXPECT_EQ(payField, "175000");

    // Test 3: Listing ID 19 with null fields
    getRes = (l->getListing(19, true))[0];
    // std::cout << "getRes: " << getRes << std::endl;
    // Extract and test the "Pay" field
    payStart = getRes.find("Pay: ");
    EXPECT_EQ(payStart, std::string::npos);

    // Test 4: Listing ID 143 in Listing_AI with null skill fields
    db = new Database();
    l = new Listing(*db);
    // Test with AI table with null fields to exercise additional branches
    getRes = (l->getListing(143, false))[0];
    // std::cout << "getRes: " << getRes << std::endl;
    // Extract and test the "Skills" field
    skillsStart = getRes.find("Skills required: ");
    EXPECT_EQ(skillsStart, std::string::npos);

    // Test 5: Listing_TEST with invalid lid
    std::vector<std::string> expectedRes;
    expectedRes.push_back("Error: The listing ID you provided does not exist in the database or is not associated with an employer.");
    getRes = l->getListing(-999, true)[0];
    EXPECT_EQ(getRes, expectedRes[0]);

    // Test 5: Listing_AI with invalid lid
    getRes = (l->getListing(-999, false))[0];
    EXPECT_EQ(getRes, expectedRes[0]);

    // Clean up
    delete db;
    delete l;
}

/* tests Listing::changeField(),changePosition(),changeJobDescription(), changeFlex() functions in Listing.cpp */
TEST(ListingChange, checkChangeField)
{
    Database *db = new MockDatabase();
    Listing *l = new Listing(*db);

    // Change field for invalid lid
    std::string changeRes = l->changeField(-1, "blank");
    std::string expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change field for valid lid
    changeRes = l->changeField(1, "Computer science");
    EXPECT_TRUE(changeRes.find("\"field\":\"Computer science\"") != std::string::npos);

    // Change position for invalid lid
    changeRes = l->changePosition(-1, "blank");
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change position for valid lid
    changeRes = l->changePosition(1, "Software Engineer");
    EXPECT_TRUE(changeRes.find("\"position\":\"Software Engineer\"") != std::string::npos);

    // Change job description for valid lid
    changeRes = l->changeJobDescription(2, "Microsoft is currently looking for an experienced candidate to lead our engineering team.");
    EXPECT_TRUE(changeRes.find("\"job_description\":\"Microsoft is currently looking for an experienced candidate to lead our engineering team.\"") != std::string::npos);

    // Change job description for invalid lid
    changeRes = l->changeJobDescription(9999, "Microsoft is currently looking for an experienced candidate to lead our engineering team.");
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change diversity for invalid lid
    int resCode = 0;
    changeRes = l->changeDiversity(-1, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change remote for invalid lid
    changeRes = l->changeRemote(-1, resCode);
    EXPECT_EQ(changeRes, expected);

    // Change location for invalid lid
    changeRes = l->changeLocation(-1, "blank", resCode);
    EXPECT_EQ(changeRes, expected);

    // Change MBTI for invalid lid
    changeRes = l->changeMBTI(-1, "INTJ", resCode);
    EXPECT_EQ(changeRes, expected);

    // Change modern workspace for invalid lid
    changeRes = l->changeModernWorkspace(-1, resCode);
    EXPECT_EQ(changeRes, expected);

    // Change flex for valid lid
    // get previous flex value to restore later
    int resCount = 0;
    std::vector<std::vector<std::string>> queryRes = db->query("Listing", "job_flexibility", "lid", "eq", "1", false, resCount);
    std::cout << "queryRes: " << queryRes[0][0] << std::endl;
    bool prev = queryRes[0][0] == "true";
    changeRes = l->changeFlex(1, true, resCode);
    std::cout << "changeRes: " << changeRes << std::endl;
    EXPECT_EQ(changeRes, "\"true\"");
    l->changeFlex(1, prev, resCode);

    // Change flex for invalid lid
    changeRes = l->changeFlex(9999, true, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change gender for valid lid
    // get previous flex value to restore later
    queryRes = db->query("Listing", "mixed_gender", "lid", "eq", "1", false, resCount);
    prev = queryRes[0][0] == "true";
    changeRes = l->changeGender(1, true, resCode);
    EXPECT_EQ(changeRes, "\"true\"");
    l->changeGender(1, prev, resCode);

    // Change gender for invalid lid
    changeRes = l->changeGender(9999, true, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change diversity for valid lid
    queryRes = db->query("Listing", "dicerse_workforce", "lid", "eq", "1", false, resCount);
    prev = queryRes[0][0] == "true";
    changeRes = l->changeDiversity(1, true, resCode);
    EXPECT_EQ(changeRes, "\"true\"");
    l->changeDiversity(1, prev, resCode);

    // Change diversity for invalid lid
    changeRes = l->changeDiversity(9999, true, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change remote for valid lid
    queryRes = db->query("Listing", "remote_available", "lid", "eq", "1", false, resCount);
    prev = queryRes[0][0] == "true";
    changeRes = l->changeRemote(1, true, resCode);
    EXPECT_EQ(changeRes, "\"true\"");
    l->changeRemote(1, prev, resCode);

    // Change remote for invalid lid
    changeRes = l->changeRemote(9999, true, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    // Change modern workplace for valid lid
    queryRes = db->query("Listing", "modern_building", "lid", "eq", "1", false, resCount);
    prev = queryRes[0][0] == "true";
    changeRes = l->changeModernWorkspace(1, true, resCode);
    EXPECT_EQ(changeRes, "\"true\"");
    l->changeModernWorkspace(1, prev, resCode);

    // Change modern workplace for invalid lid
    changeRes = l->changeModernWorkspace(9999, true, resCode);
    expected = "Error: The listing ID you provided does not exist in the database.";
    EXPECT_EQ(changeRes, expected);

    delete db;
    delete l;
}

/* tests Listing::insertListing() function in Listing.cpp */
TEST(ListingInsert, checkInsertListing)
{

    Database *db = new MockDatabase();
    Listing *l = new Listing(*db);

    std::map<std::string, std::string> basicInfo = {{"field", "HealthTech"}, {"position", "Healthcare Data Analyst"}, {"job_description", "Analyze and interpret healthcare data to aid in decision-making"}, {"location", "Boston"}};
    std::map<std::string, std::string> skillsPersonality = {{"skill1_req", "Data Analysis"}, {"skill2_req", "SQL"}, {"skill3_req", "Healthcare Industry Knowledge"}, {"skill4_req", "Problem-solving"}, {"skill5_req", "Communication"}, {"personality_types", "INTJ"}};
    std::map<std::string, bool> boolFields = {{"job_flexibility", false}, {"modern_building", true}, {"mixed_gender", true}, {"diverse_workforce", true}, {"remote_available", false}};
    int64_t pay = 75000;
    int insertRes = l->insertListing(basicInfo, skillsPersonality, pay, boolFields);
    EXPECT_GT(insertRes, 0);

    delete db;
    delete l;
}

TEST(ListingDelete, checkDeleteListing)
{
    int resCode = 0;

    // Create a MockDatabase instance
    Database *db = new MockDatabase();
    Listing *l = new Listing(*db);

    // Define the listing data to insert
    std::map<std::string, std::string> basicInfo = {
        {"field", "HealthTech"},
        {"position", "Healthcare Data Analyst"},
        {"job_description", "Analyze and interpret healthcare data to aid in decision-making"},
        {"location", "Boston"}};
    std::map<std::string, std::string> skillsPersonality = {
        {"skill1_req", "Data Analysis"},
        {"skill2_req", "SQL"},
        {"skill3_req", "Healthcare Industry Knowledge"},
        {"skill4_req", "Problem-solving"},
        {"skill5_req", "Communication"},
        {"personality_types", "INTJ"}};
    std::map<std::string, bool> boolFields = {
        {"job_flexibility", false},
        {"modern_building", true},
        {"mixed_gender", true},
        {"diverse_workforce", true},
        {"remote_available", false}};
    int64_t pay = 75000;

    // Insert the listing
    int insertRes = l->insertListing(basicInfo, skillsPersonality, pay, boolFields);
    EXPECT_GT(insertRes, 0);
    std::cout << "insertRes: " << insertRes << std::endl;

    // // Delete listing for invalid lid
    l->deleteListing(-1, resCode);
    EXPECT_EQ(resCode, 404);
    // std::cout << "deleteRes (invalid lid): " << deleteRes << std::endl;

    // Delete listing for valid lid
    l->deleteListing(insertRes, resCode);
    EXPECT_EQ(resCode, 200);

    // Clean up
    delete l;
    delete db;
}
/* Tests Listing::changePay(), changeSkillRequirements(), changePersonalityTypes() functions in Listing.cpp */
TEST(ListingChangeAdditional, checkChangePaySkillPersonality)
{
    // Create a MockDatabase instance
    Database *db = new MockDatabase();
    Listing *l = new Listing(*db);
    int resCode = 0;
    // Step 1: Insert a listing to work with
    std::map<std::string, std::string> basicInfo = {
        {"field", "Engineering"},
        {"position", "Software Developer"},
        {"job_description", "Develop and maintain software applications."},
        {"location", "San Francisco"}};
    std::map<std::string, std::string> skillsPersonality = {
        {"skill1_req", "C++"},
        {"skill2_req", "Python"},
        {"skill3_req", "Algorithms"},
        {"skill4_req", "Data Structures"},
        {"skill5_req", "Problem Solving"},
        {"personality_types", "INTP"}};
    std::map<std::string, bool> boolFields = {
        {"job_flexibility", true},
        {"modern_building", true},
        {"mixed_gender", true},
        {"diverse_workforce", true},
        {"remote_available", true}};
    int64_t initialPay = 100000;
    int lid = l->insertListing(basicInfo, skillsPersonality, initialPay, boolFields);
    EXPECT_GT(lid, 0); // Ensure the listing was inserted successfully

    // Step 2: Test changePay function
    int64_t newPay = 120000;
    l->changePay(lid, newPay, resCode);
    EXPECT_EQ(resCode, 200); // Expect the response code to be 200

    // Verify that the pay was updated in the database
    int resCount = 0;
    std::vector<std::vector<std::string>> listing = db->query("Listing", "pay", "lid", "eq", std::to_string(lid), false, resCount);
    EXPECT_EQ(resCount, 1);
    EXPECT_EQ(listing[0][0], std::to_string(newPay));

    // // Step 3: Test changeSkillRequirements function
    std::map<std::string, std::string> newSkills = {
        {"skill1_req", "Java"},
        {"skill2_req", "Kotlin"},
        {"skill3_req", "Android Development"}};
    l->changeSkillRequirements(lid, newSkills, resCode);
    EXPECT_EQ(resCode, 200); // Expect the response code to be 200

    // // Verify that the skills were updated in the database
    listing = db->query("Listing", "skill1_req,skill2_req,skill3_req", "lid", "eq", std::to_string(lid), false, resCount);
    EXPECT_EQ(resCount, 1);

    // // Step 4: Test changePersonalityTypes function
    std::string newPersonality = "ENFP";
    l->changePersonalityTypes(lid, newPersonality, resCode);
    EXPECT_EQ(resCode, 200); // Expect the response code to be 200

    // EXPECT_TRUE(changePersonalityResult); // Expect the personality type update to succeed

    // // Verify that the personality types were updated in the database
    listing = db->query("Listing", "personality_types", "lid", "eq", std::to_string(lid), false, resCount);
    std::string newPersonalityWithQuotes = "\"" + newPersonality + "\"";
    EXPECT_EQ(listing[0][0], newPersonalityWithQuotes);
    EXPECT_EQ(resCount, 1);

    // // Step 5: Test error handling with invalid listing ID
    int invalidLid = -1;
    l->changePay(invalidLid, newPay, resCode);
    EXPECT_EQ(resCode, 404);

    // EXPECT_FALSE(changePayResult); // Expect the pay update to fail

    l->changeSkillRequirements(invalidLid, newSkills, resCode);
    EXPECT_EQ(resCode, 404);

    l->changePersonalityTypes(invalidLid, newPersonality, resCode);
    EXPECT_EQ(resCode, 404);

    // Clean up
    delete l;
    delete db;
}

TEST(GenAIListing, checkGenAIListing)
{
    Database *db = new Database();
    Listing *l = new Listing(*db);

    std::string res = l->generateAIListing("1"); // also calls parseAI
    std::cout << "res: " << res << std::endl;
    std::string errRes = "Error: OpenAI request failed.  Please check you have credits or have access to model gpt-4o.";
    EXPECT_NE(res, errRes);

    delete db;
    delete l;
}
