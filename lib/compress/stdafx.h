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

#endif

#define  DEF_BUF_SIZE 4096 

#ifdef LINUX  
#include <unistd.h>  
#include <sys/types.h>
#include <pwd.h>  
#include <grp.h>
#endif  

#ifdef _WIN32  
#include<Windows.h>  
#endif  

#ifdef WIN32
#define S_ISDIR(m) m & S_IFDIR
#define S_ISREG(m) m & S_IFREG
#define S_ISBLK(m) false
#define S_ISCHR(m) m & S_IFCHR
#define S_ISFIFO(m) m & _S_IFIFO
#define S_ISLNK(m)  false
#define S_ISSOCK(m)  false
#endif

// 在此处引用程序需要的其他标头
