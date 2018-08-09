/**
 * @brief Minimal unit
 * 
 * @file Resource.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef RESOURCE_H
#define RESOURCE_H
#include "Enums.h"
#include <vector>
#include <string>

/**
 * @brief Minimal unit
 * 
 */
class Resource
{
public:
	/**
	 * @brief Construct a new Resource object
	 * 
	 * @param[in] root Link to the base domain <protocol>://<hostname>
	 * @param[in] working_dir Root directory for downloading resources
	 * @param[in] rel Path on the website <path>
	 * @param[in] type Type of document
	 */
	Resource(std::string root, std::string working_dir, std::string rel, ResourceType type) :
		link_root_(root), working_dir_(working_dir), link_rel_(rel), type_(type) {};
	
	/**
	 * @brief Construct a new Resource object
	 * 
	 * @param[in] root Link to the base domain <protocol>://<hostname>
	 * @param[in] working_dir Root directory for downloading resources
	 * @param[in] rel Path on the website <path>
	 * @param[in] org Original link
	 * @param[in] type Type of document 
	 */
	Resource(std::string root, std::string working_dir, std::string rel, std::string org, ResourceType type) :
		link_root_(root), working_dir_(working_dir), link_rel_(rel), link_org_(org), type_(type) {};
	
	/**
	 * @brief Get the base domain link
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_link_root();
	
	/**
	 * @brief Set the base domain link
	 * 
	 * @param link Link to the base domain <protocol>://<hostname>
	 */
	void set_link_root(std::string link);

	/**
	 * @brief Get the path on the website
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_link_rel();
	
	/**
	 * @brief Set the path on the website
	 * 
	 * @param link Path on the website
	 */
	void set_link_rel(std::string link);

	/**
	 * @brief Get the orginal link
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_link_org();

	/**
	 * @brief Get the absolute link
	 * 
	 * @return std::string 
	 */
	std::string get_link_abs();
	
	/**
	 * @brief Get the filename
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_file_name();
	
	/**
	 * @brief Set the file name object
	 * 
	 * @param name Name of downloaded file
	 */
	void set_file_name(std::string name);
	
	/**
	 * @brief Get the name of the working directory
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_working_dir();
	
	/**
	 * @brief Get the downloaded content of the resource
	 * 
	 * @return const std::string& 
	 */
	const std::string & get_content();
	
	/**
	 * @brief Set the content of the resource
	 * 
	 * @param content Content of the resource
	 */
	void set_content(std::string content);
	
	/**
	 * @brief Get reference to the downloaded content of the resource
	 * 
	 * @return std::string& 
	 */
	std::string &modify_content();
	
	/**
	 * @brief Get the type of the resource
	 * 
	 * @return ResourceType 
	 */
	ResourceType get_type();

	/**
	 * @brief Tells if resourse saved to the disk
	 * 
	 * @return true 
	 * @return false 
	 */
	bool get_is_saved();
	
	/**
	 * @brief Set flag that resource is saved to the disk or not
	 * 
	 * @param flag 
	 */
	void set_is_saved(bool flag);

	/**
	 * @brief Tells if resourse handled by the saver
	 * 
	 * @return true 
	 * @return false 
	 */
	bool get_is_handled();

	/**
	 * @brief Set flag that resource is handled by the saver
	 * 
	 * @param flag 
	 */
	void set_is_handled(bool flag);

	/**
	 * @brief Get the reference to the vector of child resources
	 * 
	 * @return const std::vector<std::shared_ptr<Resource>>& 
	 */
	const std::vector<std::shared_ptr<Resource>> & get_resources();
	
	/**
	 * @brief Add new resource to list of child reosurces
	 * 
	 * @param resource 
	 * @return std::shared_ptr<Resource> 
	 */
	std::shared_ptr<Resource> add_resource(std::shared_ptr<Resource> &resource);
	
	/**
	 * @brief Remove all handled resources from the list of child resources
	 * 
	 */
	void remove_handled_resources();

private:
	std::string link_root_;   							//!< Link to the base domain <protocol>://<hostname>
	std::string link_rel_;    							//!< Path on the website <path>
	std::string file_name_;								//!< Name of downloaded file
	std::string working_dir_; 							//!< Name of the working directory <hostname>_<epoch_timestamp>
	std::string link_org_;	  							//!< Original link
	std::string content_ = "";							//!< Downloaded content of the resource
	ResourceType type_;									//!< Type of the resource
	bool is_saved_ = false;								//!< Is resource saved to the disk
	bool is_handled_ = false;							//!< Is resource handled by the saver
	std::vector<std::shared_ptr<Resource>> resources_;	//!< Vector of the child resources
};
#endif 