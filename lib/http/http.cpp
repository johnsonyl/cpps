// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include "cpps_http_request.h"

using namespace cpps;
using namespace std;
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
#else
extern "C" void  cpps_attach(cpps::C* c)
#endif
{
    curl_global_init(CURL_GLOBAL_ALL);

	cpps::cpps_init_cpps_class(c);

	module(c, "http")[
        _class<cpps_http_request>("httprequest")
        .def("setcookiesfile",&cpps_http_request::setcookiesfile)
        .def("setproxy",&cpps_http_request::setproxy)
        .def("setproxyaccount",&cpps_http_request::setproxyaccount)
        .def("addheaders",&cpps_http_request::addheaders)
        .def("post",&cpps_http_request::post)
        .def("get",&cpps_http_request::get)
        .def("call",&cpps_http_request::call)
        .def("getcookies",&cpps_http_request::getcookies)
        .def("getheaders",&cpps_http_request::getheaders)
        .def("settimeout",&cpps_http_request::settimeout)
        .def("followlocation",&cpps_http_request::followlocation)
	];



   
}
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_detach(cpps::C *c)
#else
extern "C" void  cpps_detach(cpps::C * c)
#endif
{
    curl_global_cleanup();
}

#ifdef LINUX


extern "C" const CPPS_ST_API  LIBAPI = {
   cpps_attach,
   cpps_detach,
};

#endif