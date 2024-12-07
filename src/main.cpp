#include "Matcher.h"
#include "RouteController.h"
#include "Worker.h"
#include <cpp_redis/cpp_redis>
#include <crow.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

cpp_redis::client redis_client;

// Redis task queue initialization
void setup_redis()
{
    redis_client.connect("127.0.0.1", 6379, [](const std::string &host, std::size_t port, cpp_redis::connect_state status)
                         {
        if (status == cpp_redis::connect_state::dropped)
        {
            std::cerr << "Redis connection lost to " << host << ":" << port << std::endl;
        } });
}

int main(int argc, char *argv[])
{
    // Parse command-line arguments for test mode
    bool testMode = false;
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--test")
        {
            testMode = true;
        }
    }

    setup_redis();

    crow::SimpleApp app;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Initialize the appropriate database
    Database *db = nullptr;
    if (testMode)
    {
        db = new MockDatabase();
        std::cout << "Running in test mode with MockDatabase." << std::endl;
    }
    else
    {
        db = new Database();
        std::cout << "Running in normal mode with Database." << std::endl;
    }

    // Initialize RouteController with the selected database
    RouteController routeController;
    routeController.setDatabase(db);
    routeController.initRoutes(app);

    // Initialize Matcher and Worker
    Matcher m(*db);
    Worker worker(redis_client, &m);

    // Start task queue with 4 worker threads
    worker.start_worker_pool(4);

    // Run the app on port 18080
    app.port(18080).bindaddr("0.0.0.0").multithreaded().run();

    // Clean up
    delete db;
    curl_global_cleanup();

    return 0;
}