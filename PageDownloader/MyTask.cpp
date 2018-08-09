/**
 * @brief Implementation of the MyTask class
 * 
 * @file MyTask.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#include "MyTask.h"

/**
	 * @brief Get task's resource
	 * 
	 * @return const std::shared_ptr<Resource> 
	 */
const std::shared_ptr<Resource> MyTask::get_resource() { return resource_; };

/**
	 * @brief Get current target of the task
	 * 
	 * @return TaskTarget 
	 */
TaskTarget MyTask::get_target() { return target_; };

/**
	 * @brief Set task's target
	 * 
	 * @param target 
	 */
void MyTask::set_target(TaskTarget target) { target_ = target; };