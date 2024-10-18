/* Dallas Scott - ds4015
 * Database.cpp
 * Database class implementation */

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "Database.h"
#include <cstdlib>

using namespace std;

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

    const std::string url = url_char;
    std::cout << "in Database(), url: " << url << std::endl;
    const std::string api = api_char;
    std::cout << "in Database(), api: " << api << std::endl;

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
    CURL *curl;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "failed to initialize" << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");    
    headers = curl_slist_append(headers, ("apikey: " + this->api_key).c_str());
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->api_key).c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

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

    return response;
}

std::string Database::insert(string table, string data)
{
    string statusCode = "";

    string url = this->url + "/rest/v1/" + table;

    const string fURL = url;

    const string method = "POST";

    const string json = data;

    string status = request(method, fURL, json, statusCode);

    return status;
}

std::string Database::update(string table, string data, string column, string op, string val)
{
    string statusCode = "";

    string url = this->url + "/rest/v1/" + table + "?" + column + "=" + op + "." + val;

    const string fURL = url;

    const string method = "PATCH";

    const string json = data;

    string status = request(method, fURL, json, statusCode);

    return status;
}


std::vector<std::vector<std::string>>
Database::query(std::string table, std::string selectColumns,
                std::string filterColumn, std::string op,
                std::string value, bool printResults, int &resCount)
{
    string statusCode = "";

    string url = this->url + "/rest/v1/" + table + "?" + "select=" +
                 selectColumns + "&" + filterColumn + "=" + op + "." + value;

    const string fURL = url;

    const string& method = "GET";
    const string& insertData = "";

    string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (i < result.size() && result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    vector<vector<string>> queryLists;

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

    string statusCode = "";

    const std::string fURL = url;

    const string& method = "GET";
    const string& insertData = "";

    string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    vector<vector<string>> queryLists;

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

    string statusCode = "";

    const std::string fURL = url;

    const string& method = "GET";
    const string& insertData = "";

    string result = request(method, fURL, insertData, statusCode);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    vector<vector<string>> queryLists;

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

void Database::tokenize(string res, int cR, int listCount, vector<vector<string>> &queryLists)
{
    queryLists.resize(listCount);
    for (int i = 0; i < cR; i++)
    {
        for (int j = 0; j < listCount; j++)
        {
            res.erase(0, res.find(":") + 1);

            string token = res.substr(0, res.find(","));

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

void Database::iterateLists(vector<vector<string>> queryLists)
{
    int listCount = 0;
    std::cout << "\nTokenized and listified: " << std::endl;
    for (auto &v : queryLists)
    {
        int e = 0;
        int listSize = v.size();
        std::cout << "List " << listCount++ << ": ";
        std::cout << "(";
        for (string &s : v)
        {
            cout << s;
            if (++e != listSize)
                std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }
}