#include "stdafx.h"
#include "curl/curl.h"
#include "CurlUsage.h"
#include <string>
#include <fstream>

#define WEBSITE "https://lenta.ru"

int main(int argc, char *argv)
{
	std::string file_name = DownloadHtml(WEBSITE);
	std::ifstream infile(file_name);
	std::string curr_line;
	while (std::getline(infile, curr_line))
	{}
	return 0;
}

