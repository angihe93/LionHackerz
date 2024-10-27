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
    char *url_char = std::getenv("SUPABASE_URL");
    if (url_char == NULL) {
        std::cout << "ERROR: did not find SUPABASE_URL, check it is set and accessible in the current environment" << std::endl;
    }

    char *api_char = std::getenv("SUPABASE_API_KEY");
    if (api_char == NULL) {
        std::cout << "ERROR: did not find SUPABASE_API_KEY, check it is set and accessible in the current environment" << std::endl;
    }

    // // // commented out to check docker env vars:
    // const std::string url = url_char;
    // // std::cout << "in Database(), url: " << url << std::endl;
    // const std::string api = api_char;
    // // std::cout << "in Database(), api: " << api << std::endl;

    // added for to check docker env vars:
    std::string url = "https://alcpkkevodekihwyjzvl.supabase.co";
    std::string api = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFsY3Bra2V2b2Rla2lod3lqenZsIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTcyODQxNDY2OCwiZXhwIjoyMDQzOTkwNjY4fQ.qQaXij0b6rgniZpmsImn4AIC6Oh2OGUxFwJgpHbdeu4";

    this->url = url;
    this->api_key = api;
    std::cout << "Database::Database() ends with this->url " << this->url << " and this->api_key " << this->api_key << std::endl;
}


Database::Database(const std::string url, const std::string api_key)
{
    this->url = url;
    this->api_key = api_key;
}

std::string Database::request(const std::string &getPostPatch, const std::string url,
                              const std::string &insertData, std::string &httpStatusCode)
{
    CURL *curl;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    std::cout << "in Database::request() after curl_init" << std::endl;

    if (!curl)
    {
        std::cerr << "failed to initialize" << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    std::cout << "in Database::request() after curl_easy_setopt(curl, CURLOPT_URL, url.c_str());" << std::endl;


    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");    
    headers = curl_slist_append(headers, ("apikey: " + this->api_key).c_str());
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->api_key).c_str());
    std::cout << "in Database::request() after curl_slist_append" << std::endl;


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    std::cout << "in Database::request() after curl_easy_setopt(curl, CURLOPT_URL, url.c_str());" << std::endl;


    if (getPostPatch != "POST" && getPostPatch != "GET" && getPostPatch != "PATCH")
    {
        std::cout << "invalid option. please specify whether you wish to perform a GET or POST or PATCH"
                  << "request." << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return "exiting request.";
    }

    if (getPostPatch == "POST" || getPostPatch == "PATCH")
    {
        if (getPostPatch == "POST")
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (getPostPatch == "PATCH")
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");        
        headers = curl_slist_append(headers, "Prefer: return=representation");
        if (!insertData.empty())
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, insertData.c_str());
        }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);

    if (res != CURLE_OK)
    {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    std::cout << "in Database::request() after curl_easy_cleanup;" << std::endl;
    std::cout << "response: " << response << std::endl;

    try {
        std::cout << "Entering return block" << std::endl;
        std::cout << "response: " << response << std::endl;
        std::cout << "Exiting return block" << std::endl;

        return response;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
        throw;
    }
}

// std::string Database::insert(std::string table, std::string data) {
//     std::cout << "Entering Database::insert" << std::endl;
//     std::cout << "Table: " << table << std::endl;
//     std::cout << "Data: " << data << std::endl;

//     // Simulate some processing
//     try {
//         // Example of processing
//         if (table.empty() || data.empty()) {
//             throw std::runtime_error("Table or data is empty");
//         }

//         // Simulate database insertion
//         std::string response = "some response"; // Replace with actual database response
//         std::cout << "Database response: " << response << std::endl;

//         return response;
//     } catch (const std::exception& e) {
//         std::cerr << "Exception in Database::insert: " << e.what() << std::endl;
//         throw; // Re-throw the exception to be handled by the caller
//     }

//     std::cout << "Exiting Database::insert" << std::endl;
// }

std::string Database::insert(std::string table, std::string data)
{
    std::string statusCode = "";

    std::string url = this->url + "/rest/v1/" + table;

    const std::string fURL = url;

    const std::string method = "POST";

    const std::string json = data;

    std::string status = request(method, fURL, json, statusCode);
    std::cout << "in Database::insert() after request()" << std::endl;
    // std::cout << "status: " << status << std::endl;
    try {
        std::cout << "Returning status: " << status << std::endl;
        return status;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
        throw;
    }
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
    try {

         std::cout << "Entering Database::query" << std::endl;
    std::cout << "Table: " << table << std::endl;
    std::cout << "Select Columns: " << selectColumns << std::endl;
    std::cout << "Filter Column: " << filterColumn << std::endl;
    std::cout << "Operator: " << op << std::endl;
    std::cout << "Value: " << value << std::endl;
    std::cout << "Print Results: " << (printResults ? "true" : "false") << std::endl;

        std::string statusCode = "";

    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" +
                 selectColumns + "&" + filterColumn + "=" + op + "." + value;
    
    std::cout << "in Database::query() after this->url + ..." << std::endl;

    const std::string fURL = url;

    const std::string& method = "GET";
    const std::string& insertData = "";

    std::string result = request(method, fURL, insertData, statusCode);
    std::cout << "in Database::query() after request()" << std::endl;

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

    std::cout << "Database::query() ends wtih resCount " << resCount << std::endl;

    return queryLists;

    } catch (const std::exception& e) {
        std::cerr << "Exception in Database::query: " << e.what() << std::endl;
        // throw; // Re-throw the exception to be handled by the caller
    }
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

void Database::tokenize(std::string res, int cR, int listCount, std::vector<std::vector<std::string>> &queryLists)
{
    queryLists.resize(listCount);
    for (int i = 0; i < cR; i++)
    {
        for (int j = 0; j < listCount; j++)
        {
            res.erase(0, res.find(":") + 1);

            std::string token = res.substr(0, res.find(","));

            if (token[token.size() - 1] == '}')
                token = token.substr(0, token.size() - 1);

            if (token[token.size() - 1] == ']')
                token = token.substr(0, token.size() - 2);

            if (token == "null")
                token = "\"null\"";

            if (token == "true")
                token = "\"true\"";

            if (token == "false")
                token = "\"false\"";

            queryLists[j].push_back(token);
        }
    }
    return;
}

void Database::iterateLists(std::vector<std::vector<std::string>> queryLists)
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