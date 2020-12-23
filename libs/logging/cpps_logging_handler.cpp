#include "cpps_logging_handler.h"
#include "cpps_logger.h"
#include <sstream>


cpps::usint8	cpps_slevel_to_nlevel(std::string level);
std::string		cpps_nlevel_to_slevel(cpps::usint8 level);
namespace cpps
{
	std::string cpps_time_time2str(cpps_integer nt);
	std::string cpps_io_getfilename(std::string str);
	cpps_logging_handler::cpps_logging_handler( cpps_logging_handler_type type)
	{
		handler_type = type;
		logger = NULL;
		level = 0;
		encoding = 0;
		runstate = true;
		workthread = NULL;
		nomsg = true;
	}

	cpps_logging_handler::~cpps_logging_handler()
	{
		runstate = false;
		if (workthread) {
			workthread->join();//等待结束
			delete workthread;
			workthread = NULL;
		}
		for (auto msg : msgqueue)
		{
			delete msg;
		}
		msgqueue.clear();
	}

	void cpps_logging_handler::push(cpps_logger_message& msg)
	{
		cpps_logger_message* copymsg = new cpps_logger_message();
		*copymsg = msg; //copy

		msglock.lock();
		msgqueue.push_back(copymsg);
		nomsg = false;
		msglock.unlock();
	}

	void cpps_logging_handler::work()
	{
		msglock.lock();
		if (nomsg) {
			msglock.unlock();
			/*释放cpu.*/
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			return;
		}
		std::vector<cpps_logger_message*> tmplist;
		tmplist = msgqueue;
		msgqueue.clear();
		nomsg = true;
		msglock.unlock();

		for (auto msg : tmplist)
		{
			std::string msgstr;
			for (auto n : formatter)
			{
				if (n.type == 0)
					msgstr += n.s;
				else
					msgstr += make_format(n.type, msg);
			}
			pop(msg->level,msgstr);
			delete msg;
		}
	}
	usint8		cpps_logging_handler::make_format_id(std::string kn)
	{
		usint8 ret = 11;
		if (kn == "asctime")
			ret = 1;
		else if (kn == "created")
			ret = 2;
		else if (kn == "levelname")
			ret = 3;
		else if (kn == "levelno")
			ret = 4;
		else if (kn == "name")
			ret = 5;
		else if (kn == "message")
			ret = 6;
		else if (kn == "pathname")
			ret = 7;
		else if (kn == "filename")
			ret = 8;
		else if (kn == "lineno")
			ret = 9;
		else if (kn == "funcName")
			ret = 10;
		return ret;
	}
	std::string		cpps_logging_handler::make_format(usint8 k, cpps_logger_message* msg)
	{
		std::string ret;
		switch (k)
		{
			case 1:/*asctime*/
			{
				ret = cpps_time_time2str(msg->created);
				break;
			}
			case 2:/*created*/
			{
				std::stringstream s;
				s << msg->created;
				ret = s.str();
				break;
			}
			case 3:/*levelname*/
			{
				ret = cpps_nlevel_to_slevel(msg->level);
				break;
			}
			case 4:/*levelno*/
			{
				std::stringstream s;
				s << msg->level;
				ret = s.str();
				break;
			}
			case 5:/*name*/
			{
				ret = logger->logger_name;
				break;
			}
			case 6:/*message*/
			{
				ret = msg->message;
				break;
			}
			case 7:/*pathname*/
			{
				ret = msg->pathname;
				break;
			}
			case 8:/*filename*/
			{
				ret = cpps_io_getfilename(msg->filename);
				break;
			}
			case 9:/*lineno*/
			{
				std::stringstream s;
				s << msg->lineno;
				ret = s.str();
				break;
			}
			case 10:/*funcName*/
			{
				std::stringstream s;
				s << msg->funcname;
				ret = s.str();
				break;
			}
		}
		return ret;
	}
	void cpps_logging_handler::pop(usint8 level, std::string& msg)
	{
	}

	bool cpps_logging_handler::runing()
	{
		return runstate;
	}

	void cpps_logging_handler::run(cpps_logging_handler* handler)
	{
		while (handler->runing())
			handler->work();
	}

	void cpps_logging_handler::setformatter(std::string format)
	{
		//分析format.
		size_t off = 0;
		while (off < format.size())
		{
			size_t pos = format.find("%(", off);
			if (pos != std::string::npos)
			{
				std::string zeronode = format.substr(off, pos - off);
				
				if (!zeronode.empty())
				{
					cpps_formatter_node node;
					node.s = zeronode;
					node.type = 0;
					formatter.push_back(node);
				}

				pos += 2;
				size_t pos2 = format.find(")", pos);
				if (pos2 != std::string::npos)
				{
					std::string kn = format.substr(pos, pos2 - pos);
					//std::cout << kn << std::endl;
					cpps_formatter_node node;
					node.type = make_format_id(kn);
					formatter.push_back(node);
					off = pos2 + 2;
				}
				else
				{
					cpps_formatter_node node;
					node.s = format.substr(pos);
					node.type = 0;
					formatter.push_back(node);
					break;
				}
			}
			else
			{
				cpps_formatter_node node;
				node.s = format.substr(off);
				node.type = 0;
				formatter.push_back(node);
				break;
			}
		}
	}

	bool cpps_logging_handler::canpush(bool propagate, usint8 lev)
	{
		if (lev < level) {
			return false;
		}
		if (!propagate && lev == level) {
			return false;
		}
		return true;
	}

	void cpps_logging_handler::setlevel(usint8 lev)
	{
		level = lev;
	}

	void cpps_logging_handler::setlogger(cpps_logger* p)
	{
		logger = p;
		workthread = new std::thread(cpps_logging_handler::run, this);//启动线程
	}

}