#pragma once

namespace cpps
{
	template<class T>
	bool	cpps_reg_cpps_class(cpps::C* c, std::string classname, std::string node = "_G")
	{
		cpps_domain* root = c->_G;
		cpps_domain* leftdomain = NULL;
		if (node != "_G")
		{
			cpps::cpps_regvar* d = c->_G->getvar(node, leftdomain, false);
			if (!d)
				return false;
			if (!d->getval().isdomain())
				return false;

			root = d->getval().value.domain;
		}
		leftdomain = NULL;
		cpps::cpps_regvar* var = root->getvar(classname, leftdomain, false);
		cpps_cppsclass* cls = (cpps_cppsclass*)var->getval().value.domain;
		cpps_class_singleton<T*>::instance()->setsls(cls);
		return true;
	}
	inline void	cpps_init_cpps_class(cpps::C* c)
	{
		CPPSMEMORYSETHANDLER(c->memory_handler); //init handler;
		cpps::cpps_reg_cpps_class<cpps::string>(c, "String", "_G");
		cpps::cpps_reg_cpps_class<cpps_vector>(c, "vector", "_G");
		cpps::cpps_reg_cpps_class<Buffer>(c, "Buffer", "_G");
		cpps::cpps_reg_cpps_class<C>(c, "C_STATE", "_G");
		cpps::cpps_reg_cpps_class<FILE>(c, "FILE", "_G");
		cpps::cpps_reg_cpps_class<cpps_lambda_function>(c, "CPPS_LAMBDA_FUNCTION", "_G");
		cpps::cpps_reg_cpps_class<cpps_io_stat>(c, "statinfo", "_G");
		cpps::cpps_reg_cpps_class<cpps_trycatch_error>(c, "cpps_trycatch_error", "_G");
		cpps::cpps_reg_cpps_class<cpps_map>(c, "map", "_G");
		cpps::cpps_reg_cpps_class<cpps_pair>(c, "pair", "_G");
		cpps::cpps_reg_cpps_class<cpps_range>(c, "RANGE", "_G");
		cpps::cpps_reg_cpps_class<cpps_async_object>(c, "ASYNC_OBJECT", "asyncio");
		cpps::cpps_reg_cpps_class<cpps_async_loop>(c, "ASYNC_LOOP", "asyncio");
		cpps::cpps_reg_cpps_class<cpps_async_task>(c, "ASYNC_TASK", "asyncio");
		cpps::cpps_reg_cpps_class<cpps_thread>(c, "thread", "_G");
		cpps::cpps_reg_cpps_class<cpps_lock>(c, "mutex", "_G");
		
	}
}