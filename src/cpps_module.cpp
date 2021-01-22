#include "cpps/cpps.h"

namespace cpps {

	cpps_module _module(C* c, std::string _domain /*= ""*/)
	{
		return cpps_module(c, _domain,true);
	}

	void _unmodule(C* c, std::string _domain /*= ""*/)
	{
		cpps_module(c, _domain,false);
	}

	cpps::_enum& _enum::value(std::string varname, cpps_integer v)
	{
		cpps_reg* r = make_regvar(varname, cpps_cpp_to_cpps_converter<cpps_integer>::apply(c, v));
		_enum_domain->regfunc(r);
		return *this;
	}

}
