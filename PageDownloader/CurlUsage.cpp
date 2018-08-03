#include "stdafx.h"
#include "CurlUsage.h"
#include "boost/filesystem.hpp"

std::string DownloadHtml(std::string link)
{
	CURL *curl;
	FILE *page_file;
	std::string file_name = link + ".html";
	std::regex re("(https?://)?(www\.)?(.+\..+)");
	std::smatch match;
	if (std::regex_search(file_name, match, re))
		file_name = match[3].str();
	std::replace(file_name.begin(), file_name.end(), '/', '_');

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		fopen_s(&page_file, file_name.c_str(), "wb");
		if (page_file)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, page_file);
			curl_easy_perform(curl);
			fclose(page_file);
		}

		curl_easy_cleanup(curl);
	}

	return file_name;
}

std::string SaveLocally(std::string link)
{
	CURL *curl;
	FILE *file;
	std::string file_name;
	std::string file_path;
	std::regex re;
	std::smatch match;
	
	re = std::regex("//.*?/(.*)/");
	if (std::regex_search(link, match, re))
		file_path = match[1].str();

	re = std::regex("//.*?/(.*)");
	if (std::regex_search(link, match, re))
		file_name = match[1].str();

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		if (file_path != "")
		{
			boost::filesystem::path path{ file_path };
			boost::filesystem::create_directories(path);
		}

		std::replace(file_name.begin(), file_name.end(), '?', '_');
		fopen_s(&file, file_name.c_str(), "wb");
		if (file)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
			curl_easy_perform(curl);
			fclose(file);
		}

		curl_easy_cleanup(curl);
	}

	return file_name;
}
