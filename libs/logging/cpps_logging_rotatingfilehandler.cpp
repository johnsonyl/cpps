#include "cpps_logging_rotatingfilehandler.h"
namespace cpps {
	cpps_integer cpps_io_mkdirs(std::string szdir);
	std::string cpps_io_getfilepath(std::string str);
	bool cpps_io_file_exists(std::string path);
	cpps_logging_rotatingfilehandler::cpps_logging_rotatingfilehandler()
		:cpps_logging_handler( cpps_logging_handler_type_rotatingfilehandler)
	{
		file = NULL;
		delay = false;
		mode = "ab+";
		maxbytes = 0;
		backupCount = 0;
	}

	cpps_logging_rotatingfilehandler::~cpps_logging_rotatingfilehandler()
	{
		if (file) fclose(file);
		file = NULL;
	}

	void cpps_logging_rotatingfilehandler::openfile()
	{
		file = fopen(filename.c_str(), mode.c_str());
	}

	void cpps_logging_rotatingfilehandler::pop(usint8 level, std::string& msg)
	{
		while (!file) {
			openfile();/*尝试重新打开*/
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		fprintf(file,"%s\r\n", msg.c_str());
		fflush(file);

		checkchangefilename();

	}

	void cpps_logging_rotatingfilehandler::setfile(std::string name, std::string filemode)
	{
		/*创建文件夹*/
		std::string path = cpps_io_getfilepath(name);
		cpps_io_mkdirs(path);
		filename = name;
		mode = filemode;
	}

	void cpps_logging_rotatingfilehandler::setdelay(bool d)
	{
		delay = d;
	}

	void cpps_logging_rotatingfilehandler::setmaxbytes(cpps_integer bytes)
	{
		maxbytes = (size_t)bytes;
	}

	void cpps_logging_rotatingfilehandler::setbackupcount(cpps_integer c)
	{
		backupCount = c;
	}

	void cpps_logging_rotatingfilehandler::checkchangefilename()
	{
		if (maxbytes != 0) {

			size_t currfilesize = ftell(file);
			fseek(file, 0, SEEK_END);
			size_t filesize = ftell(file);
			fseek(file, (long)currfilesize, SEEK_SET);
			if (filesize > maxbytes)
			{
				if (file) fclose(file);

				/*需要重构文件.
				backupCount = 2
				log.txt
				log.txt.1
				log.txt.2
				*/

				int32 maxfileid = 1;
				while (true)
				{
					if (backupCount != 0 && maxfileid >= backupCount) /*不限制 则需要循环找到最大的文件*/
					{
						break;
					}

					char ids[64];
					sprintf(ids, "%d", maxfileid);

					std::string tmp = filename + "." + ids;
					if (!cpps_io_file_exists(tmp))
					{
						break;
					}
					maxfileid++;
				}

				for (int32 i = maxfileid; i > 0; i--)
				{  
					char ids2[64];
					sprintf(ids2, "%d", i - 1);

					char ids[64];
					sprintf(ids, "%d", i);

					std::string oldfilename = i == 1 ? filename : filename + "." + ids2;
					std::string newfilename = filename + "." + ids;

					if(backupCount != 0 && i == maxfileid) remove(newfilename.c_str());
					rename(oldfilename.c_str(), newfilename.c_str());
				}
				/*再次打开*/
				openfile();
			}
		}
	}

}