/**
 * @brief Writes resource's downloaded content to the file or downloads resource to file
 * 
 * @file Saver.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */
#ifndef SAVER_H
#define SAVER_H
#include "Resource.h"

/**
 * @brief Writes resource's downloaded content to the file or downloads resource to file
 * 
 */
class Saver
{
public:
	/**
	 * @brief Writes resource's downloaded content to the file or downloads resource to file
	 * 
	 * @param resource
	 */
	static void Save(std::shared_ptr<Resource> resource);
};
#endif
