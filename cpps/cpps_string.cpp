#include "cpps.h"
namespace cpps
{
	cpps_integer	cpps_string_len(std::string v)
	{
		return v.size();
	}
	cpps_integer	cpps_string_find(std::string v, std::string v2)
	{
		return v.find(v2);
	}
	cpps_integer	cpps_string_rfind(std::string v, std::string v2)
	{
		return v.rfind(v2);
	}
	
	std::string	cpps_string_replace(std::string v, std::string v2, std::string v3)
	{
		std::string::size_type pos = 0;

		while ((pos = v.find(v2, pos)) != std::string::npos)
		{
			v.replace(pos, v2.length(), v3);
			pos += v3.length();
		}

		return v;
	}
	cpps_value cpps_string_split(C *c,std::string v, std::string v2)
	{
		cpps_vector *vec;
		cpps_value ret = newClassPtr<cpps_vector>(c,&vec);

		if (v.empty()) return ret;

		const char *a = v.c_str(); const char *b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vec->push_back(s);

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}

		return ret;
	}
	std::string	cpps_string_strcut(std::string v, std::string v2, std::string v3)
	{
		const char * strtmp1 = strstr(v.c_str(), v2.c_str());
		if (!strtmp1) return "";

		const char * strtmp2 = strstr(strtmp1 + v2.size(), v3.c_str());
		if (!strtmp2) return "";

		std::string out;
		out.append(strtmp1 + v2.size(), strtmp2 - strtmp1 - v2.size());


		return out;
	}
	bool	cpps_string_empty(std::string v)
	{
		return v.empty();
	}
	std::string	cpps_string_sub(std::string v,cpps_integer pos,cpps_integer n)
	{
		if (v.size() < size_t(pos + n)) return "";
		return v.substr((unsigned int) pos, (unsigned int)n);
	}
	cpps_integer	cpps_string_npos()
	{
		return std::string::npos;
	}
	void cpps_regstring(C *c)
	{
		module(c,"string")[
			def("find", cpps_string_find),
			def("rfind", cpps_string_rfind),
			def("length", cpps_string_len),
			def("strlen", cpps_string_len),
			def("replace", cpps_string_replace),
			def_inside("split", cpps_string_split),
			def("strcut", cpps_string_strcut),
			def("empty", cpps_string_empty),
			def("substr",cpps_string_sub),
			def("npos",cpps_string_npos)
			
		];

	}

}

