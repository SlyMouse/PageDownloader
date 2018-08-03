#pragma once
#include <string>
#include "Enums.h"
#include <vector>

class Resource
{
public:
	Resource();
	Resource(std::string root, std::string abs, std::string rel, ResourceType type) : 
		link_root_(root), link_abs_(abs), link_rel_(rel) {};
	std::string link_root_;
	std::string link_abs_;
	std::string link_rel_;
	char *content_;
	size_t content_size_;
	bool is_saved_ = false;
	std::vector<Resource> resources_;
	ResourceType type_;
};

