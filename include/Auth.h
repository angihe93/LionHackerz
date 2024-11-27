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

                /**
                 * create an API user with the given email and password or look up an existing user by email
                 * 
                 * @param email   a string of the user's email
                 * @param password    a string of the user's password
                 * @return   the user id of the newly created user or the already existing user, or -1 on failure
                 */
                int createAPIUser(std::string email, std::string password);

                /**
                 * Get the username and password values from the HTTP Basic Auth Authorization header
                 * 
                 * @param auth   the Authorization header from the request
                 * @return   pair of strings for username and password 
                 */
                std::pair<std::string, std::string> decodeBasicAuth(const std::string& auth);


                /**
                 * A function to generate an API key for a user with the given role
                 * currently only support role = "admin", will eventually expand to other kinds of user roles with different permissions
                 * 
                 * @param role   the role for the API key to be generated
                 * @param uid    the id of the user to generate API key for
                 * @return   a string of the generated API key
                 */
                std::string genAPIKey(std::string role, int uid);

        private:
                Database *db;

                /**
                * Helper function to generate API key 
                * 
                * @param length   number of bytes of random data to generate
                * @return a string of random bytes for use as API key
                */
                std::string generateRandomHex(int length);
                
};

#endif