#include <string>
#include <regex>
#include <iostream>
#include <chrono>
#include <array>
#include "MyTask.h"
#include "ThreadPool.h"
#include "boost/filesystem.hpp"

#define THREADS 4
static constexpr std::array<const char*, 12U> whitelist_ = { "png", "jpg", "svg", "bmp", "gif", "woff", "woff2", "tff", "css", "eot", "js", "pdf" };
static const std::regex re("^((?:.+)?://)?(?:www\\.)?([^/].+?\\.[^/]+?)(/(?:.+)|$)"); //Correct link. 1 - Protocol, 2 - Hostname, 3 - Path
static const std::regex re_format = std::regex("\\.((?:.(?!\\.))+?)($|\\?)"); //File format
//static const std::regex re_prot("^https?:\/\/.+");
//static const std::regex re_dir("(https?://)?(www\.)?(.+\..+)");

int main(int argc, char *argv)
{
	
	
	//std::vector<std::shared_ptr<Resource>> vec;
	{
		ThreadPool& pool = ThreadPool::Instance();
		pool.initializeWithThreads(THREADS);
		for (std::string line; std::getline(std::cin, line);)
		{
			std::smatch match;
			if (line == "")
				break;
			if (std::regex_search(line, match, re))
			{
				std::string root = (match[1].str() == "" ? "https://" : match[1].str()) + match[2].str();
				std::string dir = match[2].str() + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
				std::string path = match[3].str();
				dir += "/";
				boost::filesystem::create_directory(dir);
				ResourceType type;
				TaskTarget target = TaskTarget::DownloadAndParse;
				
				if(match[3].str() == "")
					type = ResourceType::Page;
				else
				{
					if (std::regex_search(path, match, re_format))
					{
						std::string format = match[1].str();
						if(format == "")
							type = ResourceType::Page;
						else if (format == "css")
							type = ResourceType::Css;
						else if (std::find(whitelist_.begin(), whitelist_.end(), format) != whitelist_.end())
						{
							type = ResourceType::Other;
							target = TaskTarget::Save;
						}
						else
						{
							std::cout << "Unsupported file format: " << line << std::endl;
							continue;
						}
					}
					else
						type = ResourceType::Page;
				}
				
				MyTask task(std::shared_ptr<Resource>(new Resource(root, dir, path, type)), target);
				pool.schedule(task);
			}
			else
				std::cout << "Incorrect link format: " << line << std::endl;
		}
	}

	return 0;
}