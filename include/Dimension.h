/*
Kelvin Kim sk4802
Dimension of user
*/
#ifndef DIMENSION_H
#define DIMENSION_H

#include <string>
#include "Database.h"
#include "crow/json.h"

class Dimension {
 public:
    int user_id;
    std::string loc;
    std::string field;
    int64_t pay;
    bool gender;
    bool diversity;
    std::string mbti;
    bool flexibility;
    bool remote;
    bool workspace;

    Dimension();
    std::string fromJson(const crow::json::rvalue &json);
    std::string save(Database &db);

 private:
    std::string escapeJson(const std::string &input);
};

#endif // DIMENSION_H
