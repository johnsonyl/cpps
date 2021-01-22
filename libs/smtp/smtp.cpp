

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include "cpps_smtp.h"


using namespace cpps;
using namespace std;
cpps_export_void cpps_attach(cpps::C* c)
{
    curl_global_init(CURL_GLOBAL_ALL);

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "smtp")[
		_class<cpps_smtp>("smtpclient")
			.def("setsmtp", &cpps_smtp::setsmtp)
			.def("setmailfrom", &cpps_smtp::setmailfrom)
			.def("setuserinfo", &cpps_smtp::setuserinfo)
			.def("openssl", &cpps_smtp::openssl)
			.def("addrecipient", &cpps_smtp::addrecipient)
			.def("setverbose", &cpps_smtp::setverbose)
			.def("settimeout", &cpps_smtp::settimeout)
			.def("setreadcallback", &cpps_smtp::setreadcallback)
			.def("setcontent", &cpps_smtp::setcontent)
			.def_inside("send", &cpps_smtp::send)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "smtp");
    curl_global_cleanup();
}

cpps_export_finish