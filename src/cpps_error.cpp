#include "cpps/cpps.h"

namespace cpps
{
	inline void fail(std::string msg, int32 n /*= cpps_error_normalerror*/, const char* f /*= __FILE__*/, usint32 l /*= __LINE__*/)
	{
		throw cpps_error(f, l, n, msg.c_str());
	}

	

	inline void error(C* c, const char* format, ...)
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

		throw cpps_error(c->curnode->filename, c->curnode->line, cpps_error_normalerror, szString);
	}

}