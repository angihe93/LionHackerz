// Copyright 2024 LionHackerz
/* Dallas Scott - ds4015
 * Database.cpp
 * Database class implementation */

#include "Database.h"
#include <curl/curl.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


// using namespace std;

Database::Database()
{
    openai_api_key = "";
    char *url_char = std::getenv("SUPABASE_URL");
    if (url_char == NULL) {
        std::cout << "ERROR: did not find SUPABASE_URL, check it is set and accessible in the current environment" << std::endl;
    }

    char *api_char = std::getenv("SUPABASE_API_KEY");
    if (api_char == NULL) {
        std::cout << "ERROR: did not find SUPABASE_API_KEY, check it is set and accessible in the current environment" << std::endl;
    }

    char *openai_api_char = std::getenv("OPENAI_API_KEY");
    if (openai_api_char == NULL) {
        std::cout << "ERROR: did not find OPENAI_API_KEY, check it is set and accessible in thh current environment." << std::endl
                  << "Continuing without AI." << std::endl;
    }

    const std::string url = url_char;
   //  std::cout << "in Database(), url: " << url << std::endl;
    const std::string api = api_char;
   //  std::cout << "in Database(), api: " << api << std::endl;
    if (openai_api_char != NULL) {
        const std::string openai_api = openai_api_char; 
        this->openai_api_key = openai_api; 
    }

    // for ci workflow debug
    // this->url = "https://alcpkkevodekihwyjzvl.supabase.co";
    this->url = url;
    this->api_key = api;

}


Database::Database(const std::string url, const std::string api_key)
{
    this->url = url;
    this->api_key = api_key;
}

std::string Database::request(const std::string &getPostPatch, const std::string url,
                              const std::string &insertData, std::string &httpStatusCode)
{
    if (getPostPatch == "AI" && this->openai_api_key == "") {
        std::cout << "You must have an Open AI API key set as an environmental variable to use" 
                  << "any AI-related functions.  Please set this and try again." << std::endl;
                  return "";
    }
    

    CURL *curl = curl_easy_init();
    std::string response;
   
    if (!curl)
    {
        std::cerr << "failed to initialize" << std::endl;
        return "";
    }

    // debug
    // std::cout << "url: " << url << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");    
    headers = curl_slist_append(headers, ("apikey: " + this->api_key).c_str());

    if (getPostPatch == "AI")
        headers = curl_slist_append(headers, ("Authorization: Bearer " + this->openai_api_key).c_str());
    else
        headers = curl_slist_append(headers, ("Authorization: Bearer " + this->api_key).c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    if (getPostPatch != "POST" && getPostPatch != "GET" && getPostPatch != "PATCH" && getPostPatch != "AI")
    {
        std::cout << "invalid option. please specify whether you wish to perform a GET or POST or PATCH"
                  << "request." << std::endl;
        curl_easy_cleanup(curl);
        httpStatusCode = "400";
        return "exiting request.";
    }


    if (getPostPatch == "POST" || getPostPatch == "AI") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else if (getPostPatch == "PATCH") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    } else if (getPostPatch == "GET" || getPostPatch == "AI") {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }
    if (!insertData.empty() && (getPostPatch == "POST" || getPostPatch == "PATCH" || getPostPatch == "AI"))
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, insertData.c_str());



    headers = curl_slist_append(headers, "Prefer: return=representation");    

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    long response_code; /* NOTE: this must be type long to work on deployed server */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    httpStatusCode = std::to_string(response_code);

    if (res != CURLE_OK)
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}

std::string Database::insert(std::string table, std::string data)
{
    std::string statusCode = "";

    std::string url = this->url + "/rest/v1/" + table;

    const std::string fURL = url;

    const std::string method = "POST";

    const std::string json = data;

    std::string status = request(method, fURL, json, statusCode);

    return status;
}

std::string Database::update(std::string table, std::string data, std::string column, std::string op, std::string val)
{
    std::string statusCode = "";

    std::string url = this->url + "/rest/v1/" + table + "?" + column + "=" + op + "." + val;

    const std::string fURL = url;

    const std::string method = "PATCH";

    const std::string json = data;

    std::string status = request(method, fURL, json, statusCode);

    return status;
}


