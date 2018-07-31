#include "stdafx.h"
#include "CurlUsage.h"

std::string DownloadHtml(std::string website)
{
	CURL *curl;
	FILE *page_file;
	std::string file_name = website + ".html";
	
	std::regex re("\/\/(.+)");
	std::smatch match;
	if (std::regex_search(file_name, match, re))
		file_name = match[1].str();
	std::replace(file_name.begin(), file_name.end(), '/', '_');

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, website);
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
