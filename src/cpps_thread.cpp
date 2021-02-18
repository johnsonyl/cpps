#include "cpps/cpps.h"

namespace cpps
{
	void cpps_create_root_G(C* c);
	cpps_thread::cpps_thread()
	{
		_cx_thread = NULL;
		_cx_parent_thread_c = NULL;
	}


	cpps_thread::~cpps_thread()
	{
		if (_cx_thread) {
			delete _cx_thread;
		}
	}

	void cpps_thread::constructor(C* _cx_parent_thread,object func, object v)
	{
		_cx_parent_thread_c = _cx_parent_thread;
		
		_cx_thread = new std::thread(cpps_thread::_cx_thread_func, this, func,v);
	}

	void cpps_thread::join()
	{
		_cx_thread->join();
	}
	bool cpps_thread::joinable() 
	{
		return _cx_thread->joinable();
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
		C* c = new cpps::C();
		cpps_create_root_G(c);
		c->clone(pthis->_cx_parent_thread_c);
		_CPPS_TRY
			cpps::dofunction(c, func, v);
		_CPPS_CATCH
		c->_G->cleanup();
		cpps::close(c);
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
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::nanoseconds));
		return ret.toobject();
	}
	object cpps_chrono_microseconds(C* c, cpps_integer i) {
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::microseconds));
		return ret.toobject();
	}
	object cpps_chrono_milliseconds(C* c, cpps_integer i) {
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::milliseconds));
		return ret.toobject();
	}
	object cpps_chrono_seconds(C* c, cpps_integer i) {
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::seconds));
		return ret.toobject();
	}
	object cpps_chrono_minutes(C* c, cpps_integer i) {
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::minutes));
		return ret.toobject();
	}
	object cpps_chrono_hours(C* c, cpps_integer i) {
		object::pair ret = object::pair::create(c, object::create(c, i), object::create(c, (cpps_integer)cpps_chrono_type::hours));
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