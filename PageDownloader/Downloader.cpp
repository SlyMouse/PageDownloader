#include "Downloader.h"
#include <iostream>
#include "curl/curl.h"

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		std::cout << "Not enough memory (realloc returned NULL)" << std::endl;
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void Downloader::Download(std::shared_ptr<Resource> resource, bool to_file)
{
	CURLcode res;
	FILE *file = nullptr;
	struct MemoryStruct chunk;
	chunk.memory = nullptr;
	chunk.size = 0;
	CURL *curl_ = curl_easy_init();
	std::string link_abs = resource->get_link_abs();
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl_, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl_, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.84 Safari/537.36");
	curl_easy_setopt(curl_, CURLOPT_URL, link_abs.c_str());
	//curl_easy_setopt(curl_, CURLOPT_VERBOSE, TRUE);

	if (to_file)
	{
		fopen_s(&file, (resource->get_working_dir() + resource->get_file_name()).c_str(), "wb");
		if (!file)
			return;
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, file);
	}
	else
	{
		chunk.memory = (char *)malloc(1);
		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void *)&chunk);
	}

	res = curl_easy_perform(curl_);
	if (to_file)
		fclose(file);
	if (res != CURLE_OK)
		std::cout << link_abs << ": Failed\nDownloader failed: " << curl_easy_strerror(res) << std::endl;
	else
	{
		long response_code;
		res = curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
		if ((response_code / 100) == 3)
		{
			char *location;
			res = curl_easy_getinfo(curl_, CURLINFO_REDIRECT_URL, &location);

			if ((res == CURLE_OK) && location)
			{
				std::shared_ptr<Resource> redirected_resource(new Resource(*resource));
				redirected_resource->set_link_root(std::string(location));
				redirected_resource->set_link_rel("");
				Downloader::Download(redirected_resource, to_file);
				if (!to_file)
					resource->set_content(redirected_resource->get_content());
				resource->set_is_saved(redirected_resource->get_is_saved());
			}
		}
		else
		{
			std::cout << link_abs << ": Success" << std::endl;
			if (!to_file)
				resource->set_content(std::string(chunk.memory, chunk.size));
			resource->set_is_saved(true);
		}
	}

	if (!to_file)
		free(chunk.memory);

	curl_easy_cleanup(curl_);
}
