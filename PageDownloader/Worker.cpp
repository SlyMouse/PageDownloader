#include "Worker.h"
#include <regex>
#include "Enums.h"
#include "ThreadPool.h"
#include "Downloader.h"
#include "Saver.h"
#include "Replacer.h"
#include "Parser.h"
#include "boost/filesystem.hpp"


void Worker::AddResource(std::shared_ptr<Resource> resource)
{
	TaskTarget target;
	if (resource->type_ == ResourceType::Page || resource->type_ == ResourceType::Css)
		target = TaskTarget::DownloadAndParse;
	else
		target = TaskTarget::Save;
	MyTask task(resource, target);
	ThreadPool::Instance().schedule(task);
}

void Worker::Work(MyTask *task_)
{
	switch (task_->target_)
	{
		case TaskTarget::DownloadAndParse:
			if (task_->resource_->type_ == ResourceType::Page || task_->resource_->type_ == ResourceType::Css)
			{
				Downloader::Download(task_->resource_, false);
				Parser::Parse(task_->resource_);
				task_->target_ = TaskTarget::Replace;
			}

		case TaskTarget::Replace: 
			Replacer::Replace(task_->resource_); 
			if(task_->resource_->resources_.size() == 0)
				task_->target_ = TaskTarget::Save;
			else break;

		case TaskTarget::Save: Saver::Save(task_->resource_); task_->target_ = TaskTarget::Done;

		case TaskTarget::Done: return;
	}
}