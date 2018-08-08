#include "stdafx.h"
#include "Replacer.h"
#include <string>
#include <algorithm>

bool Replacer::Replace(std::string *str, const std::string &from, const std::string &to) {
	size_t start_pos = (*str).find(from);
	if (start_pos == std::string::npos)
		return false;
	(*str).replace(start_pos, from.length(), to);
	return true;
}

void replace(Resource &child, Resource *parent)
{
	Replacer::Replace(parent->content_, child.link_rel_, child.link_abs_);
}

bool isHandled(Resource *res)
{
	return res->is_handled;
}

void Replacer::Replace(Resource *parent)
{
	for (std::vector<Resource *>::iterator i = parent->resources_.begin(); i != parent->resources_.end(); ++i)
	{
		if ((*i)->is_handled)
		{
			if ((*i)->is_saved_)
			{
				Replacer::Replace(parent->content_, (*i)->link_rel_, (*i)->file_name);
			}
			else
				if ((*i)->link_rel_ != (*i)->link_abs_)
					Replacer::Replace(parent->content_, (*i)->link_rel_, (*i)->link_abs_);

			//parent->resources_.erase(std::remove(parent->resources_.begin(), parent->resources_.end(), *i), parent->resources_.end());
		}
	}

	parent->resources_.erase(std::remove_if(parent->resources_.begin(), parent->resources_.end(), isHandled), parent->resources_.end());
}
