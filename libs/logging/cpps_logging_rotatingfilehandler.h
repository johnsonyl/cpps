#ifndef cpps_logging_rotatingfilehandler_h__
#define cpps_logging_rotatingfilehandler_h__


#include "cpps_logging_handler.h"
namespace cpps {
	class cpps_logging_rotatingfilehandler :
		public cpps_logging_handler
	{
	public:
		cpps_logging_rotatingfilehandler();
		virtual ~cpps_logging_rotatingfilehandler();

		void			openfile();
		virtual void	pop(usint8 level, std::string& msg);
		void			setfile(std::string name, std::string filemode);
		void			setdelay(bool d);
		void			setmaxbytes(cpps_integer bytes);
		void			setbackupcount(cpps_integer c);

		void			checkchangefilename();

	public:
		std::string filename;
		bool delay;
		FILE* file;
		std::string mode;
		size_t maxbytes;
		cpps_integer backupCount;
	};
}


#endif // cpps_logging_rotatingfilehandler_h__
