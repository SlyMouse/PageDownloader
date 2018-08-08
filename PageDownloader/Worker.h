#ifndef WORKER_H
#define WORKER_H
#include "MyTask.h"
class Worker
{
public:
	static void AddResource(std::shared_ptr<Resource>);
	static void Work(MyTask *);
};
#endif