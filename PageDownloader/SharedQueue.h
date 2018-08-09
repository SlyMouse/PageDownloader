/**
 * @brief Template of a thread-safe queue
 * 
 * @file SharedQueue.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef QUEUE_H
#define QUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @brief Template of a thread-safe queue
 * 
 * @tparam T Type of elements in the queue
 */
template <typename T>
class SharedQueue
{
public:
	/**
	* @brief Returns the first element
	* 
	* @return T& 
	*/
	T& front();

	/**
	 * @brief Removes the first element
	 * 
	 */
	void pop_front();

	/**
	 * @brief Adds a new element
	 * 
	 * @param item 
	 */
	void push_back(const T& item);

	/**
	 * @brief Adds a new element
	 * 
	 * @param item 
	 */
	void push_back(T&& item);

	/**
	 * @brief Current size of queue
	 * 
	 * @return int 
	 */
	int size();

	/**
	 * @brief Tells if queue is empty or not
	 * 
	 * @return true 
	 * @return false 
	 */
	bool empty();

private:
	std::deque<T> queue_;			//!< Queue
	std::mutex mutex_;				//!< Mutex
	std::condition_variable cond_;	//!< Condition variable for queue
};
#endif