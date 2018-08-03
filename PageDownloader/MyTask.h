#pragma once
#include "Resource.h"
#include "Enums.h"

class MyTask
{
public:
	MyTask() {};
	MyTask(Resource resource, TaskTarget status) : resource_(resource), target_(status) {};
	void SetStatus(TaskTarget status);
	Resource resource_;
	TaskTarget target_;
};

