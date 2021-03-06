#include "cpps/cpps.h"
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
			std::string* tmpStr = cpps_get_string(v);
			return strlen(tmpStr->c_str());
		}
		return 0;
	}
	
	cpps_integer	cpps_string_find(cpps_value v, std::string v2,cpps_integer off)
	{
		if (v.tt != CPPS_TSTRING) return std::string::npos;
		std::string* tmpStr = cpps_get_string(v);
		return tmpStr->find(v2, size_t(off));
	}
	cpps_integer	cpps_string_rfind(cpps_value v, std::string v2, cpps_integer off)
	{
		if (v.tt != CPPS_TSTRING) return std::string::npos;
		std::string* tmpStr = cpps_get_string(v);
		return tmpStr->rfind(v2, size_t(off));
	}
	
	void	cpps_string_copyto(cpps_value src, cpps_value tar, cpps_value off,cpps_value len ) {
		if (src.tt != CPPS_TSTRING) return ;
		if (tar.tt != CPPS_TSTRING) return ;

		size_t noff = 0;
		size_t count = std::string::npos;

		noff = cpps_isint(off) ? (size_t)cpps_to_integer(off) : noff;
		count = cpps_isint(len) ? (size_t)cpps_to_integer(len) : count;

		std::string* srcstr = cpps_get_string(src);
		std::string* tarstr = cpps_get_string(tar);

		*tarstr = srcstr->substr(noff, count);

	}
	void	cpps_string_clear(cpps_value v) {
		if (v.tt != CPPS_TSTRING) return ;

		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)v.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

		tmpStr->clear();
	}
	cpps_value	cpps_string_replace(cpps_value v, std::string v2, std::string v3)
	{
		if (v.tt != CPPS_TSTRING) return nil;
		std::string::size_type pos = 0;
		std::string* tmpStr = cpps_get_string(v);
		while ((pos = tmpStr->find(v2, pos)) != std::string::npos)
		{
			tmpStr->replace(pos, v2.length(), v3);
			pos += v3.length();
		}

		return v;
	}
	cpps_value cpps_string_cut(C* c, cpps_value v, cpps_integer count) {
		if (v.tt != CPPS_TSTRING) return nil;
		cpps_vector* vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec,&ret);


		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)v.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

		size_t ncount = (size_t)count;
		size_t FileSize = tmpStr->size();
		size_t pos1 = 0;
		for (size_t j = 0; j < FileSize / ncount + 1; j++)
		{
			size_t res = FileSize - pos1 < ncount ? FileSize - pos1 : ncount;
			if (res != 0)
			{
				std::string *s;
				cpps_value sv;
				newclass<std::string>(c, &s,&sv);
				s->append(tmpStr->c_str() + pos1, res);
				vec->push_back(sv);
				pos1 += res;
				
			}
		}

		return ret;
	}
	void cpps_string_real_split(std::vector<std::string>&vec, std::string &v, std::string v2,cpps_integer count)
	{
		size_t ncount = (size_t)count;
		if (v.empty()) return;

		const char *a = v.c_str(); const char *b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vec.push_back(s);

			if (count != -1 && vec.size() >= ncount) break;

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}
	}
	cpps_value cpps_string_split(C *c, cpps_value v, std::string v2,cpps_value count)
	{
		if (v.tt != CPPS_TSTRING) return nil;
		cpps_integer ncount = -1;
		if (count.tt == CPPS_TINTEGER) ncount = count.value.integer;
		cpps_vector *vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec,&ret);

		std::string* tmpStr = cpps_get_string(v);

		if (tmpStr->empty()) return ret;

		const char *a = tmpStr->c_str(); const char *b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vec->push_back(cpps_value(c,s));

			if (ncount != -1 && vec->size() >= ncount) break;

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}

		return ret;
	}
	std::string	cpps_string_real_strcut(std::string &v, std::string v2, std::string v3)
	{

		const char * strtmp1 = strstr(v.c_str(), v2.c_str());
		if (!strtmp1) return "";

		const char * strtmp2 = strstr(strtmp1 + v2.size(), v3.c_str());
		if (!strtmp2) return "";

		std::string out;
		out.append(strtmp1 + v2.size(), strtmp2 - strtmp1 - v2.size());


		return out;
	}
	std::string	cpps_string_strcut(cpps_value v, std::string v2, std::string v3)
	{
		if (v.tt != CPPS_TSTRING) return "";
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)v.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

		const char* strtmp1 = strstr(tmpStr->c_str(), v2.c_str());
		if (!strtmp1) return "";

		const char* strtmp2 = strstr(strtmp1 + v2.size(), v3.c_str());
		if (!strtmp2) return "";

		std::string out;
		out.append(strtmp1 + v2.size(), strtmp2 - strtmp1 - v2.size());


		return out;
	}
	cpps_value cpps_string_strcuts(C *c, cpps_value old, std::string start, std::string end)
	{

		if (old.tt != CPPS_TSTRING) return nil;
		cpps_vector *vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec,&ret);

		std::string* tmpStr = cpps_get_string(old);

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
		if (v.tt != CPPS_TSTRING) return true;
		std::string* tmpStr = cpps_get_string(v);

		return tmpStr->empty();
	}
	std::string	cpps_string_sub(cpps_value v,cpps_integer pos,cpps_value nn)
	{
		size_t n = std::string::npos;
		n = cpps_isint(nn) ? (size_t)cpps_to_integer(nn) : n;

		if (v.tt != CPPS_TSTRING) return "";
		std::string* tmpStr = cpps_get_string(v);
		size_t tmpn = (size_t)n;
		if (tmpStr->size() <= size_t(pos)) return "";
		if (tmpStr->size() <= size_t(pos + tmpn) && tmpn != std::string::npos) tmpn = std::string::npos;
		return tmpStr->substr((size_t) pos, (size_t)n);
	}
	cpps_integer	cpps_string_npos()
	{
		return std::string::npos;
	}
	std::string cpps_string_at(cpps_value v,cpps_integer pos)
	{
		if (v.tt != CPPS_TSTRING) return "";
		std::string* tmpStr = cpps_get_string(v);

		return tmpStr->substr(size_t(pos), 1);
	}
	std::string cpps_string_format(const char* fmt, ...)
	{
		return fmt;
	}
	void cpps_string_real_tolower(std::string& s) {
		for (size_t i = 0;i < s.size(); ++i){
			s[i] = (char)tolower((int)s[i]);
		}
	}
	cpps_value cpps_string_lower(cpps_value v)
	{
		if (v.tt != CPPS_TSTRING) return nil;
		std::string* tmpStr = cpps_get_string(v);
		cpps_string_real_tolower(*tmpStr);
		return v;
	}

	void cpps_string_real_toupper(std::string& s) {
		for (size_t i = 0; i < s.size(); ++i) {
			s[i] = (char)toupper((int)s[i]);
		}
	}
	cpps_value cpps_string_upper(cpps_value v)
	{
		if (v.tt != CPPS_TSTRING) return nil;
		std::string* tmpStr = cpps_get_string(v);
		cpps_string_real_toupper(*tmpStr);
		return v;
	}
	
	void cpps_string_pop_back(cpps_value s, cpps_integer c) {

		if (s.tt != CPPS_TSTRING) return;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)s.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();
		for (cpps_integer i = 0; i < c; i++) {
			tmpStr->pop_back();
		}
	}
	bool cpps_string_startswith(cpps_value s,std::string end) {

		if (s.tt != CPPS_TSTRING) return false;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)s.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();
		size_t pos = tmpStr->find(end);
		if (pos == std::string::npos) return false;
		if (pos == 0) return true;
		return false;
	}
	bool cpps_string_endswith(cpps_value s, std::string end) {
		if (s.tt != CPPS_TSTRING) return false;

		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)s.value.domain;
		std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();
		size_t pos = tmpStr->rfind(end);
		if (pos == std::string::npos) return false;
		if (pos + end.size() == tmpStr->size()) return true;
		return false;
	}
	
	cpps_value cpps_string_ltrim(cpps_value s)
	{
		std::string* tmpStr = cpps_get_string(s);
		if (tmpStr->empty())
		{
			return s;
		}
		std::string::iterator   p = find_if(tmpStr->begin(), tmpStr->end(), [](char code) { return !isspace(code); });
		tmpStr->erase(tmpStr->begin(), p);
		return s;
	}
	std::string cpps_string_real_join(std::string sep, std::vector<std::string> & vec) {
		std::string ret;
		for (auto s : vec) {
			ret += s;
			ret += sep;
		}
		return ret;
	}
	std::string cpps_string_join(std::string sep, cpps_vector* vec) {
		std::string ret;
		for (auto s : vec->realvector()) {
			ret += cpps_to_string(s);
			ret += sep;
		}
		return ret;
	}
	std::string cpps_string_between(std::string front, std::string before, cpps_vector* vec) {
		std::string ret;
		for (auto s : vec->realvector()) {
			ret += front;
			ret += cpps_to_string(s);
			ret += before;
		}
		return ret;
	}
	cpps_value cpps_string_rtrim(cpps_value s)
	{
		if (s.tt != CPPS_TSTRING) return nil;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)s.value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		if (tmpStr->empty())
		{
			return s;
		}

		std::string::reverse_iterator  p = find_if(tmpStr->rbegin(), tmpStr->rend(), [](char code) { return !isspace(code); });
		tmpStr->erase(p.base(), tmpStr->end());
		return s;
	}
	cpps_value cpps_string_trim(cpps_value s)
	{
		if (s.tt != CPPS_TSTRING) return false;
		std::string* tmpStr = cpps_get_string(s);

		if (tmpStr->empty())
		{
			return s;
		}


		cpps_string_ltrim(cpps_string_rtrim(s));
		return  s;
	}

	cpps_value cpps_string_chr(C*c,cpps_integer ch)
	{
		std::string ret;
		ret.push_back(static_cast<char>(ch));
		return cpps_value(c,ret);
	}
	cpps_value cpps_string_push_back(cpps_value src, cpps_integer c)
	{
		if (src.tt != CPPS_TSTRING) return nil;
		std::string* tmpStr = cpps_get_string(src);

		tmpStr->push_back(static_cast<char>(c));
		return src;
	}
	cpps_integer cpps_string_unicode_charCodeAt(cpps_value src, cpps_integer pos)
	{
		if (src.tt != CPPS_TSTRING) return 0;
		std::string* tmpStr = cpps_get_string(src);

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
		return cpps_value(c,ret);
	}
	cpps_integer cpps_string_isalnum(cpps_integer i) {
		return (cpps_integer)isalnum((int32)i);
	}
	cpps_integer cpps_string_isalpha(cpps_integer i) {
		return (cpps_integer)isalpha((int32)i);
	}
	cpps_integer cpps_string_isspace(cpps_integer i) {
		return (cpps_integer)isspace((int32)i);
	}

	void cpps_regstring(C *c)
	{
		cpps::_module(c)[
			_class<cpps::string>("String")
				.def("size", &cpps::string::cpps_size)
				.def("find", &cpps::string::cpps_string_find)
				.def("rfind", &cpps::string::cpps_string_rfind)
 				.def("replace", &cpps::string::cpps_string_replace)
 				.def("clear", &cpps::string::cpps_string_clear)
 				.def("copyto", &cpps::string::cpps_string_copyto)
 				.def_inside("split", &cpps::string::cpps_string_split)
 				.def_inside("cut", &cpps::string::cpps_string_cut)
 				.def("strcut", &cpps::string::cpps_string_strcut)
 				.def_inside("strcuts", &cpps::string::cpps_string_strcuts)
 				.def("empty", &cpps::string::cpps_string_empty)
 				.def("substr", &cpps::string::cpps_string_sub)
 				.def("at", &cpps::string::cpps_string_at)
 				.def("tolower", &cpps::string::cpps_string_tolower)
 				.def("toupper", &cpps::string::cpps_string_toupper)
 				.def("trim", &cpps::string::cpps_string_trim)
 				.def("ltrim", &cpps::string::cpps_string_ltrim)
 				.def("rtrim", &cpps::string::cpps_string_rtrim)
 				.def("join", &cpps::string::cpps_string_join)
 				.def("startswith", &cpps::string::cpps_string_startswith)
 				.def("endswith", &cpps::string::cpps_string_endswith)
 				.def("pop_back", &cpps::string::cpps_string_pop_back)
 				.def("push_back", &cpps::string::cpps_string_push_back)
 				.def("append", &cpps::string::cpps_string_append)
 				.def("title", &cpps::string::cpps_string_title)
 				.def("center", &cpps::string::cpps_string_center)
 				.def("isalnum", &cpps::string::cpps_string_isalnum)
 				.def("isalpha", &cpps::string::cpps_string_isalpha)
 				.def("isdecimal", &cpps::string::cpps_string_isdecimal)
		];

		cpps::_module(c,"string")[
			def("find", cpps_string_find),
			def("rfind", cpps_string_rfind),
			def("length", cpps_string_len),
			def("strlen", cpps_string_strlen),
			def("replace", cpps_string_replace),
			def("clear", cpps_string_clear),
			def("copyto", cpps_string_copyto),
			def_inside("split", cpps_string_split),
			def_inside("cut", cpps_string_cut),
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
			def("join", cpps_string_join),
			def("between", cpps_string_between),
			def("startswith", cpps_string_startswith),
			def("endswith", cpps_string_endswith),
			def("pop_back", cpps_string_pop_back),
			def_inside("chr",cpps_string_chr),
			def("push_back", cpps_string_push_back),
			def("unicode_charCodeAt",cpps_string_unicode_charCodeAt),
			def_inside("unicode_fromCodeAt",cpps_string_unicode_fromCodeAt),
			def("isalnum",cpps_string_isalnum),
			def("isalpha", cpps_string_isalpha),
			def("isspace",cpps_string_isspace)
		];

	}

	cpps_integer string::cpps_size()
	{
		return (cpps_integer)__str.size();
	}

	cpps_integer string::cpps_string_find(std::string v2, object off)
	{
		size_t pos = 0;
		if (off.isint()) pos = (size_t)off.toint();
		return (cpps_integer)__str.find(v2, pos);
	}

	cpps_integer string::cpps_string_rfind(std::string v2, object off)
	{
		size_t pos = 0;
		if (off.isint()) pos = (size_t)off.toint();
		return (cpps_integer)__str.rfind(v2, pos);
	}

	cpps::string* string::cpps_string_replace(std::string v2, std::string v3)
	{
		std::string::size_type pos = 0;

		while ((pos = __str.find(v2, pos)) != std::string::npos)
		{
			__str.replace(pos, v2.length(), v3);
			pos += v3.length();
		}
		return this;
	}

	void string::cpps_string_clear()
	{
		__str.clear();
	}

	void string::cpps_string_copyto(cpps::string* tar, object off, object len)
	{
		size_t noff = 0;
		size_t nlen = std::string::npos;
		if (off.isint()) noff = (size_t)off.toint();
		if (len.isint()) nlen = (size_t)len.toint();

		tar->__str.append(__str.substr(noff, nlen));
	}

	cpps::cpps_value string::cpps_string_split(C*c,std::string v2, object len)
	{
		cpps_integer nlen = -1;
		if (len.isint()) nlen = len.toint();

		cpps_vector* vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec,&ret);


		if (__str.empty()) return ret;

		const char* a = __str.c_str(); const char* b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vec->push_back(cpps_value(c, s));

			if (nlen != -1 && vec->size() >= nlen) break;

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}

		return ret;
	}

	cpps::cpps_value string::cpps_string_cut(C* c, cpps_integer len)
	{
		cpps_vector* vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec, &ret);

		size_t ncount = (size_t)len;
		size_t FileSize = __str.size();
		size_t pos1 = 0;
		for (size_t j = 0; j < FileSize / ncount + 1; j++)
		{
			size_t res = FileSize - pos1 < ncount ? FileSize - pos1 : ncount;
			if (res != 0)
			{
				std::string* s;
				cpps_value sv;
				newclass<std::string>(c, &s,&sv);
				s->append(__str.c_str() + pos1, res);
				vec->push_back(sv);
				pos1 += res;

			}
		}

		return ret;
	}

	std::string string::cpps_string_strcut(std::string v2, std::string v3)
	{
		const char* strtmp1 = strstr(__str.c_str(), v2.c_str());
		if (!strtmp1) return "";

		const char* strtmp2 = strstr(strtmp1 + v2.size(), v3.c_str());
		if (!strtmp2) return "";

		std::string out;
		out.append(strtmp1 + v2.size(), strtmp2 - strtmp1 - v2.size());

		return out;
	}

	cpps::cpps_value string::cpps_string_strcuts(C* c, std::string start, std::string end)
	{
		cpps_vector* vec;
		cpps_value ret;
		newclass<cpps_vector>(c, &vec,&ret);


		const char* begin = __str.c_str();
		while (true)
		{
			const char* strtmp1 = strstr(begin, start.c_str());
			if (!strtmp1) return ret;

			const char* strtmp2 = strstr(strtmp1 + start.size(), end.c_str());
			if (!strtmp2) return ret;

			std::string out;
			out.append(strtmp1 + start.size(), strtmp2 - strtmp1 - start.size());

			vec->push_back(cpps_value(c, out));

			begin = strtmp2 + end.size();
		}
		return ret;
	}

	bool string::cpps_string_empty()
	{
		return __str.empty();
	}

	bool string::cpps_string_startswith(std::string v)
	{
		return __str.find(v) == 0;
	}

	bool string::cpps_string_endswith(std::string v)
	{
		size_t pos = __str.rfind(v);
		if (pos == std::string::npos) return false;
		if (pos + v.size() == __str.size()) return true;
		return false;
	}

	std::string string::cpps_string_sub(cpps_integer off, object len)
	{
		size_t nlen = std::string::npos;
		if (len.isint()) nlen = (size_t)len.toint();

		return __str.substr(size_t(off), nlen);
	}

	cpps_integer string::cpps_string_at(cpps_integer off)
	{
		return (cpps_integer)__str.at(size_t(off));
	}

	std::string string::cpps_string_tolower()
	{
		std::string ret;
		ret.resize(__str.size());
		for (size_t i = 0; i < __str.size(); i++) {
			ret[i] = (char)tolower(__str.at(i));
		}
		return ret;
	}

	std::string string::cpps_string_toupper()
	{
		std::string ret;
		ret.resize(__str.size());
		for (size_t i = 0; i < __str.size(); i++) {
			ret[i] = (char)toupper(__str.at(i));
		}
		return ret;
	}

	std::string string::cpps_string_join(cpps_vector* vec)
	{
		std::string ret;
		for (auto s : vec->realvector()) {
			ret += cpps_to_string(s);
			ret += __str.c_str();
		}
		return ret;
	}

	void string::cpps_string_trim()
	{
		cpps_string_rtrim();
		cpps_string_ltrim();
	}

	void string::cpps_string_ltrim()
	{
		std::string::iterator   p = find_if(__str.begin(), __str.end(), [](char code) { return !isspace(code); });
		__str.erase(__str.begin(), p);
	}

	void string::cpps_string_rtrim()
	{
		std::string::reverse_iterator  p = find_if(__str.rbegin(), __str.rend(), [](char code) { return !isspace(code); });
		__str.erase(p.base(), __str.end());
	}

	void string::cpps_string_pop_back(object len)
	{
		size_t nlen = 1;
		if (len.isint()) nlen = (size_t)len.toint();
		for (size_t i = 0; i < nlen; i++)
		{
			__str.pop_back();
		}
	}

	void string::cpps_string_push_back(cpps_integer charcode)
	{
		__str.push_back((const char)charcode);
	}

	void string::cpps_string_append(std::string v)
	{
		__str.append(v);
	}
	std::string string::cpps_string_title()
	{
		std::string ret;
		ret.resize(__str.size());

		for (size_t i = 0; i < __str.size(); ) {
			if (i == 0 && __str[i] >= 'a' && __str[i] <= 'z') {
				ret[i] = __str[i] - 32;
				++i;
			} 
			else if((__str[i] == '\t' || __str[i] == ' ' ) && __str[i+1] >= 'a' && __str[i+1] <= 'z') {
				ret[i] = __str[i];
				ret[i+1] = __str[i+1] - 32;
				i+=2;
			}
			else {
				ret[i] = __str[i];
				++i;
			}
		}
		return ret;
	}
	std::string string::cpps_string_center(cpps_integer width)
	{
		if (width <= (cpps_integer)__str.size()) return __str;

		std::string ret;
		ret.resize(width);
		cpps_integer srcwidth = (cpps_integer)__str.size();
		cpps_integer leftwidth = (cpps_integer)((width - srcwidth) / 2);
		cpps_integer rightwidth = width - leftwidth - srcwidth;
		cpps_integer i = 0;
		for (;i < leftwidth; ++i){
			ret[i] = ' ';
		}
		for (i = 0; i < srcwidth; ++i) {
			ret[i] = __str[i];
		}
		for (i = 0; i < rightwidth; ++i) {
			ret[i] = ' ';
		}
		return ret;
	}

	bool string::cpps_string_isalnum()
	{
		for (size_t i = 0; i < __str.size(); ++i) {
			if (!isalnum(__str[i])) return false;
		}
		return true;
	}
	bool string::cpps_string_isalpha()
	{
		for (size_t i = 0; i < __str.size(); ++i) {
			if (!isalpha(__str[i])) return false;
		}
		return true;
	}
	bool string::cpps_string_isspace()
	{
		for (size_t i = 0; i < __str.size(); ++i) {
			if (!isspace(__str[i])) return false;
		}
		return true;
	}
	bool string::cpps_string_isdecimal() {
		for (int i = 0; i < __str.size(); i++) {
			if (__str.at(i) == '-' && __str.size() > 1)  // 有可能出现负数
				continue;
			if (__str.at(i) > '9' || __str.at(i) < '0')
				return false;
		}
		return true;
	}
	

	std::string& string::real()
	{
		return __str;
	}

}

