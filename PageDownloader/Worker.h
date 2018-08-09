/**
 * @brief Worker sends resource to required handler and sends new resources to the ThreadPool
 * 
 * @file Worker.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef WORKER_H
#define WORKER_H
#include "MyTask.h"

/**
 * @brief Worker sends resource to required handler and sends new resources to the ThreadPool
 * 
 */
class Worker
{
public:
	/**
	 * @brief Sends new resources to the ThreadPool
	 * 
	 * @param resource New resource
	 */
	static void AddResource(std::shared_ptr<Resource> resource);\

	/**
	 * @brief Sends resource to required handler
	 * 
	 * @param task 
	 */
	static void Work(MyTask *task);
};
#endif