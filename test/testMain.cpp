// Copyright 2024 LionHackerz

#include <gtest/gtest.h>
#include <cpp_redis/cpp_redis>
#include "Global.h"

cpp_redis::client redis_client;

void setup_redis()
{
    redis_client.connect("127.0.0.1", 6379, [](const std::string &host, std::size_t port, cpp_redis::connect_state status)
                         {
        if (status == cpp_redis::connect_state::dropped) {
            std::cerr << "Redis connection lost to " << host << ":" << port << std::endl;
        } });
}

int main(int argc, char **argv)
{

    setup_redis();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}