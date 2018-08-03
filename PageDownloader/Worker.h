#pragma once
#include "Downloader.h"
#include "Parser.h"
#include "Saver.h"
#include "ThreadPool.h"
#include "MyTask.h"

class Worker
{
public:
	Worker();
	void AddResource(Resource resource);
	void ChangeDir();
	void Work();
private:
	Downloader downloader_;
	Parser parser_;
	Saver saver_;
	ThreadPool *threads_;
	MyTask task_;
};
