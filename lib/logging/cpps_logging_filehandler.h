#ifndef cpps_logging_filehandler_h__
#define cpps_logging_filehandler_h__


#include "cpps_logging_handler.h"
namespace cpps {
	class cpps_logging_filehandler :
		public cpps_logging_handler
	{
	public:
		cpps_logging_filehandler();
		virtual ~cpps_logging_filehandler();

		void			openfile();
		virtual void	pop(usint8 level, std::string& msg);
		void			setfile(std::string name, std::string filemode);
		void			setdelay(bool d);

	public:
		std::string filename;
		bool delay;
		FILE* file;
		std::string mode;
	};

}
#endif // cpps_logging_filehandler_h__