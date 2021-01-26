#include "cpps/cpps.h"

namespace cpps
{
	void fail(std::string msg, int32 n /*= cpps_error_normalerror*/, const char* f /*= __FILE__*/, usint32 l /*= __LINE__*/)
	{
		throw cpps_error(f, l, n, msg.c_str());
	}
}