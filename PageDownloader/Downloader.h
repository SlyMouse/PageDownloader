#pragma once
#include "Resource.h"
#include "curl/curl.h"

class Downloader
{
public:
	~Downloader();
	void Download(Resource &);
private:
	CURL *curl_;
};

