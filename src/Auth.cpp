// Copyright 2024 LionHackerz

#include "Auth.h"
#include "Database.h"
#include <string>

Auth::Auth(Database &db)
{
	this->db = &db;
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
                hexStream << std::setw(2) << std::setfill('0') << std::hex << (int)byte;
        }
        
        return hexStream.str();
}

std::string Auth::escapeJson(const std::string& input) {
    std::string output;
    for (char c : input) {
        if (c == '\"') {
            output += "\\\"";
        } else if (c == '\\') {
            output += "\\\\";
        } else {
            output += c;
        }
    }
    return output;
}

std::string Auth::signUp(std::string role) {

        std::cout << "in Auth::signUp" << std::endl;

        if (role != "admin") {
                std::cout << "Error: Roles other than admin are not yet implemented" << std::endl;
                return "";
        }

        // if role is admin
        std::string apikey = generateRandomHex(32);
        std::string data = "{\"apikey\": \"" + escapeJson(apikey) + "\", \"permission\": \"" + escapeJson(role) + "\"}";
        
        std::string insertRes = db->insert("Authentication",data);
        std::cout << "insertRes: " << insertRes << std::endl;

        return apikey;

}