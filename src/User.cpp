// Copyright 2024 LionHackerz
/*  Kelvin Kim sk4802
    Let's save a user to the DB! :D */
#include "User.h"
#include <iostream>
#include <stdexcept>

User::User(const std::string& rname, const std::string& name, const std::string& email)
    : real_name(rname), name(name), email(email), id(-1) {}

std::string User::escapeJson(const std::string& input) {
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

std::string User::save(Database& db) {
    // Construct JSON data for insertion
    std::string data = "{\"real_name\": \"" + escapeJson(real_name) + "\", \"uname\": \"" + escapeJson(name) + "\", \"email\": \"" + escapeJson(email) + "\"}";

    // Insert into 'users' table
    std::string response = db.insert("User", data);
    std::cout << "Insert Response: " << response << std::endl;

    // Parse the response to retrieve the user ID
    // Assuming response format: [{"id":2, "name":"Kelvin Kim", "email":"sk4802™columbia.edu"}]
    size_t id_pos = response.find("\"id\":");
    if (id_pos == std::string::npos) {
        throw std::runtime_error("Failed to retrieve user ID from insert response.");
    }

    size_t start = id_pos + 5;  // Move past '"id":'
    size_t end = response.find_first_of(",}]", start);
    if (end == std::string::npos) {
        throw std::runtime_error("Malformed insert response.");
    }

    std::string id_str = response.substr(start, end - start);
    try {
        id = std::stoi(id_str);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Invalid user ID format in insert response.");
    }

    return "User created with ID: " + std::to_string(id);
}