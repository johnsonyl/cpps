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
		void											loop(C* c, cpps_async_task* roottask);

		void											terminate_all_task();

		void											popemptytask();
		bool											isrunning();
		void											push_task(C *c, cpps_value task);
		cpps_value										run_until_complete(C* c, cpps_value task);
		cpps_value										create_task(C* c, cpps_async_object* obj, cpps_async_task** outtask);
		coroutine::Ordinator							ordinator;
		std::vector< cpps_value >						_tasks;
		C*												c;
		bool											runstate;
	};
}
#endif //CPPS_ASYNC_LOOP_CPPS_HEAD_