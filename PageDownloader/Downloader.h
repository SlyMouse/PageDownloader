/**
 * @brief Downloads web resource to the disk or memory with help of libcurl
 * 
 * @file Downloader.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef DOWNLOADER_H
#define DONELOADER_H
#include "Resource.h"

/**
 * @brief Downloads web resource to the disk or memory with help of libcurl
 * 
 */
class Downloader
{
public:
	/**
	 * @brief Downloads web resource to the disk or memory with help of libcurl
	 * 
	 * @param resource Resource
	 * @param to_file Tells if resource should be downloaded to file or memory
	 */
	static void Download(std::shared_ptr<Resource> resource, bool to_file);
};

#endif