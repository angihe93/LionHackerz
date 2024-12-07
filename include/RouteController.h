// Copyright 2024 LionHackerz

#ifndef ROUTECONTROLLER_H
#define ROUTECONTROLLER_H

#include "crow.h"
#include "Database.h"
#include <cpp_redis/cpp_redis>
#include "Global.h"

struct SkillInput
{
    std::string name;
    std::optional<int> rank;
};

struct InterestInput
{
    std::string name;
    int rank;
};

class RouteController
{
private:
    Database *db;

public:
    void initRoutes(crow::App<> &app);
    void setDatabase(Database *db);

    /**
     * Redirects to the homepage.
     *
     * @return A crow::response object containing the welcome message and an HTTP 200 response
     */
    void index(crow::response &res);

    /**
     * Client sign up route
     *
     * @param req
     * @param res
     */
    void signUp(const crow::request &req, crow::response &res);

    /**
     * Check Auth Headers for API credentials
     *
     * @param req
     * @param res
     */
    bool checkAuthHeaders(const crow::request &req, crow::response &res);

    /* Route: /getMatches?uid=X
     *       This route checks all job listings and performs
     *       the matching algorithm in the Matcher.cpp class,
     *       returning a sorted list of matches based on
     *       the user's stated preferences.  */
    void getMatches(const crow::request &req, crow::response &res);

    /* Route: /listing/changeField?lid=X&newField=Y
     *       This route updates the 'field' parameter of the job
     *       listing with listing ID lid and sets the field to
     *       the string following 'newField=' in the URL. */
    void changeField(const crow::request &req, crow::response &res);

    /* Route: /listing/changePosition?lid=X&newPosition=Y
     *       This route updates the 'position' parameter of the job
     *       listing with listing ID lid and sets the position to
     *       the string following 'newPosition=' in the URL. */
    void changePosition(const crow::request &req, crow::response &res);

    /* Route: /listing/changeJobDescription?lid=X&newDescription=Y
     *       This route updates the 'job_description' parameter of the job
     *       listing with listing ID lid and sets the description to
     *       the string following 'newDescription=' in the URL. */
    void changeJobDescription(const crow::request &req, crow::response &res);

    /* Route: /listing/changeFlex?lid=X
     *       This route updates the 'job_flexibility' parameter of the job
     *       listing with listing ID lid and sets the position to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void changeFlex(const crow::request &req, crow::response &res);

    /* Route: /listing/changeModernWorkspace?lid=X
     *       This route updates the 'modern_building' parameter of the job
     *       listing with listing ID lid and sets the position to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void changeModernWorkspace(const crow::request &req, crow::response &res);

    /* Route: /listing/deleteListing?lid=X
     *       This route deletes the
     *       listing with listing ID lid. */
    void deleteListing(const crow::request &req, crow::response &res);

    /* Route: /listing/changePay?lid=X&newPay=Y
     *       This route updates the 'pay' parameter of the job
     *       listing with listing ID lid and sets the pay to
     *       the integer following 'newPay=' in the URL. */
    void changePay(const crow::request &req, crow::response &res);

    /* Route: /listing/changeSkillRequirements?lid=X&newSkills=Y
     *       This route updates the 'skill1_req', 'skill2_req', 'skill3_req',
     *       'skill4_req', and 'skill5_req' parameters of the job listing with
     *       listing ID lid and sets the skills to the strings following
     *       'newSkills=' in the URL. */
    void changeSkillRequirements(const crow::request &req, crow::response &res);

    /* Route: /listing/changePersonalityType?lid=X&newPersonalityTypes=Y
     *       This route updates the 'personality_types' parameter of the job
     *       listing with listing ID lid and sets the personality types to
     *       the strings following 'newPersonalityTypes=' in the URL. */
    void changePersonalityType(const crow::request &req, crow::response &res);

    /* EMPLOYER ROUTES */

    // Add description of function here

