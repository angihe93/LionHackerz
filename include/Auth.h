// Copyright 2024 LionHackerz

#ifndef AUTH_H
#define AUTH_H

#include "Database.h"
#include <string>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>


class Auth {
        public: 
                Auth(Database &db); 


                // gets Authorization header, decode username and password. if no password provided, username would be the api key
                std::pair<std::string, std::string> decodeBasicAuth(const std::string& auth);


                /**
                 * @brief 
                 * 
                 * @param role 
                 * @return std::string 
                 */
                std::string genAPIKey(std::string role);

        private:
                Database *db;

                /**
                * Helper function to generate API key 
                * 
                * @param length   number of bytes of random data to generate
                * @return a string of the random bytes for use as API key
                */
                std::string generateRandomHex(int length);

                /**/
                std::string escapeJson(const std::string& input);

                
};

#endif