#ifndef  MYTASK_H
#define MYTASK_H
#include "Resource.h"
#include "Enums.h"

class MyTask
{
public:
	MyTask() {};
	MyTask(std::shared_ptr<Resource> resource, TaskTarget status) : resource_(resource), target_(status) {};
	std::shared_ptr<Resource> resource_;
	TaskTarget target_;
};
#endif