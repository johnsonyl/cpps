#include "cpps.h"

namespace cpps
{


	cpps::int32 type(object o)
	{
		return o.value.tt;
	}

	std::string type_s(object o)
	{
		switch (type(o))
		{
		case CPPS_TNIL:
			return "nil";
		case CPPS_TINTEGER:
			return "integer";
		case CPPS_TNUMBER:
			return "number";
		case CPPS_TBOOLEAN:
			return "boolean";
		case CPPS_TSTRING:
			return "string";
		case CPPS_TCLASSVAR:
			return "userdata";
		case CPPS_TFUNCTION:
			return "function";
		}
		return "unknow";
	}




}
