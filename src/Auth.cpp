// Copyright 2024 LionHackerz

#include "Auth.h"
#include "Database.h"
#include <string>
#include <crow.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/beast/core/detail/base64.hpp>


Auth::Auth(Database &db)
{
	this->db = &db;
}

// std::string Auth::generateRandomHex(int length) {
//         unsigned char buffer[length];
        
//         // Generate random bytes
//         if (RAND_bytes(buffer, sizeof(buffer)) != 1) {
//                 throw std::runtime_error("Failed to generate random bytes");
//         }
        
//         // Convert bytes to hex string
//         std::ostringstream hexStream;
//         for (unsigned char byte : buffer) {
//                 hexStream << std::setw(2) << std::setfill('0') << std::hex << (int)byte;
//         }
        
//         return hexStream.str();
// }

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

// std::string Auth::genAPIKey(std::string role) {

//         std::cout << "in Auth::genAPIKey" << std::endl;

//         if (role != "admin") {
//                 std::cout << "Error: Roles other than admin are not yet implemented" << std::endl;
//                 return "Error: Roles other than admin are not yet implemented";
//                 // return "";
//         }

//         std::string apikey = generateRandomHex(32);
//         // std::string data = "{\"apikey\": \"" + escapeJson(apikey) + "\", \"permission\": \"" + escapeJson(role) + "\"}";
//         std::string data = "{\"apikey\": \"" + apikey + "\", \"permission\": \"" + role + "\"}";

//         std::string insertRes = db->insert("Authentication",data);
//         std::cout << "insertRes: " << insertRes << std::endl;

//         return apikey;

// }