std::vector<std::vector<std::string>>
Database::query(std::string table, std::string selectColumns,
                std::string filterColumn, std::string op,
                std::string value, bool printResults, int &resCount)
{
    std::string statusCode = "";

    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" +
                 selectColumns + "&" + filterColumn + "=" + op + "." + value;

    const std::string fURL = url;

    const std::string& method = "GET";
    const std::string& insertData = "";

    std::string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (i < result.size() && result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::vector<std::vector<std::string>> queryLists;

    int cR = this->countResults(result);
    resCount = this->countResults(result);
    if (printResults)
    {
        std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
                  << filterColumn << " " << op << " " << value << std::endl;
        std::cout << "Query complete." << std::endl;
        std::cout << "\tNumber of results: " << cR << std::endl;
        std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

        std::cout << std::endl
                  << "Results: " << result << std::endl;
    }

    tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(queryLists);

    return queryLists;
}

std::vector<std::vector<std::string>>
Database::query(std::string table, std::string selectColumns,
                std::string filterColumn1, std::string op1, std::string value1,
                std::string filterColumn2, std::string op2, std::string value2,
                bool printResults, int &resCount)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" +
                      filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" +
                      op2 + "." + value2;

    std::string statusCode = "";

    const std::string fURL = url;

    const std::string& method = "GET";
    const std::string& insertData = "";

    std::string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::vector<std::vector<std::string>> queryLists;

    int cR = this->countResults(result);

    resCount = this->countResults(result);

    if (printResults)
    {
        std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
                  << filterColumn1 << " " << op1 << " " << value1 << " AND "
                  << filterColumn2 << " " << op2 << " " << value2 << std::endl;

        std::cout << "Query complete." << std::endl;
        std::cout << "\tNumber of results: " << cR << std::endl;
        std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

        std::cout << std::endl
                  << result << std::endl;
    }

    tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(queryLists);

    return queryLists;
}

std::vector<std::vector<std::string>>
Database::query(std::string table, std::string selectColumns,
                std::string filterColumn1, std::string op1, std::string value1,
                std::string filterColumn2, std::string op2, std::string value2,
                std::string filterColumn3, std::string op3, std::string value3,
                bool printResults, int &resCount)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" +
                      filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" +
                      op2 + "." + value2 + "&" + filterColumn3 + "=" + op3 + "." + value3;

    std::string statusCode = "";

    const std::string fURL = url;

    const std::string& method = "GET";
    const std::string& insertData = "";

    std::string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::vector<std::vector<std::string>> queryLists;

    int cR = this->countResults(result);
    resCount = this->countResults(result);

    if (printResults)
    {
        std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
                  << filterColumn1 << " " << op1 << " " << value1 << " AND "
                  << filterColumn2 << " " << op2 << " " << value2 << " AND "
                  << filterColumn3 << " " << op3 << " " << value3 << " AND " << std::endl;

        std::cout << "Query complete.." << std::endl;
        std::cout << "\tNumber of results: " << cR << std::endl;
        std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

        std::cout << std::endl
                  << result << std::endl;
    }

    tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(queryLists);

    return queryLists;
}

int Database::countResults(std::string results)
{
    int count = 0;
    for (int i = 0; i < results.size(); i++)
        if (results[i] == '}')
            count++;
    return count;
}

std::vector<std::vector<std::string>> Database::tokenize(const std::string& res, int cR, int listCount, 
    std::vector<std::vector<std::string>> &queryLists)
{
    std::string localRes = res;
    queryLists.resize(listCount);

    for (int j = 0; j < listCount; j++)
	    queryLists[j].resize(cR);

    for (int i = 0; i < cR; i++)
    {
        for (int j = 0; j < listCount; j++)
        {
	    int delim_pos = localRes.find(":");
	    if (delim_pos == std::string::npos) break;
	    
            localRes.erase(0, delim_pos + 1);

            std::string token = localRes.substr(0, localRes.find(","));

	    if (!token.empty()) {
            if (token.back() == ']') {
                token.pop_back();
            }
            if (token.back() == '}') {
                token.pop_back();
            }
		}

           if (token == "null")
                token = "\"null\"";

            if (token == "true")
                token = "\"true\"";

            if (token == "false")
                token = "\"false\""; 
            queryLists[j][i] = token;
        }
    }
    return queryLists;
}


void Database::iterateLists(std::vector<std::vector<std::string>> &queryLists)
{
    int listCount = 0;
    std::cout << "\nTokenized and listified: " << std::endl;
    for (auto &v : queryLists)
    {
        int e = 0;
        int listSize = v.size();
        std::cout << "List " << listCount++ << ": ";
        std::cout << "(";
        for (std::string &s : v)
        {
            std::cout << s;
            if (++e != listSize)
                std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }
}

std::string Database::escapeString(const std::string &input)
{
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

bool Database::skillExists(const std::string &skillName)
{
    std::string full_url = url + "/rest/v1/skill?name=eq." + escapeString(skillName);
    std::string statusCode;
    std::string response = request("GET", full_url, "", statusCode);

    // Check if response contains at least one entry
    crow::json::rvalue json_response = crow::json::load(response);
    if (json_response && json_response.size() > 0)
    {
        return true;
    }
    return false;
}

bool Database::interestExists(const std::string &interestName)
{
    std::string full_url = url + "/rest/v1/interest?name=eq." + escapeString(interestName);
    std::string statusCode;
    std::string response = request("GET", full_url, "", statusCode);

    // Check if response contains at least one entry
    crow::json::rvalue json_response = crow::json::load(response);
    if (json_response && json_response.size() > 0)
    {
        return true;
    }
    return false;
}
