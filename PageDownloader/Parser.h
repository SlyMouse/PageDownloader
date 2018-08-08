#ifndef PARSER_H
#define PARSER_H
#include "Resource.h"
class Worker;
class Parser
{
public:
	static void Parse(std::shared_ptr<Resource>);
};
#endif