#ifndef cpps_logging_timerotatingfilehandler_h__
#define cpps_logging_timerotatingfilehandler_h__


#include "cpps_logging_handler.h"
namespace cpps {
	class cpps_logging_timerotatingfilehandler :
		public cpps_logging_handler
	{
	public:
		cpps_logging_timerotatingfilehandler();
		virtual ~cpps_logging_timerotatingfilehandler();

		void			openfile();
		virtual void	pop(usint8 level, std::string& msg);

		void			setfile(std::string name, std::string filemode);
		void			setdelay(bool d);
		void			setwhen(std::string w,cpps_integer iv);
		void			setbackupcount(cpps_integer c);
		void			checkchangefilename();
		bool			isneedchange();

		int8	getwhent(std::string whens);
	public:
		std::string filename;
		bool delay;
		FILE* file;
		std::string mode;
		cpps_integer backupCount;
		std::string when;
		int8	whent;
		int32	interval;
		cpps_integer lasttime;
	};
}

#endif // cpps_logging_timerotatingfilehandler_h__
