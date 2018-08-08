#ifndef DOWNLOADER_H
#define DONELOADER_H
#include "Resource.h"
#include "curl/curl.h"

class Downloader
{
public:
	Downloader() {};
	~Downloader();
	void Download(Resource *);
private:
	CURL *curl_;
};

#endif