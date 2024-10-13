/* Dallas Scott - ds4015
 * Database.cpp
 * Database class implementation */

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <list>
#include "Database.h"
#include <cstdlib>

size_t Database::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

Database::Database()
{
    char* url_char = std::getenv("SUPABASE_URL"); 
    const std::string url = url_char;
    if (url_char == NULL) {
        std::cout << "did not find SUPABASE_URL, check it is set and accessible in the current environment" << std::endl;
    }

    char* api_char = std::getenv("SUPABASE_API_KEY"); 
    const std::string api = api_char;
    if (api_char == NULL) {
        std::cout << "did not find SUPABASE_API_KEY, check it is set and accessible in the current environment" << std::endl;
    }

    this->url = url;
    this->api_key = api;
}

Database::Database(const std::string url, const std::string api_key)
{
    this->url = url;
    this->api_key = api_key;
}

std::string Database::get(const std::string url)
{
    CURL *curl;
    std::string response;

    curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "failed to initialize" << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("apikey: " + this->api_key).c_str());
    headers = curl_slist_append(headers, ("Authorization: Bearer " + this->api_key).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);

    return response;
}

std::list<std::string> *Database::query(std::string table, std::string selectColumns,
                                        std::string filterColumn, std::string op,
                                        std::string value, bool printResults, int &resCount)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" +
                      selectColumns + "&" + filterColumn + "=" + op + "." + value;

    const std::string fURL = url;

    std::string result = get(fURL);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::list<std::string> *queryLists = new std::list<std::string>[listCount];

    int cR = this->countResults(result);
    resCount = this->countResults(result);

    if (printResults)
        std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
                  << filterColumn << " " << op << " " << value << std::endl;

    std::cout << "Query complete." << std::endl;
    std::cout << "\tNumber of results: " << cR << std::endl;
    std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

    if (printResults)
        std::cout << std::endl
                  << "Results: " << result << std::endl
                  << std::endl;

    queryLists = tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(listCount, queryLists);

    return queryLists;
}

std::list<std::string> *Database::query(std::string table, std::string selectColumns,
                                        std::string filterColumn1, std::string op1, std::string value1,
                                        std::string filterColumn2, std::string op2, std::string value2,
                                        bool printResults, int &resCount)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" +
                      filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" +
                      op2 + "." + value2;

    const std::string fURL = url;

    std::string result = get(fURL);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::list<std::string> *queryLists = new std::list<std::string>[listCount];

    int cR = this->countResults(result);

    resCount = this->countResults(result);

    std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
              << filterColumn1 << " " << op1 << " " << value1 << " AND "
              << filterColumn2 << " " << op2 << " " << value2 << std::endl;

    std::cout << "Query complete." << std::endl;
    std::cout << "\tNumber of results: " << cR << std::endl;
    std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

    if (printResults)
        std::cout << std::endl
                  << result << std::endl;

    queryLists = tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(listCount, queryLists);

    return queryLists;
}

std::list<std::string> *Database::query(std::string table, std::string selectColumns,
                                        std::string filterColumn1, std::string op1, std::string value1,
                                        std::string filterColumn2, std::string op2, std::string value2,
                                        std::string filterColumn3, std::string op3, std::string value3,
                                        bool printResults, int &resCount)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" +
                      filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" +
                      op2 + "." + value2 + "&" + filterColumn3 + "=" + op3 + "." + value3;

    const std::string fURL = url;

    std::string result = get(fURL);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    std::list<std::string> *queryLists = new std::list<std::string>[listCount];

    int cR = this->countResults(result);
    resCount = this->countResults(result);

    std::cout << "SELECT " << selectColumns << " FROM " << table << " WHERE "
              << filterColumn1 << " " << op1 << " " << value1 << " AND "
              << filterColumn2 << " " << op2 << " " << value2 << " AND "
              << filterColumn3 << " " << op3 << " " << value3 << " AND " << std::endl;

    std::cout << "Query complete.." << std::endl;
    std::cout << "\tNumber of results: " << cR << std::endl;
    std::cout << "\tNumber of columns returned:  " << listCount << std::endl;

    if (printResults)
        std::cout << std::endl
                  << result << std::endl;

    queryLists = tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(listCount, queryLists);

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

std::list<std::string> *Database::tokenize(std::string res, int cR, int listCount,
                                           std::list<std::string> *queryLists)
{
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

            queryLists[j]
                .push_back(token);
        }
    }
    return queryLists;
}

void Database::iterateLists(int listCount, std::list<std::string> *queryLists)
{
    std::cout << "Tokenized and listified: " << std::endl;
    for (int i = 0; i < listCount; i++)
    {
        std::cout << "List " << i + 1 << ": ";
        std::list<std::string>::iterator it = queryLists[i].begin();
        std::cout << "(";
        while (it != queryLists[i].end())
        {
            std::cout << *it;
            if (it != std::prev(queryLists[i].end()))
                std::cout << ", ";
            it++;
        }
        std::cout << ")" << std::endl;
    }
}