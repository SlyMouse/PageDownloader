#include "stdafx.h"
#include "curl/curl.h"
#include "CurlUsage.h"
#include "PageParser.h"
#include <string>

#define WEBSITE "https://lenta.ru"

int main(int argc, char *argv)
{
	PageParser pp(WEBSITE);
	pp.Parse();
	
	return 0;
}

