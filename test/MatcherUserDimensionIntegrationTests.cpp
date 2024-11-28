// Copyright 2024 LionHackerz
#include <gtest/gtest.h>
#include "Database.h"
#include "User.h"
#include "Dimension.h"
#include "Matcher.h"


/* tests multiple methods in Matcher, User, Dimension together */
TEST(MatcherUserDimensionIntegration, CheckMatcherUserDimensionIntegration) {

    Database *db = new MockDatabase();
    Matcher *m = new Matcher(*db);

    // call displayMatches on a uid that doesn't exit
    int resCount = 0;
    std::vector<std::vector<std::string>> queryRes = db->query("User", "id", "", "", "", false, resCount);
    int last_id = std::stoi(queryRes[0][resCount-1]);
    std::string next_id = std::to_string(last_id+1);
    std::vector<JobMatch> res = m->displayMatches(std::stoi(next_id), true);
    EXPECT_EQ(res.size(), 0);

    // make user with User::save 
    User user("test" + next_id, "test" + next_id + "@email.com");
    std::string saveRes = user.save(*db);
    std::cout << "saveRes: " << saveRes << std::endl;
    EXPECT_EQ(saveRes, "User created with ID: " + next_id);

    // call displayMatches again
    int uid = std::stoi(next_id);
    res = m->displayMatches(uid, true);
    // std::cout << "res.size(): " << res.size() << std::endl;
    EXPECT_EQ(res.size(), 0);

    // add dimensions for user
    // see makeUser in route controller
    Dimension dimension;
    dimension.user_id = uid;
    dimension.field = "Computer Science";
    dimension.flexibility = true;
    std::string dimension_result = dimension.save(*db);
    std::cout << dimension_result << std::endl;
    EXPECT_EQ(dimension_result, "Dimension data saved for user ID: " + std::to_string(uid));

    // call displayMatches again
    // res = m->displayMatches(uid, true);
    // std::cout << "res.size(): " << res.size() << std::endl;
    // check res size why still 0?

    delete db;
    delete m;


}
// call getMatches on a uid that doesn't exit
// make user with User::save
// call getMatches again
// add dimensions for user
// get matches again