#include "cpps/cpps.h"

namespace cpps {
	cpps_async_loop::cpps_async_loop()
	{
		c = NULL;
		runstate = false;
		ordinator = Singleton<coroutine::Ordinator>::getInstance();
	}

	cpps_async_loop::~cpps_async_loop()
	{
		/*remove not used data*/
		_tasks.clear();
	}

	void cpps_async_loop::setcstate(C* pc)
	{
		c = pc;
		c->ordinator = Singleton< coroutine::Ordinator>::getInstance();
	}
	cpps_value	 cpps_async_loop::run_until_complete(C* c, cpps_value task)
	{
		/*
		* task 可能是3种类型
		* 1. cpps_async_object 类型
		* 2. cpps_async_task 类型
		* 3. vector<cpps_async_task> 类型  数组结构传进来,则数组结构传出去.
		*/
		if (isrunning()) throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the loop is running,please use await call async function. ")); /*已经运行中,不能再次调用*/
		_tasks.clear(); /*说明用户自己启用协程列表,不由默认*/
		cpps_value ret;
		cpps_async_task* roottask = NULL;
		if (task.isdomain() && task.is_kindof<cpps_async_object>()) {
			cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(task);
			if (vobj->get_task() == NULL) {
				ret = create_task(c, vobj, &roottask);
				push_task(c, ret);
			}
			else {
				roottask = vobj->get_task();
				object task(c, roottask);
				ret = task.getval();
			}
		}
		else if (task.isdomain() && task.is_kindof<cpps_async_task>()) {
			roottask = cpps_converter<cpps_async_task*>::apply(task);
			ret = task;
			if (roottask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running. "));
			push_task(c, ret);
		}
		else if (task.isdomain() && task.is_kindof<cpps_vector>()){
			cpps_vector* retvec;
			newclass<cpps_vector>(c, &retvec,&ret);
			cpps_vector* vec = cpps_converter<cpps_vector*>::apply(task);
			for (auto v : vec->realvector()) {
				if (v.isdomain() && v.is_kindof<cpps_async_object>()) {
					cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(v);
					cpps_value vtask_value;
					cpps_async_task* vtask;
					if (vobj->get_task() == NULL) {
						vtask_value = create_task(c, vobj, &vtask);
						push_task(c, vtask_value);
					}
					else {
						vtask = vobj->get_task();
						object task(c, vtask);
						vtask_value = task.getval();
					}
					retvec->push_back(vtask_value);
				}
				else if (v.isdomain() && task.is_kindof<cpps_async_task>()) {
					cpps_async_task* vtask = cpps_converter<cpps_async_task*>::apply(v);
					if (vtask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running."));
					push_task(c,v);
					retvec->push_back(v);
				}
			}
		}
		else
			throw (cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "async run_until_complete is a unknow task type."));

		/*运行*/
		loop(c, roottask);

		/*回收协程*/
		for (auto vtask : _tasks) {
			cpps_async_task* task2 = cpps_converter<cpps_async_task*>::apply(vtask);
			if (task2 && task2->rt != MAXUINT64)
				coroutine::destroy(*ordinator, task2->rt);
		}
		_tasks.clear();


		return ret;
	}
	void cpps_async_loop::loop(C* c, cpps_async_task* roottask)
	{
		runstate = true;
		std::vector<cpps_stack*> *takestacklist = c->getcallstack(); /*记录原始callstack*/
		bool hasrun = true;
		while (hasrun) {
			hasrun = false;
			/*pop empty task*/
			popemptytask();

			/*当主协程运行完毕,则关闭所有协程*/
			if (roottask && roottask->state() != cpps_async_task_running) {
				terminate_all_task();
			}

			for(size_t i = 0; i < _tasks.size(); i++){
				auto vtask = _tasks[i];
				auto task = cpps_converter<cpps_async_task*>::apply(vtask);
				if (task){
					if (task->rt == MAXUINT64)
					{
						task->runstate = cpps_async_task_done;
						_tasks[i] = nil; /*可以释放了.*/
					}
					else if (task->state() == cpps_async_task_running){
						/*将这个协程的stack设置回来*/
						c->setcallstack(&task->takestacklist);
						coroutine::resume(*ordinator, task->rt);
						
						vtask = _tasks[i]; /*需要恢复task*/
						task = cpps_converter<cpps_async_task*>::apply(vtask);
						hasrun = true;
						if (task->state() == cpps_async_task_done || task->state() == cpps_async_task_thorw) {
							if(task->state() == cpps_async_task_done) task->call_done_callback(c);
							coroutine::destroy(*ordinator, task->rt);
							task->rt = MAXUINT64;
							_tasks[i] = nil;
						}
					}
					else{
						//结束了但是没释放. timeout,terminate,cancel等状态.
						if (task->rt != MAXUINT64) {

							c->setcallstack(&task->takestacklist);
							c->isterminate = true; //通知协程停止.
							ordinator->isterminate = true;//通知协程停止.
							coroutine::resume(*ordinator, task->rt); //停止的原因是为了清理变量,否则导致GC计数永不为0 导致不能释放内存.造成内存泄露.

							vtask = _tasks[i]; /*需要恢复task*/
							task = cpps_converter<cpps_async_task*>::apply(vtask);

							c->isterminate = false;//返回正常状态.
							ordinator->isterminate = false;//返回正常状态.

							coroutine::destroy(*ordinator, task->rt);
						}
						task->rt = MAXUINT64;
						_tasks[i] = nil; /*可以释放了.*/
					}
				}
			}

		}
		/*恢复stack*/
		popemptytask();
		c->setcallstack(takestacklist);
		runstate = false;
	}

	void cpps_async_loop::terminate_all_task()
	{
		for (auto vtask : _tasks) {
			cpps_async_task* task = cpps_converter<cpps_async_task*>::apply(vtask);
			if (task && task->state() == cpps_async_task_running) {
				task->runstate = cpps_async_task_terminate;
			}
		}
	}

	void cpps_async_loop::popemptytask()
	{
		auto it = _tasks.begin();
		auto end = _tasks.end();
		for (;it != end;) {
			cpps_async_task* task = cpps_converter<cpps_async_task*>::apply (*it);
			if (task == NULL) {
				it = _tasks.erase(it);
				end = _tasks.end();
			}
			else
				++it;
		}
	}

	bool cpps_async_loop::isrunning()
	{
		return runstate;
	}

	void cpps_async_loop::push_task(C*c,cpps_value vtask) 
	{
		cpps_async_task* task = cpps_converter<cpps_async_task*>::apply(vtask);
		task->rt = coroutine::create(*ordinator,std::bind(cpps_async_task::run, task, c));/*创建协程*/
		task->start(c);
		
		_tasks.push_back(vtask); //加入默认列表
	}
	cpps_value cpps_async_loop::create_task(C*c,cpps_async_object* obj, cpps_async_task**outtask)
	{
		cpps_async_task* task;
		cpps_value ret;
		newclass< cpps_async_task>(c, &task, &ret);
		task->async_object = obj;
		obj->set_task(task);
		*outtask = task;
		return ret;
	}

}