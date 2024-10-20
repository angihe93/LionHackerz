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

        /* Route: /getMatches?uid=X 
        *       This route checks all job listings and performs
        *       the matching algorithm in the Matcher.cpp class,
        *       returning a sorted list of matches based on
        *       the user's stated preferences.  */
        void getMatches(const crow::request& req, crow::response &res);

        /* Route: /listing/changeField?lid=X&newField=Y
        *       This route updates the 'field' parameter of the job
        *       listing with listing ID lid and sets the field to
        *       the string following 'newField=' in the URL. */
        void changeField(const crow::request &req, crow::response &res);        
        
        /* Route: /listing/changePosition?lid=X&newPosition=Y
        *       This route updates the 'position' parameter of the job
        *       listing with listing ID lid and sets the position to
        *       the string following 'newPosition=' in the URL. */       
        void changePosition(const crow::request &req, crow::response &res);                
        
        /* Route: /listing/changeJobDescription?lid=X&newDescription=Y
        *       This route updates the 'job_description' parameter of the job
        *       listing with listing ID lid and sets the description to
        *       the string following 'newDescription=' in the URL. */   
        void changeJobDescription(const crow::request &req, crow::response &res); 

        /* Route: /listing/changeFlex?lid=X
        *       This route updates the 'job_flexibility' parameter of the job
        *       listing with listing ID lid and sets the position to
        *       the true if previously null, false if previously true, 
        *       and true if previously false.  Multiple calls cycle
        *       between setting true and false. */
        void changeFlex(const crow::request &req, crow::response &res);         
        
        /* Route: /dbtest
         *      This route tests the functionality of the database class and
         *      provides examples of queries and inserts. Not intended for
         *      the end user of the API, more for development purposes. */
        void dbtest(const crow::request& req, crow::response& res);

        void makeUser(const crow::request &req, crow::response &res);
};

#endif 
