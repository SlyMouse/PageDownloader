/**
 * @brief Implementation of the SharedQueue class
 * 
 * @file SharedQueue.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#include "SharedQueue.h"
#include "MyTask.h"

/**
 * @brief Returns first element
 * 
 * @tparam T Type of elements in the queue
 * @return T& 
 */
template <typename T>
T& SharedQueue<T>::front()
{
	std::unique_lock<std::mutex> mlock(mutex_);
	while (queue_.empty())
	{
		cond_.wait(mlock);
	}
	return queue_.front();
}

/**
 * @brief Removes the first element
 * 
 * @tparam T Type of elements in the queue
 */
template <typename T>
void SharedQueue<T>::pop_front()
{
	std::unique_lock<std::mutex> mlock(mutex_);
	while (queue_.empty())
	{
		cond_.wait(mlock);
	}
	queue_.pop_front();
}

/**
 * @brief  Adds a new element
 * 
 * @tparam T Type of elements in the queue
 * @param item 
 */
template <typename T>
void SharedQueue<T>::push_back(const T& item)
{
	std::unique_lock<std::mutex> mlock(mutex_);
	queue_.push_back(item);
	mlock.unlock();     // unlock before notificiation to minimize mutex con
	cond_.notify_one(); // notify one waiting thread

}

/**
 * @brief  Adds a new element
 * 
 * @tparam T Type of elements in the queue
 * @param item 
 */
template <typename T>
void SharedQueue<T>::push_back(T&& item)
{
	std::unique_lock<std::mutex> mlock(mutex_);
	queue_.push_back(std::move(item));
	mlock.unlock();     // unlock before notificiation to minimize mutex con
	cond_.notify_one(); // notify one waiting thread

}

/**
 * @brief Current size of queue
 * 
 * @tparam T Type of elements in the queue
 * @return int 
 */
template <typename T>
int SharedQueue<T>::size()
{
	std::unique_lock<std::mutex> mlock(mutex_);
	int size = queue_.size();
	mlock.unlock();
	return size;
}

/**
 * @brief Tells if queue is empty or not
 * 
 * @tparam T Type of elements in the queue
 * @return true 
 * @return false 
 */
template <typename T>
bool SharedQueue<T>::empty()
{
	std::unique_lock<std::mutex> mlock(mutex_);
	bool empty = queue_.empty();
	mlock.unlock();
	return empty;
}

/**
 * @brief Initializatoin of SharedQueue with MyTask elements
 */
template class SharedQueue<MyTask>;