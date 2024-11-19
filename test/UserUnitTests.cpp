// Copyright 2024 LionHackerz
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "User.h"

// Mock Database class
class MockDatabase : public Database
{
public:
    MOCK_METHOD(std::string, insert, (const std::string &table, const std::string &data), (override));
};

// Test case for the User class
TEST(UserTest, SaveUser_Success)
{
    MockDatabase mockDb;
    User user("Kelvin Kim", "sk4802@columbia.edu");

    std::string expectedJson = "{\"uname\": \"Kelvin Kim\", \"email\": \"sk4802@columbia.edu\"}";
    std::string mockResponse = "[{\"id\":2, \"name\":\"Kelvin Kim\", \"email\":\"sk4802@columbia.edu\"}]";

    EXPECT_CALL(mockDb, insert("users", expectedJson))
        .Times(1)
        .WillOnce(testing::Return(mockResponse));

    std::string result = user.save(mockDb);

    EXPECT_EQ(result, "User created with ID: 2");
}

TEST(UserTest, SaveUser_InvalidResponse)
{
    MockDatabase mockDb;
    User user("Invalid User", "invalid@invalid.com");

    std::string mockResponse = "{\"name\":\"Invalid User\", \"email\":\"invalid@invalid.com\"}";

    EXPECT_CALL(mockDb, insert("users", testing::_))
        .Times(1)
        .WillOnce(testing::Return(mockResponse));

    EXPECT_THROW(user.save(mockDb), std::runtime_error);
}

TEST(UserTest, SaveUser_MalformedResponse)
{
    MockDatabase mockDb;
    User user("Test User", "test@example.com");

    std::string malformedResponse = "[{\"name\":\"Test User\", \"email\":\"test@example.com\"}]";

    EXPECT_CALL(mockDb, insert("users", testing::_))
        .Times(1)
        .WillOnce(testing::Return(malformedResponse));

    EXPECT_THROW(user.save(mockDb), std::runtime_error);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}