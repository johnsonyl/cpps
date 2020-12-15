#include "cpps.h"



namespace cpps
{



	void cpps_regarray(C *c)
	{
		cpps::_module(c)[
			_class<cpps_vector>("vector")
				.def("push_back", &cpps_vector::push_back)
				.def("add", &cpps_vector::push_back)
				.def("append", &cpps_vector::push_back)
				.def("assign", &cpps_vector::assign)
				.def("push_front", &cpps_vector::push_front)
				.def("pop_back", &cpps_vector::pop_back)
				.def("pop_front", &cpps_vector::pop_front)
				.def("insert", &cpps_vector::insert)
				.def("at", &cpps_vector::at)
				.def("erase", &cpps_vector::erase)
				.def("pop", &cpps_vector::pop)
				.def("begin", &cpps_vector::begin)
				.def("end", &cpps_vector::end)
				.def("next", &cpps_vector::next)
				.def("empty", &cpps_vector::empty)
				.def("it", &cpps_vector::it)
				.def("resize", &cpps_vector::resize)
				.def("clear", &cpps_vector::clear)
				.def("size", &cpps_vector::size)
				.def("has", &cpps_vector::has)
		];
	}
}