#include "Downloader.h"
#include "curl/curl.h"
#include <iostream>

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
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl_, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl_, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.84 Safari/537.36");
	curl_easy_setopt(curl_, CURLOPT_URL, resource->link_abs_.c_str());

	if (to_file)
	{
		fopen_s(&file, (resource->working_dir_ + resource->file_name_).c_str(), "wb");
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, file);
	}
	else
	{
		chunk.memory = (char *)malloc(1);
		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void *)&chunk);
	}
	
	res = curl_easy_perform(curl_);
	if (res != CURLE_OK)
		std::cout << resource->link_abs_ << ": Failed\nDownloader failed: " << curl_easy_strerror(res) << std::endl;
	else
	{
		std::cout << resource->link_abs_ << ": Success" << std::endl;
		if(!to_file)
			resource->content_ = std::string(chunk.memory, chunk.size);
		resource->is_saved_ = true;
	}

	if (to_file)
		fclose(file);
	else
		free(chunk.memory);

	curl_easy_cleanup(curl_);
}
