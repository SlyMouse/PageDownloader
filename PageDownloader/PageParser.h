#pragma once
#include <string>
#include <regex>
#include "CurlUsage.h"

class PageParser
{
public:
	void Parse();
	
	PageParser(std::string);
	std::string ParseLine(std::string);
	void ParseManifest(std::string);
	void ParseCss(std::string &);

	std::string link_orig_;
	std::string link_base_;
	bool is_root_dir_ = true;
};
