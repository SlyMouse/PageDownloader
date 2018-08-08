#include "Parser.h"

#include <array>
#include <vector>
#include <regex>

#include "Replacer.h"
#include "Enums.h"
#include "Worker.h"


static constexpr std::array<const char*, 12U> whitelist_ = { "png", "jpg", "svg", "bmp", "gif", "woff", "woff2", "tff", "css", "eot", "eot?#iefix", "js" };

bool replace(std::string *str, const std::string &from, const std::string &to) {
	size_t start_pos = (*str).find(from);
	if (start_pos == std::string::npos)
		return false;
	(*str).replace(start_pos, from.length(), to);
	return true;
}

void Parser::Parse(std::shared_ptr<Resource> resource)
{
	std::regex re;
	std::regex link_re;
	std::smatch link_match;
	std::smatch match;
	std::string abs;
	std::string rel;
	std::string content = resource->get_content();
	ResourceType type;

	link_re = std::regex("(href=\"|src=\"|url\\()(.+?)(\\)|\")");
	std::string::const_iterator searchStart(content.cbegin());
	while (std::regex_search(searchStart, content.cend(), link_match, link_re))
	{
		rel = link_match[2].str();
		abs = rel;

		re = std::regex("^/([^/].*)");
		if (std::regex_search(rel, match, re))
			abs = resource->get_link_root() + rel;

		re = std::regex("^//(.*)");
		if (std::regex_search(rel, match, re))
		{
			re = std::regex("^(.+?)//");
			std::regex_search(resource->get_link_root(), match, re);
			std::string protocol = match[1].str();
			abs = protocol + rel;
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

		re = std::regex("\\.((?:.(?!\\.))+)$");
		if (std::regex_search(rel, match, re))
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
			if (rel != abs)
				Replacer::Replace(resource->modify_content(), rel, abs);
			searchStart += link_match.position() + link_match.length();
			continue;
		}

		std::shared_ptr<Resource> found_resource = std::shared_ptr<Resource>(new Resource(resource->get_link_root(), resource->get_working_dir(), abs, rel, type));
		Worker::AddResource(resource->add_resource(found_resource));

		searchStart += link_match.position() + link_match.length();
	}
}
