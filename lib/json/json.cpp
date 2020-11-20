// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps.h>
#include "json/json.h"
#include "jsonconvert.h"


using namespace cpps;


#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
#else
extern "C" void  cpps_attach(cpps::C* c)
#endif
{
	cpps::cpps_init_cpps_class(c);

	module(c, "json")[
		defvar(c, "encode_ascii", Json::encode_ascii),
		defvar(c, "encode_unicode", Json::encode_unicode),
		defvar(c, "encode_utf8", Json::encode_utf8),
		def("encode", &cppstojson),
		def("encodeT", &cppstojson_type),
		def_inside("decode", &jsontocpps)
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