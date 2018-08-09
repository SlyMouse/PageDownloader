/**
 * @brief Implementation of the Resource class
 * 
 * @file Resource.cpp
 * @author Artyom Pashkin
 * @date 09.08.2018
 */
#include "Resource.h"
#include <algorithm>

/**
	 * @brief Get the base domain link
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_link_root() { return link_root_; };

/**
	 * @brief Set the base domain link
	 * 
	 * @param link Link to the base domain <protocol>://<hostname>
	 */
void Resource::set_link_root(std::string link) { link_root_ = link; };

/**
	 * @brief Get the path on the website
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_link_rel() { return link_rel_; };

/**
	 * @brief Set the path on the website
	 * 
	 * @param link Path on the website
	 */
void Resource::set_link_rel(std::string link) { link_rel_ = link; };

/**
	 * @brief Get the orginal link
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_link_org() { return link_org_; };

/**
	 * @brief Get the filename
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_file_name() { return file_name_; };

/**
	 * @brief Set the file name object
	 * 
	 * @param name Name of downloaded file
	 */
void Resource::set_file_name(std::string name) { file_name_ = name; };

/**
	 * @brief Get the name of the working directory
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_working_dir() { return working_dir_; };

/**
	 * @brief Get the downloaded content of the resource
	 * 
	 * @return const std::string& 
	 */
const std::string & Resource::get_content() { return content_; };

/**
	 * @brief Set the content of the resource
	 * 
	 * @param content Content of the resource
	 */
void Resource::set_content(std::string content) { content_ = content; };

/**
	 * @brief Get reference to the downloaded content of the resource
	 * 
	 * @return std::string& 
	 */
std::string & Resource::modify_content() { return content_; };

/**
	 * @brief Get the type of the resource
	 * 
	 * @return ResourceType 
	 */
ResourceType Resource::get_type() { return type_; };

	/**
	 * @brief Tells if resourse saved to the disk
	 * 
	 * @return true 
	 * @return false 
	 */
bool Resource::get_is_saved() { return is_saved_; };

/**
	 * @brief Set flag that resource is saved to the disk or not
	 * 
	 * @param flag 
	 */
void Resource::set_is_saved(bool flag) { is_saved_ = flag; };

/**
	 * @brief Tells if resourse handled by the saver
	 * 
	 * @return true 
	 * @return false 
	 */
bool Resource::get_is_handled() { return is_handled_; };

/**
	 * @brief Set flag that resource is handled by the saver
	 * 
	 * @param flag 
	 */
void Resource::set_is_handled(bool flag) { is_handled_ = flag; };

/**
	 * @brief Get the reference to the vector of child resources
	 * 
	 * @return const std::vector<std::shared_ptr<Resource>>& 
	 */
const std::vector<std::shared_ptr<Resource>> & Resource::get_resources() { return resources_; }

/**
	 * @brief Add new resource to list of child reosurces
	 * 
	 * @param resource 
	 * @return std::shared_ptr<Resource> 
	 */
std::shared_ptr<Resource> Resource::add_resource(std::shared_ptr<Resource> &resource)
{
	resources_.push_back(std::move(resource));
	return resources_.back();
}

/**
 * @brief Tells if resource is handled by the saver
 * 
 * @param res 
 * @return true 
 * @return false 
 */
bool isHandled(std::shared_ptr<Resource> res)
{
	return res->get_is_handled();
}

/**
	 * @brief Remove all handled resources from the list of child resources
	 * 
	 */
void Resource::remove_handled_resources()
{
	resources_.erase(std::remove_if(resources_.begin(), resources_.end(), isHandled), resources_.end());
}

/**
	 * @brief Get the absolute link
	 * 
	 * @return std::string 
	 */
std::string Resource::get_link_abs() { return link_root_ + link_rel_; }