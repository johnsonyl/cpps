#ifndef compress_h__
#define compress_h__

#define  DEF_BUF_SIZE 4096 

#ifdef LINUX  
#include <unistd.h>  
#include <sys/types.h>
#include <pwd.h>  
#include <grp.h>
#endif  

#ifdef _WIN32  
#include <Windows.h>  
#endif  
#endif // compress_h__
