/**
 * @brief Implementation of the ThreadPool class
 * 
 * @file ThreadPool.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */
#include "ThreadPool.h"
#include <direct.h>
#include <chrono>
#include "Worker.h"
#include "boost/filesystem.hpp"
#include "curl/curl.h"

/**
	 * @brief Construct a new Thread Pool object
	 * 
	 */
ThreadPool::ThreadPool()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

/**
	 * @brief Destroy the Thread Pool object
	 * 
	 */
ThreadPool::~ThreadPool()
{
	stop_ = true;
	condition_.notify_all();
	for (std::thread& w : workers_)
		w.join();
	curl_global_cleanup();
}

/**
	 * @brief Start ThreadPool's work
	 * 
	 * @param threads Number of threads
	 */
void ThreadPool::initializeWithThreads(size_t threads)
{
	for (size_t i = 0; i < threads; i++)
	{
		workers_.emplace_back([this]() -> void
		{
			while (true)
			{
				MyTask task;
				{
					std::unique_lock<std::mutex> lock(mutex_);
					condition_.wait(lock, [this]() -> bool {
						return !queue_.empty() || stop_;
					});

					if (stop_ && queue_.empty()) {
						return;
					}

					task = queue_.front();
					queue_.pop_front();
				}   //release lock
				
				Worker::Work(&task);
				if (task.get_target() != TaskTarget::Done)
					schedule(task);
				count_--;
			}
		});
	}
}

/**
	 * @brief Add new task to the queue
	 * 
	 * @param task 
	 */
void ThreadPool::schedule(MyTask task)
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push_back(task);
	}
	count_++;
	condition_.notify_one();
}

/**
	 * @brief Tells thread to wait
	 * 
	 */
void ThreadPool::wait() const
{
	while (count_ != 0)
		std::this_thread::sleep_for(std::chrono::microseconds(1));
}

/**
	 * @brief Create or get Singleton instance of the ThreadPool
	 * 
	 * @return ThreadPool& 
	 */
ThreadPool& ThreadPool::Instance() 
{
	static ThreadPool instance;
	return instance;
}
