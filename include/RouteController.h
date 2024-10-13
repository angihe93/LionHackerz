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

        void index(crow::response& res);
        void dbtest(const crow::request& req, crow::response& res);
};

#endif 
