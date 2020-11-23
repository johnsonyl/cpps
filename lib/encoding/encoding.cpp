// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps.h>
#include <iostream>
#include <string>
#include <fstream>
#include "strnormalize.h"

bool cpps_check_utf8(char* str, size_t length)
{
	cpps::usint32 nBytes = 0;
	cpps::usint8 chr;
	/*bool bAllAscii = true;*/
	for (size_t i = 0; i < length; ++i)
	{
		chr = *(str + i);
		/*if ((chr & 0x80) != 0)
			bAllAscii = false;*/
		if (nBytes == 0)
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
					return false;

				nBytes--;
			}
		}
		else
		{
			if ((chr & 0xC0) != 0x80)
				return false;

			nBytes--;
		}
	}
	if (nBytes > 0)
		return false;
	//if (bAllAscii) 
	//	return false;

	return TRUE;
}
#define CPPS_ENCODING_UTF8 0
#define CPPS_ENCODING_GBK 1

std::string  encode(cpps::cpps_value src, cpps::usint8 encoding)
{
	std::string ret;
	std::string* s = cpps_get_string(src);
	if (encoding == CPPS_ENCODING_UTF8)
	{
		bool b = cpps_check_utf8((char*)s->c_str(), s->size());
		if (b) return *s;

		size_t utf8buffer_len = s->size() * 3 + 1;
		char* utf8buffer = (char*)malloc(utf8buffer_len);
		memset(utf8buffer, 0, utf8buffer_len);

		gbk_to_utf8(s->c_str(), (unsigned int)s->size(), &utf8buffer, (unsigned int *)&utf8buffer_len);

		ret = utf8buffer;

		free(utf8buffer);
	}
	else if(encoding == CPPS_ENCODING_GBK)
	{
		bool b = cpps_check_utf8((char*)s->c_str(), s->size());
		if (!b) return *s;

		size_t gbkbuffer_len = s->size() * 2 + 1;
		char* gbkbuffer = (char*)malloc(gbkbuffer_len);
		memset(gbkbuffer, 0, gbkbuffer_len);

		utf8_to_gbk(s->c_str(), (unsigned int)s->size(), &gbkbuffer, (unsigned int*)&gbkbuffer_len);
		ret = gbkbuffer;
		free(gbkbuffer);

	}
	return ret;
}

using namespace cpps;
using namespace std;
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
#else
extern "C" void  cpps_attach(cpps::C* c)
#endif
{

	cpps::cpps_init_cpps_class(c);

	str_normalize_init();

	module(c, "encoding")[
		def("encode", encode),
		defvar(c,"UTF8", CPPS_ENCODING_UTF8),
		defvar(c,"GBK", CPPS_ENCODING_GBK)
	];



   
}
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_detach(cpps::C *c)
#else
extern "C" void  cpps_detach(cpps::C * c)
#endif
{
}

#ifdef LINUX


extern "C" const CPPS_ST_API  LIBAPI = {
   cpps_attach,
   cpps_detach,
};

#endif