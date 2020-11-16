// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include "cpps_curl.h"

using namespace cpps;
using namespace std;

void     cpps_curl_easy_cleanup(cpps_curl* curl)
{
    if (curl == NULL) return ;
    for (auto slist : curl->slists)
    {
        curl_slist_free_all(slist);
    }
    curl->slists.clear();

    if (curl->curl != NULL) {
        curl_easy_cleanup(curl->curl);
    }
}
cpps_integer cpps_curl_easy_getinfo(C*c,cpps_curl*curl, cpps_integer info, cpps_vector* vec)
{
    cpps_integer ret = CURLE_OK;
    struct curl_slist* cookies = NULL;
     ret = curl_easy_getinfo(curl->curl,(CURLINFO) info, &cookies);

    while (cookies) {
        vec->push_back( cpps_value(c,cookies->data) );
        cookies = cookies->next;
    }
    curl_slist_free_all(cookies);
    return ret;
}
cpps_curl*     cpps_curl_easy_init()
{
    cpps_curl* ret = new cpps_curl();
    ret->curl = curl_easy_init();
    return ret;
}

size_t cpps_curl_write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    cpps_curl* curl = (cpps_curl*)stream;
    size_t len = size * nmemb;
    curl->append_write_data((const char*)ptr, len);
    return len;
}
size_t cpps_curl_header_callback(const char* ptr, size_t size, size_t nmemb, void* stream)
{
    cpps_curl* curl = (cpps_curl*)stream;
    size_t len = size * nmemb;
    curl->append_header(ptr, len);
    return len;
}
size_t cpps_curl_read_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t num_bytes = size * nmemb;
    char* data = (char*)ptr;
    cpps_curl* curl = (cpps_curl*)stream;

    curl->strstream.read(data, num_bytes);

    return  curl->strstream.gcount();
}
cpps_integer     cpps_curl_easy_perform(cpps_curl* curl)
{
    curl_easy_setopt(curl->curl, CURLOPT_WRITEDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_WRITEFUNCTION, cpps_curl_write_data);

    curl_easy_setopt(curl->curl, CURLOPT_HEADERDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_HEADERFUNCTION, cpps_curl_header_callback);

    curl_easy_setopt(curl->curl, CURLOPT_READDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_READFUNCTION, cpps_curl_read_data);

    return curl_easy_perform(curl->curl);
}
cpps_integer cpps_curl_option_type(cpps_integer option)
{
    cpps_integer ret = -1;
    switch (option)
    {
    case CURLOPT_SSL_VERIFYPEER:
    case CURLOPT_SSL_VERIFYHOST:
    case CURLOPT_HTTPPROXYTUNNEL:
    case CURLOPT_VERBOSE:
    case CURLOPT_PROXYTYPE:
    case CURLOPT_POST:
    case CURLOPT_FOLLOWLOCATION:
    case CURLOPT_TIMEOUT:
    case CURLOPT_NOSIGNAL:
    case CURLOPT_UPLOAD:
    case CURLOPT_PUT:
    case CURLOPT_USE_SSL:
        ret = 1;
        break;
    case CURLOPT_PROXY:
    case CURLOPT_COOKIEJAR:
    case CURLOPT_COOKIEFILE:
    case CURLOPT_POSTFIELDS:
    case CURLOPT_URL:
    case CURLOPT_MAIL_FROM:
        ret = 2;
        break;
    case CURLOPT_HTTPHEADER:
    case CURLINFO_COOKIELIST:
    case CURLOPT_MAIL_RCPT:
        ret = 3;
        break;
    default:
        break;
    }
    return ret;
}
cpps_integer     cpps_curl_easy_setopt(cpps_curl* curl, cpps_integer option, cpps_value arg)
{
    cpps_integer ret = -1;
    cpps_integer t = cpps_curl_option_type(option);
    switch (t)
    {
    case 1: //int
        {
            cpps_integer v = cpps_to_integer(arg);
            curl_easy_setopt(curl->curl, (CURLoption)option, v);
        }
        break;
    case 2://string
        {
            std::string v = cpps_to_string(arg);
            curl_easy_setopt(curl->curl, (CURLoption)option, v.c_str());
        }
        break;
    case 3://vector
        {
            cpps_vector* v = cpps_to_cpps_vector(arg);
            struct curl_slist* slist = NULL;
            for (auto i = 0; i < v->realvector().size(); i++)
            {
                slist = curl_slist_append(slist, cpps_to_string(v->realvector()[i]).c_str());
            }
            curl_easy_setopt(curl->curl, (CURLoption)option, slist);
            curl->slists.push_back(slist);
        }
        break;
    default:

        break;
    }
    return ret;
}
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
#else
extern "C" void  cpps_attach(cpps::C* c)
#endif
{
    curl_global_init(CURL_GLOBAL_ALL);

	cpps::cpps_init_cpps_class(c);

    module(c, "curl")[
        _class<cpps_curl>("CURL")
            .def("getdata", &cpps_curl::getdata)
            .def("getheader", &cpps_curl::getheader)
            .def("append", &cpps_curl::append_get_data),
        def("curl_easy_init", cpps_curl_easy_init),
        def("curl_easy_cleanup", cpps_curl_easy_cleanup),
        def("curl_easy_setopt", cpps_curl_easy_setopt),
        def_inside("curl_easy_getinfo", cpps_curl_easy_getinfo),
        def("curl_easy_perform", cpps_curl_easy_perform),
        defvar(c,"CURLOPT_SSL_VERIFYPEER", (cpps_integer)CURLOPT_SSL_VERIFYPEER),
        defvar(c,"CURLOPT_SSL_VERIFYHOST", (cpps_integer)CURLOPT_SSL_VERIFYHOST),
        defvar(c,"CURLOPT_HTTPPROXYTUNNEL", (cpps_integer)CURLOPT_HTTPPROXYTUNNEL),
        defvar(c,"CURLOPT_VERBOSE", (cpps_integer)CURLOPT_VERBOSE),
        defvar(c,"CURLOPT_PROXY", (cpps_integer)CURLOPT_PROXY),
        defvar(c,"CURLOPT_PROXYTYPE", (cpps_integer)CURLOPT_PROXYTYPE),
        defvar(c,"CURLPROXY_HTTP", (cpps_integer)CURLPROXY_HTTP),
        defvar(c,"CURLPROXY_HTTP_1_0", (cpps_integer)CURLPROXY_HTTP_1_0),
        defvar(c,"CURLPROXY_HTTPS", (cpps_integer)CURLPROXY_HTTPS),
        defvar(c,"CURLPROXY_SOCKS4", (cpps_integer)CURLPROXY_SOCKS4),
        defvar(c,"CURLPROXY_SOCKS5", (cpps_integer)CURLPROXY_SOCKS5),
        defvar(c,"CURLPROXY_SOCKS4A", (cpps_integer)CURLPROXY_SOCKS4A),
        defvar(c,"CURLPROXY_SOCKS5_HOSTNAME", (cpps_integer)CURLPROXY_SOCKS5_HOSTNAME),
        defvar(c,"CURLOPT_POST", (cpps_integer)CURLOPT_POST),
        defvar(c,"CURLOPT_COOKIEJAR", (cpps_integer)CURLOPT_COOKIEJAR),
        defvar(c,"CURLOPT_COOKIEFILE", (cpps_integer)CURLOPT_COOKIEFILE),
        defvar(c,"CURLOPT_HTTPHEADER", (cpps_integer)CURLOPT_HTTPHEADER),
        defvar(c,"CURLOPT_POSTFIELDS", (cpps_integer)CURLOPT_POSTFIELDS),
        defvar(c,"CURLOPT_URL", (cpps_integer)CURLOPT_URL),
        defvar(c,"CURLOPT_FOLLOWLOCATION", (cpps_integer)CURLOPT_FOLLOWLOCATION),
        defvar(c,"CURLOPT_TIMEOUT", (cpps_integer)CURLOPT_TIMEOUT),
        defvar(c,"CURLOPT_NOSIGNAL", (cpps_integer)CURLOPT_NOSIGNAL),
        defvar(c,"CURLOPT_UPLOAD", (cpps_integer)CURLOPT_UPLOAD),
        defvar(c,"CURLINFO_COOKIELIST", (cpps_integer)CURLINFO_COOKIELIST),
        defvar(c,"CURLOPT_MAIL_FROM", (cpps_integer)CURLOPT_MAIL_FROM),
        defvar(c,"CURLOPT_USE_SSL", (cpps_integer)CURLOPT_USE_SSL),
        defvar(c,"CURLUSESSL_ALL", (cpps_integer)CURLUSESSL_ALL),
        defvar(c,"CURLUSESSL_NONE", (cpps_integer)CURLUSESSL_NONE),
        defvar(c,"CURLUSESSL_TRY", (cpps_integer)CURLUSESSL_TRY),
        defvar(c,"CURLUSESSL_CONTROL", (cpps_integer)CURLUSESSL_CONTROL),
        defvar(c,"CURLUSESSL_LAST", (cpps_integer)CURLUSESSL_LAST),
        defvar(c,"CURLOPT_MAIL_RCPT", (cpps_integer)CURLOPT_MAIL_RCPT),
        defvar(c,"CURLOPT_PUT", (cpps_integer)CURLOPT_PUT)
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