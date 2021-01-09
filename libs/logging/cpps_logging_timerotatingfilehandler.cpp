#include "cpps_logging_timerotatingfilehandler.h"
namespace cpps {
	bool cpps_io_file_exists(std::string path);
	cpps_integer cpps_io_mkdirs(std::string szdir);
	std::string cpps_io_getfilepath(std::string str);
	cpps_integer	cpps_time_gettime();
	cpps_integer cpps_time_getwday(cpps_integer t1);
	bool		cpps_time_issomeday(cpps_integer t1, cpps_integer t2);
	cpps_logging_timerotatingfilehandler::cpps_logging_timerotatingfilehandler()
		:cpps_logging_handler( cpps_logging_handler_type_timerotatingfilehandler)
	{
		file = NULL;
		delay = false;
		mode = "ab+";
		whent = 0;
		backupCount = 0;
		interval = 0;
		lasttime = cpps_time_gettime();
	}

	cpps_logging_timerotatingfilehandler::~cpps_logging_timerotatingfilehandler()
	{
		if (file) fclose(file);
		file = NULL;
	}

	void cpps_logging_timerotatingfilehandler::openfile()
	{
		file = fopen(filename.c_str(), mode.c_str());
	}

	void cpps_logging_timerotatingfilehandler::pop(usint8 level, std::string& msg)
	{
		while (!file) {
			openfile();/*尝试重新打开*/
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		fprintf(file,"%s\r\n", msg.c_str());
		fflush(file);

		checkchangefilename();

	}

	void cpps_logging_timerotatingfilehandler::setfile(std::string name, std::string filemode)
	{
		/*创建文件夹*/
		std::string path = cpps_io_getfilepath(name);
		cpps_io_mkdirs(path);
		filename = name;
		mode = filemode;
	}

	void cpps_logging_timerotatingfilehandler::setdelay(bool d)
	{
		delay = d;
	}

	void cpps_logging_timerotatingfilehandler::setwhen(std::string w, cpps_integer iv)
	{
		when = w;
		interval = (int32)iv;
	}

	void cpps_logging_timerotatingfilehandler::setbackupcount(cpps_integer c)
	{
		backupCount = c;
	}

	void cpps_logging_timerotatingfilehandler::checkchangefilename()
	{
		bool needchange = isneedchange();

		if (needchange)
		{
			lasttime = cpps_time_gettime();

			if (file) fclose(file);


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

	cpps::int8 cpps_logging_timerotatingfilehandler::getwhent(std::string whens)
	{
		int8 ret = 0;
		if (whens == "S")
		{
			ret = 1;
		}
		else if (whens == "M")
		{
			ret = 2;
		}
		else if (whens == "H")
		{
			ret = 3;
		}
		else if (whens == "D")
		{
			ret = 4;
		}
		else if (whens == "W0")
		{
			ret = 5;
		}
		else if (whens == "W1")
		{
			ret = 6;
		}
		else if (whens == "W2")
		{
			ret = 7;
		}
		else if (whens == "W3")
		{
			ret = 8;
		}
		else if (whens == "W4")
		{
			ret = 9;
		}
		else if (whens == "W5")
		{
			ret = 10;
		}
		else if (whens == "W6")
		{
			ret = 11;
		}
		else if (whens == "midnight")
		{
			ret = 12;
		}
		return ret;
	}

	bool cpps_logging_timerotatingfilehandler::isneedchange()
	{
		if (interval <= 0) return false;
		cpps_integer curtime = cpps_time_gettime();
		switch (whent)
		{
		case 1: {
			return curtime - lasttime > interval;
			break;
		}
		case 2: {
			return ((curtime - lasttime) / 60) > interval;
			break;
		}
		case 3: {
			return ((curtime - lasttime) / 60 / 60) > interval;
			break;
		}
		case 4: {
			return ((curtime - lasttime) / 60 / 60 / 24) > interval;
			break;
		}
		case 5: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 0;
			break;
		}
		case 6: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 1;
			break;
		}
		case 7: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 2;
			break;
		}
		case 8: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 3;
			break;
		}
		case 9: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 4;
			break;
		}
		case 10: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 5;
			break;
		}
		case 11: {
			cpps_integer wday = cpps_time_getwday(curtime);
			return wday == 6;
			break;
		}
		case 12: {
			return !cpps_time_issomeday(curtime, lasttime);
			break;
		}
		default:
			break;
		}
		return false;
	}

}