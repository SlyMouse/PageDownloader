#include "stdafx.h"
#include "Downloader.h"

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		printf("Not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

Downloader::~Downloader()
{
	curl_easy_cleanup(curl_);
}

void Downloader::Download(Resource &resource)
{
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = (char *)malloc(1);
	chunk.size = 0;

	curl_ = curl_easy_init();
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(curl_, CURLOPT_URL, resource.link_abs_.c_str());
	curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void *)&chunk);

	res = curl_easy_perform(curl_);
	if (res != CURLE_OK)
		fprintf(stderr, "Downloader failed: %s\n", curl_easy_strerror(res));
	else
	{
		resource.content_ = chunk.memory;
		resource.content_size_ = chunk.size;
	}
}
