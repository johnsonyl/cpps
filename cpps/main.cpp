#include <iostream>
#include "cpps.h"

using namespace cpps;
using namespace std;


int32 getline_(char s[], int32 lim){
	int32 c, i;
	i = 0;
	while ((c = getchar()) != EOF&&c != '\n'&&i < lim - 1)
		s[i++] = c;
	s[i] = '\0';
	return i;
}

namespace cpps { std::string cpps_io_getfilepath(std::string str); }

int32 main(int argc,char **argv)
{
	std::string path = "script/main.cpp";

	if (argc == 2) {
		path = argv[1];
#ifdef WIN32
		SetCurrentDirectoryA(cpps_io_getfilepath(path).c_str());
#else
		chdir(cpps_io_getfilepath(path).c_str());
#endif

	}

	C* c = cpps::create();

	printf("[Cpps 1.2.0 Copyright (C) 2015-2020 By Johnson, Pub-test]\n");

	cpps_try
	cpps::dofile(c, path.c_str());
	cpps_catch



	while (true)
	{
		cout << ">";
		int8 s[4096];
		getline_(s, 4096);

		//如果输入了quit直接退出。
		if (strcmp("quit",s) == 0 )
		{
			break;
		}

		cpps_try
		cpps::dostring(c, s);
		cpps_catch
	}

	cpps::close(c);

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}