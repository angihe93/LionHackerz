// Copyright 2024 LionHackerz

#ifndef ROUTECONTROLLER_H
#define ROUTECONTROLLER_H

#include "crow.h"
// #include "Globals.h"
#include "Database.h"

class RouteController {
    private:
        Database* db;

    public:
        void initRoutes(crow::App<>& app);
        void setDatabase(Database* db);


        /**
        * Redirects to the homepage.
        *
        * @return A crow::response object containing the welcome message and an HTTP 200 response
        */
        void index(crow::response& res);

        /**
         * Client sign up route
         * 
         * @param req   
         * @param res 
         */
        void signUp(const crow::request& req, crow::response &res);


        void getMatches(const crow::request& req, crow::response &res);
        void changeField(const crow::request &req, crow::response &res);        
        void changePosition(const crow::request &req, crow::response &res);                
        void changeJobDescription(const crow::request &req, crow::response &res);        
        void dbtest(const crow::request& req, crow::response& res);
        void makeUser(const crow::request &req, crow::response &res);
};

#endif 
