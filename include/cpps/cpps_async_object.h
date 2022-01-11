#ifndef CPPS_ASYNC_OBJECT_CPPS_HEAD_
#define CPPS_ASYNC_OBJECT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/5 (yy/mm/dd)
//@Module		:	CPPS_ASYNC_OBJECT
//@Description	:	协程对象
//@website		:	http://cppscript.org
//==================================
namespace cpps
{
	struct cpps_async_task;
	struct cpps_async_object
	{
	public:
		cpps_async_object() {
			leftdomain = NULL;
			f = NULL;
			line = 0;
			_task = NULL;
			iscatchd = false;
		}
		virtual~cpps_async_object() {}

		cpps_value			call(C* c);
		void				done_cb(C* c, object ret);
		bool				catch_cb(C* c, object ret);
		cpps_async_object*  done(C* c, cpps::object cb);
		cpps_async_object*  _catch(C* c, cpps::object cb);
		
		cpps_async_task*	get_task();
		void				set_task(cpps_async_task*task);
		cpps_std_vector params;
		cpps_domain* leftdomain;
		cpps_function* f;
		cpps_async_task* _task;
		std::string filename;
		std::string funcname;
		int32 line;
		cpps::object _cb;
		cpps::object _catchcb;
		bool  iscatchd;
	};

}
#endif //CPPS_ASYNC_OBJECT_CPPS_HEAD_