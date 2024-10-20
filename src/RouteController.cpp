// Copyright 2024 LionHackerz

#include "RouteController.h"
#include "Database.h"
#include "Matcher.h"
#include "Listing.h"
#include "User.h"
#include "Augment.h"
#include "Auth.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "../external_libraries/Crow/include/crow.h"


void RouteController::index(crow::response &res)
{
    res.write("Welcome, in order to make an API call direct your browser or Postman to an endpoint "
              "\n\n This can be done using the following format: \n\n http://127.0.0.1:18080/endpoint?arg=value");
    res.end();
}

void RouteController::signUp(const crow::request &req, crow::response &res) {

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    std::string role;
    if (params.get("role") != nullptr) {
        role = params.get("role");
    } else {
        res.code = 400; 
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a role in the 'role' parameter.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    if (role == "admin") {
        Auth *a = new Auth(*db);
        std::string retStr = a->signUp(role);

        size_t id_pos = retStr.find("Error:");
        if (id_pos != std::string::npos) { // error
            std::cout << retStr << std::endl;
        }

        res.code = 201; 
        jsonRes["data"]["apikey"] = retStr;
        res.write(jsonRes.dump());
        res.end();
        return;

    } else {
        res.code = 400; 
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Roles other than admin are yet implemented";
        res.write(jsonRes.dump());
        res.end();
        return;
    }



}

void RouteController::setDatabase(Database *db)
{
    this->db = db;
}

void RouteController::getMatches(const crow::request &req, crow::response &res)
{
    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    auto header = req.get_header_value("apikey");
    std::cout << "header: " << header << std::endl;
    // look up key in DB, check permissions

    int uid = 0;

    if (params.get("uid") != nullptr) {
        uid = std::stoi(params.get("uid"));
    } else {
        res.code = 400;    
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a user ID with '?uid=X' to retrieve job matches.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Matcher *m = new Matcher(*db);
    Listing *l = new Listing(*db);

    if (uid != 1 && uid != 5)
    {
        res.code = 404;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Oops. That user doesn't exist yet.  We can't find any matches.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    else
    {
        std::string result = m->displayMatches(uid);
        res.code = 200;
        res.write(result);
        res.end();
    }

    delete m;
    delete l;
    return;
}

void RouteController::changeField(const crow::request &req, crow::response &res)
{
    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newField;

    if (params.get("lid") != nullptr) {
        lid = std::stoi(params.get("lid"));
    } else {
        res.code = 400; 
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X' to update the 'field' parameter.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newField") != nullptr) {
        newField = params.get("newField");
    } else {
        res.code = 400; 
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new field with 'newField=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changeField(lid, newField);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changePosition(const crow::request &req, crow::response &res)
{
    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newPosition;

    if (params.get("lid") != nullptr) {
        lid = std::stoi(params.get("lid"));
    } else {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X' to update the 'position' parameter.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newPosition") != nullptr) {
        newPosition = params.get("newPosition");
    } else {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new position with 'newPosition=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changePosition(lid, newPosition);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changeJobDescription(const crow::request &req, crow::response &res)
{
    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newDescription;

    if (params.get("lid") != nullptr) {
        lid = std::stoi(params.get("lid"));
    } else {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X' to update the 'position' parameter.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newDescription") != nullptr) {
        newDescription = params.get("newDescription");
    } else {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new job description with 'newDescription=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changeJobDescription(lid, newDescription);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::dbtest(const crow::request &req, crow::response &res)
{
    /* test query user id value */
    int uid = 1;

    /* example queries:
     * These return an array of vectors with the values for each column requested,
     * e.g., req3[0] = vector of dimension ids in the results (vector<string>)
     *       req3[1] = vector of dimension names in the results (vector<string>)
     *       req3[2] = vector of dimenion weights in the results (vector<string>) */

    /* SELECT uname,email FROM User WHERE id = 1 */
    int resCount = 0;
    std::vector<std::vector<std::string>> req1 = db->query("User", "uname,email", "id", "eq", std::to_string(uid), true, resCount);

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,weight_mod FROM Has_Augment WHERE id = 1 */
    std::vector<std::vector<std::string>> req2 =
        db->query("Has_Augment", "dim_id,weight_mod", "id", "eq", std::to_string(uid), true, resCount);

    std::cout << std::endl
              << "-----------------------------------------" << std::endl
              << std::endl;

    /* SELECT dim_id,name,def_weight FROM Dimension */
    std::vector<std::vector<std::string>> req3 = db->query("Dimension", "dim_id,name,def_weight", "", "", "", true, resCount);

    /* examples of INSERT request: these have already been inserted, so will return some errors from the DB *

    INSERT into User (uname, email) VALUES (TestUser, blah@blah.com) 
            string req4 = db->insert("User", "{\"uname\": \"TestUser\", \"email\": \"blah@blah.com\"}");
            std::cout << req4 << std::endl << std::endl;

    INSERT INTO Skill (name, category) VALUES (drawing, art)
            string req5 = db->insert("Skill", "{\"name\": \"drawing\", \"category\": \"art\"}");
            std::cout << req5 << std::endl << std::endl;

    can insert using string literal: R"( data here, no escapes required )" 
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

        creates new job listing
            string req6 = db->insert("Listing", insert_data);
            std::cout << req6 << std::endl << std::endl; */

    Listing *l = new Listing(*db);
    l->getListing(1);

    res.code= 200;
    res.end();
}
void RouteController::makeUser(const crow::request &req, crow::response &res) {
    try {
        // Parse the JSON body
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue error;
            error["status"] = "error";
            error["message"] = "Invalid JSON.";
            res.code = 400;
            res.write(error.dump());
            res.end();
            return;
        }

        // Extract name and email
        if (!body.has("name") || !body.has("email")) {
            crow::json::wvalue error;
            error["status"] = "error";
            error["message"] = "Missing 'name' or 'email' fields.";
            res.code = 400;
            res.write(error.dump());
            res.end();
            return;
        }

        std::string name = body["name"].s();
        std::string email = body["email"].s();

        // Create and save the user
        User user(name, email);
        std::string save_result = user.save(*db);
        std::cout << save_result << std::endl;

        // Extract augmentations if provided
        std::vector<AugmentInput> augments;
        //Assumes that "augments" come in a list format
        if (body.has("augments")) {
            for (const auto& item : body["augments"]) {
                if (!item.has("dim_id") || !item.has("importance")) {
                    std::cerr << "Invalid augmentation entry. Skipping." << std::endl;
                    continue;  // Skip invalid entries
                }
                // AugmentInput ai;
                AugmentInput ai(*db);
                try {
                    ai.dim_id = std::stoi(item["dim_id"].s());  // Convert to integer
                }
                catch (...) {
                    std::cerr << "Invalid dim_id format. Skipping." << std::endl;
                    continue;  // Skip if dim_id is not an integer
                }
                ai.importance = item["importance"].s();
                augments.emplace_back(ai);
            }
        }

        // Process augmentations
        if (!augments.empty()) {
            // std::string augment_result = processAugments(*db, user.id, augments);
            std::string augment_result = processAugments(user.id, augments);
            std::cout << augment_result << std::endl;
        }

        // Prepare the response
        crow::json::wvalue response;
        response["status"] = "success";
        response["user_id"] = user.id;
        res.code = 201;
        res.write(response.dump());
        res.end();
    }
    catch (const std::exception& e) {
        crow::json::wvalue error;
        error["status"] = "error";
        error["message"] = e.what();
        res.code = 500;
        res.write(error.dump());
        res.end();
    }
}

void RouteController::initRoutes(crow::App<> &app)
{
    CROW_ROUTE(app, "/")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { index(res); });

    CROW_ROUTE(app, "/signup")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response&res)
                                        { signUp(req, res); });

    CROW_ROUTE(app, "/dbtest")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { dbtest(req, res); });

    CROW_ROUTE(app, "/getMatches")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { getMatches(req, res); });

    CROW_ROUTE(app, "/listing/changeField")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { changeField(req, res); });

    CROW_ROUTE(app, "/listing/changePosition")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { changePosition(req, res); });

    CROW_ROUTE(app, "/listing/changeJobDescription")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { changeJobDescription(req, res); });

    CROW_ROUTE(app, "/makeUser")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response&res)
                                        { makeUser(req, res); });
}
