#pragma once
#include "curl/curl.h"
#include <string>
#include <regex>

std::string DownloadHtml(std::string);
std::string SaveLocally(std::string);
