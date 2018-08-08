#include <string>
#include <regex>
#include <iostream>
#include <chrono>
#include <array>
#include "MyTask.h"
#include "ThreadPool.h"
#include "boost/filesystem.hpp"

#define THREADS 4
static constexpr std::array<const char*, 13U> whitelist_ = { "png", "jpg", "svg", "bmp", "gif", "woff", "woff2", "tff", "css", "eot", "eot?#iefix", "js", "pdf" };

int main(int argc, char *argv)
{
	std::regex re("(https?://)?(www\.)?(.+\..+)");
	std::regex re_prot("^https?:\/\/.+");
	std::smatch match;
	std::smatch match_prot;
	std::string root;
	std::string dir;
	std::vector<std::shared_ptr<Resource>> vec(500);
	{
		ThreadPool& pool = ThreadPool::Instance();
		pool.initializeWithThreads(THREADS);
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
				ResourceType type;
				TaskTarget target = TaskTarget::DownloadAndParse;
				std::regex re_format = std::regex("\\.((?:.(?!\\.))+)$");
				if (std::regex_search(line, match, re_format))
				{
					std::string format = match[1].str();
					if (format == "css")
						type = ResourceType::Css;
					else if (std::find(whitelist_.begin(), whitelist_.end(), format) != whitelist_.end())
					{
						type = ResourceType::Other;
						target = TaskTarget::Save;
					}
					else
					{
						type = ResourceType::Page;
					}
				}
				vec.push_back(std::shared_ptr<Resource>(new Resource(root, dir, line, type)));
				MyTask task(vec.back(), target);
				pool.schedule(task);
			}
			else
				std::cout << "Wrong link format: " << line << std::endl;
		}
	}

	return 0;
}