#include "cpps/cpps.h"

namespace cpps
{
	cpps_value cpps_range_range(C* c,cpps_integer end)
	{
		cpps_create_class_var(cpps_range, c, cpps_range_var, cpps_range_ptr);
		cpps_range_ptr->begin = 0;
		cpps_range_ptr->end = end;
		cpps_range_ptr->inc = 1;
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
		_class<cpps_range>("RANGE"),
		def_inside("range", cpps_range_range),
		def_inside("xrange",cpps_xrange),
		def_inside("nrange",cpps_nrange)
		];
	}

}