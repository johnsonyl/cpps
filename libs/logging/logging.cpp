// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

#include "cpps_logger.h"
#include "cpps_logger_config.h"
#include "cpps_logger_message.h"
#include "cpps_logging_handler.h"
#include "cpps_logging_streamhandler.h"
#include "cpps_logging_filehandler.h"
#include "cpps_logging_rotatingfilehandler.h"
#include "cpps_logging_timerotatingfilehandler.h"

using namespace cpps;
using namespace std;

struct cpps_logging_data :public cpps_module_data
{
	phmap::flat_hash_map<std::string, cpps_logger*> loggerslist;
	cpps_logger* defaultlogger = NULL;
};

#define cpps_map_get_value(m,k) m->find(cpps_value(c,k))
namespace cpps { cpps_integer	cpps_time_gettime(); }
usint8	cpps_slevel_to_nlevel(std::string level)
{
	usint8 ret = 0;
	if (level == "DEBUG"){
		ret = 10;
	}
	else if (level == "INFO"){
		ret = 20;
	}
	else if (level == "WARNING"){
		ret = 30;
	}
	else if (level == "ERROR"){
		ret = 40;
	}
	else if (level == "CRITICAL"){
		ret = 50;
	}
	return ret;
}
std::string cpps_nlevel_to_slevel(usint8 level)
{
	std::string ret = "";
	level = level / 10 * 10; /*取整十*/
	if (level == 10){
		ret = "DEBUG";
	}
	else if (level == 20){
		ret = "INFO";
	}
	else if (level == 30){
		ret = "WARNING";
	}
	else if (level == 40){
		ret = "ERROR";
	}
	else if (level == 50){
		ret = "CRITICAL";
	}
	return ret;
}

cpps_logging_handler* cpps_create_logging_handler(C* c, std::string cls, cpps_map* formatters,cpps_map *config)
{
	if (cls == "logging.StreamHandler")
	{
		cpps_logging_streamhandler* handler = new cpps_logging_streamhandler();
		std::string level = cpps_to_string(cpps_map_get_value(config, "level"));
		cpps_value formatter_key = cpps_map_get_value(config, "formatter");
		cpps_map* formatters_config = cpps_to_cpps_map( formatters->find(formatter_key));
		if (formatters_config)
		{
			handler->setformatter( cpps_to_string(cpps_map_get_value(formatters_config,"format")));
		}
		handler->level = cpps_slevel_to_nlevel(level);
		cpps_vector* color = cpps_to_cpps_vector(cpps_map_get_value(config, "color"));
		if (color)
		{
			size_t c = color->size() < 5 ? color->size() : 5;
			for (size_t i = 0; i < c; i++) {
				handler->color[i] = (int8)cpps_to_integer(color->at(i));
			}
		}

		return handler;
	}
	else if (cls == "logging.handlers.RotatingFileHandler")
	{
		cpps_logging_rotatingfilehandler* handler = new cpps_logging_rotatingfilehandler();
		std::string level = cpps_to_string(cpps_map_get_value(config, "level"));
		cpps_value formatter_key = cpps_map_get_value(config, "formatter");
		cpps_map* formatters_config = cpps_to_cpps_map(formatters->find(formatter_key));
		if (formatters_config)
		{
			handler->setformatter(cpps_to_string(cpps_map_get_value(formatters_config, "format")));
		}
		handler->level = cpps_slevel_to_nlevel(level);
		std::string filename = cpps_to_string(cpps_map_get_value(config, "filename"));
		std::string mode = cpps_to_string(cpps_map_get_value(config, "mode"));
		cpps_value delay_val = cpps_map_get_value(config, "delay");
		bool delay = delay_val.tt == CPPS_TBOOLEAN ? delay_val.value.b : false;
		cpps_integer maxbytes = cpps_to_integer(cpps_map_get_value(config, "maxBytes"));
		cpps_integer backupCount = cpps_to_integer(cpps_map_get_value(config, "backupCount"));
		handler->delay = delay;
		handler->setfile(filename, mode.empty() ? "ab+" : mode);
		handler->maxbytes = (size_t)maxbytes;
		handler->backupCount = backupCount;
		handler->openfile();
		

		return handler;
	}
	else if (cls == "logging.FileHandler")
	{
		cpps_logging_filehandler* handler = new cpps_logging_filehandler();
		std::string level = cpps_to_string(cpps_map_get_value(config, "level"));
		cpps_value formatter_key = cpps_map_get_value(config, "formatter");
		cpps_map* formatters_config = cpps_to_cpps_map(formatters->find(formatter_key));
		if (formatters_config)
		{
			handler->setformatter(cpps_to_string(cpps_map_get_value(formatters_config, "format")));
		}
		handler->level = cpps_slevel_to_nlevel(level);
		std::string filename = cpps_to_string(cpps_map_get_value(config, "filename"));
		std::string mode = cpps_to_string(cpps_map_get_value(config, "mode"));
		cpps_value delay_val = cpps_map_get_value(config, "delay");
		bool delay = delay_val.tt == CPPS_TBOOLEAN ? delay_val.value.b : false;
		handler->delay = delay;
		handler->setfile(filename, mode.empty() ? "ab+" : mode);
		handler->openfile();

		return handler;
	}
	else if (cls == "logging.handlers.TimedRotatingFileHandler")
	{
		cpps_logging_timerotatingfilehandler* handler = new cpps_logging_timerotatingfilehandler();
		std::string level = cpps_to_string(cpps_map_get_value(config, "level"));
		cpps_value formatter_key = cpps_map_get_value(config, "formatter");
		cpps_map* formatters_config = cpps_to_cpps_map(formatters->find(formatter_key));
		if (formatters_config)
		{
			handler->setformatter(cpps_to_string(cpps_map_get_value(formatters_config, "format")));
		}
		handler->level = cpps_slevel_to_nlevel(level);
		std::string filename = cpps_to_string(cpps_map_get_value(config, "filename"));
		std::string when = cpps_to_string(cpps_map_get_value(config, "when"));
		std::string mode = cpps_to_string(cpps_map_get_value(config, "mode"));
		cpps_value delay_val = cpps_map_get_value(config, "delay");
		bool delay = delay_val.tt == CPPS_TBOOLEAN ? delay_val.value.b : false;
		cpps_integer backupCount = cpps_to_integer(cpps_map_get_value(config, "backupCount"));
		cpps_integer interval = cpps_to_integer(cpps_map_get_value(config, "interval"));
		handler->delay = delay;
		handler->setfile(filename, mode.empty() ? "ab+" : mode);
		handler->backupCount = backupCount;
		handler->interval = (int32)interval;
		handler->when = when;
		handler->whent = handler->getwhent(when);
		handler->openfile();


		return handler;
	}
	else if (cls == "logging.handlers.HTTPHandler")
	{

	}
	else if (cls == "logging.handlers.SMTPHandler")
	{

	}
	return NULL;
}

