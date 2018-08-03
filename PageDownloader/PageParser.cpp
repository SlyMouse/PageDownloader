#include "stdafx.h"
#include "PageParser.h"
#include <direct.h>
#include "boost/filesystem.hpp"
#include "boost/property_tree//ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void changeWorkDir(std::string link, bool &is_root_dir)
{
	std::regex re("(https?://)?(www\.)?(.+\..+)");
	std::smatch match;
	std::string path;
	if (std::regex_search(link, match, re))
		path = match[3].str();
	std::replace(path.begin(), path.end(), '/', '_');
	boost::filesystem::create_directory(path);

	if (!is_root_dir)
	{
		_chdir("..");
		is_root_dir = !is_root_dir;
	}
	_chdir(path.c_str());
	is_root_dir = !is_root_dir;
}

PageParser::PageParser(std::string link)
{
	link_orig_ = link;
	std::regex re("^(.+\..+?)(/.*)?$");
	std::smatch match;
	if (std::regex_search(link, match, re))
		link_base_ = match[1].str();
}

void PageParser::Parse()
{
	changeWorkDir(link_orig_, is_root_dir_);
	std::string file_name = DownloadHtml(link_orig_);
	rename(file_name.c_str(), (file_name + ".original").c_str());
	std::ifstream infile(file_name + ".original");
	std::ofstream outfile(file_name);
	std::string curr_line;
	while (std::getline(infile, curr_line))
	{
		curr_line = ParseLine(curr_line);
		outfile << curr_line << std::endl;
	}
}

std::string PageParser::ParseLine(std::string line)
{
	std::regex re;
	std::smatch match;

	/*re = std::regex("<link(?:.(?!link))*rel=\"manifest\".*?/>");
	if (std::regex_search(line, match, re))
		ParseManifest(match.str());*/

	re = std::regex("rel=\"stylesheet\"");
	if (std::regex_search(line, match, re))
		ParseCss(line);

	re = std::regex("<img.*?src=\"(.+?(jpg|png|bmp))\"");
	std::string::const_iterator searchStart(line.begin());
	while (std::regex_search(searchStart, line.cend(), match, re))
	{
		std::string image_link = match[1].str();
		std::string image_path = SaveLocally(image_link);
		replace(line, image_link, image_path);
		searchStart += match.position() + match.length() - (image_link.length() - image_path.length());
	}

	return line;
}

void PageParser::ParseManifest(std::string line)
{
	std::regex re;
	std::smatch match;
	std::string manifest;

	/*re = std::regex("<link(?:.(?!link))*rel=\"manifest\".*?/>");
	if (std::regex_search(line, match, re))
		manifest_tag = match.str();*/
	
	re = std::regex("href=\"(.+?)\"");
	if (std::regex_search(line, match, re))
		manifest = SaveLocally(link_base_ + match[1].str());

	//replace(line, match[1].str(), manifest);

	boost::property_tree::ptree root;
	boost::property_tree::read_json(manifest, root);

	for (boost::property_tree::ptree::value_type &icon : root.get_child("icons"))
		for (boost::property_tree::ptree::value_type &icon_params : icon.second)
			if (icon_params.first == "src")
			{
				std::string source = icon_params.second.data();
				SaveLocally(link_base_ + "/" + source);
			}
}

void PageParser::ParseCss(std::string &line)
{
	std::regex re;
	std::smatch match;
	std::string stylesheet;
	std::string stylesheet_tag;

	re = std::regex("<link(?:.(?!link))*rel=\"stylesheet\".*?/>");
	if (std::regex_search(line, match, re))
		stylesheet_tag = match.str();

	re = std::regex("href=\"(.+?)\"");
	if (std::regex_search(stylesheet_tag, match, re))
		stylesheet = SaveLocally(match[1].str());
	replace(line, match[1].str(), stylesheet);

	rename(stylesheet.c_str(), (stylesheet + ".original").c_str());
	std::ifstream infile(stylesheet + ".original");
	std::ofstream outfile(stylesheet);
	std::string curr_line;
	while (std::getline(infile, curr_line))
	{
		re = std::regex("url\((((?!data:image).)*?)\)");
		std::string::const_iterator searchStart(line.begin());
		while (std::regex_search(searchStart, line.cend(), match, re))
		{
			std::string resource_link = match[1].str();
			std::string resource_path = SaveLocally(resource_link);
			replace(line, resource_link, resource_path);
			searchStart += match.position() + match.length();
		}

		outfile << curr_line << std::endl;
	}
}