    /* Route: /employer/ChangeField?eid=X&lid=Y&newField=Z
     *       This route updates the 'field' parameter of the job
     *       listing with listing ID lid and sets the field to
     *       the string following 'newField=' in the URL. */
    void employerChangeField(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangePosition?eid=X&lid=Y&newPosition=Z
     *       This route updates the 'position' parameter of the job
     *       listing with listing ID lid and sets the position to
     *       the string following 'newPosition=' in the URL. */
    void employerChangePosition(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeJobDescription?eid=X&lid=Y&newDescription=Z
     *       This route updates the 'job_description' parameter of the job
     *       listing with listing ID lid and sets the description to
     *       the string following 'newDescription=' in the URL. */
    void employerChangeJobDescription(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeFlex?eid=X&lid=Y
     *       This route updates the 'job_flexibility' parameter of the job
     *       listing with listing ID lid and sets the position to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void employerChangeFlex(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeGender?eid=X&lid=Y
     *       This route updates the 'mixed_gender' parameter of the job
     *       listing with listing ID lid and sets the mixed_gender to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void employerChangeGender(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeDiversity?eid=X&lid=Y
     *       This route updates the 'diverse_workforce' parameter of the job
     *       listing with listing ID lid and sets the diverse_workforce to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void employerChangeDiversity(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeRemote?eid=X&lid=Y
     *       This route updates the 'remote_available' parameter of the job
     *       listing with listing ID lid and sets the remote_available to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void employerChangeRemote(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeLocation?eid=X&lid=Y?newLocation=Z
     *       This route updates the 'location' parameter of the job
     *       listing with listing ID lid and sets the location to
     *       the string following 'newLocation=' in the URL. */
    void employerChangeLocation(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeMBTI?eid=X&lid=Y?newMBTI=Z
     *       This route updates the 'personality_types' parameter of the job
     *       listing with listing ID lid and sets the personality types to
     *       the string following 'newMBTI=' in the URL. */
    void employerChangeMBTI(const crow::request &req, crow::response &res);

    /* Route: /employer/ChangeModernWorkspace?eid=X&lid=Y
     *       This route updates the 'modern_building' parameter of the job
     *       listing with listing ID lid and sets the modern_building to
     *       the true if previously null, false if previously true,
     *       and true if previously false.  Multiple calls cycle
     *       between setting true and false. */
    void employerChangeModernWorkspace(const crow::request &req, crow::response &res);

    void employerChangeFieldAll(const crow::request &req, crow::response &res);
    void employerChangePositionAll(const crow::request &req, crow::response &res);
    void employerChangeFlexAll(const crow::request &req, crow::response &res);
    void employerChangeModernWorkspaceAll(const crow::request &req, crow::response &res);
    void employerChangeGenderAll(const crow::request &req, crow::response &res);
    void employerChangeDiversityAll(const crow::request &req, crow::response &res);
    void employerChangeRemoteAll(const crow::request &req, crow::response &res);

    void employerPostListing(const crow::request &req, crow::response &res);
    void employerCreateEmployer(const crow::request &req, crow::response &res);
    void employerDeleteListing(const crow::request &req, crow::response &res);
    void employerChangePay(const crow::request &req, crow::response &res);
    void employerChangeSkillRequirements(const crow::request &req, crow::response &res);
    void employerChangePersonalityType(const crow::request &req, crow::response &res);

    // Helper function to handle boolean return of Employer methods
    void handleEmployerBoolResult(crow::response &res, bool result, int resCode, const std::string &successMsg);

    // Helper function to handle boolean return of Employer methods
    void handleEmployerBoolResult2(crow::response &res, std::string result, int resCode, const std::string &successMsg);

    // END EMPLOYER ROUTES

    /* Route: /listing/generateAIListing?n=X
     *       This route generates n new listings in the database generated
     *       by AI.  The listings are from random fields with random
     *       dimensions generated via the AI but corresponding to
     *       values one would expect from real jobs.  n must be
     *       between 1-20. Listings are generated in Listing_AI. */
    void generateAIListing(const crow::request &req, crow::response &res);

    /* Route: /dbtest
     *      This route tests the functionality of the database class and
     *      provides examples of queries and inserts. Not intended for
     *      the end user of the API, more for development purposes. */
    void dbtest(const crow::request &req, crow::response &res);

    /* Route: /user/makeUser
     *      This route allows a user to create a new user profile
     *      in the database.  The user must provide a name, email,
     *      and dimensions.  The dimensions are a JSON object
     *      containing the user's skills and interests. */
    void makeUser(const crow::request &req, crow::response &res);

    /* Route: /returnError
     *      This route is a test route to return an error message
     *      with a specified error code. */
    void returnError(crow::response &res, int code, const std::string &message);

    /* Route: /parseSkills
     *      This route is a test route to parse a JSON object
     *      containing a list of skills and their ranks. */
    std::string parseSkills(const crow::json::rvalue &skills_json, std::vector<SkillInput> &skills);

    /* Route: /parseInterests
     *      This route is a test route to parse a JSON object
     *      containing a list of interests and their ranks. */
    std::string parseInterests(const crow::json::rvalue &interests_json, std::vector<InterestInput> &interests);

    /* Route: /user/processSkills
     *      This route is a test route to process a list of skills
     *      and their ranks for a user. */
    std::string processSkills(Database &db, int user_id, const std::vector<SkillInput> &skills);

    /* Route: /user/processInterests
     *      This route is a test route to process a list of interests
     *      and their ranks for a user. */
    std::string processInterests(Database &db, int user_id, const std::vector<InterestInput> &interests);
};

#endif
