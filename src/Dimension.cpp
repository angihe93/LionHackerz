// Copyright 2024 LionHackerz
#include "Dimension.h"
#include <iostream>
#include <stdexcept>

Dimension::Dimension()
    : user_id(-1), pay(0), gender(false), diversity(false),
      flexibility(false), remote(false), workspace(false) {}

std::string Dimension::fromJson(const crow::json::rvalue &json)
{
    try
    {
        if (!json.has("loc") || !json.has("field") || !json.has("pay") ||
            !json.has("gender") || !json.has("diversity") || !json.has("mbti") ||
            !json.has("flexibility") || !json.has("remote") || !json.has("workspace"))
        {
            return "Missing one or more dimension fields.";
        }

        loc = json["loc"].s();
        field = json["field"].s();
        pay = json["pay"].i();
        gender = json["gender"].b();
        diversity = json["diversity"].b();
        mbti = json["mbti"].s();
        flexibility = json["flexibility"].b();
        remote = json["remote"].b();
        workspace = json["workspace"].b();
    }
    catch (const std::exception &e)
    {
        return std::string("Error parsing dimensions: ") + e.what();
    }

    return "";
}

std::string Dimension::escapeJson(const std::string &input)
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
        default:
            output += c;
            break;
        }
    }
    return output;
}

std::string Dimension::save(Database &db)
{
    std::string data = "{";
    data += "\"id\": " + std::to_string(user_id) + ",";
    data += "\"loc\": \"" + escapeJson(loc) + "\",";
    data += "\"field\": \"" + escapeJson(field) + "\",";
    data += "\"pay\": " + std::to_string(pay) + ",";
    data += "\"gender\": " + std::string(gender ? "true" : "false") + ",";
    data += "\"diversity\": " + std::string(diversity ? "true" : "false") + ",";
    data += "\"mbti\": \"" + escapeJson(mbti) + "\",";
    data += "\"flexibility\": " + std::string(flexibility ? "true" : "false") + ",";
    data += "\"remote\": " + std::string(remote ? "true" : "false") + ",";
    data += "\"workspace\": " + std::string(workspace ? "true" : "false");
    data += "}";

    // Insert into DB
    std::string response = db.insert("Has_Dimension", data);
    std::cout << "Dimension Insert Response: " << response << std::endl;

    return "Dimension data saved for user ID: " + std::to_string(user_id);
}
