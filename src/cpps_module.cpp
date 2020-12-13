#include "cpps.h"

namespace cpps {

	cpps_module _module(C* c, std::string _domain /*= ""*/)
	{
		return cpps_module(c, _domain);
	}

}
