#include "cpps.h"
namespace cpps
{
	cpps_integer	cpps_string_len(cpps_value v)
	{
		if (v.tt == CPPS_TSTRING) {
			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
			return tmpStr->size();
		}
		return 0;
	}
	cpps_integer	cpps_string_strlen(cpps_value v)
	{
		if (v.tt == CPPS_TSTRING) {
			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
			std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
			return strlen(tmpStr->c_str());
		}
		return 0;
	}
	
	cpps_integer	cpps_string_find(cpps_value v, std::string v2,cpps_integer off)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		return tmpStr->find(v2,off);
	}
	cpps_integer	cpps_string_rfind(cpps_value v, std::string v2, cpps_integer off)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		return tmpStr->rfind(v2, off);
	}
	
	cpps_value	cpps_string_replace(cpps_value v, std::string v2, std::string v3)
	{
		std::string::size_type pos = 0;
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		while ((pos = tmpStr->find(v2, pos)) != std::string::npos)
		{
			tmpStr->replace(pos, v2.length(), v3);
			pos += v3.length();
		}

		return v;
	}
	cpps_value cpps_string_split(C *c, cpps_value v, std::string v2)
	{
		cpps_vector *vec;
		cpps_value ret = newClassPtr<cpps_vector>(c,&vec);

		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		if (tmpStr->empty()) return ret;

		const char *a = tmpStr->c_str(); const char *b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vec->push_back(cpps_value(c,s));

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}

		return ret;
	}
	std::string	cpps_string_strcut(cpps_value v, std::string v2, std::string v3)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		const char * strtmp1 = strstr(tmpStr->c_str(), v2.c_str());
		if (!strtmp1) return "";

		const char * strtmp2 = strstr(strtmp1 + v2.size(), v3.c_str());
		if (!strtmp2) return "";

		std::string out;
		out.append(strtmp1 + v2.size(), strtmp2 - strtmp1 - v2.size());


		return out;
	}
	cpps_value cpps_string_strcuts(C *c, cpps_value old, std::string start, std::string end)
	{

		cpps_vector *vec;
		cpps_value ret = newClassPtr<cpps_vector>(c, &vec);

		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)old.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		const char *begin = tmpStr->c_str();
		while (true)
		{
			const char * strtmp1 = strstr(begin, start.c_str());
			if (!strtmp1) return ret;

			const char * strtmp2 = strstr(strtmp1 + start.size(), end.c_str());
			if (!strtmp2) return ret;

			std::string out;
			out.append(strtmp1 + start.size(), strtmp2 - strtmp1 - start.size());

			vec->push_back(cpps_value(c,out));

			begin = strtmp2 + end.size();
		}
		return ret;
	}
	bool	cpps_string_empty(cpps_value v)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		return tmpStr->empty();
	}
	std::string	cpps_string_sub(cpps_value v,cpps_integer pos,cpps_integer n)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		if (tmpStr->size() <= size_t(pos)) return "";
		if (tmpStr->size() <= size_t(pos + n) && n != std::string::npos) n = std::string::npos;
		return tmpStr->substr((unsigned int) pos, (unsigned int)n);
	}
	cpps_integer	cpps_string_npos()
	{
		return std::string::npos;
	}
	std::string cpps_string_at(cpps_value v,cpps_integer pos)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		return tmpStr->substr(pos, 1);
	}
	std::string cpps_string_format(const char* fmt, ...)
	{
		return fmt;
	}
	cpps_value cpps_string_lower(cpps_value v)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		for (size_t i = 0; i < tmpStr->size(); i++)
		{
			std::string &s = *(tmpStr);
			s[i] = tolower(tmpStr->at(i));
		}
		return v;
	}
	cpps_value cpps_string_upper(cpps_value v)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)v.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		for (size_t i = 0; i < tmpStr->size(); i++)
		{
			std::string &s = *(tmpStr);
			s[i] = toupper(tmpStr->at(i));
		}
		return v;
	}
	cpps_value cpps_string_trim(cpps_value s)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)s.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		if (tmpStr->empty())
		{
			return s;
		}

		tmpStr->erase(0, tmpStr->find_first_not_of(" "));
		tmpStr->erase(tmpStr->find_last_not_of(" ") + 1);
		return s;
	}
	cpps_value cpps_string_ltrim(cpps_value s)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)s.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		if (tmpStr->empty())
		{
			return s;
		}

		tmpStr->erase(0, tmpStr->find_first_not_of(" "));
		return s;
	}
	cpps_value cpps_string_rtrim(cpps_value s)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)s.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		if (tmpStr->empty())
		{
			return s;
		}

		tmpStr->erase(tmpStr->find_last_not_of(" ") + 1);
		return s;
	}
	bool cpps_string_regex_match(std::string src, std::string reg)
	{
		std::regex e(reg.c_str());
		return std::regex_match(src, e);
	}
	/*
	cpps_value cpps_string_regex_search(C *c,std::string src, std::string reg)
	{
		std::smatch *vec;
		cpps_value ret = newClassPtr<std::smatch>(c, &vec);

		std::regex e(reg.c_str());
		std::regex_search(src,*vec, e);

		return ret;
	}
	*/
	std::string cpps_string_regex_replace(cpps_value src, std::string reg,std::string pos)
	{
		std::regex e(reg.c_str());
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)src.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		return std::regex_replace(*(tmpStr), e, pos);
	}
	cpps_value cpps_string_chr(C*c,cpps_integer ch)
	{
		std::string ret;
		ret.push_back(static_cast<char>(ch));
		return cpps_value(c,ret);
	}
	cpps_value cpps_string_push_back(cpps_value src, cpps_integer c)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)src.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		tmpStr->push_back(static_cast<char>(c));
		return src;
	}
	cpps_integer cpps_string_unicode_charCodeAt(cpps_value src, cpps_integer pos)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)src.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		if (tmpStr->size() % 2 != 0) return 0;
		if (tmpStr->size() / 2 <= size_t(pos)) return 0;
		unsigned short ret = *((unsigned short*)(tmpStr->c_str() + pos * 2));
		return static_cast<cpps_integer>(ret);
	}
	cpps_value cpps_string_unicode_fromCodeAt(C*c,cpps_vector* vec)
	{
		std::string ret = "";
		for (size_t i = 0; i < size_t(vec->size()); i++)
		{
			unsigned short code = static_cast<unsigned short>(vec->at(i).value.integer);
			ret.append((char *)&code,2);
		}
		ret.append("\0\0", 2);
		return cpps_value(c,ret);
	}
	cpps_integer cpps_string_charCodeAt(cpps_value src, cpps_integer pos)
	{
		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)src.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

		if (tmpStr->size() <= size_t(pos)) return 0;

		unsigned char ret = *((char*)(tmpStr->c_str() + pos));
		return static_cast<cpps_integer>(ret);
	}
	cpps_value cpps_string_fromCodeAt(C*c,cpps_vector* vec)
	{
		std::string ret = "";
		for (size_t i = 0; i < size_t(vec->size()); i++)
		{
			char code = static_cast<char>(vec->at(i).value.integer);
			ret.push_back(code);
		}
		ret.append("\0", 2);
		return cpps_value(c,ret);
	}
	void cpps_regstring(C *c)
	{
		module(c,"string")[
			def("find", cpps_string_find),
			def("rfind", cpps_string_rfind),
			def("length", cpps_string_len),
			def("strlen", cpps_string_strlen),
			def("replace", cpps_string_replace),
			def_inside("split", cpps_string_split),
			def("strcut", cpps_string_strcut),
			def_inside("strcuts", cpps_string_strcuts),
			def("empty", cpps_string_empty),
			def("substr",cpps_string_sub),
			defvar(c, "npos",(cpps_integer)std::string::npos),
			def("at", cpps_string_at),
			def("format", cpps_string_format),
			def("lower", cpps_string_lower),
			def("upper", cpps_string_upper),
			def("trim", cpps_string_trim),
			def("ltrim", cpps_string_ltrim),
			def("rtrim", cpps_string_rtrim),
			def("regex_match", cpps_string_regex_match),
			def("regex_replace", cpps_string_regex_replace),
			def_inside("chr",cpps_string_chr),
			def("push_back", cpps_string_push_back),
			def("unicode_charCodeAt",cpps_string_unicode_charCodeAt),
			def_inside("unicode_fromCodeAt",cpps_string_unicode_fromCodeAt),
			def("charCodeAt",cpps_string_charCodeAt),
			def_inside("fromCodeAt",cpps_string_fromCodeAt)

			
		];

	}

}

