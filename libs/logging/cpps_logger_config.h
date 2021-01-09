#ifndef cpps_logger_config_h__
#define cpps_logger_config_h__
#include <cpps/cpps.h>
#include "cpps_logging_handler.h"
namespace cpps
{
	enum cpps_logger_config_encoding
	{
		CPPS_LOGGER_ENCODING_GBK = 1,
		CPPS_LOGGER_ENCODING_UTF8,
		CPPS_LOGGER_ENCODING_UNICODE,
	};

	class cpps_logger_config
	{
	public:
		cpps_logger_config() {
			version = 0;
		}
		cpps_integer version;
		std::vector< cpps_logging_handler*> handlers;
	};

}
#endif // cpps_logger_config_h__