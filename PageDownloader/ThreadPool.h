#ifndef POOL_H
#define POOL_H
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "MyTask.h"
#include "SharedQueue.h"

class ThreadPool
{
public:
	void schedule(MyTask);
	static ThreadPool& Instance();
	void initializeWithThreads(size_t threads);

private:
	ThreadPool();
	~ThreadPool();
	void wait() const;

	std::vector<std::thread> workers_;
	SharedQueue<MyTask> queue_;
	std::atomic_uint count_ = 0;
	std::mutex mutex_;
	std::condition_variable condition_;
	std::atomic_bool stop_ = false;
};
#endif

