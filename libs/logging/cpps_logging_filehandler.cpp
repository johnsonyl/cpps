#include "cpps_logging_filehandler.h"
#include "cpps_logger_config.h"

namespace cpps {
	cpps_integer cpps_io_mkdirs(std::string szdir);
	std::string cpps_io_getfilepath(std::string str);
	cpps_logging_filehandler::cpps_logging_filehandler()
		:cpps_logging_handler( cpps_logging_handler_type_filehandler)
	{
		file = NULL;
		delay = false;
		mode = "ab+";
	}

	cpps_logging_filehandler::~cpps_logging_filehandler()
	{
		if (file) fclose(file);
		file = NULL;
	}

	void cpps_logging_filehandler::pop(usint8 level, std::string& msg)
	{
		while (!file) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			openfile();/*尝试重新打开*/
		}

		fprintf(file,"%s\r\n", msg.c_str());
		fflush(file);
	}

	void cpps_logging_filehandler::setfile(std::string name, std::string filemode)
	{
		/*创建文件夹*/
		std::string path = cpps_io_getfilepath(name);
		cpps_io_mkdirs(path);

		filename = name;
		mode = filemode;
	}

	void cpps_logging_filehandler::setdelay(bool d)
	{
		delay = d;
	}

	void cpps_logging_filehandler::openfile()
	{
		if (filename.empty() || mode.empty()) return;

		file = fopen(filename.c_str(), mode.c_str());
	}

}