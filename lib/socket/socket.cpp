// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <cpps.h>
#ifdef _WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif
using namespace cpps;
using namespace std;
#include "cpps_socket.h"
#include "cpps_socket_client.h"
#include "cpps_socket_server.h"
#include "cpps_socket_server_client.h"
#include "cpps_socket_httpserver.h"
#include "cpps_socket_httpserver_request.h"

cpps_export_void  cpps_attach(cpps::C* c)
{
#ifdef _WIN32
	WSADATA wsadata;
	WSAStartup(0x0202, &wsadata);
#endif
	cpps::cpps_init_cpps_class(c);

	cpps::_module(c,"socket")[
		_class<cpps_socket_server>("server")
		.def("setoption", &cpps_socket_server::setoption)
		.def_inside("listen", &cpps_socket_server::listen)
		.def("run", &cpps_socket_server::run)
		.def("send", &cpps_socket_server::send)
		.def("closesocket", &cpps_socket_server::closesocket)
		.def("stop", &cpps_socket_server::stop)
		.def("isrunning", &cpps_socket_server::isrunning),
		_class<cpps_socket_httpserver>("httpserver")
		.def("setoption", &cpps_socket_httpserver::setoption)
		.def_inside("listen", &cpps_socket_httpserver::listen)
		.def("run", &cpps_socket_httpserver::run)
		.def("register_handlefunc", &cpps_socket_httpserver::register_handlefunc)
		.def("register_controller", &cpps_socket_httpserver::register_controller)
		.def("stop", &cpps_socket_httpserver::stop)
		.def("isrunning", &cpps_socket_httpserver::isrunning),
		_class<cpps_socket_client>("client")
		.def("setoption", &cpps_socket_client::setoption)
		.def("run", &cpps_socket_client::run)
		.def_inside("connect", &cpps_socket_client::connect)
		.def("send", &cpps_socket_client::send)
		.def("isconnect", &cpps_socket_client::isconnect)
		.def("closesocket", &cpps_socket_client::closesocket),
		_class< cpps_socket_httpserver_request>("httprequest")
		.def("addheader", &cpps_socket_httpserver_request::addheader)
		.def("append", &cpps_socket_httpserver_request::append)
		.def("send", &cpps_socket_httpserver_request::send)
		.def("getparam", &cpps_socket_httpserver_request::getparam)
		.def("getheader", &cpps_socket_httpserver_request::getheader)
		.def("geturi", &cpps_socket_httpserver_request::geturi)
		.def("getpath", &cpps_socket_httpserver_request::getpath)
		.def("getbuffer", &cpps_socket_httpserver_request::getbuffer)
	];

}
cpps_export_void  cpps_detach(cpps::C * c)
{
#ifdef _WIN32
	WSACleanup();
#endif
}

cpps_export_finish