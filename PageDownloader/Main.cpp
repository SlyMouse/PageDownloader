#include "MyTask.h"
#include "ThreadPool.h"
#include "boost/filesystem.hpp"
#include <string>
#include <regex>
#include <iostream>
#include <chrono>

#define THREADS 4

int main(int argc, char *argv)
{
	std::regex re("(https?://)?(www\.)?(.+\..+)");
	std::regex re_prot("^https?:\/\/.+");
	std::smatch match;
	std::smatch match_prot;
	std::string root;
	std::string dir;
	std::vector<Resource *> vec;
	{
		ThreadPool pool(THREADS);
		for (std::string line; std::getline(std::cin, line);)
		{
			if (line == "")
				break;
			if (std::regex_search(line, match, re))
			{
				root = match[1].str() + match[3].str();
				if (!std::regex_search(root, match_prot, re_prot))
					root = "https://" + root;
				std::regex re_dir("(https?://)?(www\.)?(.+\..+)");
				std::regex_search(root, match, re);
				dir = match[3].str() + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
				std::replace(dir.begin(), dir.end(), '/', '_');
				std::replace(dir.begin(), dir.end(), '?', '_');
				dir += "/";
				boost::filesystem::create_directory(dir);
				vec.push_back(new Resource(root, dir, line, ResourceType::Page));
				MyTask task(vec.back(), TaskTarget::DownloadAndParse);
				pool.schedule(task);
			}
			else
				std::cout << "Wrong link format: " << line << std::endl;
		}
	}

	return 0;
}