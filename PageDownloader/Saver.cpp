#include "stdafx.h"
#include "Saver.h"
#include "boost/filesystem.hpp"
#include <fstream>
#include <regex>

void Saver::Save(Resource &resource)
{
	std::ofstream file;
	std::string file_path = "/";

	std::regex re("(.*/).*");
	std::smatch match;

	if (std::regex_search(resource.link_rel_, match, re))
		file_path = match[1].str();

	if (file_path != "/")
	{
		boost::filesystem::path path{ file_path };
		boost::filesystem::create_directories(path);
	}

	file.open(resource.link_rel_, std::ofstream::out);
	file.write(resource.content_, resource.content_size_);
	file.close();

	resource.is_saved_ = true;
}