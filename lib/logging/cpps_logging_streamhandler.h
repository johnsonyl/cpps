#ifndef cpps_logging_streamhandler_h__
#define cpps_logging_streamhandler_h__

#include "cpps_logging_handler.h"
namespace cpps {
	class cpps_logger;
	class cpps_logging_streamhandler :
		public cpps_logging_handler
	{
	public:
		cpps_logging_streamhandler();
		virtual ~cpps_logging_streamhandler();

		virtual void	pop(usint8 level, std::string& msg);
		void			setcolor(cpps_value v);
	public:
		int8		color[5];
	};
}


#endif // cpps_logging_streamhandler_h__
