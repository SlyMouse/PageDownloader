#include "stdafx.h"
#include "Worker.h"
#include "boost/filesystem.hpp"
#include <regex>
#include <direct.h>
#include "Enums.h"

Worker::Worker()
{
	downloader_ = Downloader();
	parser_ = Parser(this);
	saver_ = Saver();
}

void Worker::ChangeDir()
{
	std::regex re("(https?://)?(www\.)?(.+\..+)");
	std::smatch match;
	std::string path;
	if (std::regex_search(task_.resource_.link_rel_, match, re))
		path = match[3].str();
	std::replace(path.begin(), path.end(), '/', '_');
	_chdir("..");
	boost::filesystem::create_directory(path);
	_chdir(path.c_str());
}

void Worker::AddResource(Resource resource)
{
	TaskTarget target;
	if (resource.type_ == Page || resource.type_ == Css)
		target = DownloadAndParse;
	else
		target = Save;
	task_ = MyTask(resource, target);
	threads_->schedule(task_);
}

void Worker::Work()
{
	switch (task_.target_)
	{
		case SetRoot: ChangeDir(); task_.target_ = DownloadAndParse;
		
		case DownloadAndParse:
			if (task_.resource_.type_ == Page || task_.resource_.type_ == Css)
			{
				downloader_.Download(task_.resource_);
				parser_.Parse(task_.resource_);
				task_.target_ = Save;
			}

		case Save: saver_.Save(task_.resource_); task_.target_ = Done;
	}
}