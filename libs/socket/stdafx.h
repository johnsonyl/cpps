// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//
#pragma once
#include "targetver.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <string>
//windows libevent error msg is gbk.
inline std::string cpps_socket_g2u(std::string gb2312)
{
	std::string ret = "";
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[(size_t)len + 1];
	memset(wstr, 0, (size_t)len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = (char*)malloc((size_t)len + 1);
	memset(str, 0, (size_t)len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	ret = str;
	if (wstr) delete[] wstr;
	if (str) free(str);
	return ret;
}

#else
#define TCP_NODELAY 0x1
#endif
// 在此处引用程序需要的其他标头
