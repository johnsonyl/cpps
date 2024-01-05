#include <iostream>
#include "cpps/cpps.h"
#ifdef _WIN32
#include "resource.h"
#endif

#ifdef USE_ZLIB_CPPS
#include <zlib.h>
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

#ifdef USE_ZLIB_CPPS
std::string zlib_decompress(std::string& data, cpps_integer nbufsize)
{

	const Bytef* buf = NULL;
	uLong len = 0;

	buf = (Bytef*)data.c_str();
	len = (uLong)data.size();
	

	Bytef* dest = new Bytef[size_t(nbufsize)];
	uLong destlen = (uLong)nbufsize;

	int32 err = uncompress2(dest, &destlen, buf, &len);
	if (err != Z_OK) {
		return "";
	}
	std::string ret;
	ret.append((const char*)dest, destlen);
	delete[]dest;
	dest = NULL;
	return ret;
}

#endif

std::string _cxo_handle_func(C*c,std::string& content)
{
#ifdef USE_ZLIB_CPPS
	Buffer buffer;
	buffer.writestring(content);
	buffer.seek(0);
	
	cpps_integer _version = buffer.readint();
	cpps_integer _src_size = buffer.readint();
	cpps_integer _compress_size = buffer.readint();
	std::string _compress_content = buffer.readstring(_compress_size);
	std::string __out_content = zlib_decompress(_compress_content, _src_size);
#else
	C* _c = cpps::create(c);
	cpps_init_cpps_class(_c);
	std::string path = cpps_rebuild_filepath("lib/cpps_compile/cxo.cpp");
	std::string __out_content;
	cpps_try
	if (!path.empty()) cpps::dofile(_c, path.c_str());
	object func = object::globals(_c)["__cxo_to_buffer"];
	object ret = cpps::dofunction(_c, func, content);
	__out_content = ret.tostring();
	cpps_catch
	
	cpps::close(_c);
#endif
	return __out_content;
}

int32 main(int argc,char **argv)
{
#ifdef _WIN32
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	HWND  hwnd = GetConsoleWindow();
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
#endif
	std::string path = "-script";

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
			printf("-v,-version\t: print the cpps version and exit (also -version)\r\n");
			printf("-vno\t: print the cpps version number and exit \r\n");
			printf("-script\t: type scripts in the console. \r\n");
			printf("-init\t: Initial installation of all modules. \r\n");
			printf("-i,-install module [-g,-global]\t: install a module in the current project(-g to the global). \r\n");
			printf("-up,-update module [-g,-global]\t: update a module in the current project(-g to the global). \r\n");
			printf("-un,-uninstall module [-g,-global]\t: uninstall a module in the current project(-g to the global). \r\n");
			printf("\r\n---\r\n");
			printf("filepath: run a cpps script file.\r\n");
			return 0;
		}
		else if (path == "-script") {
			path = "lib/@script/main.cpp";
			path = cpps_rebuild_filepath(path);
#ifdef WIN32
			if (SetCurrentDirectoryA(cpps_real_path().c_str())) {}
#else
			if (chdir(cpps_real_path().c_str())) {}
#endif
		}
		else {
			printf("Unknow option:%s\r\n", path.c_str());
			printf("Try 'cpps -h,-help' for more infomation.\r\n");
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

	c->set_cxo_handle_func(_cxo_handle_func);

	cpps_try
		if (!path.empty()) cpps::dofile(c, path.c_str());
	cpps_catch

	cpps::close(c);

	return 0;
}