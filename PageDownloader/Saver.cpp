#include "stdafx.h"
#include "Saver.h"
#include "boost/filesystem.hpp"
#include <fstream>
#include <regex>

Saver::Saver() {}

Saver::~Saver()
{
	curl_easy_cleanup(curl_);
}

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

	if (boost::filesystem::exists(resource->working_dir_ + file_path + file_name))
	{
		resource->is_handled = true;
		return;
	}

	if (file_path != "")
	{
		boost::filesystem::create_directories(resource->working_dir_ + file_path);
	}
	
	if (resource->content_)
	{
		if (resource->type_ == ResourceType::Page)
			file_name += ".html";
		std::ofstream file(resource->working_dir_ + file_path + file_name);
		file << *(resource->content_);
		file.close();
	}
	else
	{
		FILE *file;
		fopen_s(&file, (resource->working_dir_ + file_path + file_name).c_str(), "wb");
		curl_ = curl_easy_init();
		curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl_, CURLOPT_URL, resource->link_abs_.c_str());
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, file);
		//curl_easy_setopt(curl_, CURLOPT_VERBOSE, TRUE);

		CURLcode res = curl_easy_perform(curl_);
		fclose(file);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "Downloader failed: %s\n", curl_easy_strerror(res));
			resource->is_handled = true;
			return;
		}
	}

	resource->file_name = file_path + file_name;
	resource->is_saved_ = true;
	resource->is_handled = true;
}