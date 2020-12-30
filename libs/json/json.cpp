// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps/cpps.h>
#include "json/json.h"
#include "jsonconvert.h"


using namespace cpps;


cpps_export_void  cpps_attach(cpps::C* c)
{
	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "json")[
		defvar(c, "encode_ascii", Json::encode_ascii),
		defvar(c, "encode_unicode", Json::encode_unicode),
		defvar(c, "encode_utf8", Json::encode_utf8),
		def("encode", &cppstojson),
		def_inside("decode", &jsontocpps)
	];
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish