/**
 * @brief Replaces resource's links to child resource with path to locally saved copy
 * 
 * @file Replacer.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef REPLACER_H
#define REPLACER_H
#include "Resource.h"

/**
 * @brief Replaces resource's links to child resource with path to locally saved copy
 * 
 */
class Replacer
{
public:
	/**
	 * @brief Find substring in string and replace it with another string
	 * 
	 * @param str Original string
	 * @param from Original substring
	 * @param to Replacement
	 * @return true 
	 * @return false 
	 */
	static bool Replace(std::string &str, const std::string &from, const std::string &to);
	
	/**
	 * @brief Replaces resource's links to child resource with path to locally saved copy
	 * 
	 */
	static void Replace(std::shared_ptr<Resource>);
};
#endif