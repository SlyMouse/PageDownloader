#include "ThreadPool.h"
#include <direct.h>
#include <chrono>
#include "Worker.h"
#include "boost/filesystem.hpp"
#include "curl/curl.h"

ThreadPool::ThreadPool()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

ThreadPool::~ThreadPool()
{
	stop_ = true;
	condition_.notify_all();
	for (std::thread& w : workers_)
		w.join();
	curl_global_cleanup();
}

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

void ThreadPool::schedule(MyTask task)
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push_back(task);
	}
	count_++;
	condition_.notify_one();
}

void ThreadPool::wait() const
{
	while (count_ != 0)
		std::this_thread::sleep_for(std::chrono::microseconds(1));
}

ThreadPool& ThreadPool::Instance() 
{
	static ThreadPool instance;
	return instance;
}
