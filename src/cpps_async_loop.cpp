#include "cpps/cpps.h"

namespace cpps {
	cpps_async_loop::cpps_async_loop()
	{
		c = NULL;
		runstate = false;
	}

	cpps_async_loop::~cpps_async_loop()
	{
		/*cleanup not used data*/
		for (auto n : _tasks)
		{
			delete n;
		}
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
		
		if (task.isdomain() && task.value.domain->domainname == "ASYNC_OBJECT") {
			cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(task);
			cpps_async_task* vtask = create_task(vobj);
			push_task(c,vtask);
			ret = cpps_cpp_to_cpps_converter<cpps_async_task*>::apply(c, vtask);
		}
		else if (task.isdomain() && task.value.domain->domainname == "ASYNC_TASK") {
			cpps_async_task* vtask = cpps_converter<cpps_async_task*>::apply(task);
			if (vtask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running. "));
			push_task(c,vtask);
			ret = cpps_cpp_to_cpps_converter<cpps_async_task*>::apply(c, vtask);
		}
		else if (task.isdomain() && task.value.domain->domainname == "vector"){
			cpps_vector* retvec;
			ret = newclass<cpps_vector>(c, &retvec);
			cpps_vector* vec = cpps_converter<cpps_vector*>::apply(task);
			for (auto v : vec->realvector()) {
				if (v.isdomain() && v.value.domain->domainname == "ASYNC_OBJECT") {
					cpps_async_object* vobj = cpps_converter<cpps_async_object*>::apply(v);
					cpps_async_task* vtask = create_task(vobj);
					push_task(c,vtask);
					retvec->push_back(cpps_cpp_to_cpps_converter<cpps_async_task*>::apply(c, vtask));
				}
				else if (v.isdomain() && task.value.domain->domainname == "ASYNC_TASK") {
					cpps_async_task* vtask = cpps_converter<cpps_async_task*>::apply(v);
					if (vtask->state())  throw(cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "the task state is running."));
					push_task(c,vtask);
					retvec->push_back(cpps_cpp_to_cpps_converter<cpps_async_task*>::apply(c, vtask));
				}
			}
		}
		else
			throw (cpps_error(c->curnode->filename, c->curnode->line, cpps_error_asyncerror, "async run_until_complete is a unknow task type."));

		/*����*/
		loop(c);

		/*����Э��*/
		for (auto task : _tasks) {
			if(task)
				coroutine::destroy(ordinator,task->rt); 
		}
		_tasks.clear();

		return ret;
	}
	void cpps_async_loop::loop(C* c)
	{
		runstate = true;
		std::vector<cpps_stack*> *takestacklist = c->getcallstack(); /*��¼ԭʼcallstack*/
		bool hasrun = true;
		while (hasrun) {
			hasrun = false;
			/*pop empty task*/
			popemptytask();

			for(size_t i = 0; i < _tasks.size(); i++){
				auto task = _tasks[i];
				if (task){
					if (task->rt == MAXUINT64)
					{
						task->runstate = cpps_async_task_done;
						_tasks[i] = NULL; /*�����ͷ���.*/
					}
					else if (task->state() == cpps_async_task_running){
						/*�����Э�̵�stack���û���*/
						c->setcallstack(&task->takestacklist);
						
						coroutine::resume(ordinator, task->rt);
						
						task = _tasks[i]; /*��Ҫ�ָ�task*/
						hasrun = true;
						if (task->state() == cpps_async_task_done || task->state() == cpps_async_task_thorw) {
							task->call_done_callback(c);
							coroutine::destroy(ordinator, task->rt);
							task->rt = MAXUINT64;
							_tasks[i] = NULL;
						}
					}
					else if (task->state() == cpps_async_task_cancelled) { /*�û�ȡ����Ҫ���⴦��ʲô��?*/
						if(task->rt != MAXUINT64) coroutine::destroy(ordinator, task->rt);
						task->rt = MAXUINT64;
						_tasks[i] = NULL;
					}
					else{
						if (task->rt != MAXUINT64) coroutine::destroy(ordinator, task->rt);
						task->rt = MAXUINT64;
						_tasks[i] = NULL; /*�����ͷ���.*/
					}
				}
			}
		}
		/*�ָ�stack*/
		c->setcallstack(takestacklist);
		runstate = false;
	}

	void cpps_async_loop::popemptytask()
	{
		std::vector< cpps_async_task* >::iterator it = _tasks.begin();
		std::vector< cpps_async_task* >::iterator end = _tasks.end();
		for (;it != end;) {
			cpps_async_task* task = *it;
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

	void cpps_async_loop::push_task(C*c,cpps_async_task* task) 
	{
		task->rt = coroutine::create(ordinator,std::bind(cpps_async_task::run, task, c));/*����Э��*/
		task->start(c);
		
		_tasks.push_back(task); //����Ĭ���б�
	}
	cpps_async_task* cpps_async_loop::create_task(cpps_async_object* obj)
	{
		cpps_async_task* task = new cpps_async_task();
		task->async_object = obj;
		return task;
	}

}