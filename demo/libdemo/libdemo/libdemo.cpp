// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps.h>
using namespace cpps;

void demotest()
{
	printf("hit demo function.\r\n");
}
extern "C" _declspec(dllexport) void __stdcall cpps_attach(cpps::C *c)
{
	printf("attach demo\r\n");

	module(c)[
		def("demotest", &demotest)
	];
}
extern "C" _declspec(dllexport) void __stdcall cpps_detach(cpps::C *c)
{
	printf("detach demo\r\n");
}