// Copyright 2024 LionHackerz

#include "Auth.h"
#include "Database.h"
#include <crow.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <boost/beast/core/detail/base64.hpp>


Auth::Auth(Database &db)
{
	this->db = &db;
}

int Auth::createAPIUser(std::string email, std::string password) {
        
        // invalid enail or password
        if (email.empty() || password.empty()) {
                std::cout << "Error: Email or password is empty" << std::endl;
                return -1;
        }
        // check email doesn't already exist
        int resCount = 0;
        std::vector<std::vector<std::string>> queryRes = db->query("API_User", "", "email", "eq", email, false, resCount);
        if (resCount > 0) {
                int uid = std::stoi(queryRes[0][0]);
                std::cout << "Email already exists" << std::endl;
                return uid;
        }

        std::string data = "{\"email\": \"" + email + "\", \"password\": \"" + password + "\"}";
        std::string res = db->insert("API_User", data);
        size_t id_pos = res.find("Error:");
        if (id_pos != std::string::npos) {
                std::cout << res << std::endl;
                return -1;
        }
        std::cout << "insertRes: " << res << std::endl;

        queryRes = db->query("API_User", "id", "email", "eq", email, false, resCount);
        if (resCount == 0) {
                std::cout << "Error inserting to API_User" << std::endl;
                return -1;
        }

        // get user id
        int uid = std::stoi(queryRes[0][0]);
 
        return uid;
}

std::string Auth::generateRandomHex(int length) {
        unsigned char buffer[length];
        
        // Generate random bytes
        if (RAND_bytes(buffer, sizeof(buffer)) != 1) {
                throw std::runtime_error("Failed to generate random bytes");
        }
        
        // Convert bytes to hex string
        std::ostringstream hexStream;
        for (unsigned char byte : buffer) {
                hexStream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
        }
        
        return hexStream.str();
}

std::pair<std::string, std::string> Auth::decodeBasicAuth(const std::string& auth) {
        if (auth.substr(0, 6) != "Basic ") {
                std::cout << "Invalid Authorization header" << std::endl;
                return {"",""};
                // throw std::invalid_argument("Invalid Authorization header");
        }

        std::string encoded_credentials = auth.substr(6);
        std::string decoded_credentials = crow::utility::base64decode(encoded_credentials, encoded_credentials.size());

        size_t delimiter_pos = decoded_credentials.find(':');
        if (delimiter_pos == std::string::npos) {
                std::cout << "Invalid credentials format" << std::endl;
                return {"",""};
                // throw std::invalid_argument("Invalid credentials format");
        }

        std::string username = decoded_credentials.substr(0, delimiter_pos);
        std::string password = decoded_credentials.substr(delimiter_pos + 1);

        return {username, password};
}

std::string Auth::genAPIKey(std::string role, int uid) {

        std::cout << "in Auth::genAPIKey" << std::endl;

        if (role != "admin" && role != "matching_platform") {
                std::cout << "Error: Roles other than admin and matching_platform are not yet implemented" << std::endl;
                return "Error: Roles other than admin and matching_platform are not yet implemented";
        }

        int resCount = 0;
        std::vector<std::vector<std::string>> queryRes = db->query("API_User", "", "id", "eq", std::to_string(uid), false, resCount);
        if (resCount == 0) {
                std::cout << "Error: API user with uid does not exist" << std::endl;
                return "Error: API user with uid does not exist, please create a user first";
        }

        std::string apikey = generateRandomHex(32);
        std::string data = "{\"key\": \"" + apikey + "\", \"role\": \"" + role + "\", \"uid\": \"" + std::to_string(uid) +"\"}";

        std::string insertRes = db->insert("Authentication",data);
        std::cout << "insertRes: " << insertRes << std::endl;

        if (insertRes.find("Error:") != std::string::npos) {
                std::cout << "Error inserting to Authentication" << std::endl;
                return "Error inserting to Authentication";
        }

        return apikey;

}

int Auth::getAid(std::string apiKey) {
        if (apiKey.empty()) {
                return -1; 
        }
        int resCount = 0;

        std::vector<std::vector<std::string>> queryRes = db->query("Authentication", "aid,key", "key", "eq", apiKey, false, resCount);
        if (resCount == 0) {
                std::cout << "Error: API key does not exist, please check the input" << std::endl;
                return -1;
        }
        int aid = std::stoi(queryRes[0][0]);

        return aid;
}

            
std::string Auth::getRole(int aid) {
        int resCount = 0;
        std::vector<std::vector<std::string>> queryRes = db->query("Authentication", "role", "aid", "eq", std::to_string(aid), false, resCount);
        if (resCount == 0) {
                std::cout << "Error: aid does not exist, please check the input" << std::endl;
                return "Error: aid does not exist, please check the input";
        }
        std::string role = queryRes[0][0];

        // remove leading and trailing quotation marks
        if (!role.empty() && role.front() == '\"') {
                role.erase(0, 1);
        }
        if (!role.empty() && role.back() == '\"') {
                role.erase(role.size() - 1);
        }
        // remove backslashes
        size_t pos = 0;
        while ((pos = role.find("\\\"", pos)) != std::string::npos) {
                role.replace(pos, 2, "\"");
                pos += 1; // Move past the replaced character
        }
        return role;
}