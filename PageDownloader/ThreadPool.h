#ifndef POOL_H
#define POOL_H
#include "MyTask.h"
#include "SharedQueue.h"
#include "curl/curl.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool();
	ThreadPool(size_t threads);
	~ThreadPool();

	void initializeWithThreads(size_t threads);
	void schedule(MyTask);
	void wait() const;

private:
	std::vector<std::thread> workers_;
	SharedQueue<MyTask> queue_;
	std::atomic_uint count_ = 0;
	std::mutex mutex_;
	std::condition_variable condition_;
	std::atomic_bool stop_ = false;
};
#endif

