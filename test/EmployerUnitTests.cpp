// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include "Database.h"
#include "Employer.h"
#include <curl/curl.h>
#include <vector>

// use MockDatabase for testing

/* tests Employer::checkHasListing() function in Employer.cpp */
TEST(EmployerCheckHasListing, checkHasListing)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where employer does not have the listing
        bool res = e->checkHasListing(1, 2);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where employer has the listing
        res = e->checkHasListing(1, 1);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeField() function in Employer.cpp */
TEST(EmployerChangeField, checkChangeField)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change field should fail
        bool res = e->changeField(1, 2, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change field should fail
        res = e->changeField(1, 9999, "Computer science");
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change field should succeed
        res = e->changeField(1, 1, "Computer science");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changePosition() function in Employer.cpp */
TEST(EmployerChangePosition, checkChangePosition)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change position should fail
        bool res = e->changePosition(2, 20, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change position should fail
        res = e->changePosition(2, 9999, "Lead Engineer");
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change position should succeed
        res = e->changePosition(2, 2, "Lead Engineer");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeJobDescription() function in Employer.cpp */
TEST(EmployerChangeJobDescription, checkChangeJobDescription)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change job description should fail
        bool res = e->changeJobDescription(3, 5, "blank");
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change job description should fail
        res = e->changeJobDescription(3, 9999, "We are looking for a few new programmers to round off our team.  Students welcome to apply.");
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change job description should succeed
        res = e->changeJobDescription(3, 3, "We are looking for a few new programmers to round off our team.  Students welcome to apply.");
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* tests Employer::changeFlex() function in Employer.cpp */
TEST(EmployerChangeFlex, checkChangeFlex)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change flex should fail because eid 4 does not have lid 3
        int resCode = 0;
        bool res = e->changeFlex(4, 3, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change flex should fail
        res = e->changeFlex(4, 9999, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change flex should succeed
        res = e->changeFlex(4, 4, resCode);
        expected = true;
        EXPECT_EQ(res, expected);
        // revert change if success
        if (res == expected)
                e->changeFlex(4, 4, resCode);

        // change flex to a boolean value
        // change flex should fail because eid 4 does not have lid 3
        res = e->changeFlex(4, 3, true, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // may need mocking? to test the error case: size_t id_pos = res.find("Error:"); if (id_pos != std::string::npos)


        delete db;
        delete e;
}

/* tests Employer::changeGender() function in Employer.cpp */
TEST(EmployerChangeGender, checkChangeGender)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change gender should fail
        int resCode = 0;
        bool res = e->changeGender(5, 6, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change gender should fail
        res = e->changeGender(5, 9999, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change gender should succeed
        res = e->changeGender(7, 6, resCode);
        expected = true;
        EXPECT_EQ(res, expected);
        // revert change if success
        if (res == expected)
                e->changeGender(7, 6, resCode);

        // case where change gender should fail
        res = e->changeGender(5, 6, true, resCode);
        expected = false;
        EXPECT_EQ(res, expected);


        delete db;
        delete e;
}

/* Tests Employer::changeDiversity() function in Employer.cpp */
TEST(EmployerChangeDiversity, checkChangeDiversity)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change diversity should fail
        int resCode = 0;
        bool res = e->changeDiversity(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change diversity should fail
        res = e->changeDiversity(1, 9999, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change diversity should succeed
        res = e->changeDiversity(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);
        // revert change if success
        if (res == expected)
                e->changeDiversity(1, 1, resCode);

        // case where change diversity should fail
        res = e->changeDiversity(1, 2, true, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeRemote() function in Employer.cpp */
TEST(EmployerChangeRemote, checkChangeRemote)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change remote should fail
        int resCode = 0;
        bool res = e->changeRemote(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change remote should fail
        res = e->changeRemote(1, 9999, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change remote should succeed
        res = e->changeRemote(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);
        // revert change if success
        if (res == expected)
                e->changeRemote(1, 1, resCode);

        // case where change remote should fail
        res = e->changeRemote(1, 2, true, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeLocation() function in Employer.cpp */
TEST(EmployerChangeLocation, checkChangeLocation)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change location should fail
        int resCode = 0;
        bool res = e->changeLocation(1, 2, "blank", resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change location should fail
        res = e->changeLocation(1, 9999, "San Francisco", resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change location should succeed
        res = e->changeLocation(7, 6, "San Francisco", resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeMBTI() function in Employer.cpp */
TEST(EmployerChangeMBTI, checkChangeMBTI)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change MBTI should fail
        int resCode = 0;
        bool res = e->changeMBTI(1, 2, "blank", resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change MBTI should fail
        res = e->changeMBTI(1, 9999, "INTJ", resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change MBTI should succeed
        res = e->changeMBTI(2, 2, "INTJ", resCode);
        expected = true;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeModernWorkspace() function in Employer.cpp */
TEST(EmployerChangeModernWorkspace, checkChangeModernWorkspace)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where change modern workspace should fail
        int resCode = 0;
        bool res = e->changeModernWorkspace(1, 2, resCode);
        bool expected = false;
        EXPECT_EQ(res, expected);

        // case where change modern workspace should fail
        res = e->changeModernWorkspace(1, 9999, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        // case where change modern workspace should succeed
        res = e->changeModernWorkspace(1, 1, resCode);
        expected = true;
        EXPECT_EQ(res, expected);
        // revert change if success
        if (res == expected)
                e->changeModernWorkspace(1, 1, resCode);

        // case where change modern workspace should fail
        res = e->changeModernWorkspace(1, 2, true, resCode);
        expected = false;
        EXPECT_EQ(res, expected);

        delete db;
        delete e;
}

/* Tests Employer::changeFieldAll() function in Employer.cpp */
TEST(EmployerChangeFieldAll, checkChangeFieldAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeFieldAll should succeed
        bool res = e->changeFieldAll(4, "Arts");
        // std::cout << "res from changeFieldAll: " << res << std::endl;
        EXPECT_EQ(res, true);

        // case where changeFieldAll should fail
        res = e->changeFieldAll(9999, "Arts");
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changePositionAll() function in Employer.cpp */
TEST(EmployerChangePositionAll, checkChangePositionAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changePositionAll should succeed
        bool res = e->changePositionAll(3, "Programmer");
        // std::cout << "res from changePositionAll: " << res << std::endl;
        EXPECT_EQ(res, true);

        // case where changePositionAll should fail
        res = e->changePositionAll(9999, "Programmer");
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changeFlexAll() function in Employer.cpp */
TEST(EmployerChangeFlexAll, checkChangeFlexAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeFlexAll should succeed
        int resCode = 0;
        bool res = e->changeFlexAll(7, true, resCode);
        EXPECT_EQ(res, true);

        // case where changeFlexAll should fail
        res = e->changeFlexAll(9999, true, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changeModernWorkspaceAll() function in Employer.cpp */
TEST(EmployerChangeModernWorkspaceAll, checkChangeModernWorkspaceAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeModernWorkspaceAll should succeed
        int resCode = 0;
        bool res = e->changeModernWorkspaceAll(7, true, resCode);
        EXPECT_EQ(res, true);

        // case where changeModernWorkspaceAll should fail
        res = e->changeModernWorkspaceAll(9999, true, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changeGenderAll() function in Employer.cpp */
TEST(EmployerChangeGenderAll, checkChangeGenderAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeGenderAll should succeed
        int resCode = 0;
        bool res = e->changeGenderAll(7, true, resCode);
        EXPECT_EQ(res, true);

        // case where changeGenderAll should fail
        res = e->changeGenderAll(9999, true, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changeDiversityAll() function in Employer.cpp */
TEST(EmployerChangeDiversityAll, checkChangeDiversityAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeDiversityAll should succeed
        int resCode = 0;
        bool res = e->changeDiversityAll(7, true, resCode);
        EXPECT_EQ(res, true);

        // case where changeDiversityAll should fail
        res = e->changeDiversityAll(9999, true, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changeRemoteAll() function in Employer.cpp */
TEST(EmployerChangeRemoteAll, checkChangeRemoteAll)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        // case where changeRemoteAll should succeed
        int resCode = 0;
        bool res = e->changeRemoteAll(7, false, resCode);
        EXPECT_EQ(res, true);

        // case where changeRemoteAll should fail
        res = e->changeRemoteAll(9999, false, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::postListing() function in Employer.cpp */
TEST(EmployerPostListing, checkPostListing)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        std::map<std::string, std::string> basicInfo = {{"field", "Education"}, {"position", "Education Consultant"}, {"job_description", "Develop and implement educational programs for schools and other institutions, providing guidance on curriculum and teaching strategies"}, {"location", "Boston"}};
        std::map<std::string, std::string> skillsPersonality = {{"skill1_req", "Curriculum Development"}, {"skill2_req", "Teaching Experience"}, {"skill3_req", "Instructional Design"}, {"skill4_req", "Project Management"}, {"skill5_req", "Communication"}, {"personality_types", "ENFJ"}};
        std::map<std::string, bool> boolFields = {{"job_flexibility", false}, {"modern_building", true}, {"mixed_gender", true}, {"diverse_workforce", true}, {"remote_available", false}};
        int64_t pay = 65000;

        int resLid = e->postListing(7, basicInfo, skillsPersonality, pay, boolFields);
        EXPECT_GT(resLid, 0);

        delete db;
        delete e;
}

/* Tests Employer::createEmployer() function in Employer.cpp */
TEST(EmployerCreate, checkCreateEmployer)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        std::string company_name = "MediMetrics";
        std::string size = "Medium";

        int res = e->createEmployer(company_name, size);
        EXPECT_GT(res, 0);

        delete db;
        delete e;
}

/* Tests Employer::changePay() function in Employer.cpp */
TEST(EmployerChangePay, checkPayListing)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        int resCode = 0;
        bool res = e->changePay(1, 1, 100000, resCode);
        EXPECT_EQ(res, true);

        res = e->changePay(1, 2, 100000, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::changePay() function in Employer.cpp */
TEST(EmployerChangeSkillRequirements, checkChangeSkillRequirements)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        int resCode = 0;
        bool res = e->changeSkillRequirements(1, 1, {{"skill1_req", "Data Analysis"}, {"skill2_req", "SQL"}, {"skill3_req", "Healthcare Industry Knowledge"}, {"skill4_req", "Problem-solving"}, {"skill5_req", "Communication"}}, resCode);
        EXPECT_EQ(res, true);

        res = e->changeSkillRequirements(1, 2, {{"skill1_req", "Data Analysis"}, {"skill2_req", "SQL"}, {"skill3_req", "Healthcare Industry Knowledge"}, {"skill4_req", "Problem-solving"}, {"skill5_req", "Communication"}}, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

TEST(EmployerChangePersonalityType, checkChangePersonalityType)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        int resCode = 0;
        bool res = e->changePersonalityTypes(1, 1, "INTJ", resCode);
        EXPECT_EQ(res, true);

        res = e->changePersonalityTypes(1, 2, "INTJ", resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}

/* Tests Employer::deleteListing() function in Employer.cpp */
TEST(EmployerDeleteListing, checkDeleteListing)
{

        Database *db = new MockDatabase();
        Employer *e = new Employer(*db);

        int resCode = 0;
        bool res = e->deleteListing(1, true, resCode);
        EXPECT_EQ(res, true);

        // deleteilsting for eid 1 lid 2, should fail
        res = e->deleteListing(1, 2, resCode);
        EXPECT_EQ(res, false);

        delete db;
        delete e;
}