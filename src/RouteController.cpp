// Copyright 2024 LionHackerz

// to have returned wvalue key value pairs be sorted (using std::map)  https://crowcpp.org/master/guides/json/
#define CROW_JSON_USE_MAP

#include "RouteController.h"
#include "Database.h"
#include "Matcher.h"
#include "Listing.h"
#include "User.h"
#include "Augment.h"
#include "Auth.h"
#include "Dimension.h"
#include "Employer.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <regex>
#include <exception>
#include <iostream>
#include <typeinfo>

void RouteController::index(crow::response &res)
{
    res.write("Welcome, in order to make an API call direct your browser or Postman to an endpoint "
              "\n\n This can be done using the following format: \n\n http://127.0.0.1:18080/endpoint?arg=value");
    res.end();
}

void RouteController::signUp(const crow::request &req, crow::response &res)
{

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    std::string role;
    if (params.get("role") != nullptr)
    {
        role = params.get("role");
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a role in the 'role' parameter.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    const auto &auth_header = req.get_header_value("Authorization");
    if (auth_header.empty())
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "No Authorization header found, it is required to proceed";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Auth *a = new Auth(*db);
    auto [username, password] = a->decodeBasicAuth(auth_header);
    if (username == "" || password == "")
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Invalid credentials";
        res.write(jsonRes.dump());
        res.end();
        delete a;
        return;
    }

    // if username email not in API_User table yet, add it and get the created user id
    int uid = a->createAPIUser(username, password);
    if (uid == -1)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Error creating or looking up API user";
        res.write(jsonRes.dump());
        res.end();
        delete a;
        return;
    }

    if (role == "admin")
    {
        std::cout << "Username: " << username << ", Password: " << password << std::endl;
        // check in database if this is admin, if so, issue admin api key
        int resCount = 0;
        std::vector<std::vector<std::string>> queryRes = 
            db->query("Admin", "*", "username", "eq", username, "password", "eq", password, 
                false, resCount);

        if (resCount == 0)
        {
            std::cout << "test1" << std::endl;

            res.code = 400;
            jsonRes["error"]["code"] = res.code;
            jsonRes["error"]["message"] = "Wrong username or password";
            res.write(jsonRes.dump());
            res.end();
            delete a;
            return;
        }
        // gen api key
        std::string retStr = a->genAPIKey(role, uid);
        size_t id_pos = retStr.find("Error:");

        if (id_pos != std::string::npos)
        { // error
            std::cout << retStr << std::endl;
            res.code = 500;
            jsonRes["error"]["code"] = res.code;
            jsonRes["error"]["message"] = "Error generating API key";
            res.write(jsonRes.dump());
            res.end();
            delete a;
            return;
        }

        res.code = 201;
        jsonRes["data"]["apikey"] = retStr;
        res.write(jsonRes.dump());
        res.end();
        delete a;
        return;
    }
    else if (role == "matching_platform")
    {

        // gen api key
        std::string retStr = a->genAPIKey(role, uid);

        size_t id_pos = retStr.find("Error:");
        if (id_pos != std::string::npos)
        { // error
            std::cout << retStr << std::endl;
            res.code = 500;
            jsonRes["error"]["code"] = res.code;
            jsonRes["error"]["message"] = "Error generating API key";
            res.write(jsonRes.dump());
            res.end();
            delete a;
            return;
        }

        res.code = 201;
        jsonRes["data"]["apikey"] = retStr;
        res.write(jsonRes.dump());
        res.end();
        delete a;
        return;
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Other roles are not yet implemented";
        res.write(jsonRes.dump());
        res.end();
        delete a;
        return;
    }
}

bool RouteController::checkAuthHeaders(const crow::request &req, crow::response &res)
{
    const auto &auth_header = req.get_header_value("Authorization");
    std::cout << "auth_header: " << auth_header << std::endl;
    if (auth_header.empty())
    {
        crow::json::wvalue error;
        res.code = 400;
        error["error"]["code"] = res.code;
        error["error"]["message"] = "No Authorization header found, it is required to proceed";
        res.write(error.dump());
        res.end();
        return false;
    }
    Auth *a = new Auth(*db);
    auto [username, password] = a->decodeBasicAuth(auth_header);
    if (username == "")
    {
        crow::json::wvalue error;
        res.code = 400;
        error["error"]["code"] = res.code;
        error["error"]["message"] = "No API key found in header, please put API key in Basic Auth username field and leave password blank";
        res.write(error.dump());
        res.end();
        delete a;
        return false;
    }

    // get aid from api key, check it belongs to an admin or matching platform
    std::string apiKey = username;
    int aid = a->getAid(apiKey);
    if (aid == -1)
    {
        crow::json::wvalue error;
        res.code = 400;
        error["error"]["code"] = res.code;
        error["error"]["message"] = "Invalid API key";
        res.write(error.dump());
        res.end();
        delete a;
        return false;
    }

    std::string role = a->getRole(aid);
    if (role != "admin" && role != "matching_platform")
    {
        crow::json::wvalue error;
        res.code = 400;
        error["error"]["code"] = res.code;
        error["error"]["message"] = "You must be an admin or matching platform to proceed";
        res.write(error.dump());
        res.end();
        delete a;
        return false;
    }
    delete a;
    return true;
}

void RouteController::setDatabase(Database *db)
{
    this->db = db;
}

/* MATCH ROUTE */

