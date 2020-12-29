#include "cpps/cpps.h"

namespace cpps
{
	void cpps_console_clearcolor()
	{
#ifdef _WIN32
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
		printf("\033[0m");
#endif
	}
	void cpps_console_color(cpps_integer color)
	{
#ifdef _WIN32
		WORD tcolor[] = {0, 4,2,6,3,5,11 };
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, tcolor[color%7]);
#else
		printf("\033[%dm", int(color % 7 + 30));
#endif
	}

	int32 cpps_console_clear()
	{
#ifdef _WIN32
		return system("cls");
#else
		return system("clear");
#endif

	}
	void cpps_regconsole(C* c)
	{
		cpps::_module(c,"console")[
			def("color", cpps_console_color),
			def("clearcolor", cpps_console_clearcolor),
			def("clear", cpps_console_clear)
		];
	}
}