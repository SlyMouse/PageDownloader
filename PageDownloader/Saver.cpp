#include "Saver.h"
#include "Downloader.h"
#include "boost/filesystem.hpp"
#include <fstream>
#include <regex>

void Saver::Save(Resource *resource)
{
	std::string file_name = resource->link_rel_;
	std::string file_path = "";
	std::regex re("(?:https?:)?(?://)?(?:/)?(.+/)(.+)");
	std::smatch match;

	if (std::regex_search(resource->link_rel_, match, re))
	{
		file_path = match[1].str();
		file_name = match[2].str();
	}

	std::replace(file_name.begin(), file_name.end(), '?', '_');

	if (resource->type_ == ResourceType::Page)
		file_name += ".html";

	if (boost::filesystem::exists(resource->working_dir_ + file_path + file_name))
	{
		resource->is_handled_ = true;
		return;
	}

	if (file_path != "")
	{
		boost::filesystem::create_directories(resource->working_dir_ + file_path);
	}
	
	resource->file_name_ = file_path + file_name;

	if (resource->content_)
	{
		std::ofstream file(resource->working_dir_ + file_path + file_name);
		file << *(resource->content_);
		file.close();
		resource->is_saved_ = true;
	}
	else
	{
		Downloader::Download(resource, true);
	}
	
	resource->is_handled_ = true;
}