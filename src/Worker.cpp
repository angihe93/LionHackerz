// Copyright 2024 LionHackerz

#include "Worker.h"
#include <cpp_redis/cpp_redis>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <iostream>


Worker::Worker(cpp_redis::client &redis, Matcher &m, std::mutex &mutex)
	: redis_client(redis), m(&m), redis_mutex(mutex) {}

void Worker::process_tasks()
{
	while (true)
	{
		try
		{
			// Fetch a task from the queue
			auto future_reply = redis_client.lpop("task_queue");
			redis_client.commit();

			// Get the actual reply
			cpp_redis::reply reply = future_reply.get();

			if (!reply.is_string())
			{
				// No valid task available, sleep briefly before retrying
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			// Extract user ID from the task
			std::string user_id = reply.as_string();

			int uid = std::stoi(user_id);
			if (!uid)
				return;
			std::cout << "Processing task for UID: " << uid << std::endl;

			if (std::find(in_queue.begin(), in_queue.end(), uid) != in_queue.end()) {
        		std::cout << "That UID is already in the queue.  Give it time.." << std::endl;
				return;
    		} else {
				in_queue.push_back(uid);
   			}

			redis_client.set("job" + user_id, "1", [](cpp_redis::reply& reply) {
				if (reply.is_simple_string() && reply.as_string() == "OK") {
					std::cout << "Exists key successfully set." << std::endl;
				} else {
					std::cerr << "Failed to set exists key." << std::endl;
				}
        	});
			redis_client.expire("job" + user_id, 120);

			// Track progress in Redis
			redis_client.set("progress:" + user_id, "{\"status\": \"processing\", \"progress\": 0}");
			redis_client.commit();

			// Perform matching algorithm
			auto matches = m->displayMatches(uid, false);

			int upd_interv = matches.size() / 10;
			int progress = 85;
			int count = 0;
			nlohmann::json jsonArray = nlohmann::json::array();
		    for (const auto& match : matches) {
				if (count % upd_interv == 0 && progress < 95) {
					progress++;
					redis_client.set("progress:" + user_id, "{\"status\": \"completed\", \"progress\": " + std::to_string(progress) + "}");
					redis_client.commit();
				}
        		jsonArray.push_back(match.to_json());
    		}

			// Cache match results in Redis with expiration
			redis_client.set("matches:" + user_id, jsonArray.dump());
			redis_client.expire("matches:" + user_id, 200);
			redis_client.commit();

			// Mark task as completed
			redis_client.set("progress:" + user_id, "{\"status\": \"completed\", \"progress\": 100}");
			redis_client.commit();

			std::cout << "Task completed for UID: " << uid << std::endl;
			in_queue.erase(find(in_queue.begin(), in_queue.end(), uid));

			auto del_future = redis_client.del({"job" + user_id});
            redis_client.commit();
            cpp_redis::reply del_reply = del_future.get();

            if (del_reply.is_integer() && del_reply.as_integer() > 0) {
                std::cout << "Exists key erased successfully" << std::endl;
            } else {
                std::cerr << "Failed to erase exists key." << std::endl;
            }
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error processing task: " << e.what() << std::endl;
		}
	}
}

void Worker::start_worker_pool(int num_threads)
{
	for (int i = 0; i < num_threads; ++i)
	{
		worker_threads.emplace_back(&Worker::process_tasks, this);
		
	}
}

void Worker::join_threads()
{
	for (auto &thread : worker_threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}