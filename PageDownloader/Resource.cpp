#include "Resource.h"
#include <algorithm>

const std::string & Resource::get_link_root() { return link_root_; };

const std::string & Resource::get_link_abs() { return link_abs_; };
void Resource::set_link_abs(std::string link) { link_abs_ = link; };

const std::string & Resource::get_link_rel() { return link_rel_; };
void Resource::set_link_rel(std::string link) { link_rel_ = link; };

const std::string & Resource::get_file_name() { return file_name_; };
void Resource::set_file_name(std::string name) { file_name_ = name; };

const std::string & Resource::get_working_dir() { return working_dir_; };

const std::string & Resource::get_content() { return content_; };
void Resource::set_content(std::string content) { content_ = content; };
std::string * Resource::modify_content() { return &content_; };

ResourceType Resource::get_type() { return type_; };

bool Resource::get_is_saved() { return is_saved_; };
void Resource::set_is_saved(bool flag) { is_saved_ = flag; };

bool Resource::get_is_handled() { return is_handled_; };
void Resource::set_is_handled(bool flag) { is_handled_ = flag; };

const std::vector<std::shared_ptr<Resource>> & Resource::get_resources() { return resources_; }

std::shared_ptr<Resource> Resource::add_resource(std::shared_ptr<Resource> &resource)
{
	resources_.push_back(std::move(resource));
	return resources_.back();
}

bool isHandled(std::shared_ptr<Resource> res)
{
	return res->get_is_handled();
}

void Resource::remove_handled_resources()
{
	resources_.erase(std::remove_if(resources_.begin(), resources_.end(), isHandled), resources_.end());
}