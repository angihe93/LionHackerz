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
    // Arrange
    MockDatabase mockDb;
    User user("Kelvin Kim", "sk4802@columbia.edu");

    // Expected JSON string for insertion
    std::string expectedJson = "{\"uname\": \"Kelvin Kim\", \"email\": \"sk4802@columbia.edu\"}";

    // Define the response from the mock database
    std::string mockResponse = "[{\"id\":2, \"name\":\"Kelvin Kim\", \"email\":\"sk4802@columbia.edu\"}]";

    // Set up expectation for insert method
    EXPECT_CALL(mockDb, insert("users", expectedJson))
        .Times(1)
        .WillOnce(testing::Return(mockResponse));

    // Act
    std::string result = user.save(mockDb);

    // Assert
    EXPECT_EQ(result, "User created with ID: 2");
}

TEST(UserTest, SaveUser_InvalidResponse)
{
    // Arrange
    MockDatabase mockDb;
    User user("Invalid User", "invalid@invalid.com");

    // Define the response from the mock database with invalid id format
    std::string mockResponse = "{\"name\":\"Invalid User\", \"email\":\"invalid@invalid.com\"}";

    // Set up expectation for insert method
    EXPECT_CALL(mockDb, insert("users", testing::_))
        .Times(1)
        .WillOnce(testing::Return(mockResponse));

    // Act & Assert
    EXPECT_THROW(user.save(mockDb), std::runtime_error);
}

TEST(UserTest, SaveUser_MalformedResponse)
{
    // Arrange
    MockDatabase mockDb;
    User user("Test User", "test@example.com");

    // Malformed response
    std::string malformedResponse = "[{\"name\":\"Test User\", \"email\":\"test@example.com\"}]";

    // Set up expectation for insert method
    EXPECT_CALL(mockDb, insert("users", testing::_))
        .Times(1)
        .WillOnce(testing::Return(malformedResponse));

    // Act & Assert
    EXPECT_THROW(user.save(mockDb), std::runtime_error);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}