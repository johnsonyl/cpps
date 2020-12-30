#include "cpps/cpps.h"

namespace cpps {
	cpps_async_loop::cpps_async_loop()
	{
		c = NULL;
		runstate = false;
	}

	cpps_async_loop::~cpps_async_loop()
	{
		/*remove not used data*/
		for (auto n : _tasks)
			n = nil;
		_tasks.clear();
	}

	void cpps_async_loop::setcstate(C* pc)
	{
		c = pc;
	}
	cpps_value	 cpps_async_loop::run_until_complete(C* c, cpps_value task)
	{
		/*
		* task ������3������
		* 1. cpps_async_object ����
		* 2. cpps_async_task ����
		* 3. vector<cpps_async_task> ����  ����ṹ������,������ṹ����ȥ.
		*/
		if (isrunning()) throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the loop is running,please use await call async function. ")); /*�Ѿ�������,�����ٴε���*/
		_tasks.clear(); /*˵���û��Լ�����Э���б�,����Ĭ��*/
		cpps_value ret;
		cpps_async_task* roottask = NULL;
		if (task.isdomain() && task.value.domain->domainname == "ASYNC_OBJECT") {
			cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(task);
			ret = create_task(c,vobj, &roottask);
			push_task(c, ret);
		}
		else if (task.isdomain() && task.value.domain->domainname == "ASYNC_TASK") {
			roottask = cpps_converter<cpps_async_task*>::apply(task);
			ret = task;
			if (roottask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running. "));
			push_task(c, ret);
		}
		else if (task.isdomain() && task.value.domain->domainname == "vector"){
			cpps_vector* retvec;
			ret = newclass<cpps_vector>(c, &retvec);
			cpps_vector* vec = cpps_converter<cpps_vector*>::apply(task);
			for (auto v : vec->realvector()) {
				if (v.isdomain() && v.value.domain->domainname == "ASYNC_OBJECT") {
					cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(v);
					cpps_async_task* vtask;
					cpps_value vtask_value = create_task(c, vobj,&vtask);
					push_task(c, vtask_value);
					retvec->push_back(vtask_value);
				}
				else if (v.isdomain() && task.value.domain->domainname == "ASYNC_TASK") {
					cpps_async_task* vtask = cpps_converter<cpps_async_task*>::apply(v);
					if (vtask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running."));
					push_task(c,v);
					retvec->push_back(v);
				}
			}
		}
		else
			throw (cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "async run_until_complete is a unknow task type."));

		/*����*/
		loop(c, roottask);

		/*����Э��*/
		for (auto vtask : _tasks) {
			cpps_async_task* task = cpps_converter<cpps_async_task*>::apply(vtask);
			if (task)
				coroutine::destroy(ordinator,task->rt); 
		}
		_tasks.clear();


		return ret;
	}
	void cpps_async_loop::loop(C* c, cpps_async_task* roottask)
	{
		runstate = true;
		std::vector<cpps_stack*> *takestacklist = c->getcallstack(); /*��¼ԭʼcallstack*/
		bool hasrun = true;
		while (hasrun) {
			hasrun = false;
			/*pop empty task*/
			popemptytask();

			/*����Э���������,��ر�����Э��*/
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
						_tasks[i] = nil; /*�����ͷ���.*/
					}
					else if (task->state() == cpps_async_task_running){
						/*�����Э�̵�stack���û���*/
						c->setcallstack(&task->takestacklist);
						
						coroutine::resume(ordinator, task->rt);
						
						vtask = _tasks[i]; /*��Ҫ�ָ�task*/
						task = cpps_converter<cpps_async_task*>::apply(vtask);
						hasrun = true;
						if (task->state() == cpps_async_task_done || task->state() == cpps_async_task_thorw) {
							if(task->state() == cpps_async_task_done) task->call_done_callback(c);
							coroutine::destroy(ordinator, task->rt);
							task->rt = MAXUINT64;
							_tasks[i] = nil;
						}
					}
					else if (task->state() == cpps_async_task_cancelled) { /*�û�ȡ����Ҫ���⴦��ʲô��?*/
						if(task->rt != MAXUINT64) coroutine::destroy(ordinator, task->rt);
						task->rt = MAXUINT64;
						_tasks[i] = nil;
					}
					else if (task->state() == cpps_async_task_terminate) { /*ǿ��ֹͣ?*/
						if(task->rt != MAXUINT64) coroutine::destroy(ordinator, task->rt);
						task->rt = MAXUINT64;
						_tasks[i] = nil;
					}
					else{
						if (task->rt != MAXUINT64) coroutine::destroy(ordinator, task->rt);
						task->rt = MAXUINT64;
						_tasks[i] = nil; /*�����ͷ���.*/
					}
				}
			}

		}
		/*�ָ�stack*/
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
		task->rt = coroutine::create(ordinator,std::bind(cpps_async_task::run, task, c));/*����Э��*/
		task->start(c);
		
		_tasks.push_back(vtask); //����Ĭ���б�
	}
	cpps_value cpps_async_loop::create_task(C*c,cpps_async_object* obj, cpps_async_task**outtask)
	{
		cpps_async_task* task;
		cpps_value ret = newclass< cpps_async_task>(c, &task);
		task->async_object = obj;
		*outtask = task;
		return ret;
	}

}