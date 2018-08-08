#ifndef  MYTASK_H
#define MYTASK_H
#include "Resource.h"
#include "Enums.h"

class MyTask
{
public:
	MyTask() {};
	MyTask(std::shared_ptr<Resource> resource, TaskTarget status) : resource_(resource), target_(status) {};

	const std::shared_ptr<Resource> get_resource();

	TaskTarget get_target();
	void set_target(TaskTarget target);

private:
	std::shared_ptr<Resource> resource_;
	TaskTarget target_;
};
#endif