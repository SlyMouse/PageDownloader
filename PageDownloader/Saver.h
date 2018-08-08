#ifndef SAVER_H
#define SAVER_H
#include "Resource.h"
#include "curl/curl.h"

class Saver
{
public:
	Saver();
	~Saver();
	void Save(Resource *);
private:
	CURL *curl_;
	
};
#endif
