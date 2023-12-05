#include "cpps/cpps.h"

namespace cpps
{
	void cpps_create_root_G(C* c);
	void cpps_gc_add_gen0(C* c, const cpps_value& v);
	void gc_swap(C* src, C* dest);
	cpps_thread::cpps_thread()
	{
		_cx_thread = NULL;
		_cx_parent_thread_c = NULL;
		_isdone = false;
	}


	cpps_thread::~cpps_thread()
	{
		if (_cx_thread) {
			CPPSFREE( _cx_thread );
			_cx_thread = NULL;
		}
		_cx_parent_thread_c = NULL;
		_isdone = false;
	}

	void cpps_thread::constructor(C* _cx_parent_thread,object func, object v)
	{
		_cx_parent_thread_c = _cx_parent_thread;
		
		_cx_thread = CPPSNEW( std::thread)(cpps_thread::_cx_thread_func, this, func,v);
	}

	void cpps_thread::join()
	{
		_cx_thread->join();
	}
	bool cpps_thread::joinable() 
	{
		return _cx_thread->joinable();
	}
	bool cpps_thread::isdone()
	{
		return _isdone;
	}
	void cpps_thread::detach()
	{
		_cx_thread->detach();
	}
	void cpps_thread::swap(cpps_thread* _swap_thread)
	{
		_cx_thread->swap(*_swap_thread->_cx_thread);
	}

	void cpps_thread::_cx_thread_func(cpps_thread* pthis, object func, object v)
	{
		C* c = cpps::create(pthis->_cx_parent_thread_c);
		_CPPS_TRY
			object ret = cpps::dofunction(c, func, v);
			pthis->set_return(ret);
			//这里需要特殊处理，因为返回来的对象可能在此线程的GC中。但是需要把这个指针放到主线程。否则会内存泄露。但是如何拿到主GC呢？
		_CPPS_CATCH
		cpps::close(c,pthis->_cx_parent_thread_c);
		pthis->_isdone = true;
	}
	void cpps_thread::set_return(cpps::object v)
	{
		_ret_Value = v.realval();
	}
	cpps_value cpps_thread::get_return()
	{
		return _ret_Value;
	}
	cpps_integer cpps_this_thread_get_id() {
		std::thread::id tid = std::this_thread::get_id();
#ifdef _WIN32
		_Thrd_id_t t = *((_Thrd_id_t*)(&tid));
#else

		usint32 t = 0;
		memcpy(&t, &tid, sizeof(t));
#endif
		return (cpps_integer)t;
	}
	enum cpps_chrono_type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};
	void cpps_this_thread_sleep_for(cpps::cpps_pair *_pair) {
		cpps_integer i = cpps_to_integer(_pair->first());
		cpps_integer type = cpps_to_integer(_pair->second());
		switch (type)
		{
		case cpps_chrono_type::nanoseconds:
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(i));
			break;
		}
		case cpps_chrono_type::microseconds:
		{
			std::this_thread::sleep_for(std::chrono::microseconds(i));
			break;
		}
		case cpps_chrono_type::milliseconds:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(i));
			break;
		}
		case cpps_chrono_type::seconds:
		{
			std::this_thread::sleep_for(std::chrono::seconds(i));
			break;
		}
		case cpps_chrono_type::minutes:
		{
			std::this_thread::sleep_for(std::chrono::minutes(i));
			break;
		}
		case cpps_chrono_type::hours:
		{
			std::this_thread::sleep_for(std::chrono::hours(i));
			break;
		}
		default:
			break;
		}
	}
	void cpps_this_thread_sleep_until(cpps_integer dt) {
		std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(dt));
	}
	
	object cpps_chrono_nanoseconds(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::nanoseconds);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	object cpps_chrono_microseconds(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::microseconds);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	object cpps_chrono_milliseconds(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::milliseconds);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	object cpps_chrono_seconds(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::seconds);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	object cpps_chrono_minutes(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::minutes);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	object cpps_chrono_hours(C* c, cpps_integer i) {
		auto first = object::create(c, i);
		auto second = object::create(c, (cpps_integer)cpps_chrono_type::hours);
		object::pair ret = object::pair::create(c, first, second);
		return ret.toobject();
	}
	void cpps_regthread(C* c)
	{
		cpps::_module(c)[
			_class< cpps_thread>("thread")
				.def_inside("constructor",&cpps_thread::constructor)
				.def("join", &cpps_thread::join)
				.def("joinable", &cpps_thread::joinable)
				.def("swap", &cpps_thread::swap)
				.def("detach", &cpps_thread::detach)
				.def("get",&cpps_thread::get_return)
				.def("isdone",&cpps_thread::isdone)
		];
		cpps::_module(c, "chrono")[
			def_inside("nanoseconds", cpps_chrono_nanoseconds),
			def_inside("microseconds", cpps_chrono_microseconds),
			def_inside("milliseconds", cpps_chrono_milliseconds),
			def_inside("seconds", cpps_chrono_seconds),
			def_inside("minutes", cpps_chrono_minutes),
			def_inside("hours", cpps_chrono_hours)
		];
		cpps::_module(c,"this_thread")[
			def("get_id", cpps_this_thread_get_id),
			def("yield", std::this_thread::yield),
			def("sleep_for", cpps_this_thread_sleep_for),
			def("sleep_until", cpps_this_thread_sleep_until)
		];

	}

}