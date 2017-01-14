#include "cpps.h"

namespace cpps
{
	cpps_number cpps_to_number(cpps_value obj)
	{
		cpps_number ret = 0;
		if (obj.tt == CPPS_TNUMBER)
		{
			ret = obj.value.number;
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			ret = cpps_integer2number(obj.value.integer);
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			cpps_str2d(obj.str.c_str(), &ret);
		}
		return ret;
	}

	cpps_integer cpps_to_integer(cpps_value obj)
	{
		cpps_integer ret = 0;
		if (obj.tt == CPPS_TNUMBER)
		{
			ret = cpps_number2integer(obj.value.number);
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			ret = obj.value.integer;
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			cpps_str2i64(obj.str.c_str(), &ret);
		}
		return ret;
	}

	std::string cpps_to_string(cpps_value obj)
	{
		std::stringstream strStream;
		if (obj.tt == CPPS_TNUMBER)
		{
			strStream << obj.value.number;
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			strStream << obj.value.integer;
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			strStream << obj.str;
		}
		else if (obj.tt == CPPS_TBOOLEAN)
		{
			strStream << (obj.value.b == 0 ? "false" : "true");
		}

		return strStream.str();
	}

}
