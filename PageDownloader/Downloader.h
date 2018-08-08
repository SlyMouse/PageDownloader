#ifndef DOWNLOADER_H
#define DONELOADER_H
#include "Resource.h"

class Downloader
{
public:
	static void Download(std::shared_ptr<Resource>, bool to_file);
};

#endif