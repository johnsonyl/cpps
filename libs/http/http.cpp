// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include "cpps_http_request.h"
#include "cpps_http_encode.h"
#include "cpps_http_downloader.h"
#include "cpps_http_uploader.h"

using namespace cpps;
using namespace std;
cpps_export_void cpps_attach(cpps::C* c)
{
    curl_global_init(CURL_GLOBAL_ALL);

	cpps::cpps_init_cpps_class(c);

    cpps::_module(c, "http")[
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
            .def("followlocation",&cpps_http_request::followlocation),
        def("urlencode",urlencode),
        def("urldecode",urldecode),
        _class< cpps_http_downloader>("downloader")
            .def("seturl",&cpps_http_downloader::seturl)
            .def("setfilepath",&cpps_http_downloader::setfilepath)
            .def_inside("download",&cpps_http_downloader::download),
        _class<cpps_http_uploader>("uploader")
            .def("addvalue", &cpps_http_uploader::addvalue)
            .def("addfile", &cpps_http_uploader::addfile)
            .def("setcookie", &cpps_http_uploader::setcookie)
            .def_inside("upload", &cpps_http_uploader::upload)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
    curl_global_cleanup();
}

cpps_export_finish