#ifndef CPPS_ASYNC_CPPS_HEAD_
#define CPPS_ASYNC_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/5 (yy/mm/dd)
//@Module		:	CPPS_ASYNC
//@Description	:	Ð­³Ì
//@website		:	http://cpps.wiki
//==================================
namespace cpps
{
	void	cpps_regasyncio(C* c);
	void	cpps_unregasyncio(C* c);

	cpps_async_loop* cpps_async_get_event_loop(C* c);
	cpps_value cpps_async_await(C* c, cpps_value var);
	cpps_value cpps_async_wait_for(C* c, cpps_value var, cpps_integer ms);
	cpps_value cpps_async_create_task(C* c, cpps_async_object* obj);
	void	cpps_async_sleep(C* c, cpps_integer ms);
}
#endif