#ifndef MOCKMATCHER_H
#define MOCKMATCHER_H

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <variant>
#include <curl/curl.h>
#include <wn.h>
#include "Global.h"
#include <string>
#include <mutex>
#include <regex>
#include <cpp_redis/cpp_redis>
#include "Database.h"
#include <nlohmann/json.hpp>
#include "Listing.h"
#include "Matcher.h"
#include <curl/curl.h>
#include <crow.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <vector>

class MockMatcher : public Matcher
{
    public:
        MockMatcher(Database &db) : Matcher(db) {
            this->db = &db;
        }

        // for testing matchDimensions:
        int matchDimensionsTester(std::string d) {
            return matchDimensions(d);
        }


    private:
        Database *db;

};

#endif // MOCKMATCHER_H