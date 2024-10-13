/* Dallas Scott - ds4015
 * Database.cpp
 * Database class implementation */

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "Database.h"

using namespace std;

size_t Database::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
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

vector<string> *Database::query(std::string table, std::string selectColumns,
                                std::string filterColumn, std::string op,
                                std::string value, bool printResults, int &resCount)
{
    string url = this->url + "/rest/v1/" + table + "?" + "select=" +
                 selectColumns + "&" + filterColumn + "=" + op + "." + value;

    const string fURL = url;

    string result = get(fURL);

    int i = 0;
    int listCount = 0;

    while (result[i] != '}')
    {
        if (result[i] == ':')
            listCount++;
        i++;
    }

    vector<string> *queryLists = new vector<string>[listCount];

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
                  << "Results: " << result << std::endl
                  << std::endl;
    }

    tokenize(result, cR, listCount, queryLists);

    if (printResults)
        iterateLists(listCount, queryLists);

    return queryLists;
}

vector<string> *Database::query(std::string table, std::string selectColumns,
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

    vector<string> *queryLists = new vector<string>[listCount];

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
        iterateLists(listCount, queryLists);

    return queryLists;
}

vector<string> *Database::query(std::string table, std::string selectColumns,
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

    vector<string> *queryLists = new vector<string>[listCount];

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

void Database::tokenize(string res, int cR, int listCount,
                        vector<string> *queryLists)
{
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

            queryLists[j]
                .push_back(token);
        }
    }
    return;
}

void Database::iterateLists(int listCount, vector<string> *queryLists)
{
    std::cout << "Tokenized and listified: " << std::endl;
    for (int i = 0; i < listCount; i++)
    {
        int e = 0;
        int listSize = queryLists[i].size();
        std::cout << "List " << i << ": ";
        std::cout << "(";
        for (string &s : queryLists[i])
        {
            cout << s;
            if (++e != listSize)
                std::cout << ", ";
        }
        std::cout << ")";
    }
}