#include "cpps/cpps.h"

namespace cpps
{
	void cpps_base_printfln_new(C* c, cpps::cpps_value args, ...);
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
#ifdef _WIN32
	bool cls() //编程方式实现清除屏幕
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
		DWORD dwConSize;                 /* number of character cells in the current buffer */

		/* get the number of character cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			return false;
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		/* fill the entire screen with blanks */
		if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
			return false;

		/* get the current text attribute */
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			return false;

		/* now set the buffer's attributes accordingly */
		if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
			return false;

		/* put the cursor at (0, 0) */
		if (!SetConsoleCursorPosition(hConsole, coordScreen))
			return false;

		return true;
	}
#endif

	int32 cpps_console_clear()
	{
#ifdef _WIN32
		return (int32)cls();
#else
		printf("\033[2J");
		return 1;
#endif

	}
	void cpps_console_cursor(bool visible)
	{
#ifdef _WIN32
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hOut, &cci);
		cci.bVisible = visible;
		SetConsoleCursorInfo(hOut, &cci);
#endif
	}
	void cpps_regconsole(C* c)
	{
		cpps::_module(c,"console")[
			def("color", cpps_console_color),
			def("cursor", cpps_console_cursor),
			def("clearcolor", cpps_console_clearcolor),
			def("clear", cpps_console_clear),
			def("log",cpps_base_printfln_new)
		];
	}
}