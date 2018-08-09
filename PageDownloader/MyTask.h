/**
 * @brief Task has pointer to resource that he must handle and target for how it suppose to handle it
 * 
 * @file MyTask.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef  MYTASK_H
#define MYTASK_H
#include "Resource.h"
#include "Enums.h"

/**
 * @brief Task has pointer to resource that he must handle and target for how it suppose to handle it
 * 
 */
class MyTask
{
public:
	/**
	 * @brief Construct a new My Task object
	 * 
	 */
	MyTask() {};

	/**
	 * @brief Construct a new My Task object
	 * 
	 * @param resource Resource that task must handle
	 * @param status Target for task
	 */
	MyTask(std::shared_ptr<Resource> resource, TaskTarget status) : resource_(resource), target_(status) {};

	/**
	 * @brief Get task's resource
	 * 
	 * @return const std::shared_ptr<Resource> 
	 */
	const std::shared_ptr<Resource> get_resource();

	/**
	 * @brief Get current target of the task
	 * 
	 * @return TaskTarget 
	 */
	TaskTarget get_target();

	/**
	 * @brief Set task's target
	 * 
	 * @param target 
	 */
	void set_target(TaskTarget target);

private:
	std::shared_ptr<Resource> resource_;	//!< Resource that task is handling
	TaskTarget target_;						//!< Task's current target
};
#endif