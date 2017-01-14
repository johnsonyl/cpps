#include "cpps.h"

namespace cpps
{
	object dofunction(C *c, object func)
	{
		cpps_value ret;
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function *f = func.value.value.func;
			if (f)
			{
				std::vector<cpps_value> paramlist;

				bool isCheckGen1 = !c->isCheckGen1;
				f->callfunction(c, &ret, c->_G, &paramlist, NULL, isCheckGen1);
			}
		}
		return ret;
	}
}
