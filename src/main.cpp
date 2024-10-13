#include "Database.h"
#include "Matcher.h"
#include "RouteController.h"
#include "crow.h"
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    crow::SimpleApp app; //define your crow application

    RouteController routeController;
    routeController.initRoutes(app);
    Database *db = new Database();
    routeController.setDatabase(db);

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();

    return 0;
}
