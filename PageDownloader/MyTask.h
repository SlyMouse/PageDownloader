#ifndef  MYTASK_H
#define MYTASK_H
#include "Resource.h"
#include "Enums.h"

class MyTask
{
public:
	MyTask() {};
	MyTask(Resource *resource, TaskTarget status) : resource_(resource), target_(status) {};
	Resource *resource_;
	TaskTarget target_;
};
#endif