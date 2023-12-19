#include "cpps/cpps.h"


cpps_trycatch_error::cpps_trycatch_error() { _erron = 0; _line = 0; }

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

	_erron = n;
	_file = f;
	_line = l;
	_s = szString;
}

void cpps_trycatch_error::attach(cpps_error e)
{
	_erron = e.error();
	_file = e.file();
	_line = e.line();
	_s = e.what();
}

cpps::cpps_value cpps_trycatch_error::value() { return _value; }

int cpps_trycatch_error::error() { return _erron; }

std::string cpps_trycatch_error::file() { return _file; }

int cpps_trycatch_error::line() { return _line; }

std::string cpps_trycatch_error::what() { return _s; }

std::string cpps_trycatch_error::callstack() { return _callstackstr; }

std::string cpps_trycatch_error::tostring() {
	char* fmt = new char[40960]; //40K´íÎó×Ü¹»ÁË°É
	memset(fmt, 0, 40960);
	std::string ret;
	sprintf(fmt, "ERROR INFOMATION: %s  file:%s line:%d\r\n%s", what().c_str(), file().c_str(), line(), callstack().c_str());
	ret.append(fmt);
	delete[] fmt;
	return ret;
}

cpps_trycatch_error::~cpps_trycatch_error()
{
}
