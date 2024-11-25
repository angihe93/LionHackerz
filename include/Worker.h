#ifndef WORKER_TASK_H
#define WORKER_TASK_H

#include <cpp_redis/cpp_redis>
#include <nlohmann/json.hpp>
#include "Matcher.h" 
#include "crow.h"

/* Worker class redis task queue for processing getMatches calls */
class Worker {
private:
    cpp_redis::client &redis_client; 
    Matcher *m;       
    std::vector<std::thread> worker_threads;

public:
    Worker(cpp_redis::client &redis, Matcher *matcher);

    /* Starts the worker pool */
	void start_worker_pool(int num_threads);

	void join_threads();

    /* Process getMatches task calls in a queue */
    void process_tasks();

};

#endif
