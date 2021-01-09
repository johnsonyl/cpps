#include "cpps/cpps.h"

namespace cpps
{
	void cpps_reglock(C *c)
	{
		cpps::_module(c)[
			_class<cpps_lock>("threadlock")
				.def("lock", &cpps_lock::lock)
				.def("unlock", &cpps_lock::unlock)
		];
	}
}