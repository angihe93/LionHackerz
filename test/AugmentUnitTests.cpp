// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Augment.h"
#include <iostream>
#include <stdexcept>

// use MockDatabase for testing

TEST(AugmentFunctions, checkAugmentFunctions) {

    Database *db = new MockDatabase();

    // test dimExists() in Augment.cpp
    // case where dimExists should return true
    int dim_id = 1;
    bool res = dimExists(db, dim_id);
    EXPECT_EQ(res, true);
    // case where dimExists should return false
    dim_id = -1;
    res = dimExists(db, dim_id);
    EXPECT_EQ(res, false);

    // test processAugments() in Augment.cpp
    std::vector<AugmentInput> augments = {{1, "very"}, {2, "somewhat"}, {-3, "not"}};
    std::string result = processAugments(db, 40, augments);
    std::string expected = "Augmentation processing completed.";
    EXPECT_EQ(result, expected);
    augments = {{3, "not"}};
    result = processAugments(db, 40, augments);
    EXPECT_EQ(result, expected);

    delete db;
} 