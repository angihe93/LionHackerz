/*  Kelvin Kim sk4802
    Let's save a user to the DB! :D */
// User.cpp
#include "User.h"
#include <iostream>
#include <stdexcept>
#include <cctype> // For std::isspace and std::isdigit
#include <cstdio> // For snprintf

User::User(const std::string &name, const std::string &email)
    : name(name), email(email), id(-1) {}

std::string User::escapeJson(const std::string &input)
{
    std::string output;
    for (char c : input)
    {
        switch (c)
        {
        case '\"':
            output += "\\\"";
            break;
        case '\\':
            output += "\\\\";
            break;
        case '\b':
            output += "\\b";
            break;
        case '\f':
            output += "\\f";
            break;
        case '\n':
            output += "\\n";
            break;
        case '\r':
            output += "\\r";
            break;
        case '\t':
            output += "\\t";
            break;
        default:
            // Control characters (0x00 - 0x1F) must be escaped
            if (static_cast<unsigned char>(c) <= 0x1F)
            {
                char buffer[7];
                snprintf(buffer, sizeof(buffer), "\\u%04x", c);
                output += buffer;
            }
            else
            {
                output += c;
            }
        }
    }
    return output;
}

std::string User::save(Database &db)
{
    try
    {
        // Construct JSON data for insertion
        std::string data = "{\"uname\": \"" + escapeJson(name) + "\", \"email\": \"" + escapeJson(email) + "\"}";
        std::cout << "Constructed JSON Data: " << data << std::endl;

        // Insert into 'users' table
        std::string response = db.insert("User", data);
        std::cout << "Insert Response: " << response << std::endl;

        // Check if response is empty
        if (response.empty())
        {
            throw std::runtime_error("Empty response from database insert.");
        }

        // Parse the response to retrieve the user ID
        size_t id_pos = response.find("\"id\":");
        if (id_pos == std::string::npos)
        {
            throw std::runtime_error("Failed to retrieve user ID from insert response.");
        }

        size_t start = id_pos + 5; // Move past '"id":'
        if (start >= response.size())
        {
            throw std::runtime_error("Unexpected end of response after '\"id\":'.");
        }

        // Skip any whitespace after the colon
        while (start < response.size() && std::isspace(static_cast<unsigned char>(response[start])))
        {
            start++;
        }

        // Find the end of the id number
        size_t end = start;
        while (end < response.size() && std::isdigit(static_cast<unsigned char>(response[end])))
        {
            end++;
        }

        // Extract and validate the user ID
        std::string id_str = response.substr(start, end - start);
        std::cout << "Extracted ID String: " << id_str << std::endl;

        if (id_str.empty())
        {
            throw std::runtime_error("User ID not found in insert response.");
        }

        try
        {
            id = std::stoi(id_str);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Invalid user ID format in insert response: " + std::string(e.what()));
        }

        std::cout << "Parsed User ID: " << id << std::endl;

        return "User created with ID: " + std::to_string(id);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in User::save: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging
    }
}
