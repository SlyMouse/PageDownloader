#include "Parser.h"
#include "Enums.h"
#include "Worker.h"
#include <regex>

bool replace(std::string *str, const std::string &from, const std::string &to) {
	size_t start_pos = (*str).find(from);
	if (start_pos == std::string::npos)
		return false;
	(*str).replace(start_pos, from.length(), to);
	return true;
}

void Parser::Parse(Resource *resource)
{
	std::regex re;
	std::regex link_re;
	std::smatch link_match;
	std::smatch match;
	std::string abs;
	std::string rel;
	std::string content = *resource->content_;
	ResourceType type;

	link_re = std::regex("(href=\"|src=\"|url\\()(.+?)(\\)|\")");
	std::string::const_iterator searchStart(content.cbegin());
	while (std::regex_search(searchStart, content.cend(), link_match, link_re))
	{
		rel = link_match[2].str();
		abs = rel;

		re = std::regex("^/([^/].*)");
		if (std::regex_search(rel, match, re))
			abs = resource->link_root_ + rel;

		re = std::regex("^//(.*)");
		if (std::regex_search(rel, match, re))
		{
			re = std::regex("^(.+?)//");
			std::regex_search(resource->link_root_, match, re);
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
				worker_->replacer_.Replace(resource->content_, rel, abs);
			searchStart += link_match.position() + link_match.length();
			continue;
		}

		resource->resources_.push_back(new Resource(resource->link_root_, resource->working_dir_, abs, rel, type));
		worker_->AddResource(resource->resources_.back());

		searchStart += link_match.position() + link_match.length();
	}
}
