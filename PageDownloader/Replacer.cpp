#include "Replacer.h"
#include <string>

bool Replacer::Replace(std::string &str, const std::string &from, const std::string &to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

//void replace(Resource &child, Resource *parent)
//{
//	Replacer::Replace(parent->modify_content(), child.get_link_rel(), child.get_link_abs());
//}

void Replacer::Replace(std::shared_ptr<Resource> parent)
{
	for (std::vector<std::shared_ptr<Resource>>::const_iterator i = parent->get_resources().begin(); i != parent->get_resources().end(); ++i)
	{
		if ((*i)->get_is_handled())
		{
			if ((*i)->get_is_saved())
			{
				Replacer::Replace(parent->modify_content(), (*i)->get_link_org(), (*i)->get_file_name());
			}
			else
				if ((*i)->get_link_rel() != (*i)->get_link_abs())
					Replacer::Replace(parent->modify_content(), (*i)->get_link_org(), (*i)->get_link_abs());
		}
	}

	parent->remove_handled_resources();
}
