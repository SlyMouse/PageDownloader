#ifndef REPLACER_H
#define REPLACER_H
#include "Resource.h"

class Replacer
{
public:
	static bool Replace(std::string *str, const std::string &from, const std::string &to);
	void Replace(Resource *);
};
#endif