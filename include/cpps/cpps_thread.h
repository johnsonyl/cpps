#ifndef CPPS_THREAD_CPPS_HEAD_
#define CPPS_THREAD_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2018/09/26 (yy/mm/dd)
//@Module		:	CPPS_THREAD
//@Description	:	œﬂ≥Ã¿‡
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	class cpps_thread
	{
	public:
		cpps_thread();
		~cpps_thread();

		void			constructor(C* __parent_thread, object func, object v);
		void			join();
		bool			joinable();
		bool			isdone();
		void			detach();
		void			swap(cpps_thread* _swap_thread);
		static	void	_cx_thread_func(cpps_thread* pthis, object func, object v);
		void			set_return(cpps::object v);
		cpps_value		get_return();
		C*				_cx_parent_thread_c;
		std::thread		*_cx_thread;
		cpps_value		_ret_Value;
		std::atomic<bool> _isdone;
	};

	void		cpps_regthread(C* c);
}

#endif // CPPS_THREAD_CPPS_HEAD_