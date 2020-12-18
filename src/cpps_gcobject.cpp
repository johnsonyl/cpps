#include "cpps.h"

namespace cpps
{

	
	cpps_gcobject::cpps_gcobject()
	{
		gclevel = 0;
		gc_usecount = 0;
	}

	cpps_gcobject::~cpps_gcobject()
	{

	}

	void cpps_gcobject::setgcLevel(char l)
	{
		gclevel = l;
	}

	char cpps_gcobject::getgcLevel()
	{
		return gclevel;
	}

	void cpps_gcobject::incruse()
	{
		gc_usecount++;
	}

	void cpps_gcobject::decruse()
	{
		gc_usecount--;
	}

	cpps::int32 cpps_gcobject::count()
	{
		return gc_usecount;
	}

}
