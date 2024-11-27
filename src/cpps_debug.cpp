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
	void cpps_debug_dump_memory(C* c)
	{
#if defined _DEBUG || RELDEBUG
		cpps::memory_allocal::instance().gethandler()->dump();
#endif
	}
	void cpps_regdebug(C *c)
	{
		cpps::_module(c,"debug")[
			def_inside("open", cpps_debug_open),
			def_inside("close", cpps_debug_close),
			def_inside("log", cpps_debug_log),
			def_inside("breakpoint",cpps_debug_breakpoint),
			def_inside("trace",cpps_debug_trace),
			def_inside("dump_memory",cpps_debug_dump_memory)
		];
		cpps::_module(c)[
			_class<cpps_trycatch_error>("cpps_trycatch_error")
			.def("error", &cpps_trycatch_error::error)
			.def("what", &cpps_trycatch_error::what)
			.def("file", &cpps_trycatch_error::file)
			.def("line", &cpps_trycatch_error::line)
			.def("value", &cpps_trycatch_error::value)
			.def("callstack", &cpps_trycatch_error::callstack)
			.def("tostring",&cpps_trycatch_error::tostring)
		];
	}	
}