#include "cpps/cpps.h"


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
	void cpps_debug_log(C *c, object b)
	{
		if (c->debug) cpps_base_printfln(c,b);
	}
	void cpps_debug_breakpoint(C *c)
	{
		
	}
	void cpps_debug_trace(C* c)
	{

	}
	void cpps_regdebug(C *c)
	{
		cpps::_module(c,"debug")[
			def_inside("open", cpps_debug_open),
			def_inside("close", cpps_debug_close),
			def_inside("log", cpps_debug_log),
			def_inside("breakpoint",cpps_debug_breakpoint),
			def_inside("trace",cpps_debug_trace)
		];
		cpps::_module(c)[
			_class<cpps_trycatch_error>("cpps_trycatch_error")
			.def("geterrno", &cpps_trycatch_error::geterrno)
			.def("geterrstr", &cpps_trycatch_error::geterrstr)
			.def("getfile", &cpps_trycatch_error::getfile)
			.def("getline", &cpps_trycatch_error::getline)
			.def("getvalue", &cpps_trycatch_error::getvalue)
			.def("callstack", &cpps_trycatch_error::callstack)
		];
	}
}