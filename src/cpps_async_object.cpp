#include "cpps/cpps.h"

namespace cpps {
	cpps_value cpps_execute_callfunction(C* c, cpps_function* f, cpps_domain* domain, std::string filename, int32 line, std::string funcname, cpps_std_vector& params);
	cpps_value cpps_async_object::call(C* c)
	{
		return cpps_execute_callfunction(c, f, leftdomain, filename, line, funcname, params);
	}
	void	cpps_async_object::done_cb(C*c,object ret) {
		if (iscatchd) return;
		if (_cb.isfunction()) {
			if (ret.istuple()) {
				function_caller(c, _cb, object::vector(ret).realvector());
			}
			else {
				dofunction(c, _cb, ret);
			}
		}
	}
	bool	cpps_async_object::catch_cb(C* c, object e) {
		iscatchd = true;
		if (_catchcb.isfunction()) {
			dofunction(c, _catchcb, e);
			return true;
		}
		return false;
	}
	cpps_async_task* cpps_async_object::get_task() {
		return _task;
	}
	void cpps_async_object::set_task(cpps_async_task* task) {
		_task = task;
	}
	cpps_async_object* cpps_async_object::done(C*c,object cb) {
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		if (get_task() == NULL) {
			cpps_value ret;
			cpps_async_task* task;
			ret = loop->create_task(c, this, &task);
			loop->push_task(c, ret); /*加入默认执行队列*/
		}
		_cb = cb;
		return this;
	}

	cpps_async_object* cpps_async_object::_catch(C* c, cpps::object cb) {
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		if (get_task() == NULL) {
			cpps_value ret;
			cpps_async_task* task;
			ret = loop->create_task(c, this, &task);
			loop->push_task(c, ret); /*加入默认执行队列*/
		}
		_catchcb = cb;
		return this;
	}
}
