#ifndef cpps_logger_h__
#define cpps_logger_h__
#include <cpps/cpps.h>
#include "cpps_logger_message.h"
#include "cpps_logger_config.h"
namespace cpps
{
	class cpps_logger
	{
	public:
		cpps_logger();
		virtual ~cpps_logger();

	public:
		bool			canpush(usint8 lev);
		void			push(cpps_logger_message& msg);
		void			debug(C* c, std::string msg);
		void			info(C* c, std::string msg);
		void			warning(C* c, std::string msg);
		void			error(C* c, std::string msg);
		void			critical(C* c, std::string msg);
	public:
		void			addhandler(cpps_logging_handler* handler);
		void			removehandler(cpps_logging_handler* handler);
	public:
		cpps_logger_config	config;
		std::string			logger_name;
		usint8				level;
		bool				propagate;
	};
}

#endif // cpps_logger_h__
