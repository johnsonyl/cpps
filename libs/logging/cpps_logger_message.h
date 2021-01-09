#ifndef cpps_logger_message_h__
#define cpps_logger_message_h__
#include <cpps/cpps.h>

namespace cpps
{
	struct cpps_logger_message
	{
		cpps_logger_message() {
			created = 0;
			level = 0;
			lineno = 0;
		}
		usint32		created;
		usint8		level;
		std::string message;
		std::string pathname;
		std::string filename;
		usint32		lineno;
		std::string funcname;
	};

}
#endif // cpps_logger_message_h__