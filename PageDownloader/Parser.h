/**
 * @brief Parse resources's content
 * 
 * @file Parser.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef PARSER_H
#define PARSER_H
#include "Resource.h"
class Worker;

/**
 * @brief Parse resources's content
 * 
 */
class Parser
{
public:
	/**
	 * @brief Parse resources's content 
	 * 
	 * @param resource
	 */
	static void Parse(std::shared_ptr<Resource> resource);
};
#endif