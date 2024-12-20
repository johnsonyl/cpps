#include "cpps/cpps.h"
namespace cpps {
	void printcallstack(std::string& errmsg, C* c);
	cpps_async_task::cpps_async_task()
	{
		runstate = cpps_async_task_pending;
		rt = 0;
		async_object = NULL;
	}

	cpps_async_task::~cpps_async_task()
	{
	}

	void cpps_async_task::run(cpps_async_task* p,C*c)
	{
		try
		{
			p->ret = p->async_object->call(c);
			p->runstate = cpps_async_task_done;
		}
		catch (cpps_trycatch_error& e)
		{
			std::string errmsg;
			printcallstack(errmsg, c);

			p->throwerr = e;
			p->throwerr._callstackstr += errmsg;
			p->runstate = cpps_async_task_thorw;

			if (p->async_object->catch_cb(c, object(c, &p->throwerr)))
				p->runstate = cpps_async_task_done;
		}
		catch (cpps_error& e)
		{
			std::string errmsg;
			printcallstack(errmsg, c);

			p->throwerr.attach(e);
			p->throwerr._callstackstr = errmsg;
			p->runstate = cpps_async_task_thorw;

			if (p->async_object->catch_cb(c, object(c, &p->throwerr)))
				p->runstate = cpps_async_task_done;
		}
	}

	cpps_value cpps_async_task::getresult()
	{
		return ret;
	}

	cpps_async_object* cpps_async_task::getobject()
	{
		return async_object;
	}

	int8 cpps_async_task::state()
	{
		return runstate;
	}

	bool cpps_async_task::cancelled()
	{
		return runstate == cpps_async_task_cancelled;
	}

	bool cpps_async_task::done()
	{
		return runstate == cpps_async_task_done;
	}
	cpps_integer cpps_async_task::get_id()
	{
		return (cpps_integer)rt;
	}

	bool cpps_async_task::timeout()
	{
		return runstate == cpps_async_task_timeouted;
	}

	bool cpps_async_task::pending()
	{
		return runstate == cpps_async_task_pending;
	}
	bool cpps_async_task::running()
	{
		return runstate == cpps_async_task_running;
	}

	void cpps_async_task::cancel()
	{
		runstate = cpps_async_task_cancelled;
	}


	void cpps_async_task::start(C * cstate)
	{
		c = cstate;
		runstate = cpps_async_task_running;
	}

	void cpps_async_task::add_done_callback(cpps_value func, cpps_value context)
	{
		callback_func = func;
		callback_context = context;
	}

	void cpps_async_task::remove_done_callback()
	{
		callback_func = nil;
		callback_context = nil;
	}

	void cpps_async_task::call_done_callback(C* c)
	{
		async_object->done_cb(c,ret);

		if(callback_func.tt == CPPS_TFUNCTION || callback_func.tt == CPPS_TLAMBDAFUNCTION)
			dofunction(c, callback_func, this,callback_context);
	}

	void cpps_async_task::set_name(std::string n)
	{
		name = n;
	}

	std::string cpps_async_task::get_name()
	{
		return name;
	}

}
