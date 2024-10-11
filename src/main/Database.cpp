#include <iostream>
#include <curl/curl.h>
#include <string>
#include "Database.h"

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

const std::string Database::query(std::string table, std::string selectColumns, std::string filterColumn,
                                  std::string op, std::string value)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" + filterColumn + "=" + op + "." + value;
    const std::string fURL = url;

    std::cout << fURL << std::endl;

    std::string result = get(fURL);

    return result.c_str();
}

const std::string Database::query(std::string table, std::string selectColumns, std::string filterColumn1,
                                  std::string op1, std::string value1, std::string filterColumn2, std::string op2,
                                  std::string value2)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" + filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" + op2 + "." + value2;

    const std::string fURL = url;

    std::string result = get(fURL);

    return result.c_str();
}

const std::string Database::query(std::string table, std::string selectColumns, std::string filterColumn1,
                                  std::string op1, std::string value1, std::string filterColumn2, std::string op2,
                                  std::string value2, std::string filterColumn3, std::string op3,
                                  std::string value3)
{
    std::string url = this->url + "/rest/v1/" + table + "?" + "select=" + selectColumns + "&" + filterColumn1 + "=" + op1 + "." + value1 + "&" + filterColumn2 + "=" + op2 + "." + value2 + "&" + filterColumn3 + "=" + op3 + "." + value3;

    const std::string fURL = url;

    std::string result = get(fURL);

    return result.c_str();
}