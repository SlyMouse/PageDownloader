#include "stdafx.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool()
{
	curl_global_init(CURL_GLOBAL_ALL);
}


ThreadPool::~ThreadPool()
{
	curl_global_cleanup();
}
