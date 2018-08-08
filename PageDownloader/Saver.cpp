#include "Saver.h"
#include "Downloader.h"
#include "boost/filesystem.hpp"
#include <fstream>
#include <regex>

void Saver::Save(std::shared_ptr<Resource> resource)
{
	std::string file_name = resource->get_link_rel();
	std::string file_path = "";
	std::regex re("(?:https?:)?(?://)?(?:/)?(.+/)(.+)");
	std::smatch match;

	if (std::regex_search(resource->get_link_rel(), match, re))
	{
		file_path = match[1].str();
		file_name = match[2].str();
	}

	std::replace(file_name.begin(), file_name.end(), '?', '_');

	if (resource->get_type() == ResourceType::Page)
		file_name += ".html";

	if (boost::filesystem::exists(resource->get_working_dir() + file_path + file_name))
	{
		resource->set_is_handled(true);
		return;
	}

	if (file_path != "")
	{
		boost::filesystem::create_directories(resource->get_working_dir() + file_path);
	}
	
	resource->set_file_name(file_path + file_name);

	if (resource->get_content() != "")
	{
		std::ofstream file(resource->get_working_dir() + file_path + file_name);
		file << resource->get_content();
		file.close();
		resource->set_is_saved(true);
	}
	else
	{
		Downloader::Download(resource, true);
	}
	
	resource->set_is_handled(true);
}