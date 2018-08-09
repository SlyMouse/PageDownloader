#include "Parser.h"

#include <array>
#include <vector>
#include <regex>

#include "Replacer.h"
#include "Enums.h"
#include "Worker.h"


static constexpr std::array<const char*, 11U> whitelist_ = { "png", "jpg", "svg", "bmp", "gif", "woff", "woff2", "tff", "css", "eot", "js" };
static const std::regex re_link("(href=\"|src=\"|url\\(|&quot;)(.+?)(\\)|\"|&quot;)"); // Link inside href="" / src="" / url()
static const std::regex re_abs("^((?:.+)?//)(?:www\\.)?([^/].+?\\.[^/]+?)(/(?:.+)|$)"); //If starts with "//" or "<protocol>://". 1 - Protocol, 2 - Hostname, 3 - Path
static const std::regex re_format = std::regex("\\.((?:.(?!\\.))+?)($|\\?)"); //File format

bool replace(std::string *str, const std::string &from, const std::string &to) {
	size_t start_pos = (*str).find(from);
	if (start_pos == std::string::npos)
		return false;
	(*str).replace(start_pos, from.length(), to);
	return true;
}

void Parser::Parse(std::shared_ptr<Resource> resource)
{
	std::smatch link_match;
	std::smatch match;
	std::string content = resource->get_content();
	ResourceType type;
	
	std::string::const_iterator searchStart(content.cbegin());
	while (std::regex_search(searchStart, content.cend(), link_match, re_link))
	{
		std::regex re;
		std::string original = link_match[2].str();
		if (original.size() > 200)
		{
			searchStart += link_match.position() + link_match.length();
			continue;
		}
		std::string hostname;
		std::string path = link_match[2].str();

		re = std::regex("^/([^/].*)"); //If starts with "/"
		if (std::regex_search(path, match, re))
			hostname = resource->get_link_root();
		else if (std::regex_search(path, match, re_abs))
		{
			std::string protocol = match[1].str();
			hostname = match[2].str();
			path = match[3].str();
			if (protocol == "//")
			{
				re = std::regex("^(.+?//)"); // Protocol from parent hostname
				std::regex_search(resource->get_link_root(), match, re);
				hostname = match[1].str() + hostname;
			}
			else
				hostname = protocol + hostname;
		}
		else
		{
			searchStart += link_match.position() + link_match.length();
			continue;
		}
		
		/* Skip if found link's base domain is different from original domain
		re = std::regex("(https?://.*?)/");
		if (std::regex_search(abs, match, re))
		{
			std::string root = match[1].str();
			if (resource->link_root_ != root)
			{
				searchStart += link_match.position() + link_match.length();
				continue;
			}
		}
		else
		{
			searchStart += link_match.position() + link_match.length();
			continue;
		}*/

		if (std::regex_search(path, match, re_format))
		{
			std::string format = match[1].str();
			/*if (format.compare("html"))
				type = ResourceType::Page;
			else*/ 
			if (format == "css")
				type = ResourceType::Css;
			else if (std::find(whitelist_.begin(), whitelist_.end(), format) != whitelist_.end())
				type = ResourceType::Other;
			else
			{
				searchStart += link_match.position() + link_match.length();
				continue;
			}
		}
		else
		{
			if (link_match[2].str() != (hostname + path))
				Replacer::Replace(resource->modify_content(), link_match[2].str(), hostname + path);
			searchStart += link_match.position() + link_match.length();
			continue;
		}

		std::shared_ptr<Resource> found_resource = std::shared_ptr<Resource>(new Resource(hostname, resource->get_working_dir(), path, original, type));
		Worker::AddResource(resource->add_resource(found_resource));

		searchStart += link_match.position() + link_match.length();
	}
}
