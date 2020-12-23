#include "cpps_logger.h"

namespace cpps
{
	cpps_integer	cpps_time_gettime();
	cpps_logger::cpps_logger()
	{
		level = 0;
		propagate = true;
	}

	cpps_logger::~cpps_logger()
	{
		for (auto handler : config.handlers)
		{
			delete handler;
		}
		config.handlers.clear();
	}

	bool cpps_logger::canpush(usint8 lev)
	{
		if (lev < level) {
			return false;
		}
		/*向上传递*/
		if (!propagate && lev != level) {
			return false;
		}
		return true;
	}

	void cpps_logger::push(cpps_logger_message& msg)
	{
		/*日志等级*/
		if (msg.level < level) {
			return;
		}
		/*向上传递*/
		if (!propagate && msg.level != level){
			return;
		}
		/*push给所有handler*/
		for (auto handler : config.handlers){
			if(handler->canpush(propagate,msg.level))
				handler->push(msg);
		}
	}

	void cpps_logger::debug(C* c, std::string msg)
	{
		if (!canpush(10)) return;

		cpps_logger_message message;
		message.created = (usint32)cpps_time_gettime();
		message.filename = c->curnode ? c->curnode->filename : "";
		message.funcname = c->_callstack->size() >= 2 ? (*(c->_callstack))[c->_callstack->size() - 2]->func : "";
		message.level = 10;
		message.lineno = c->curnode ? c->curnode->line : 0;
		message.message = msg;
		message.pathname = c->curnode ? c->curnode->filename : "";

		push(message);
	}

	void cpps_logger::info(C* c, std::string msg)
	{
		if (!canpush(20)) return;

		cpps_logger_message message;
		message.created = (usint32)cpps_time_gettime();
		message.filename = c->curnode ? c->curnode->filename : "";
		message.funcname = c->_callstack->size() >= 2 ? (*(c->_callstack))[c->_callstack->size() - 2]->func : "";
		message.level = 20;
		message.lineno = c->curnode ? c->curnode->line : 0;
		message.message = msg;
		message.pathname = c->curnode ? c->curnode->filename : "";

		push(message);
	}

	void cpps_logger::warning(C* c, std::string msg)
	{
		if (!canpush(30)) return;

		cpps_logger_message message;
		message.created = (usint32)cpps_time_gettime();
		message.filename = c->curnode ? c->curnode->filename : "";
		message.funcname = c->_callstack->size() >= 2 ? (*(c->_callstack))[c->_callstack->size() - 2]->func : "";
		message.level = 30;
		message.lineno = c->curnode ? c->curnode->line : 0;
		message.message = msg;
		message.pathname = c->curnode ? c->curnode->filename : "";

		push(message);
	}

	void cpps_logger::error(C* c, std::string msg)
	{
		if (!canpush(40)) return;

		cpps_logger_message message;
		message.created = (usint32)cpps_time_gettime();
		message.filename = c->curnode ? c->curnode->filename : "";
		message.funcname = c->_callstack->size() >= 2 ? (*(c->_callstack))[c->_callstack->size() - 2]->func : "";
		message.level = 40;
		message.lineno = c->curnode ? c->curnode->line : 0;
		message.message = msg;
		message.pathname = c->curnode ? c->curnode->filename : "";

		push(message);
	}

	void cpps_logger::critical(C* c, std::string msg)
	{
		if (!canpush(50)) return;

		cpps_logger_message message;
		message.created = (usint32)cpps_time_gettime();
		message.filename = c->curnode ? c->curnode->filename : "";
		message.funcname = c->_callstack->size() >= 2 ? (*(c->_callstack))[c->_callstack->size() - 2]->func : "";
		message.level = 50;
		message.lineno = c->curnode ? c->curnode->line : 0;
		message.message = msg;
		message.pathname = c->curnode ? c->curnode->filename : "";

		push(message);
	}

	void cpps_logger::addhandler(cpps_logging_handler* handler)
	{
		handler->setlogger(this);
		config.handlers.push_back(handler);
	}

	void cpps_logger::removehandler(cpps_logging_handler* handler)
	{
		//查找handler
		std::vector< cpps_logging_handler*>::iterator it = config.handlers.begin();
		std::vector< cpps_logging_handler*>::iterator end = config.handlers.end();
		for (; it != end; ++it) {
			cpps_logging_handler* take = *it;
			if (take == handler)
			{
				delete take;
				config.handlers.erase(it);
				return;
			}
		}
	}

}