#include "RouteController.h"
#include "Database.h"
#include "Matcher.h"
#include "crow.h"
#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

/**
 * Redirects to the homepage.
 *
 * @return A string containing the name of the html file to be loaded.
 */
void RouteController::index(crow::response& res) {
    res.write("Welcome, in order to make an API call direct your browser or Postman to an endpoint "
              "\n\n This can be done using the following format: \n\n http://127.0.0.1:18080/endpoint?arg=value");
    res.end();
}

void RouteController::setDatabase(Database *db) {
    db = db;
}

void RouteController::dbtest(const crow::request& req, crow::response& res) {
        // taken from Main.cpp 
        Database *db = new Database();

    /* test query user id value */
    int uid = 1;

    /* example queries:
     * These return an array of vectors with the values for each column requested,
     * e.g., req3[0] = vector of dimension ids in the results (vector<string>)
     *       req3[1] = vector of dimension names in the results (vector<string>)
     *       req3[2] = vector of dimenion weights in the results (vector<string>) */

    /* SELECT uname,email FROM User WHERE id = 1 */
    int resCount = 0;
    vector<vector<string> > req1 = db->query("User", "uname,email", "id", "eq", to_string(uid), true, resCount); 

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,weight_mod FROM Has_Augment WHERE id = 1 */
    vector<vector<string> > req2 = db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", to_string(uid), true, resCount);

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,name,def_weight FROM Dimension */
    vector<vector<string> > req3 = db->query("Dimension", "dim_id,name,def_weight", "", "", "", true, resCount);

    /* examples of INSERT request: these have already been inserted, so will return some errors from the DB */

    /* INSERT into User (uname, email) VALUES (TestUser, blah@blah.com)*
    string req4 = db->insert("User", "{\"uname\": \"TestUser\", \"email\": \"blah@blah.com\"}");
    std::cout << req4 << std::endl << std::endl;

    string req5 = db->insert("Skill", "{\"name\": \"drawing\", \"category\": \"art\"}");
    std::cout << req5 << std::endl << std::endl;

    /* can insert using string literal: R"( data here, no escapes required )" *
    string insert_data = R"(
    {
        "field": "arts",
        "position": "professor of art", 
        "job_description": "We are looking for a skilled artist who also has the capacity for teaching.",
        "skill1_req": "drawing",
        "skill2_req": "painting",
        "skill3_req": "sculpting",
        "skill4_req": "teaching",
        "pay": 75000,
        "job_flexibility": true,
        "personality_types": "INFP",
        "modern_building": true,
        "location": "New York"
    })";
    /* creates new job listing *
    string req6 = db->insert("Listing", insert_data);
    std::cout << req6 << std::endl << std::endl; /* prints results of request */

    /* Matcher test */
    Matcher m(*db);
    m.gatherRelevantDimensions(1);
    m.filterJobs();
    m.match(1);

}

void RouteController::initRoutes(crow::App<>& app) {
    CROW_ROUTE(app, "/")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, crow::response& res) {
            index(res);
        });

    CROW_ROUTE(app, "/dbtest")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, crow::response& res) {
            dbtest(req, res);
        });

}


