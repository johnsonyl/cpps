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

#else
#define TCP_NODELAY 0x1
#endif
// 在此处引用程序需要的其他标头
