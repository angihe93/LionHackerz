/*Kelvin Kim sk4802*/
/*Let's make a new user class!*/
#ifndef USER_H
#define USER_H

#include <string>
#include "Database.h"

class User {
public:
    int id; 
    std::string name;
    std::string real_name;
    std::string email;

    // Constructor
    User(const std::string& rname, const std::string& name, const std::string& email);

    /**
     * @brief Save a new user to the database
     * 
     * Takes the user's information provided during registration and places it in
     * the appropriate table in the database
     * 
     * @param db 
     */
    // Method to save the user to the database
    std::string save(Database& db);

private:
    // Utility function to escape double quotes and backslashes in JSON strings
    std::string escapeJson(const std::string& input);
};

#endif // USER_H
