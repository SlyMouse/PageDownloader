#ifndef RESOURCE_H
#define RESOURCE_H
#include "Enums.h"
#include <vector>
#include <string>

class Resource
{
public:
	Resource(std::string root, std::string working_dir, std::string rel, ResourceType type) : link_root_(root), working_dir_(working_dir), link_rel_(rel), link_abs_(rel), type_(type) {};
	Resource(std::string root, std::string working_dir, std::string abs, std::string rel, ResourceType type) :
		link_root_(root), working_dir_(working_dir), link_abs_(abs), link_rel_(rel), type_(type) {};
	
	const std::string & get_link_root();
	
	const std::string & get_link_abs();
	void set_link_abs(std::string link);
	
	const std::string & get_link_rel();
	void set_link_rel(std::string link);
	
	const std::string & get_file_name();
	void set_file_name(std::string name);
	
	const std::string & get_working_dir();
	
	const std::string & get_content();
	void set_content(std::string content);
	std::string *modify_content();
	
	ResourceType get_type();

	bool get_is_saved();
	void set_is_saved(bool flag);

	bool get_is_handled();
	void set_is_handled(bool flag);

	const std::vector<std::shared_ptr<Resource>> & get_resources();
	std::shared_ptr<Resource> add_resource(std::shared_ptr<Resource> &resource);
	void remove_handled_resources();

private:
	std::string link_root_;
	std::string link_abs_;
	std::string link_rel_;
	std::string file_name_;
	std::string working_dir_;
	std::string content_ = "";
	ResourceType type_;
	bool is_saved_ = false;
	bool is_handled_ = false;
	std::vector<std::shared_ptr<Resource>> resources_;
};
#endif 