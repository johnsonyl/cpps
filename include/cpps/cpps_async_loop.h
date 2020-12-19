#ifndef CPPS_ASYNC_LOOP_CPPS_HEAD_
#define CPPS_ASYNC_LOOP_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/5 (yy/mm/dd)
//@Module		:	CPPS_ASYNC_LOOP
//@Description	:	Ð­³ÌÑ­»·
//@website		:	http://cpps.wiki
//==================================
namespace cpps
{
	struct cpps_async_loop
	{
		cpps_async_loop();
		~cpps_async_loop();

		void											setcstate(C* pc);
		void											loop(C* c);
		void											popemptytask();
		bool											isrunning();
		void											push_task(C *c,cpps_async_task* task);
		cpps_value										run_until_complete(C* c, cpps_value task);
		cpps_async_task*								create_task(cpps_async_object* obj);
		coroutine::Ordinator							ordinator;
		std::vector< cpps_async_task* >					_tasks;
		C*												c;
		bool											runstate;
	};
}
#endif //CPPS_ASYNC_LOOP_CPPS_HEAD_