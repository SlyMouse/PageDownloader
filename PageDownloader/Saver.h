#ifndef SAVER_H
#define SAVER_H
#include "Resource.h"

class Saver
{
public:
	static void Save(std::shared_ptr<Resource>);
};
#endif
