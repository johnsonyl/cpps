#include "cpps/cpps.h"


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
				.def("merge", &cpps_map::merge)
				.def("begin", &cpps_map::begin)
				.def("has", &cpps_map::has)
				.def("end", &cpps_map::end)
				.def("next", &cpps_map::next)
				.def("it", &cpps_map::it)
				.def("key", &cpps_map::key)
				.def("empty", &cpps_map::empty)
				.def("clear", &cpps_map::clear)
				.def("size", &cpps_map::size)
				.def_inside("where", &cpps_map::where)
				.def_inside("select", &cpps_map::select)
		];
	}
	

	cpps_map::~cpps_map()
	{
		_map.clear();
	}

	void cpps_map::insert(cpps_value k, cpps_value v)
	{
		_map.insert(cpps_hash_map::value_type(k, v));
	}

	cpps::cpps_value cpps_map::find(cpps_value k)
	{
		cpps_value ret;
		cpps_hash_map::iterator it = _map.find(k);
		if (it != _map.end())
		{
			ret = it->second;
		}
		return ret;
	}

	cpps_value& cpps_map::cpps_find(const cpps_value & k)
	{
		cpps_hash_map::iterator it = _map.find(k);
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
		cpps_hash_map::iterator it = _map.find(k);
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

	void cpps_map::merge(cpps_value right)
	{
		if (!cpps_base_ismap(right)) return;

		cpps_map* rightmap = cpps_to_cpps_map(right);
		for (auto it : rightmap->realmap()) {
			(*this)[it.first] = it.second;
		}
	}

	phmap::flat_hash_map<cpps::cpps_value, cpps::cpps_value, cpps::cpps_value::hash>& cpps_map::realmap()
	{
		return _map;
	}

	void cpps_map::set(cpps_value key_val, cpps_value value)
	{
		cpps_hash_map::iterator it = _map.find(key_val);
		if (it != _map.end())
		{
			it->second = value;
		}
	}
	cpps_value cpps_map::where(C* c, object o)
	{
		cpps_map* vec;
		cpps_value ret = newclass(c, &vec);
		if (o.isfunction()) {
			for (auto v : realmap()) {
				bool b = object_cast<bool>(dofunction(c, o, v.first,v.second));
				if (b) vec->insert(v.first,v.second);
			}
		}
		return ret;
	}
	cpps_value cpps_map::select(C* c, object o)
	{
		cpps_map* vec;
		cpps_value ret = newclass(c, &vec);
		if (o.isfunction()) {
			for (auto v : realmap()) {
				cpps_value b = object_cast<cpps_value>(dofunction(c, o, v.first,v.second));
				vec->insert(v.first, b);
			}
		}
		return ret;
	}
}

