#include "cpps/cpps.h"


cpps_trycatch_error::cpps_trycatch_error(std::string f, int l, int n, const char *format, ...)
{
	char szString[4096];
	va_list ap;
	va_start(ap, format);
#ifdef _WIN32
	vsprintf_s(szString, 4096, format, ap);
#else
	vsprintf(szString, format, ap);
#endif
	va_end(ap);

	erron = n;
	file = f;
	line = l;
	s = szString;
}

cpps_trycatch_error::cpps_trycatch_error(cpps_error e)
{
	erron = e.erron;
	file = e.file;
	line = e.line;
	s = e.s;
}

cpps_trycatch_error::~cpps_trycatch_error()
{
}
