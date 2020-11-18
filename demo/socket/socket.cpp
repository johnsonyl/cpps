// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <cpps.h>
#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif
using namespace cpps;
using namespace std;
#include "cpps_socket.h"
#include "cpps_socket_client.h"
#include "cpps_socket_server.h"
#include "cpps_socket_server_client.h"


#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
#else
extern "C" void  cpps_attach(cpps::C* c)
#endif
{
#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(0x0202, &wsadata);
#endif
	cpps::cpps_init_cpps_class(c);

	module(c,"socket")[
		_class<cpps_socket_server>("server")
		.def_inside("setoption", &cpps_socket_server::setoption)
		.def("listen", &cpps_socket_server::listen)
		.def("run", &cpps_socket_server::run)
		.def("send", &cpps_socket_server::send)
		.def("closesocket", &cpps_socket_server::closesocket)
		.def("stop", &cpps_socket_server::stop)
		.def("isrunning", &cpps_socket_server::isrunning),
		_class<cpps_socket_client>("client")
		.def_inside("setoption", &cpps_socket_client::setoption)
		.def("run", &cpps_socket_client::run)
		.def("connect", &cpps_socket_client::connect)
		.def("send", &cpps_socket_client::send)
		.def("isconnect", &cpps_socket_client::isconnect)
	];

}
#ifdef _WIN32
extern "C" _declspec(dllexport) void __stdcall cpps_detach(cpps::C *c)
#else
extern "C" void  cpps_detach(cpps::C * c)
#endif
{
#ifdef WIN32
	WSACleanup();
#endif
}

#ifdef LINUX


extern "C" const CPPS_ST_API  LIBAPI = {
   cpps_attach,
   cpps_detach,
};

#endif