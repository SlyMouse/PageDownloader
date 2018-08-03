#include "stdafx.h"
#include "Parser.h"
#include "Enums.h"
#include <regex>

bool replace(std::string *str, const std::string &from, const std::string &to) {
	size_t start_pos = (*str).find(from);
	if (start_pos == std::string::npos)
		return false;
	(*str).replace(start_pos, from.length(), to);
	return true;
}

void Parser::Parse(Resource &resource)
{
	std::regex re;
	std::smatch match;
	std::smatch format_match;
	std::string abs;
	std::string rel;
	std::string *content = (std::string *)resource.content_;
	ResourceType type;

	re = std::regex("(href=\"|src = \"|url\()(.*?)(\)|\")");
	std::string::const_iterator searchStart((*content).begin());
	while (std::regex_search(searchStart, (*content).cend(), match, re))
	{
		rel = match[2].str();
		abs = rel;

		re = std::regex("(https?://.*?)/");
		if (std::regex_search(rel, match, re))
		{
			std::string root = match[1].str();
			if (resource.link_root_ != root)
			{
				searchStart += match.position() + match.length();
				continue;
			}
		}

		re = std::regex("^/([^/].*)");
		if (std::regex_search(rel, match, re))
			abs = resource.link_root_ + rel;

		re = std::regex("^//(.*)");
		if (std::regex_search(rel, match, re))
		{
			re = std::regex("^(.+?)//");
			std::regex_search(resource.link_root_, match, re);
			std::string protocol = match[1].str();
			abs = protocol + rel;
		}

		re = std::regex("\.((?:.(?!\.))+)$");
		if (std::regex_search(rel, format_match, re))
		{
			std::string format = format_match[1].str();
			if (format.compare("html"))
				type = Page;
			else if (format.compare("css"))
				type = Css;
			else if (std::find(whitelist_.begin(), whitelist_.end(), format) != whitelist_.end())
				type = Other;
			else
			{
				searchStart += match.position() + match.length();
				continue;
			}
		}
		else
		{
			if (rel != abs)
				Replace(rel, abs, resource);
			searchStart += match.position() + match.length();
			continue;
		}

		Resource found_resource(resource.link_root_, abs, rel, type);

		resource.resources_.push_back(found_resource);
		worker_->AddResource(resource.resources_.back());

		searchStart += match.position() + match.length();
	}

	while (resource.resources_.size != 0)
	{
		for (std::vector<Resource>::iterator i = resource.resources_.begin(); i != resource.resources_.end(); ++i)
			if ((*i).is_saved_)
			{
				if((*i).link_rel_ != (*i).link_abs_)
					Replace(*i, resource);
				i = resource.resources_.erase(i);
			}
	}
}

void Parser::Replace(Resource &child, Resource &parent)
{
	std::string *content = (std::string *)parent.content_;
	replace(content, child.link_rel_, child.link_abs_);
}

void Parser::Replace(std::string rel, std::string abs, Resource &parent)
{
	std::string *content = (std::string *)parent.content_;
	replace(content, rel, abs);
}