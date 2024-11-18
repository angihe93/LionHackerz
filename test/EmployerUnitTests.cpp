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

/* tests Employer::changeFlex() function in Employer.cpp */
TEST(EmployerChangeFlex, checkChangeFlex) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change flex should fail
        int resCode = 0;
        bool res = e->changeFlex(4, 3, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change flex should succeed
        res = e->changeFlex(4, 4, resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeGender() function in Employer.cpp */
TEST(EmployerChangeGender, checkChangeGender) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change gender should fail
        int resCode = 0;
        bool res = e->changeGender(5, 6, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change gender should succeed
        res = e->changeGender(7, 6, resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeDiversity() function in Employer.cpp */
TEST(EmployerChangeDiversity, checkChangeDiversity) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change diversity should fail
        int resCode = 0;
        bool res = e->changeDiversity(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change diversity should succeed
        res = e->changeDiversity(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeRemote() function in Employer.cpp */
TEST(EmployerChangeRemote, checkChangeRemote) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change remote should fail
        int resCode = 0;
        bool res = e->changeRemote(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change remote should succeed
        res = e->changeRemote(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeLocation() function in Employer.cpp */
TEST(EmployerChangeLocation, checkChangeLocation) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change location should fail
        int resCode = 0;
        bool res = e->changeLocation(1, 2, "blank", resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change location should succeed
        res = e->changeLocation(7, 6, "San Francisco", resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeMBTI() function in Employer.cpp */
TEST(EmployerChangeMBTI, checkChangeMBTI) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change MBTI should fail
        int resCode = 0;
        bool res = e->changeMBTI(1, 2, "blank", resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change MBTI should succeed
        res = e->changeMBTI(2, 2, "INTJ", resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeModernWorkspace() function in Employer.cpp */
TEST(EmployerChangeModernWorkspace, checkChangeModernWorkspace) {

        Database *db = new Database();
        Employer *e = new Employer(*db);

        // case where change modern workspace should fail
        int resCode = 0;
        bool res = e->changeModernWorkspace(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change modern workspace should succeed
        res = e->changeModernWorkspace(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}