#include <iostream>
#include "cpps/cpps.h"

using namespace cpps;
using namespace std;

namespace cpps { std::string cpps_io_getfilepath(std::string str);std::string cpps_rebuild_filepath(std::string path);std::string cpps_real_path(); }

int32 main(int argc,char **argv)
{
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
		else if (path == "-install" || path == "-uninstall") {
			path = "lib/install/main.cpp";
			path = cpps_rebuild_filepath(path);
#ifdef WIN32
			SetCurrentDirectoryA(cpps_real_path().c_str());
#else
			chdir(cpps_real_path().c_str());
#endif
		}
		else if (path == "-h" || path == "-help")
		{
			printf("usage: cppsc [option or filepath] ... [arg] ...\r\n");
			printf("Options and arguments:\r\n");
			printf("-v\t: print the cpps version and exit (also -version)\r\n");
			printf("-vno\t: print the cpps version number and exit \r\n");
			printf("\r\n---\r\n");
			printf("filepath: run a cpps script file.\r\n");
			return 0;
		}
		else {
			printf("Unknow option:%s\r\n", path.c_str());
			printf("Try 'cppsc -h' for more infomation.\r\n");
			return 0;
		}

	}
	else {

		path = cpps_rebuild_filepath(path);
#ifdef WIN32
		SetCurrentDirectoryA(cpps_io_getfilepath(path).c_str());
#else
		chdir(cpps_io_getfilepath(path).c_str());
#endif
	}


	C* c = cpps::create(argc,argv);

	printf("[%s Copyright (C) 2015-2020 By Johnson, Pub-test]\n", CPPS_VER);


	cpps_try
	cpps::dofile(c, path.c_str());
	cpps_catch

	cpps::close(c);

	return 0;
}