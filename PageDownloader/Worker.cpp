#include "stdafx.h"
#include "Parser.h"
#include "Worker.h"
#include "boost/filesystem.hpp"
#include <regex>
#include <direct.h>
#include "Enums.h"

Worker::Worker(ThreadPool *threads) : downloader_(), saver_(), replacer_(), threads_(threads), parser_(new Parser(this)) {}

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
				downloader_.Download(task_->resource_);
				parser_->Parse(task_->resource_);
				task_->target_ = TaskTarget::Replace;
			}

		case TaskTarget::Replace: 
			replacer_.Replace(task_->resource_); 
			if(task_->resource_->resources_.size() == 0)
				task_->target_ = TaskTarget::Save;
			else break;

		case TaskTarget::Save: saver_.Save(task_->resource_); task_->target_ = TaskTarget::Done;

		case TaskTarget::Done: return;
	}
}