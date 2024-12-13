// Copyright 2024 LionHackerz

#include "Database.h"
#include "Global.h"
#include "Matcher.h"
#include "crow.h"
#include "RouteController.h"
#include "Worker.h"
#include <cpp_redis/cpp_redis>
#include <iostream>
#include <string>
#include <cstdlib>

std::mutex redis_mutex;

cpp_redis::client redis_client;


/* redis task queue initialization */
void setup_redis()
{
    redis_client.connect("127.0.0.1", 6379, [](const std::string &host, std::size_t port, cpp_redis::connect_state status)
                         {
        if (status == cpp_redis::connect_state::dropped) {
            std::cerr << "Redis connection lost to " << host << ":" << port << std::endl;
        } });
}

int main(int argc, char *argv[])
{
    bool testMode = false;
    if (argc > 1 && std::string(argv[1]) == "test")
        testMode = true;

    setup_redis();

    crow::SimpleApp app;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    RouteController routeController;
    routeController.initRoutes(app);
    Database *db;
    if (testMode)
    {
        db = new MockDatabase();
        std::cout << "Running in test mode with MockDatabase" << std::endl;
    }
    else
    {
        db = new Database();
        std::cout << "Running in normal mode with Database" << std::endl;
    }
    routeController.setDatabase(db);

    Matcher m(*db);
    Worker worker(redis_client, &m, redis_mutex);

    /* start task queue */
    worker.start_worker_pool(4);

    /* set the port, set the app to run on multiple threads, and run the app */
    app.port(18080).bindaddr("0.0.0.0").multithreaded().run();

    return 0;
}
