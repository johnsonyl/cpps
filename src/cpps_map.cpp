#include "cpps.h"


namespace cpps
{
	

	void cpps_regmap(C *c)
	{
		cpps::_module(c)[
			_class<cpps_map_node>("cpps_map_node")
				.def("first", &cpps_map_node::first)
				.def("second", &cpps_map_node::second),
			_class<cpps_map>("map")
				.def("insert", &cpps_map::insert)
				.def("find", &cpps_map::find)
				.def("erase", &cpps_map::erase)
				.def("pop", &cpps_map::pop)
				.def("begin", &cpps_map::begin)
				.def("end", &cpps_map::end)
				.def("next", &cpps_map::next)
				.def("it", &cpps_map::it)
				.def("key", &cpps_map::key)
				.def("empty", &cpps_map::empty)
				.def("clear", &cpps_map::clear)
				.def("size", &cpps_map::size),
			_class<cpps_unordered_map>("unordered_map")
				.def("insert", &cpps_unordered_map::insert)
				.def("find", &cpps_unordered_map::find)
				.def("erase", &cpps_unordered_map::erase)
				.def("pop", &cpps_unordered_map::pop)
				.def("begin", &cpps_unordered_map::begin)
				.def("end", &cpps_unordered_map::end)
				.def("next", &cpps_unordered_map::next)
				.def("it", &cpps_unordered_map::it)
				.def("key", &cpps_unordered_map::key)
				.def("empty", &cpps_unordered_map::empty)
				.def("clear", &cpps_unordered_map::clear)
				.def("size", &cpps_unordered_map::size)
		];
	}
	

	cpps_unordered_map::~cpps_unordered_map()
	{
		_map.clear();
	}

	void cpps_unordered_map::insert(cpps_value k, cpps_value v)
	{
		_map.insert(std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::value_type(k, v));
	}

	cpps::cpps_value cpps_unordered_map::find(cpps_value k)
	{
		cpps_value ret;
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator it = _map.find(k);
		if (it != _map.end())
		{
			ret = it->second;
		}
		if (false)
		{
			printf("=================================================:");

			printf("notfind:");
			cpps_base_printf(k);
			printf("\n");

			std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator it = _map.begin();
			for (; it != _map.end(); ++it)
			{
				printf("key:");
				cpps_base_printf(it->first);
				printf("value:");
				cpps_base_printf(it->second);
				printf("\n");
			}
		}
		return ret;
	}

	cpps_value& cpps_unordered_map::cpps_find(const cpps_value & k)
	{
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator it = _map.find(k);
		if (it != _map.end())
		{
			return it->second;
		}
		else
		{
			insert(k, cpps_value());
			return cpps_find(k);
		}
	}

	void cpps_unordered_map::erase(cpps_value k)
	{
		_map.erase(k);
	}

	void cpps_unordered_map::begin()
	{
		_begin = _map.begin();
	}

	bool cpps_unordered_map::end()
	{
		return _begin != _map.end();
	}

	bool cpps_unordered_map::empty()
	{
		return _map.empty();
	}

	void cpps_unordered_map::next()
	{
		if (_begin != _map.end())
			++_begin;
	}

	cpps::cpps_value cpps_unordered_map::it()
	{
		cpps_value ret;
		if (_begin != _map.end())
		{
			ret = _begin->second;
		}
		return ret;
	}

	void cpps_unordered_map::pop()
	{
		_begin = _map.erase(_begin);
	}

	cpps::cpps_value cpps_unordered_map::key()
	{
		cpps_value ret;
		if (_begin != _map.end())
		{
			ret = _begin->first;
		}
		return ret;
	}

	void cpps_unordered_map::clear()
	{
		_map.clear();
	}

	cpps_integer cpps_unordered_map::size()
	{
		return (cpps_integer)_map.size();
	}

}

