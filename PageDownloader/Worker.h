#ifndef WORKER_H
#define WORKER_H
#include "Downloader.h"
#include "Saver.h"
#include "ThreadPool.h"
#include "MyTask.h"
#include "Replacer.h"
class Parser;
class Worker
{
public:
	Worker(ThreadPool *);
	~Worker();
	void AddResource(Resource *);
	void Work();
	MyTask *task_;
	Replacer replacer_;
	Parser *parser_;
private:
	ThreadPool *threads_;
};
#endif