/**
 * @brief Thread pool that adds new tasks to the SharedQueue and assign them to Workers
 * 
 * @file ThreadPool.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef POOL_H
#define POOL_H
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "MyTask.h"
#include "SharedQueue.h"

/**
 * @brief Thread pool that adds new tasks to the SharedQueue and assign them to Workers
 * 
 */
class ThreadPool
{
public:
	/**
	 * @brief Add new task to the queue
	 * 
	 * @param task 
	 */
	void schedule(MyTask task);

	/**
	 * @brief Create or get Singleton instance of the ThreadPool
	 * 
	 * @return ThreadPool& 
	 */
	static ThreadPool& Instance();

	/**
	 * @brief Start ThreadPool's work
	 * 
	 * @param threads Number of threads
	 */
	void initializeWithThreads(size_t threads);

private:
	/**
	 * @brief Construct a new Thread Pool object
	 * 
	 */
	ThreadPool();

	/**
	 * @brief Destroy the Thread Pool object
	 * 
	 */
	~ThreadPool();

	/**
	 * @brief Tells thread to wait
	 * 
	 */
	void wait() const;

	std::vector<std::thread> workers_;	//! Threads
	SharedQueue<MyTask> queue_;			//!< Queue
	std::atomic_uint count_ = 0;		//!< Current number of tasks in the queue
	std::mutex mutex_;					//!< Mutex
	std::condition_variable condition_;	//!< Condition variable
	std::atomic_bool stop_ = false;		//!< Stop flag
};
#endif

