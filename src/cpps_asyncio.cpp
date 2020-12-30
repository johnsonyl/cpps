#include "cpps/cpps.h"

#include <chrono>

namespace cpps {
	cpps_integer	cpps_time_gettickcount();
	cpps_async_loop* cpps_async_get_event_loop(C* c) {
		return (cpps_async_loop*)c->getmoduledata("asyncio"); 
	}
	cpps_value cpps_async_run(C* c, cpps_value obj) {
		return cpps_async_get_event_loop(c)->run_until_complete(c,obj);
	}
	cpps_value cpps_async_await(C* c, cpps_value var) {
		cpps_async_task* task = NULL;
		cpps_value ret;
		cpps_async_loop* loop = cpps_async_get_event_loop(c);
		if (var.isdomain() && var.value.domain->domainname == "ASYNC_OBJECT") {
			ret = loop->create_task(c,cpps_converter<cpps_async_object*>::apply(var),&task);
			loop->push_task(c, ret);

		}
		else if (var.isdomain() && var.value.domain->domainname == "ASYNC_TASK") {
			task = cpps_converter<cpps_async_task*>::apply(var);
			ret = var;
		}
		else {
			throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "await just support ASYNC_OBJECT or ASYNC_TASK.")); 
		}
		int8 status = task->state();
		while (status == cpps_async_task_running) {
			coroutine::yield(loop->ordinator);
			status = task->state();
		}
		return ret;
	}
	cpps_value cpps_async_wait_for(C* c, cpps_value var,cpps_integer ms) {
		cpps_async_task* task = NULL;
		cpps_value ret;
		cpps_async_loop* loop = cpps_async_get_event_loop(c);
		if (var.isdomain() && var.value.domain->domainname == "ASYNC_OBJECT") {
			ret = loop->create_task(c,cpps_converter<cpps_async_object*>::apply(var),&task);
			loop->push_task(c, ret);
		}
		else if (var.isdomain() && var.value.domain->domainname == "ASYNC_TASK") {
			task = cpps_converter<cpps_async_task*>::apply(var);
			ret = var;
		}
		else {
			throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "wait_for just support ASYNC_OBJECT or ASYNC_TASK."));
		}
		cpps_integer start = cpps_time_gettickcount();
		int8 status = task->state();
		while (status == cpps_async_task_running) {
			if (cpps_time_gettickcount() - start > ms) {
				task->runstate = cpps_async_task_timeouted;
				break;
			}
			coroutine::yield(loop->ordinator);
			status = task->state();
		}
		return ret;
	}

	cpps_value cpps_async_create_task(C* c,cpps_async_object *obj) {
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		cpps_value ret;
		cpps_async_task* task;
		ret = loop->create_task(c, obj,&task);
		loop->push_task(c,ret); /*加入默认执行队列*/
		return ret;
	}
	void	cpps_async_sleep(C*c,cpps_integer ms) {
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		coroutine::wait_for(loop->ordinator,std::this_thread::sleep_for<cpps_integer, std::milli>, std::chrono::milliseconds(ms));
	}

	void cpps_regasyncio(C* c)
	{
		cpps_async_loop* loop = new cpps_async_loop(); 
		loop->setcstate(c);
		c->setmoduledata("asyncio", (cpps_module_data*)loop);

		cpps::_module(c, "asyncio")[
			_class<cpps_async_object>("ASYNC_OBJECT"),
			_class<cpps_async_loop>("ASYNC_LOOP")
				.def_inside("run_until_complete", &cpps_async_loop::run_until_complete),
			_class<cpps_async_task>("ASYNC_TASK")
				.def_inside("result",&cpps_async_task::getresult)
				.def("state",&cpps_async_task::state)
				.def("cancelled",&cpps_async_task::cancelled)
				.def("done",&cpps_async_task::done)
				.def("timeout",&cpps_async_task::timeout)
				.def("pending",&cpps_async_task::pending)
				.def("running",&cpps_async_task::running)
				.def("set_name",&cpps_async_task::set_name)
				.def("get_name",&cpps_async_task::get_name)
				.def("add_done_callback",&cpps_async_task::add_done_callback)
				.def("remove_done_callback",&cpps_async_task::remove_done_callback),
			def_inside("get_event_loop", cpps_async_get_event_loop),
			def_inside("create_task", cpps_async_create_task),
			def_inside("sleep", cpps_async_sleep,true),
			def_inside("wait", cpps_async_await,true),
			def_inside("wait_for", cpps_async_wait_for,true),
			def_inside("run", cpps_async_run),
			defvar(c,"pending", (cpps_integer) cpps_async_task_pending),
			defvar(c,"running", (cpps_integer)cpps_async_task_running),
			defvar(c,"done", (cpps_integer)cpps_async_task_done),
			defvar(c,"cancelled", (cpps_integer)cpps_async_task_cancelled),
			defvar(c,"timeout", (cpps_integer)cpps_async_task_timeouted)

		];
	}

	void cpps_unregasyncio(C* c)
	{
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		delete loop;
		c->setmoduledata("asyncio", NULL);
	}

}
