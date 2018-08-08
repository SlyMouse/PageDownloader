#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include "Enums.h"
#include <vector>

class Resource
{
public:
	Resource();
	~Resource() { if(content_) delete content_;};
	Resource(std::string root, std::string working_dir, std::string rel, ResourceType type) : link_root_(root), working_dir_(working_dir), link_rel_(rel), link_abs_(rel), type_(type) {};
	Resource(std::string root, std::string working_dir, std::string abs, std::string rel, ResourceType type) :
		link_root_(root), working_dir_(working_dir), link_abs_(abs), link_rel_(rel), type_(type) {};
	std::string link_root_;
	std::string link_abs_;
	std::string link_rel_;
	std::string file_name;
	std::string working_dir_;
	std::string *content_ = nullptr;
	size_t content_size_;
	bool is_saved_ = false;
	bool is_handled = false;
	std::vector<Resource *> resources_;
	ResourceType type_;
};
#endif 