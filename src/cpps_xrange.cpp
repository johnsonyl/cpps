#include "cpps/cpps.h"

namespace cpps
{
	cpps_value cpps_range_range(C* c,cpps_integer v1,object v2, object v3)
	{
		cpps_integer begin = 0;
		cpps_integer end = v1;
		cpps_integer inc = 1;

		if (!v3.isnull()) {
			begin = v1;
			end = v2.toint();
			inc = v3.toint();
		}
		else if (!v2.isnull()) {
			begin = v1;
			end = v2.toint();
		}

		cpps_create_class_var(cpps_range, c, cpps_range_var, cpps_range_ptr);
		cpps_range_ptr->begin = begin;
		cpps_range_ptr->end = end;
		cpps_range_ptr->inc = inc;
		return cpps_range_var;
	}
	cpps_value cpps_xrange(C* c, cpps_integer begin, cpps_integer end)
	{
		cpps_create_class_var(cpps_range, c, cpps_range_var, cpps_range_ptr);
		cpps_range_ptr->begin = begin;
		cpps_range_ptr->end = end;
		cpps_range_ptr->inc = 1;
		return cpps_range_var;
	}
	cpps_value cpps_nrange(C* c, cpps_integer begin, cpps_integer end,cpps_integer inc)
	{
		cpps_create_class_var(cpps_range, c, cpps_range_var, cpps_range_ptr);
		cpps_range_ptr->begin = begin;
		cpps_range_ptr->end = end;
		cpps_range_ptr->inc = inc;
		return cpps_range_var;
	}
	void cpps_regrange(C* c)
	{
		cpps::_module(c)[
		_class<cpps_range>("RANGE")
			.def("start",&cpps_range::start_func)
			.def("end",&cpps_range::end_func)
			.def("step",&cpps_range::step_func),
		def_inside("slice", cpps_nrange),
		def_inside("range", cpps_range_range),
		def_inside("xrange",cpps_xrange),
		def_inside("nrange",cpps_nrange)
		];
	}

	cpps_integer cpps_range::start_func()
	{
		return begin;
	}

	cpps_integer cpps_range::end_func()
	{
		return end;
	}

	cpps_integer cpps_range::step_func()
	{
		return inc;
	}

}