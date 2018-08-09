/**
 * @brief Implementation of the Worker class
 * 
 * @file Worker.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#include "Worker.h"
#include <regex>
#include "Enums.h"
#include "ThreadPool.h"
#include "Downloader.h"
#include "Saver.h"
#include "Replacer.h"
#include "Parser.h"
#include "boost/filesystem.hpp"

/**
	 * @brief Sends new resources to the ThreadPool
	 * 
	 * @param resource New resource
	 */
void Worker::AddResource(std::shared_ptr<Resource> resource)
{
	TaskTarget target;
	if (resource->get_type() == ResourceType::Page || resource->get_type() == ResourceType::Css)
		target = TaskTarget::DownloadAndParse;
	else
		target = TaskTarget::Save;
	MyTask task(resource, target);
	ThreadPool::Instance().schedule(task);
}

/**
	 * @brief Sends resource to required handler
	 * 
	 * @param task 
	 */
void Worker::Work(MyTask *task_)
{
	switch (task_->get_target())
	{
		case TaskTarget::DownloadAndParse:
			if (task_->get_resource()->get_type() == ResourceType::Page || task_->get_resource()->get_type() == ResourceType::Css)
			{
				Downloader::Download(task_->get_resource(), false);
				Parser::Parse(task_->get_resource());
				task_->set_target(TaskTarget::Replace);
			}

		case TaskTarget::Replace: 
			Replacer::Replace(task_->get_resource()); 
			if(task_->get_resource()->get_resources().size() == 0)
				task_->set_target(TaskTarget::Save);
			else break;

		case TaskTarget::Save: Saver::Save(task_->get_resource()); task_->set_target(TaskTarget::Done);

		case TaskTarget::Done: return;
	}
}