#ifndef PARSER_H
#define PARSER_H
#include "Resource.h"
class Worker;
class Parser
{
public:
	Parser(Worker *worker) : worker_(worker) {};
	void Parse(Resource *);
	Worker *worker_;
private:
	std::vector<std::string>whitelist_ = {"png", "jpg", "svg", "bmp", "gif", "woff", "woff2", "tff", "css", "eot", "eot?#iefix", "js"};
};
#endif