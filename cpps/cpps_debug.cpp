#include "cpps.h"


namespace cpps
{
	void cpps_debug_open(C *c)
	{
		c->debugopen();
	}
	void cpps_debug_close(C *c)
	{
		c->debugclose();
	}
	void cpps_debug_breakpoint(C *c)
	{
		
	}
	void cpps_regdebug(C *c)
	{
		module(c,"debug")[
			def_inside("open", cpps_debug_open),
			def_inside("close", cpps_debug_close),
			def_inside("breakpoint",cpps_debug_breakpoint)
		];
	}
}