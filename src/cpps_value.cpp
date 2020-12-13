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

			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)obj.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

			cpps_str2d(tmpStr->c_str(), &ret);
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
			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)obj.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
			cpps_str2i64(tmpStr->c_str(), &ret);
		}
		return ret;
	}
	std::string* cpps_get_string(cpps_value obj)
	{
		if (obj.tt == CPPS_TSTRING)
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
			std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();
			return tmpStr;
		}
		
		return NULL;
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
			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)obj.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
			return *tmpStr;
		}
		else if (obj.tt == CPPS_TBOOLEAN)
		{
			strStream << (obj.value.b == 0 ? "false" : "true");
		}

		return strStream.str();
	}


	cpps_vector* cpps_to_cpps_vector(cpps_value obj)
	{
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_vector* ret = (cpps_vector*)cppsclassvar->getclsptr();
		return ret;
	}
	cpps_map* cpps_to_cpps_map(cpps_value obj)
	{
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_map* ret = (cpps_map*)cppsclassvar->getclsptr();
		return ret;
	}
	cpps_unordered_map* cpps_to_cpps_unordered_map(cpps_value obj)
	{
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_unordered_map* ret = (cpps_unordered_map*)cppsclassvar->getclsptr();
		return ret;
	}
	cpps_cppsclass* cpps_to_cpps_cppsclass(cpps_value obj)
	{
		if (!obj.isdomain()) return NULL;
		cpps_cppsclass* cppsclassvar = (cpps_cppsclass*)obj.value.domain;
		return cppsclassvar;
	}

	bool cpps_value::operator<(const cpps_value &right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number < right.value.number;
			case CPPS_TINTEGER: return value.integer < right.value.integer;
			case CPPS_TSTRING: {
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)right.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();

				return (*(tmpStr)) < (*(tmpStr2));
			}
			default:
				throw(cpps_error("unknow file", 0, 0, "<   ... It can't be used as a key.type: %d", right.tt));
			}
		}
		return tt < right.tt;
	}

	bool cpps_value::operator>(const cpps_value &right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number > right.value.number;
			case CPPS_TINTEGER: return value.integer > right.value.integer;
			case CPPS_TSTRING: {
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)right.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();

				return (*(tmpStr)) > (*(tmpStr2));
			}
			default:
				throw(cpps_error("unknow file", 0, 0, ">   ... It can't be used as a key. type: %d", right.tt));
			}
		}
		return tt > right.tt;
	}

	bool cpps_value::operator==(const cpps_value &right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number == right.value.number;
			case CPPS_TINTEGER: return value.integer == right.value.integer;
			case CPPS_TSTRING: {
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)right.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();

				return (*(tmpStr)) == (*(tmpStr2));
			}
			default:
				throw(cpps_error("unknow file", 0, 0, "==   ... It can't be used as a key. type: %d", right.tt));
			}
		}
		return false;
	}

	cpps_value::cpps_value(C* c, const char* s)
	{
		tt = CPPS_TSTRING;

		std::string* str = NULL;
		cpps_value ret = newclass<std::string>(c, &str);
		str->append(s);
		value.domain = ret.value.domain;
	}

	cpps_value::cpps_value(C* c, const std::string& s)
	{
		tt = CPPS_TSTRING;

		std::string* str;
		cpps_value ret = newclass<std::string>(c, &str);
		str->append(s.begin(), s.end());
		value.domain = ret.value.domain;
	}

	size_t cpps_value::hash::operator()(const cpps_value& _Keyval) const
	{

		switch (_Keyval.tt)
		{
		case CPPS_TNUMBER: return std::hash<cpps_number>()(_Keyval.value.number);
		case CPPS_TINTEGER: return std::hash<cpps_integer>()(_Keyval.value.integer);
		case CPPS_TSTRING: {

			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)_Keyval.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

			return std::hash<std::string>()(*(tmpStr));
		}
		default:
			throw(cpps_error("unknow file", 0, 0, "==   ... It can't be used as a HashKey. type: %d", _Keyval.tt));
		}
		return _Keyval.tt;
	}

}
