#include "cpps/cpps.h"


namespace cpps {

	bool cpps_isbasevar(const cpps_value &v)
	{
		return cpps_isnumber(v) || cpps_isint(v) || cpps_isbool(v);
	}
	bool cpps_isstring(const cpps_value &v)
	{
		return v.tt == CPPS_TSTRING;
	}
	bool cpps_isvector(const cpps_value &v)
	{
		return (v.isdomain() && v.value.domain->domainname == "vector");
	}
	bool cpps_isrange(const cpps_value &v)
	{
		return (v.isdomain() && v.value.domain->domainname == "RANGE");
	}

	bool cpps_ismap(const cpps_value &v)
	{
		return (v.isdomain() && (v.value.domain->domainname == "map"));
	}
	bool cpps_ispair(const cpps_value &v)
	{
		return (v.isdomain() && (v.value.domain->domainname == "pair"));
	}
	bool cpps_isset(const cpps_value &v)
	{
		return (v.isdomain() && (v.value.domain->domainname == "set"));
	}
	bool cpps_isint(const cpps_value &v)
	{
		return v.tt == CPPS_TINTEGER;
	}
	bool cpps_isbool(const cpps_value &v)
	{
		return v.tt == CPPS_TBOOLEAN;
	}
	bool cpps_isnumber(const cpps_value &v)
	{
		return v.tt == CPPS_TNUMBER;
	}
	bool cpps_isnull(const cpps_value &v)
	{
		return v.tt == CPPS_TNIL;
	}
	bool cpps_isclassvar(const cpps_value &v)
	{
		return v.tt == CPPS_TCLASSVAR;
	}
	bool cpps_isclass(const cpps_value &v)
	{
		return v.tt == CPPS_TCLASS;
	}

	bool cpps_isellipsis(const cpps_value &v)
	{
		return v.tt == CPPS_TELLIPSIS;
	}

	bool cpps_istuple(const cpps_value &v)
	{
		return v.tt == CPPS_TTUPLE;
	}

	bool cpps_isfunction(const cpps_value& v)
	{
		return v.tt == CPPS_TFUNCTION || v.tt == CPPS_TLAMBDAFUNCTION;
	}
	std::string cpps_getclassname(const cpps_value& v)
	{
		return v.isdomain() ? v.value.domain->domainname : "";
	}
}