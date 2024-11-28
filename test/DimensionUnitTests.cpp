// Copyright 2024 LionHackerz!
#include <gtest/gtest.h>
#include "Dimension.h"
#include "Database.h"


// Test parsing valid JSON
TEST(DimensionTest, FromJson_Success) {
    Database *mockDb = new MockDatabase();
    Dimension dim;
    crow::json::wvalue json;

    // Populate JSON with all required fields
    json["loc"] = "New York";
    json["field"] = "Engineering";
    json["pay"] = 90000;
    json["gender"] = true;
    json["diversity"] = false;
    json["mbti"] = "INTJ";
    json["flexibility"] = true;
    json["remote"] = false;
    json["workspace"] = true;
    std::string jsonString = json.dump();
    

    // Parse the string back into an rvalue
    auto parsedJson = crow::json::load(jsonString);

    std::string error = dim.fromJson(parsedJson);
    

    // Expect no error
    EXPECT_EQ(error, "");
    
    // Verify that all fields are correctly set
    EXPECT_EQ(dim.loc, "New York");
    EXPECT_EQ(dim.field, "Engineering");
    EXPECT_EQ(dim.pay, 90000);
    EXPECT_TRUE(dim.gender);
    EXPECT_FALSE(dim.diversity);
    EXPECT_EQ(dim.mbti, "INTJ");
    EXPECT_TRUE(dim.flexibility);
    EXPECT_FALSE(dim.remote);
    EXPECT_TRUE(dim.workspace);
}

// Test parsing JSON with missing fields
TEST(DimensionTest, FromJson_MissingFields) {
    Dimension dim;
    crow::json::wvalue json;
    
    // Populate JSON with some missing fields (e.g., "mbti" and "workspace")
    json["loc"] = "Los Angeles";
    json["field"] = "Marketing";
    json["pay"] = 70000;
    json["gender"] = false;
    json["diversity"] = true;
    json["flexibility"] = false;
    json["remote"] = true;

    std::string jsonString = json.dump();

    // Parse the string back into an rvalue
    auto parsedJson = crow::json::load(jsonString);
    
    std::string error = dim.fromJson(parsedJson);
    
    // Expect an error message about missing fields
    EXPECT_NE(error, "");
    EXPECT_EQ(error, "Missing one or more dimension fields.");
}

// Test parsing JSON with invalid data types
TEST(DimensionTest, FromJson_InvalidDataTypes) {
    Dimension dim;
    crow::json::wvalue json;
    
    // Populate JSON with invalid data types (e.g., "pay" as string)
    json["loc"] = "Chicago";
    json["field"] = "Sales";
    json["pay"] = "eighty thousand"; // Should be an integer
    json["gender"] = "yes";          // Should be a boolean
    json["diversity"] = "no";        // Should be a boolean
    json["mbti"] = "ESFP";
    json["flexibility"] = "sometimes"; // Should be a boolean
    json["remote"] = "true";           // Should be a boolean
    json["workspace"] = "open";        // Should be a boolean

    std::string jsonString = json.dump();

    // Parse the string back into an rvalue
    auto parsedJson = crow::json::load(jsonString);
    
    std::string error = dim.fromJson(parsedJson);
    
    // Expect an error message due to invalid data types
    EXPECT_NE(error, "");
    EXPECT_TRUE(error.find("Error parsing dimensions") != std::string::npos);
}

/*
TEST(DimensionTest, Save_Success) {
    Database *mockDb = new MockDatabase();
    Dimension dim;
    
    // Set up Dimension object
    dim.user_id = 1;
    dim.loc = "Boston";
    dim.field = "Research";
    dim.pay = 120000;
    dim.gender = true;
    dim.diversity = true;
    dim.mbti = "ENTP";
    dim.flexibility = true;
    dim.remote = true;
    dim.workspace = false;
    
    // Expected JSON data to be inserted
    std::string expectedData = "{"
                                "\"id\": 1,"
                                "\"loc\": \"Boston\","
                                "\"field\": \"Research\","
                                "\"pay\": 120000,"
                                "\"gender\": true,"
                                "\"diversity\": true,"
                                "\"mbti\": \"ENTP\","
                                "\"flexibility\": true,"
                                "\"remote\": true,"
                                "\"workspace\": false"
                                "}";
    
    // Mock the insert method to return a success response
    EXPECT_CALL(mockDb, insert("Has_Dimension", expectedData))
        .Times(1)
        .WillOnce(Return("Success"));
    
    std::string result = dim.save(mockDb);
    
    // Verify the returned success message
    EXPECT_EQ(result, "Dimension data saved for user ID: 1");
}

// Test saving Dimension when database insert fails
TEST(DimensionTest, Save_DatabaseFailure) {
    Database *mockDb = new MockDatabase();
    Dimension dim;
    
    // Set up Dimension object
    dim.user_id = 2;
    dim.loc = "Seattle";
    dim.field = "Development";
    dim.pay = 95000;
    dim.gender = false;
    dim.diversity = false;
    dim.mbti = "ISTJ";
    dim.flexibility = false;
    dim.remote = false;
    dim.workspace = true;
    
    // Expected JSON data to be inserted
    std::string expectedData = "{"
                                "\"id\": 2,"
                                "\"loc\": \"Seattle\","
                                "\"field\": \"Development\","
                                "\"pay\": 95000,"
                                "\"gender\": false,"
                                "\"diversity\": false,"
                                "\"mbti\": \"ISTJ\","
                                "\"flexibility\": false,"
                                "\"remote\": false,"
                                "\"workspace\": true"
                                "}";
    
    // Mock the insert method to return an error response
    EXPECT_CALL(mockDb, insert("Has_Dimension", expectedData))
        .Times(1)
        .WillOnce(Return("Error: Database insert failed."));
    
    std::string result = dim.save(mockDb);
    
    // Even if insert fails, the current implementation returns a success message
    // This might be an area to improve in the actual Dimension::save method
    EXPECT_EQ(result, "Dimension data saved for user ID: 2");
}
*/