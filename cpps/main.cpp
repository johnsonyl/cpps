#include <iostream>
#include "cpps/cpps.h"
#ifdef _WIN32
#include "resource.h"
#endif

using namespace cpps;
using namespace std;

namespace cpps { std::string cpps_io_getfilepath(std::string str);std::string cpps_rebuild_filepath(std::string path);std::string cpps_real_path(); }



#ifdef USE_TCMALLOC

void*	tcmalloc_func(size_t _size) {
	return malloc(_size);
}
void	tcfree_func(void* p) {
	free(p);
}

#endif
#ifndef _DEBUG
void __check_cpps_package_json(int argc, char** argv) {
	std::string path = cpps_rebuild_filepath("lib/@check_package/main.cpp");
	
#ifdef USE_TCMALLOC
	C* c = cpps::create(argc, argv, tcmalloc_func, tcfree_func);
#else
	C* c = cpps::create(argc, argv);
#endif

	cpps_try
		if (!path.empty()) cpps::dofile(c, path.c_str());
	}
	catch (cpps_error& e)
	{
		printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(), e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) : e.line()); 
		cpps::close(c); 
		exit(0);
	}
	catch (cpps_trycatch_error& e)
	{
		printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(), e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) : e.line());
		cpps::close(c);
		exit(0);
	}
	catch (const char* s)
	{
		printf("error: %s \n", s);
		cpps::close(c);
		exit(0);
	}

	cpps::close(c);
}
#endif
int32 main(int argc,char **argv)
{
#ifdef _WIN32
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	HWND  hwnd = GetConsoleWindow();
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
#endif
	std::string path = "main.cpp";

	if (argc >= 2) {
		path = argv[1];
	}

	if (path[0] == '-') {
		if (path == "-v" || path == "-version")
		{
			printf("%s\r\n", CPPS_VER);
			return 0;
		}
		else if (path == "-vno")
		{
			printf("%d\r\n", CPPS_VERN);
			return 0;
		}
		else if (path == "-install" || path == "-i" || path == "-up" || path == "-un" || path == "-uninstall" || path == "-update" || path == "-init") {
			path = "lib/install/main.cpp";
			path = cpps_rebuild_filepath(path);
#ifdef WIN32
			if(SetCurrentDirectoryA(cpps_real_path().c_str())){}
#else
			if (chdir(cpps_real_path().c_str())) {}
#endif
		}
		else if (path == "-h" || path == "-help")
		{
			printf("usage: cpps [option or filepath] ... [arg] ...\r\n");
			printf("Options and arguments:\r\n");
			printf("-v\t: print the cpps version and exit (also -version)\r\n");
			printf("-vno\t: print the cpps version number and exit \r\n");
			printf("\r\n---\r\n");
			printf("filepath: run a cpps script file.\r\n");
			return 0;
		}
		else {
			printf("Unknow option:%s\r\n", path.c_str());
			printf("Try 'cpps -h' for more infomation.\r\n");
			return 0;
		}

	}
	else {

		path = cpps_rebuild_filepath(path);
#ifdef WIN32
		if(SetCurrentDirectoryA(cpps_io_getfilepath(path).c_str())){}
#else
		if(chdir(cpps_io_getfilepath(path).c_str())){}
#endif
	}
	printf("[CPPS %s (%s, %s) on %s %s]\n", CPPS_VER, __DATE__, __TIME__, CPPS_CURRENT_EASYPLANTFORM, CPPS_CURRENT_ARCH);
#ifndef _DEBUG
	__check_cpps_package_json(argc, argv);
#endif
#ifdef USE_TCMALLOC
	C* c = cpps::create(argc, argv, tcmalloc_func, tcfree_func);
#else
	C* c = cpps::create(argc,argv);
#endif



	cpps_try
	if(!path.empty()) cpps::dofile(c, path.c_str());
	cpps_catch

	cpps::close(c);

	return 0;
}