/**
 * @brief Implementation of the Saver class
 * 
 * @file Saver.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#include "Saver.h"
#include "Downloader.h"
#include "boost/filesystem.hpp"
#include <fstream>
#include <regex>
#include <algorithm>
const static std::regex re("/?/(.*/)(.+)$"); //!< File path and file name from link
const static std::regex re_domain("(?:.+://)(.+)$"); //!< Base domain from link
const static std::regex re_param("(.+)\\?.*"); //!< Link parameters

/**
* @brief Writes resource's downloaded content to the file or downloads resource to file
* 
* @param resource
*/
void Saver::Save(std::shared_ptr<Resource> resource)
{
	std::string file_name = resource->get_link_rel();
	std::string file_path = "/";
	std::smatch match;

	if (std::regex_search(resource->get_link_rel(), match, re))
	{
		std::smatch match_domain;
		std::regex_search(resource->get_link_root(), match_domain, re_domain);
		file_path = match_domain[1].str() + "/" + match[1].str();
		file_name = match[2].str();
	}

	if (std::regex_search(file_name, match, re_param))
		file_name = match[1].str();

	if (resource->get_type() == ResourceType::Page)
	{
		file_name = resource->get_link_root() + "_" + file_name + ".html";
		std::replace(file_name.begin(), file_name.end(), ':', '.');
		file_name.erase(std::remove(file_name.begin(), file_name.end(), '/'), file_name.end());
		file_path = "";
	}

	if (boost::filesystem::exists(resource->get_working_dir() + file_path + file_name))
	{
		resource->set_is_handled(true);
		return;
	}

	if (file_path != "/")
	{
		if (std::regex_search(file_path, match, re_param))
			file_path = match[1].str();
		std::replace(file_path.begin(), file_path.end(), ',', '.');
		std::replace(file_path.begin(), file_path.end(), '|', '.');
		boost::filesystem::create_directories(resource->get_working_dir() + file_path);
	}
	else
		file_path = "";
	
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