#include "cpps.h"


namespace cpps
{

	
	std::string transformat(std::string fmt, cpps_value v)
	{
		char buffer[1024];
		if (v.tt == CPPS_TSTRING)
		{

			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

			sprintf(buffer, fmt.c_str(), tmpStr->c_str());
		}
		else if (v.tt == CPPS_TBOOLEAN)
		{
			sprintf(buffer, fmt.c_str(), v.value.b != 0);
		}
		else if (v.tt == CPPS_TNUMBER)
		{
			sprintf(buffer, fmt.c_str(), v.value.number);
		}
		else if (v.tt == CPPS_TINTEGER)
		{
			sprintf(buffer, fmt.c_str(), v.value.integer);
		}
		return buffer;
	}

	size_t formatcheck(C *c,std::string &fmt, size_t j, size_t i, std::string &take, cpps_value &v)
	{
		std::string  newstr = "";
		switch (fmt[j])
		{
		case 'd':
		case 'u':
		case 'o':
		case 'c':
		case 'i':
		case 'X':
		case 'x':
			newstr = transformat(take, cpps_to_integer(v));
			fmt.replace(i, take.size(), newstr);
			break;
		case 'f':
			newstr = transformat(take, cpps_to_number(v));
			fmt.replace(i, take.size(), newstr);
			break;
		case 's':
			newstr = transformat(take, cpps_value(c, cpps_to_string(v)));
			fmt.replace(i, take.size(), newstr);
			break;
		default:
			break;
		}
		return newstr.size();
	}

}