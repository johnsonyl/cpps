#ifndef cpps_logging_handler_h__
#define cpps_logging_handler_h__
#include <cpps/cpps.h>
#include <thread>
#include <vector>
#include <chrono>
#include "cpps_logger_message.h"
namespace cpps
{
	class cpps_logger;
	struct cpps_formatter_node
	{
		cpps_formatter_node() {
			type = 0;
		}
		usint8 type;
		std::string s;
	};
	enum cpps_logging_handler_type
	{
		cpps_logging_handler_type_streamhandler,
		cpps_logging_handler_type_filehandler,
		cpps_logging_handler_type_rotatingfilehandler,
		cpps_logging_handler_type_timerotatingfilehandler,
		cpps_logging_handler_type_nullhandler,
	};
	class cpps_logging_handler
	{
	public:
		cpps_logging_handler():cpps_logging_handler( cpps_logging_handler_type_nullhandler){}
		cpps_logging_handler( cpps_logging_handler_type type);
		virtual ~cpps_logging_handler();
	public:
		void										push(cpps_logger_message& msg);
		void										work();
		usint8										make_format_id(std::string kn);
		std::string									make_format(usint8 k, cpps_logger_message* msg);
		virtual void								pop(usint8 level,std::string& msg);
		virtual void								close();
		bool										runing();
		static void									run(cpps_logging_handler* logger);
		void										setformatter(std::string format);
		bool										canpush(bool propagate,usint8 level);
		void										setlevel(usint8 lev);
		void										setlogger(cpps_logger* p);
	public:
		usint8										level;
		std::vector<cpps_formatter_node>			formatter;
		usint8										encoding;
	private:
		std::vector<cpps_logger_message*>			msgqueue;
		std::thread* workthread;
		cpps_lock									msglock;
		bool										runstate;
		bool										nomsg;
		cpps_logger*								logger;
		cpps_logging_handler_type					handler_type;
	public:
	};
}


#endif // cpps_logging_handler_h__
