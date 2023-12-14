#include "cpps/cpps.h"

namespace cpps
{
	void cpps_reglock(C *c)
	{
		cpps::_module(c)[
			_class<cpps_lock>("mutex")
				.def("lock", &cpps_lock::lock)
				.def("lock_shared", &cpps_lock::lock_shared)
				.def("unlock", &cpps_lock::unlock)
				.def("unlock_shared", &cpps_lock::unlock_shared)
		];
	}
}