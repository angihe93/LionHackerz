// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Employer.h"
#include <curl/curl.h>
#include <vector>

/* tests Employer::checkHasListing() function in Employer.cpp */
TEST(EmployerCheckHasListing, checkHasListing) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where employer does not have the listing
        bool res = e->checkHasListing(1,2);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where employer has the listing
        res = e->checkHasListing(1,1); 
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeField() function in Employer.cpp */
TEST(EmployerChangeField, checkChangeField) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change field should fail
        bool res = e->changeField(1, 2, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change field should succeed
        res = e->changeField(1, 1, "Computer science");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changePosition() function in Employer.cpp */
TEST(EmployerChangePosition, checkChangePosition) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change position should fail
        bool res = e->changePosition(2, 20, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change position should succeed
        res = e->changePosition(2, 2, "Lead Engineer");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeJobDescription() function in Employer.cpp */
TEST(EmployerChangeJobDescription, checkChangeJobDescription) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change job description should fail
        bool res = e->changeJobDescription(3, 5, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change job description should succeed
        res = e->changeJobDescription(3, 3, "We are looking for a few new programmers to round off our team.  Students welcome to apply.");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}