void RouteController::getMatches(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    res.add_header("Content-Type", "application/json");

    auto params = crow::query_string(req.url_params);

    crow::json::wvalue jsonRes;
    int resCount = 0;
    int uid;
    try
    {
        uid = std::stoi(params.get("uid"));
    }
    catch (const std::exception &e)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Invalid user ID provided.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    // Check for the 'uid' parameter
    const char *uid_str = params.get("uid");
    if (params.get("uid") == NULL)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a user ID with '?uid=X' to retrieve job matches.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    int resCount = 0;
    
    /* check if user ID exists */
    std::vector<std::vector<std::string>> userExists = 
        db->query("User", "", "id", "eq", std::to_string(uid), false, resCount);
    if (userExists.empty())
    {
        std::cerr << "Invalid UID: " << uid << std::endl;
        res.code = 404;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "Oops. That user doesn't exist yet. We can't find any matches.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    resCount = 0;

    /* check if user has dimensions to compare on */
    std::vector<std::vector<std::string>> hasDim = 
        db->query("Has_Dimensions", "", "id", "eq", std::to_string(uid), false, resCount);
    if (hasDim.empty())
    {
        std::cerr << "User ID: " << uid << std::endl;
        res.code = 404;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "That user doesn't have any dimensions stored for matching.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    std::cout << "Valid UID: " << uid << std::endl;

    auto res_ptr = std::make_shared<crow::response>(std::move(res));

    /* check redis cache for match */
    auto the_results = redis_client.get("matches:" + std::to_string(uid));
    cpp_redis::reply result = the_results.get();

    if (result.is_string() && !result.as_string().empty())
    {
        std::cout << "cache found" << std::endl;
        std::string the_result = result.as_string();

        the_result = std::regex_replace(the_result, std::regex("\\\\\\\\"), "\\");
        the_result = std::regex_replace(the_result, std::regex("\\\""), "\"");
        the_result = std::regex_replace(the_result, std::regex("\\\\\""), "");
        jsonRes["results"] = the_result;

        /* write JSONified string to client */
        res.write(jsonRes.dump());

        /* add CORS headers */
       res.add_header("Access-Control-Allow-Origin", "*"); 
       res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS"); 
       res.add_header("Access-Control-Allow-Headers", "Content-Type"); 

        res.end();
        return;
    }

    // No cached result, queue the task for processing
    std::cout << "No cache found for UID " << uid << ". Adding task to queue." << std::endl;

    // Push the task to the Redis task queue
    redis_client.lpush("task_queue", {std::to_string(uid)}, [](cpp_redis::reply &lpush_reply)
                       {
            if (lpush_reply.is_error())
            {
                std::cerr << "Error adding to Redis queue: " << lpush_reply.error() << std::endl;
             }
            else
            {
                std::cout << "Successfully added task to queue." << std::endl;
            } });

    redis_client.commit(); /* commit task push */

    res.code = 202;
    jsonRes["success"]["code"] = res.code;
    jsonRes["success"]["message"] = "Task was added to the job queue. Check back soon for results.";
    res.write(jsonRes.dump());

    /* add CORS headers */
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");

    res.end();

    redis_client.commit();
}

/* LISTING ROUTES */

void RouteController::createListing(const crow::request &req, crow::response &res)
{
    crow::json::wvalue jsonRes;
    auto json = crow::json::load(req.body);
    if (!json) {
        res.code = 400;
        res.write("Invalid JSON");
        return;
    }

     std::map<std::string, std::string> basicInfo;
    basicInfo["cname"] = json["basicInfo"]["cname"].s(); 
    basicInfo["csize"] = json["basicInfo"]["csize"].s();
    basicInfo["field"] = json["basicInfo"]["field"].s();
    basicInfo["position"] = json["basicInfo"]["position"].s();    
    basicInfo["job_description"] = json["basicInfo"]["job_description"].s();
    basicInfo["location"] = json["basicInfo"]["location"].s();


    std::map<std::string, std::string> skillsPersonality;
    skillsPersonality["skill1_req"] = json["skillsPersonality"]["skill1_req"].s();
    skillsPersonality["skill2_req"] = json["skillsPersonality"]["skill2_req"].s();    
    skillsPersonality["skill3_req"] = json["skillsPersonality"]["skill2_req"].s();    
    skillsPersonality["skill4_req"] = json["skillsPersonality"]["skill2_req"].s();    
    skillsPersonality["skill5_req"] = json["skillsPersonality"]["skill2_req"].s();

    skillsPersonality["personality_types"] = json["skillsPersonality"]["personality_types"].s();    

    int64_t pay = json["pay"].i();  

    std::map<std::string, bool> boolFields;
    boolFields["job_flexibility"] = json["boolFields"]["job_flexibility"].b();
    boolFields["remote_available"] = json["boolFields"]["remote_available"].b();
    boolFields["diverse_workforce"] = json["boolFields"]["diverse_workforce"].b();    
    boolFields["mixed_gender"] = json["boolFields"]["mixed_gender"].b();
    boolFields["modern_building"] = json["boolFields"]["modern_building"].b();

    Listing *l = new Listing(*db);

    if (l->insertListing(basicInfo, skillsPersonality, pay, boolFields)) {
        jsonRes["error"]["message"] = "Error creating listing.";
        res.code = 400;
        res.write(jsonRes.dump());
        res.end();
        delete l;
        return;
    }

    res.code = 200;
    jsonRes["success"]["message"] = "Listing successfully created.";
    res.write(jsonRes.dump());
    res.end();
    delete l;
    return;

}

void RouteController::changeField(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newField;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newField") != nullptr)
    {
        newField = params.get("newField");
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new field with 'newField=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changeField(lid, newField, res.code);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changePosition(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newPosition;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newPosition") != nullptr)
    {
        newPosition = params.get("newPosition");
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new position with 'newPosition=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changePosition(lid, newPosition, res.code);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changeJobDescription(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newDescription;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newDescription") != nullptr)
    {
        newDescription = params.get("newDescription");
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new job description with 'newDescription=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changeJobDescription(lid, newDescription, res.code);
    res.code = 200;
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::generateAIJobDescription(const crow::request &req, crow::response &res) 
{

    crow::json::wvalue jsonRes;

    if (this->db->getAIkey() == "")
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must have an Open AI API key set as an environmental variable to use AI.  Please set this and try again.";
    }  
    
    auto json = crow::json::load(req.body);
    if (!json) {
        res.code = 400;
        res.write("Invalid JSON");
        return;
    }

    std::string title = json["job_title"].s();
    std::string field = json["job_field"].s();
    std::string skill1 = json["skill1"].s();
    std::string skill2 = json["skill2"].s();
    std::string skill3 = json["skill3"].s();
    std::string skill4 = json["skill4"].s();
    std::string skill5 = json["skill5"].s();
    std::string pay = std::to_string(json["pay"].i());
    std::string location = json["loc"].s();

    Listing *l = new Listing(*db);

    std::string description = l->generateAIJobDescription(title, field, skill1,
        skill2, skill3, skill4, skill5, pay, location);
    res.code = 200;
    jsonRes["description"] = description;

    res.write(jsonRes.dump());
    res.end();
    delete l;
    return;
}


void RouteController::changeFlex(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    int resCode = 0;

    std::string result = l->changeFlex(lid, resCode);
    if (resCode == 404)
    {
        res.code = 404;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "The listing ID you provided does not exist in the database.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    jsonRes["success"] = true;
    jsonRes["code"] = 200;
    jsonRes["message"] = "Successfully changed the value of 'job_flexibility' for the given listing.";
    jsonRes["data"] = result;
    res.code = 200;
    res.write(jsonRes.dump());
    res.end();

    delete l;
    return;
}

void RouteController::deleteListing(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    Listing *l = new Listing(*db);

    std::string result = l->deleteListing(lid, res.code);
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changePay(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    int newPay = 0;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newPay") != nullptr)
    {
        newPay = std::stoi(params.get("newPay"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new pay with 'newPay=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changePay(lid, newPay, res.code);

    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::changeSkillRequirements(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    auto body = crow::json::load(req.body);

    if (!body)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new skill requirements in the body of the request.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    else
    {
        if (body.has("skills"))
        {
            std::vector<SkillInput> newSkills;
            std::string skillError = parseSkills(body["skills"], newSkills);
            if (!skillError.empty())
            {
                returnError(res, 400, skillError);
                return;
            }

            Listing *l = new Listing(*db);

            std::string result = l->changeSkillRequirements(lid, newSkills, res.code);

            jsonRes["skills"] = result;
            res.code = 200;
            res.write(jsonRes.dump());
            res.end();

            delete l;
            return;
        }
    }
}

void RouteController::changePersonalityType(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;
    std::string newPersonalityType;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }
    if (params.get("newPersonalityType") != nullptr)
    {
        newPersonalityType = params.get("newPersonalityType");
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a value for the new personality type with 'newPersonalityType=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string result = l->changePersonalityType(lid, newPersonalityType, res.code);
    res.write(result);
    res.end();

    delete l;
    return;
}

void RouteController::generateAIListing(const crow::request &req, crow::response &res)
{

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    if (this->db->getAIkey() == "")
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must have an Open AI API key set as an environmental variable to use AI.  Please set this and try again.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    if (params.get("n") == nullptr)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify how many listings to generate with '?n=X'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    std::string n = params.get("n");
    if (!std::all_of(n.begin(), n.end(), ::isdigit))
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "The number n specified to generate must be a numeric value.'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    if (stoi(n) > 20 || stoi(n) < 1)
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "The number n to generate must be between 1-20 listings.'";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    std::string newListings = l->generateAIListing(n);
    res.code = 200;
    jsonRes["success"]["code"] = res.code;
    res.write(newListings);
    res.end();

    delete l;
    return;
}

void RouteController::changeModernWorkspace(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }
    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    int lid = 0;

    if (params.get("lid") != nullptr)
    {
        lid = std::stoi(params.get("lid"));
    }
    else
    {
        res.code = 400;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "You must specify a listing ID with '?lid=X'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    Listing *l = new Listing(*db);

    int resCode = 0;

    std::string result = l->changeModernWorkspace(lid, resCode);
    if (resCode == 404)
    {
        res.code = 404;
        jsonRes["error"]["code"] = res.code;
        jsonRes["error"]["message"] = "The listing ID you provided does not exist in the database.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    jsonRes["success"] = true;
    jsonRes["code"] = 200;
    jsonRes["message"] = "Successfully changed the value of 'modern_building' for the given listing.";
    jsonRes["data"] = result;
    res.code = 200;
    res.write(jsonRes.dump());
    res.end();

    delete l;
    return;
}

void RouteController::dbtest(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return; // response has already been written in checkApiHeaders
    }

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
    l->getListing(1, false);

    res.code = 200;
    res.end();
}

/* USER ROUTE */

void RouteController::makeUser(const crow::request &req, crow::response &res)
{
    try
    {
        if (!checkAuthHeaders(req, res))
        {
            return; // response has already been written in checkApiHeaders
        }

        // Parse the JSON body
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["status"] = "error";
            error["message"] = "Invalid JSON.";
            res.code = 400;
            res.write(error.dump());
            res.end();
            return;
        }

        // Extract name and email
        if (!body.has("name") || !body.has("real_name") || !body.has("email") || !body.has("dimensions"))
        {
            crow::json::wvalue error;
            error["status"] = "error";
            error["message"] = "Missing 'name' or 'email' or 'dimension' fields.";
            res.code = 400;
            res.write(error.dump());
            res.end();
            return;
        }

        std::string name = body["name"].s();
        std::string real_name = body["real_name"].s();
        std::string email = body["email"].s();

        // Extract dimensions
        auto dimensions_json = body["dimensions"];
        if (!dimensions_json)
        {
            returnError(res, 400, "Invalid 'dimensions' format.");
            return;
        }

        // Validate and extract dimension fields
        Dimension dimension;
        std::string dimensionError = dimension.fromJson(dimensions_json);
        if (!dimensionError.empty())
        {
            returnError(res, 400, dimensionError);
            return;
        }

        // Create and save the user
        Database *db = new Database();
        User user(real_name, name, email);
        std::string save_result = user.save(*db);
        std::cout << save_result << std::endl;

        // Save dimensions with the user's ID
        dimension.user_id = user.id;
        std::string dimension_result = dimension.save(*db);
        std::cout << dimension_result << std::endl;

        // Process skills if provided
        if (body.has("skills"))
        {
            std::vector<SkillInput> skills;
            std::string skillError = parseSkills(body["skills"], skills);
            if (!skillError.empty())
            {
                returnError(res, 400, skillError);
                return;
            }

            std::string skill_result = processSkills(*db, user.id, skills);
            std::cout << skill_result << std::endl;
        }

        // Process interests if provided
        if (body.has("interests"))
        {
            std::cout << body["interests"] << std::endl;
            std::vector<InterestInput> interests;
            std::string interestError = parseInterests(body["interests"], interests);
            if (!interestError.empty())
            {
                returnError(res, 400, interestError);
                return;
            }

            std::string interest_result = processInterests(*db, user.id, interests);
            std::cout << interest_result << std::endl;
        }

        // Extract augmentations if provided
        std::vector<AugmentInput> augments;
        // Assumes that "augments" come in a list format
        if (body.has("augments"))
        {
            for (const auto &item : body["augments"])
            {
                if (!item.has("dim_id") || !item.has("importance"))
                {
                    std::cerr << "Invalid augmentation entry. Skipping." << std::endl;
                    continue; // Skip invalid entries
                }
                AugmentInput ai;
                try
                {
                    ai.dim_id = item["dim_id"].i(); 
                }
                catch (...)
                {
                    std::cerr << "Invalid dim_id format. Skipping." << std::endl;
                    continue; // Skip if dim_id is not an integer
                }
                ai.importance = item["importance"].s();
                augments.emplace_back(ai);
            }
        }

        // Process augmentations
        if (!augments.empty())
        {
            std::string augment_result = processAugments(db, user.id, augments);
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
    catch (const std::exception &e)
    {
        crow::json::wvalue error;
        error["status"] = "error";
        error["message"] = e.what();
        res.code = 500;
        res.write(error.dump());
        res.end();
    }
}

void RouteController::getProfile(const crow::request &req, crow::response &res) {

    crow::json::wvalue jsonRes;

    auto params = crow::query_string(req.url_params);

    int uid = std::stoi(params.get("uid"));    

    int resCount = 0;
    std::vector<std::vector<std::string>> result = 
        db->query("Has_Dimension", "", "id", "eq", std::to_string(uid), false, resCount);
    resCount = 0;

    std::vector<std::vector<std::string>> skills = 
        db->query("Has_Skill", "name", "id", "eq", std::to_string(uid), false, resCount);

    resCount = 0;
    std::vector<std::vector<std::string>> interests = 
        db->query("Has_Interest", "name", "uid", "eq", std::to_string(uid), false, resCount);

    std::string profile = "{";
    
    int count = 1;
    
    for (auto &r : skills) {
        for (auto &c : r) {
            profile += "\"skill" + std::to_string(count) + "\": " + c + ",";
            count++;
        }
    }
    count = 1;
    for (auto &r : interests) {
        for (auto &c : r) {
            profile += "\"interest" + std::to_string(count) + "\": " + c + ",";
            count++;
        }
    }

    profile += "\"location\": " + result[1][0] + ",";
    profile += "\"field\": " + result[2][0] + ",";    
    profile += "\"pay\": " + result[3][0] + ",";
    profile += "\"gender\": " + result[4][0] + ",";
    profile += "\"diversity\": " + result[5][0] + ",";
    profile += "\"mbti\": " + result[6][0] + ",";
    profile += "\"flexibility\": " + result[7][0] + ",";
    profile += "\"remote\": " + result[8][0] + ",";
    profile += "\"workspace\": " + result[9][0] + "";
    profile += "}";

    jsonRes["results"] = profile;
    res.write(jsonRes.dump());
    res.end();

    return;
}


void RouteController::returnError(crow::response &res, int code, const std::string &message)
{
    crow::json::wvalue error;
    error["status"] = "error";
    error["message"] = message;
    res.code = code;
    res.set_header("Content-Type", "application/json");
    res.write(error.dump());
    res.end();
}

std::string RouteController::parseSkills(const crow::json::rvalue &skills_json, std::vector<SkillInput> &skills)
{
    Database db;

    for (const auto &item : skills_json)
    {
        if (!item.has("name"))
        {
            return "Each skill must have a 'name'.";
        }

        SkillInput si;
        si.name = item["name"].s();


        si.rank = std::nullopt;

        skills.emplace_back(si);
    }

    return "";
}

std::string RouteController::parseInterests(const crow::json::rvalue &interests_json, std::vector<InterestInput> &interests)
{
    Database db;

    for (const auto &item : interests_json)
    {
        if (!item.has("name"))
        {
            return "Each interest must have 'name'.";
        }

        InterestInput ii;
        ii.name = item["name"].s();

        interests.emplace_back(ii);
    }

    return "";
}

std::string RouteController::processSkills(Database &db, int user_id, const std::vector<SkillInput> &skills)
{
    for (const auto &skill : skills)
    {
        std::string data = "{";
        data += "\"id\": " + std::to_string(user_id) + ",";
        data += "\"name\": \"" + db.escapeString(skill.name) + "\"";

        // Include 'rank' if provided
        if (skill.rank.has_value())
        {
            data += ",\"rank\": " + std::to_string(skill.rank.value());
        }
        else
        {
            data += ",\"rank\": null"; // Otherwise, just gonna explicitly set 'rank' to null
        }

        data += "}";

        std::string response = db.insert("Has_Skill", data);
        std::cout << "Skill Insert Response: " << response << std::endl;
    }
    return "Skills processed successfully.";
}

std::string RouteController::processInterests(Database &db, int user_id, const std::vector<InterestInput> &interests)
{
    for (const auto &interest : interests)
    {
        std::cout << interest.name << std::endl;
        std::string data = "{";
        data += "\"uid\": " + std::to_string(user_id) + ",";
        data += "\"name\": \"" + db.escapeString(interest.name) + "\"";
        data += "}";

        std::cout << data << std::endl;

        std::string response = db.insert("Has_Interest", data);
        std::cout << "Interest Insert Response: " << response << std::endl;
    }
    return "Interests processed successfully.";
}

void RouteController::handleEmployerBoolResult(crow::response &res, bool result, int resCode, const std::string &successMsg)
{
    crow::json::wvalue jsonRes;
    if (!result)
    {
        res.code = (resCode == 0) ? 400 : resCode; // default to 400 if no code set
        jsonRes["success"] = false;
        jsonRes["error"] = "Operation failed.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    // success
    res.code = 200;
    jsonRes["success"] = true;
    jsonRes["message"] = successMsg;
    res.write(jsonRes.dump());
    res.end();
}
void RouteController::employerChangeField(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int lid = 0;
    std::string newField;
    crow::json::wvalue jsonRes;

    if (params.get("eid") == nullptr || params.get("lid") == nullptr || params.get("newField") == nullptr)
    {
        res.code = 400;
        jsonRes["success"] = false;
        jsonRes["error"] = "You must specify 'eid', 'lid', and 'newField' parameters.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    eid = std::stoi(params.get("eid"));
    lid = std::stoi(params.get("lid"));
    newField = params.get("newField");

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeField(eid, lid, newField, resCode);
    handleEmployerBoolResult(res, result, resCode, "Field changed successfully.");
}

void RouteController::employerChangePosition(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int lid = 0;
    std::string newPosition;
    crow::json::wvalue jsonRes;

    if (params.get("eid") == nullptr || params.get("lid") == nullptr || params.get("newPosition") == nullptr)
    {
        res.code = 400;
        jsonRes["success"] = false;
        jsonRes["error"] = "You must specify 'eid', 'lid', and 'newPosition'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    eid = std::stoi(params.get("eid"));
    lid = std::stoi(params.get("lid"));
    newPosition = params.get("newPosition");

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changePosition(eid, lid, newPosition, resCode);
    handleEmployerBoolResult(res, result, resCode, "Position changed successfully.");
}

void RouteController::employerChangeJobDescription(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int lid = 0;
    std::string newDescription;
    crow::json::wvalue jsonRes;

    if (params.get("eid") == nullptr || params.get("lid") == nullptr || params.get("newDescription") == nullptr)
    {
        res.code = 400;
        jsonRes["success"] = false;
        jsonRes["error"] = "You must specify 'eid', 'lid', and 'newDescription'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    eid = std::stoi(params.get("eid"));
    lid = std::stoi(params.get("lid"));
    newDescription = params.get("newDescription");

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeJobDescription(eid, lid, newDescription, resCode);
    handleEmployerBoolResult(res, result, resCode, "Job description changed successfully.");
}

void RouteController::employerChangeFlex(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int lid = 0;
    crow::json::wvalue jsonRes;

    if (params.get("eid") == nullptr || params.get("lid") == nullptr)
    {
        res.code = 400;
        jsonRes["success"] = false;
        jsonRes["error"] = "You must specify 'eid' and 'lid'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    eid = std::stoi(params.get("eid"));
    lid = std::stoi(params.get("lid"));

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result;

    if (params.get("newFlex") != nullptr)
    {
        bool newFlex = (std::string(params.get("newFlex")) == "true");
        result = employer.changeFlex(eid, lid, newFlex, resCode);
    }
    else
    {
        // Calls the toggle version if no newFlex is provided
        result = employer.changeFlex(eid, lid, resCode);
    }

    handleEmployerBoolResult(res, result, resCode, "Flex changed successfully.");
}

void RouteController::employerChangePay(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int lid = 0;
    int64_t newPay = 0;
    crow::json::wvalue jsonRes;

    if (params.get("eid") == nullptr || params.get("lid") == nullptr || params.get("newPay") == nullptr)
    {
        res.code = 400;
        jsonRes["success"] = false;
        jsonRes["error"] = "You must specify 'eid', 'lid', and 'newPay'.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    eid = std::stoi(params.get("eid"));
    lid = std::stoi(params.get("lid"));
    newPay = std::stoll(params.get("newPay"));

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changePay(eid, lid, newPay, resCode);
    handleEmployerBoolResult(res, result, resCode, "Pay changed successfully.");
}
void RouteController::employerChangeSkillRequirements(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
    {
        return;
    }

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;

    const char *eid_str = params.get("eid");
    if (eid_str == nullptr)
    {
        returnError(res, 400, "Missing 'eid' (Employer ID) parameter.");
        return;
    }

    int eid;
    try
    {
        eid = std::stoi(eid_str);
    }
    catch (const std::exception &e)
    {
        returnError(res, 400, "Invalid 'eid' (Employer ID) parameter.");
        return;
    }

    const char *lid_str = params.get("lid");
    if (lid_str == nullptr)
    {
        returnError(res, 400, "Missing 'lid' (Listing ID) parameter.");
        return;
    }

    int lid;
    try
    {
        lid = std::stoi(lid_str);
    }
    catch (const std::exception &e)
    {
        returnError(res, 400, "Invalid 'lid' (Listing ID) parameter.");
        return;
    }

    // parse the JSON body for 'skills'
    auto body = crow::json::load(req.body);
    if (!body)
    {
        returnError(res, 400, "Invalid JSON body.");
        return;
    }

    if (!body.has("skills"))
    {
        returnError(res, 400, "Missing 'skills' in the request body.");
        return;
    }

    // validate and extract skills
    std::vector<SkillInput> newSkills;
    std::string skillError = parseSkills(body["skills"], newSkills);
    if (!skillError.empty())
    {
        returnError(res, 400, skillError);
        return;
    }

    // Perform the database operation
    Listing listing(*db); // Assuming Listing is required by Employer
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeSkillRequirements(eid, lid, newSkills, resCode);

    handleEmployerBoolResult(res, result, resCode, "Skill requirements updated successfully.");
}

void RouteController::employerChangeGender(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    bool useSetValue = false;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }

    if (params.get("newValue"))
    {
        useSetValue = true;
        newValue = (std::string(params.get("newValue")) == "true");
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = false;
    if (useSetValue)
    {
        result = employer.changeGender(eid, lid, newValue, resCode);
    }
    else
    {
        result = employer.changeGender(eid, lid, resCode);
    }

    handleEmployerBoolResult(res, result, resCode, "Gender field changed successfully.");
}

void RouteController::employerChangeDiversity(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    bool useSetValue = false;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }

    if (params.get("newValue"))
    {
        useSetValue = true;
        newValue = (std::string(params.get("newValue")) == "true");
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result;
    if (useSetValue)
    {
        result = employer.changeDiversity(eid, lid, newValue, resCode);
    }
    else
    {
        result = employer.changeDiversity(eid, lid, resCode);
    }

    handleEmployerBoolResult(res, result, resCode, "Diversity field changed successfully.");
}

void RouteController::employerChangeRemote(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    bool useSetValue = false;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }

    if (params.get("newValue"))
    {
        useSetValue = true;
        newValue = (std::string(params.get("newValue")) == "true");
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result;
    if (useSetValue)
    {
        result = employer.changeRemote(eid, lid, newValue, resCode);
    }
    else
    {
        result = employer.changeRemote(eid, lid, resCode);
    }

    handleEmployerBoolResult(res, result, resCode, "Remote field changed successfully.");
}

void RouteController::employerChangeLocation(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    std::string newLocation;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }
    if (params.get("newLocation"))
        newLocation = params.get("newLocation");
    else
    {
        returnError(res, 400, "No new Location");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeLocation(eid, lid, newLocation, resCode);

    handleEmployerBoolResult(res, result, resCode, "Location changed successfully.");
}

void RouteController::employerChangeMBTI(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    std::string newMBTI;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }
    if (params.get("newMBTI"))
        newMBTI = params.get("newMBTI");
    else
    {
        returnError(res, 400, "No newMBTI");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeMBTI(eid, lid, newMBTI, resCode);

    handleEmployerBoolResult(res, result, resCode, "MBTI changed successfully.");
}

void RouteController::employerChangeModernWorkspace(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    bool useSetValue = false;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }

    if (params.get("newValue"))
    {
        useSetValue = true;
        newValue = (std::string(params.get("newValue")) == "true");
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result;
    if (useSetValue)
    {
        result = employer.changeModernWorkspace(eid, lid, newValue, resCode);
    }
    else
    {
        result = employer.changeModernWorkspace(eid, lid, resCode);
    }
    handleEmployerBoolResult(res, result, resCode, "Modern workspace field changed successfully.");
}

// "All" variants

void RouteController::employerChangeFieldAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    std::string newField;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newField"))
        newField = params.get("newField");
    else
    {
        returnError(res, 400, "No newField");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeFieldAll(eid, newField, resCode);

    handleEmployerBoolResult(res, result, resCode, "Field changed for all listings successfully.");
}

void RouteController::employerChangePositionAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    std::string newPosition;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newPosition"))
        newPosition = params.get("newPosition");
    else
    {
        returnError(res, 400, "No newPosition");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changePositionAll(eid, newPosition, resCode);

    handleEmployerBoolResult(res, result, resCode, "Position changed for all listings successfully.");
}

void RouteController::employerChangeFlexAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    bool newFlex = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newFlex"))
        newFlex = (std::string(params.get("newFlex")) == "true");
    else
    {
        returnError(res, 400, "No newFlex");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeFlexAll(eid, newFlex, resCode);

    handleEmployerBoolResult(res, result, resCode, "Flex changed for all listings successfully.");
}

void RouteController::employerChangeModernWorkspaceAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newValue"))
        newValue = (std::string(params.get("newValue")) == "true");
    else
    {
        returnError(res, 400, "No newValue");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeModernWorkspaceAll(eid, newValue, resCode);

    handleEmployerBoolResult(res, result, resCode, "Modern workspace changed for all listings successfully.");
}

void RouteController::employerChangeGenderAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newValue"))
        newValue = (std::string(params.get("newValue")) == "true");
    else
    {
        returnError(res, 400, "No newValue");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeGenderAll(eid, newValue, resCode);

    handleEmployerBoolResult(res, result, resCode, "Gender field changed for all listings successfully.");
}

void RouteController::employerChangeDiversityAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newValue"))
        newValue = (std::string(params.get("newValue")) == "true");
    else
    {
        returnError(res, 400, "No newValue");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeDiversityAll(eid, newValue, resCode);

    handleEmployerBoolResult(res, result, resCode, "Diversity changed for all listings successfully.");
}

void RouteController::employerChangeRemoteAll(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    bool newValue = false;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("newValue"))
        newValue = (std::string(params.get("newValue")) == "true");
    else
    {
        returnError(res, 400, "No newValue");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.changeRemoteAll(eid, newValue, resCode);

    handleEmployerBoolResult(res, result, resCode, "Remote field changed for all listings successfully.");
}

void RouteController::employerPostListing(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0;
    int64_t pay = 0;
    // debug eid and pay params
    std::cout << "eid: " << params.get("eid") << std::endl;
    std::cout << "pay: " << params.get("pay") << std::endl;

    // print the variable type
    std::cout << typeid(params.get("eid")).name() << std::endl;
    std::cout << typeid(params.get("pay")).name() << std::endl;
    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "Missing 'eid' (Employer ID) parameter.");
        return;
    }

    if (params.get("pay"))
        pay = std::stoll(params.get("pay"));
    else
    {
        returnError(res, 400, "Missing 'pay' parameter.");
        return;
    }

    auto body = crow::json::load(req.body);
    std::cout << "body: " << body << std::endl;
    if (!body)
    {
        returnError(res, 400, "Invalid JSON body.");
        return;
    }

    // Extract 'basicInfo'
    std::map<std::string, std::string> basicInfo;
    if (body.has("basicInfo"))
    {
        for (auto &kv : body["basicInfo"])
        {
            std::cout << "key bi " << kv.key() << " value " << kv.s() << std::endl;
            basicInfo[kv.key()] = kv.s();
        }
    }
    else
    {
        returnError(res, 400, "Missing 'basicInfo' in request body.");
        return;
    }

    // Extract 'skillsPersonality'
    std::map<std::string, std::string> skillsPersonality;
    if (body.has("skillsPersonality"))
    {
        for (auto &kv : body["skillsPersonality"])
        {
            skillsPersonality[kv.key()] = kv.s();
        }
    }
    else
    {
        returnError(res, 400, "Missing 'skillsPersonality' in request body.");
        return;
    }

    // Extract 'boolFields' can't use .s() because it's a bool
    std::map<std::string, bool> boolFields;
    if (body.has("boolFields"))
    {
        for (auto &kv : body["boolFields"])
        {
            boolFields[kv.key()] = kv ? "true" : "false";
        }
    }
    else
    {
        returnError(res, 400, "Missing 'boolFields' in request body.");
        return;
    }

    // Create Employer and Listing objects
    Employer emp(*db, *(new Listing(*db)));
    int resCode = 200;

    // Post the listing
    int lid = emp.postListing(eid, basicInfo, skillsPersonality, pay, boolFields, resCode);
    if (lid == -1)
    {
        returnError(res, resCode, "Failed to post listing.");
        return;
    }

    crow::json::wvalue jsonRes;
    jsonRes["success"] = true;
    jsonRes["lid"] = lid;
    jsonRes["code"] = 200;

    res.code = 200;
    res.write(jsonRes.dump());
    res.end();
}

void RouteController::employerCreateEmployer(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    std::string company_name;
    std::string size;

    if (params.get("company_name"))
        company_name = params.get("company_name");
    else
    {
        returnError(res, 400, "Missing 'company_name' parameter.");
        return;
    }

    if (params.get("size"))
        size = params.get("size");
    else
    {
        returnError(res, 400, "Missing 'size' parameter.");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    int eid = employer.createEmployer(company_name, size, resCode);
    if (eid == -1)
    {
        res.code = (resCode != 0) ? resCode : 400;
        crow::json::wvalue jsonRes;
        jsonRes["success"] = false;
        jsonRes["error"] = "Failed to create employer.";
        res.write(jsonRes.dump());
        res.end();
        return;
    }

    crow::json::wvalue jsonRes;
    jsonRes["success"] = true;
    jsonRes["message"] = "Employer created successfully.";
    jsonRes["eid"] = eid;

    res.code = 200;
    res.write(jsonRes.dump());
    res.end();
}
void RouteController::employerDeleteListing(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    crow::json::wvalue jsonRes;
    int eid = 0;
    int lid = 0;

    const char *eid_str = params.get("eid");
    if (eid_str == nullptr)
    {
        returnError(res, 400, "Missing 'eid' (Employer ID) parameter.");
        return;
    }
    try
    {
        eid = std::stoi(eid_str);
    }
    catch (const std::exception &e)
    {
        returnError(res, 400, "Invalid 'eid' (Employer ID) parameter.");
        return;
    }

    const char *lid_str = params.get("lid");
    if (lid_str == nullptr)
    {
        returnError(res, 400, "Missing 'lid' (Listing ID) parameter.");
        return;
    }
    try
    {
        lid = std::stoi(lid_str);
    }
    catch (const std::exception &e)
    {
        returnError(res, 400, "Invalid 'lid' (Listing ID) parameter.");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);

    int resCode = 0;
    bool result = employer.deleteListing(eid, lid, resCode);

    handleEmployerBoolResult(res, result, resCode, "Listing deleted successfully.");
}

void RouteController::employerChangePersonalityType(const crow::request &req, crow::response &res)
{
    if (!checkAuthHeaders(req, res))
        return;

    auto params = crow::query_string(req.url_params);
    int eid = 0, lid = 0;
    std::string newPersonalityType;

    if (params.get("eid"))
        eid = std::stoi(params.get("eid"));
    else
    {
        returnError(res, 400, "No eid");
        return;
    }
    if (params.get("lid"))
        lid = std::stoi(params.get("lid"));
    else
    {
        returnError(res, 400, "No lid");
        return;
    }
    if (params.get("newPersonalityType"))
        newPersonalityType = params.get("newPersonalityType");
    else
    {
        returnError(res, 400, "No newPersonalityType");
        return;
    }

    Listing listing(*db);
    Employer employer(*db, listing);
    int resCode = 0;
    bool result = employer.changePersonalityType(eid, lid, newPersonalityType, resCode);
    handleEmployerBoolResult(res, result, resCode, "Personality type changed successfully.");
}

void RouteController::initRoutes(crow::App<> &app)
{
    // Root Route
    CROW_ROUTE(app, "/")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { index(res); });

    // SignUp Route
    CROW_ROUTE(app, "/signup")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response &res)
                                         { signUp(req, res); });

    // Database Test Route
    CROW_ROUTE(app, "/dbtest")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
                                        { dbtest(req, res); });

    /* MATCH ROUTES */
    CROW_ROUTE(app, "/getMatches")
        .methods(crow::HTTPMethod::GET)([this](const crow::request &req, crow::response &res)
            { getMatches(req, res); });

    CROW_ROUTE(app, "/progress/<string>")
        .methods(crow::HTTPMethod::GET, crow::HTTPMethod::OPTIONS)(
            [this](const crow::request &req, crow::response &res, const std::string &user_id)
        {
            if (req.method == crow::HTTPMethod::OPTIONS)
            {
                std::cout << "Handling OPTIONS request for /progress" << std::endl;
                res.code = 204;
                res.end();
                return;
            }

            res.code = 200;
            res.write(reply.as_string());
            res.end();
            return; });

    /* LISTING ROUTES */
    CROW_ROUTE(app, "/listing/create")
        .methods("POST"_method, "OPTIONS"_method)([this](const crow::request &req, crow::response &res)
                                          { createListing(req, res); });

    CROW_ROUTE(app, "/listing/changeField")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changeField(req, res); });

    CROW_ROUTE(app, "/listing/changePosition")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changePosition(req, res); });

    CROW_ROUTE(app, "/listing/changeJobDescription")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changeJobDescription(req, res); });

    CROW_ROUTE(app, "/listing/changeFlex")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changeFlex(req, res); });

    CROW_ROUTE(app, "/listing/changeModernWorkspace")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changeModernWorkspace(req, res); });

    CROW_ROUTE(app, "/listing/deleteListing")
        .methods(crow::HTTPMethod::DELETE)([this](const crow::request &req, crow::response &res)
                                           { deleteListing(req, res); });

    CROW_ROUTE(app, "/listing/changePay")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changePay(req, res); });

    CROW_ROUTE(app, "/listing/changeSkillRequirements")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changeSkillRequirements(req, res); });

    CROW_ROUTE(app, "/listing/changePersonalityType")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { changePersonalityType(req, res); });

    CROW_ROUTE(app, "/listing/generateAI")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response &res)
                                         { generateAIListing(req, res); });

    CROW_ROUTE(app, "/listing/generateJobDescription")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response &res)
                                         { generateAIJobDescription(req, res); });

    /* USER ROUTES */
    CROW_ROUTE(app, "/makeUser")
        .methods("POST"_method, "OPTIONS"_method)([this](const crow::request &req, crow::response &res)
                                        { makeUser(req, res); });

    CROW_ROUTE(app, "/getProfile")
        .methods("GET"_method, "OPTIONS"_method)([this](const crow::request &req, crow::response &res)
                                        { getProfile(req, res); }); 

                                         { makeUser(req, res); });

    /* EMPLOYER ROUTES */
    CROW_ROUTE(app, "/employer/changeField")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeField(req, res); });

    CROW_ROUTE(app, "/employer/changePosition")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangePosition(req, res); });

    CROW_ROUTE(app, "/employer/changeJobDescription")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeJobDescription(req, res); });

    CROW_ROUTE(app, "/employer/changeFlex")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeFlex(req, res); });

    CROW_ROUTE(app, "/employer/changeGender")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeGender(req, res); });

    CROW_ROUTE(app, "/employer/changeDiversity")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeDiversity(req, res); });

    CROW_ROUTE(app, "/employer/changeRemote")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeRemote(req, res); });

    CROW_ROUTE(app, "/employer/changeLocation")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeLocation(req, res); });

    CROW_ROUTE(app, "/employer/changeMBTI")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeMBTI(req, res); });

    CROW_ROUTE(app, "/employer/changeModernWorkspace")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeModernWorkspace(req, res); });

    CROW_ROUTE(app, "/employer/changeFieldAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeFieldAll(req, res); });

    CROW_ROUTE(app, "/employer/changePositionAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangePositionAll(req, res); });

    CROW_ROUTE(app, "/employer/changeFlexAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeFlexAll(req, res); });

    CROW_ROUTE(app, "/employer/changeModernWorkspaceAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeModernWorkspaceAll(req, res); });

    CROW_ROUTE(app, "/employer/changeGenderAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeGenderAll(req, res); });

    CROW_ROUTE(app, "/employer/changeDiversityAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeDiversityAll(req, res); });

    CROW_ROUTE(app, "/employer/changeRemoteAll")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeRemoteAll(req, res); });

    CROW_ROUTE(app, "/employer/postListing")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response &res)
                                         { employerPostListing(req, res); });

    CROW_ROUTE(app, "/employer/createEmployer")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req, crow::response &res)
                                         { employerCreateEmployer(req, res); });
    CROW_ROUTE(app, "/employer/deleteListing")
        .methods(crow::HTTPMethod::DELETE)([this](const crow::request &req, crow::response &res)
                                           { employerDeleteListing(req, res); });

    CROW_ROUTE(app, "/employer/changePay")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangePay(req, res); });

    CROW_ROUTE(app, "/employer/changeSkillRequirements")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangeSkillRequirements(req, res); });

    CROW_ROUTE(app, "/employer/changePersonalityType")
        .methods(crow::HTTPMethod::PATCH)([this](const crow::request &req, crow::response &res)
                                          { employerChangePersonalityType(req, res); });
}