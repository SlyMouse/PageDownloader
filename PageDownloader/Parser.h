#pragma once
#include "Resource.h"
#include "Worker.h"

class Parser
{
public:
	Parser() {};
	Parser(Worker *worker) : worker_(worker) {};
	void Parse(Resource &);
	void Replace(Resource &child, Resource &parent);
	void Replace(std::string rel, std::string abs, Resource &parent);
private:
	Worker *worker_;
	std::vector<std::string>whitelist_ = {"png", "jpg", "svg", "bmp", "gif"};
};