bool cpps_create_logger(C* c, cpps::object config)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	phmap::flat_hash_map<std::string, cpps_logger*>& loggerslist = data->loggerslist;
	cpps_logger*& defaultlogger = data->defaultlogger;

	for (auto n : loggerslist) {
		cpps_logger* logger = n.second;
		delete logger;
	}
	loggerslist.clear();
	defaultlogger = NULL;

	cpps_map* m = cpps_to_cpps_map(config.value);
	cpps_integer level = cpps_to_integer( cpps_map_get_value(m,"level") );
	std::string format = cpps_to_string( cpps_map_get_value(m, "format") );
	std::string filename = cpps_to_string(cpps_map_get_value(m, "filename") );
	std::string filemode = cpps_to_string(cpps_map_get_value(m, "filemode"));
	cpps_vector* loggers = cpps_to_cpps_vector(cpps_map_get_value(m, "loggers"));
	if (loggers == NULL) {
		cpps_logger* logger = new cpps_logger();
		logger->level = (usint8)level;
		logger->logger_name = "root";
		logger->propagate = true;

		cpps_logging_filehandler* handler = new cpps_logging_filehandler();
		handler->delay = false;
		handler->setformatter(format);
		handler->level = (usint8)level;
		handler->setfile(filename, filemode.empty() ? "ab+" : filemode);
		handler->openfile();

		logger->addhandler(handler);

		defaultlogger = logger;
		loggerslist.insert(phmap::flat_hash_map<std::string, cpps_logger*>::value_type("root", logger));
	}
	else /*说明有自己的列表*/
	{
		for (auto n : loggers->realvector())
		{
			cpps_logger* logger = new cpps_logger();
			logger->level = (usint8)level;
			logger->logger_name = cpps_to_string(n);
			logger->propagate = true;

			cpps_logging_filehandler* handler = new cpps_logging_filehandler();
			handler->delay = false;
			handler->setformatter(format);
			handler->level = (usint8)level;
			handler->setfile(filename, filemode.empty() ? "ab+" : filemode);
			handler->openfile();

			logger->addhandler(handler);

			defaultlogger = logger;
			loggerslist.insert(phmap::flat_hash_map<std::string, cpps_logger*>::value_type(cpps_to_string(n), logger));
		}
	}
	return true;
}
bool cpps_create_logger_with_file(C* c, cpps::object filepath)
{
	std::string s = cpps_to_string(filepath.value);
	return true;
}
bool cpps_create_logger_with_config(C*c,cpps::object config)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	phmap::flat_hash_map<std::string, cpps_logger*>& loggerslist = data->loggerslist;
	cpps_logger*& defaultlogger = data->defaultlogger;

	cpps_map* m = cpps_to_cpps_map(config.value);
	cpps_integer version = cpps_to_integer(cpps_map_get_value(m,"version"));
	cpps_value disable_existing_loggers_val = cpps_map_get_value(m, "disable_existing_loggers");
	bool disable_existing_loggers = disable_existing_loggers_val.tt == CPPS_TBOOLEAN ? disable_existing_loggers_val.value.b : false;
	/*需要清理老的日志*/
	if (disable_existing_loggers){
		for (auto n : loggerslist){
			cpps_logger* logger = n.second;
			delete logger;
		}
		loggerslist.clear();
		defaultlogger = NULL;
	}
	cpps_map* formatters = cpps_to_cpps_map(cpps_map_get_value(m, "formatters"));
	cpps_map* filters = cpps_to_cpps_map(cpps_map_get_value(m, "filters"));
	cpps_map* handlers = cpps_to_cpps_map(cpps_map_get_value(m, "handlers") );
	cpps_map* loggers = cpps_to_cpps_map(cpps_map_get_value(m, "loggers"));
	if (!loggers)
		throw(cpps_error("module logging error", 0, 0, "loggers is not defined in config."));
	if (!handlers)
		throw(cpps_error("module logging error", 0, 0, "handlers is not defined in config."));
	if (!formatters)
		throw(cpps_error("module logging error", 0, 0, "formatters is not defined in config."));
	if (!filters)
		throw(cpps_error("module logging error", 0, 0, "filters is not defined in config."));


	for (auto n : loggers->realmap())
	{
		std::string logger_name = cpps_to_string(n.first);
		cpps_map* config = cpps_to_cpps_map(n.second);
		if (config)
		{
			cpps_logger* logger = new cpps_logger();
			std::string level = cpps_to_string(cpps_map_get_value(config, "level"));
			cpps_value propagate_val = cpps_map_get_value(config, "propagate");
			bool propagate = propagate_val.tt == CPPS_TBOOLEAN ? propagate_val.value.b : true;
			cpps_vector* loggers_handlers = cpps_to_cpps_vector(cpps_map_get_value(config, "handlers"));
			if (loggers_handlers)
			{
				for (auto& n2 : loggers_handlers->realvector())
				{
					std::string n2s = cpps_to_string(n2);
					cpps_map* handler_config = cpps_to_cpps_map(handlers->find(n2));
					if (handler_config)
					{
						std::string cls = cpps_to_string(cpps_map_get_value(handler_config, "class"));
						cpps_logging_handler* handler = cpps_create_logging_handler(c, cls, formatters, handler_config);
						if(!handler) throw(cpps_error("module logging error", 0, 0, "handler config [%s] not found ..", cls.c_str()));
						logger->addhandler(handler);
					}
					else
						throw(cpps_error("module logging error", 0, 0, "handler class [%s] not found ..", n2s.c_str()));
				}
			}
			logger->level = cpps_slevel_to_nlevel(level);
			logger->propagate = propagate;
			logger->config.version = version;
			logger->logger_name = logger_name;
			//filters还不知道咋用
			if (logger_name == "root") defaultlogger = logger;
			loggerslist.insert(phmap::flat_hash_map<std::string, cpps_logger*>::value_type(logger_name, logger));
		}
	}
	return true;
}
void cpps_logging_debug(C* c, std::string msg)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (defaultlogger)
	{
		defaultlogger->debug(c, msg);
	}
}
void cpps_logging_info(C* c, std::string msg)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (defaultlogger)
	{
		defaultlogger->info(c, msg);
	}
}
void cpps_logging_warning(C* c, std::string msg)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (defaultlogger)
	{
		defaultlogger->warning(c, msg);
	}
}
void cpps_logging_error(C* c, std::string msg)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (defaultlogger)
	{
		defaultlogger->error(c, msg);
	}
}
void cpps_logging_critical(C* c, std::string msg)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (defaultlogger)
	{
		defaultlogger->critical(c, msg);
	}
}
cpps_logger* cpps_getlogger(C* c,std::string name)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	phmap::flat_hash_map<std::string, cpps_logger*>& loggerslist = data->loggerslist;
	cpps_logger*& defaultlogger = data->defaultlogger;

	if (name == "root" || name == "") return defaultlogger;
	phmap::flat_hash_map<std::string, cpps_logger*>::iterator it = loggerslist.find(name);
	if (it != loggerslist.end()) return it->second;
	return NULL;
}
cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);
	//这样可以保证每个c的moduledata 有区分.不然多脚本的时候会有问题
	cpps_logging_data* data = new cpps_logging_data();
	c->setmoduledata("logging", data);

	cpps::_module(c, "logging")[
		_class< cpps_logger >("Logger")
			.def("addhandler",&cpps_logger::addhandler)
			.def("removehandler",&cpps_logger::removehandler)
			.def_inside("debug",&cpps_logger::debug)
			.def_inside("info",&cpps_logger::info)
			.def_inside("warning",&cpps_logger::warning)
			.def_inside("error",&cpps_logger::error)
			.def_inside("critical",&cpps_logger::critical),
		_class< cpps_logging_handler >("Handler"),
		_class< cpps_logging_streamhandler >("StreamHandler")
			.def("setformatter", &cpps_logging_streamhandler::setformatter)
			.def("setcolor", &cpps_logging_streamhandler::setcolor)
			.def("setlevel", &cpps_logging_streamhandler::setlevel),
		_class< cpps_logging_filehandler >("FileHandler")
			.def("setformatter", &cpps_logging_filehandler::setformatter)
			.def("setfile", &cpps_logging_filehandler::setfile)
			.def("setdelay", &cpps_logging_filehandler::setdelay)
			.def("setlevel", &cpps_logging_filehandler::setlevel),
		_class< cpps_logging_rotatingfilehandler >("RotatingFileHandler")
			.def("setformatter", &cpps_logging_rotatingfilehandler::setformatter)
			.def("setfile", &cpps_logging_rotatingfilehandler::setfile)
			.def("setdelay", &cpps_logging_rotatingfilehandler::setdelay)
			.def("setmaxbytes", &cpps_logging_rotatingfilehandler::setmaxbytes)
			.def("setbackupcount", &cpps_logging_rotatingfilehandler::setbackupcount)
			.def("setlevel", &cpps_logging_rotatingfilehandler::setlevel),
		_class< cpps_logging_timerotatingfilehandler >("TimeRotatingFileHandler")
			.def("setformatter", &cpps_logging_timerotatingfilehandler::setformatter)
			.def("setfile", &cpps_logging_timerotatingfilehandler::setfile)
			.def("setdelay", &cpps_logging_timerotatingfilehandler::setdelay)
			.def("setwhen", &cpps_logging_timerotatingfilehandler::setwhen)
			.def("setbackupcount", &cpps_logging_timerotatingfilehandler::setbackupcount)
			.def("setlevel", &cpps_logging_timerotatingfilehandler::setlevel),
		def_inside("create_with_config",cpps_create_logger_with_config),
		def_inside("create", cpps_create_logger_with_config),
		def_inside("debug",cpps_logging_debug),
		def_inside("info",cpps_logging_info),
		def_inside("warning",cpps_logging_warning),
		def_inside("error",cpps_logging_error),
		def_inside("critical",cpps_logging_critical),
		def_inside("getlogger",cpps_getlogger),
		defvar(c,"GBK",(cpps_integer)cpps_logger_config_encoding::CPPS_LOGGER_ENCODING_GBK),
		defvar(c,"UTF8",(cpps_integer)cpps_logger_config_encoding::CPPS_LOGGER_ENCODING_UTF8),
		defvar(c,"UNICODE",(cpps_integer)cpps_logger_config_encoding::CPPS_LOGGER_ENCODING_UNICODE),
		defvar(c,"DEBUG",10),
		defvar(c,"INFO",20),
		defvar(c,"WARNING",30),
		defvar(c,"ERROR",40),
		defvar(c,"CRITICAL",50)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps_logging_data* data = (cpps_logging_data*)c->getmoduledata("logging");
	delete data;
	c->setmoduledata("logging", NULL);
}

cpps_export_finish