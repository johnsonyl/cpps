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
				.def("has", &cpps_map::has)
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
				.def("has", &cpps_unordered_map::has)
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

	bool cpps_unordered_map::has(cpps_value k)
	{
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator it = _map.find(k);
		if (it != _map.end())
		{
			return true;
		}
		return false;
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

	std::unordered_map<cpps::cpps_value, cpps::cpps_value, cpps::cpps_value::hash>& cpps_unordered_map::realmap()
	{
		return _map;
	}

	void cpps_unordered_map::set(cpps_value key_val, cpps_value value)
	{
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator it = _map.find(key_val);
		if (it != _map.end())
		{
			it->second = value;
		}
	}

	cpps_map::~cpps_map()
	{
		_map.clear();
	}

	void cpps_map::insert(cpps_value k, cpps_value v)
	{
		_map.insert(std::map<cpps_value, cpps_value>::value_type(k, v));
	}

	cpps::cpps_value cpps_map::find(cpps_value k)
	{
		cpps_value ret;
		std::map<cpps_value, cpps_value>::iterator it = _map.find(k);
		if (it != _map.end())
		{
			ret = it->second;
		}
		return ret;
	}

	cpps::cpps_value& cpps_map::cpps_find(cpps_value k)
	{
		return _map[k];
	}

	void cpps_map::erase(cpps_value k)
	{
		_map.erase(k);
	}

	void cpps_map::begin()
	{
		_begin = _map.begin();
	}

	bool cpps_map::has(cpps_value k)
	{
		std::map<cpps_value, cpps_value>::iterator it = _map.find(k);
		if (it != _map.end())
		{
			return true;
		}
		return false;
	}

	bool cpps_map::end()
	{
		return _begin != _map.end();
	}

	bool cpps_map::empty()
	{
		return _map.empty();
	}

	void cpps_map::next()
	{
		if (_begin != _map.end())
			++_begin;
	}

	cpps::cpps_value cpps_map::it()
	{
		cpps_value ret;
		if (_begin != _map.end())
		{
			ret = _begin->second;
		}
		return ret;
	}

	void cpps_map::pop()
	{
		_begin = _map.erase(_begin);
	}

	cpps::cpps_value cpps_map::key()
	{
		cpps_value ret;
		if (_begin != _map.end())
		{
			ret = _begin->first;
		}
		return ret;
	}

	void cpps_map::clear()
	{
		_map.clear();
	}

	cpps_integer cpps_map::size()
	{
		return (cpps_integer)_map.size();
	}

	std::map<cpps::cpps_value, cpps::cpps_value>& cpps_map::realmap()
	{
		return _map;
	}

	void cpps_map::set(cpps_value key_val, cpps_value value)
	{
		std::map<cpps_value, cpps_value>::iterator it = _map.find(key_val);
		if (it != _map.end())
		{
			it->second = value;
		}
	}

}

