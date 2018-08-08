#include "Parser.h"
#include "Worker.h"
#include "Enums.h"
#include "boost/filesystem.hpp"
#include <regex>

Worker::Worker(ThreadPool *threads) : threads_(threads), parser_(new Parser(this)) {}

Worker::~Worker()
{
	delete parser_;
}

void Worker::AddResource(Resource *resource)
{
	TaskTarget target;
	if (resource->type_ == ResourceType::Page || resource->type_ == ResourceType::Css)
		target = TaskTarget::DownloadAndParse;
	else
		target = TaskTarget::Save;
	MyTask task(resource, target);
	threads_->schedule(task);
}

void Worker::Work()
{
	switch (task_->target_)
	{
		case TaskTarget::DownloadAndParse:
			if (task_->resource_->type_ == ResourceType::Page || task_->resource_->type_ == ResourceType::Css)
			{
				Downloader::Download(task_->resource_, false);
				parser_->Parse(task_->resource_);
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