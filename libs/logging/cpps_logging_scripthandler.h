#pragma once
#include "cpps_logging_handler.h"
namespace cpps {
	class cpps_logging_scripthandler :
		public cpps_logging_handler
	{
	public:
		cpps_logging_scripthandler();
		cpps_logging_scripthandler(C *c);
		virtual ~cpps_logging_scripthandler();

		void			constructor(C* c, object _cls);
		void			create(std::string& _cls, cpps_map* config);
		virtual void	pop(usint8 level, std::string& msg);
		virtual	void	close();
		cpps_value		get_handler();

		C* _parent_c;
		C* _c;
		object _classvar;
		object _config;
	};
}


