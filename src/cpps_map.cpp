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
				.def_inside("select", &cpps_map::select),
			_class<cpps_set>("set")
				.def("constructor", &cpps_set::constructor)
				.def("insert", &cpps_set::insert)
				.def("add", &cpps_set::insert)
				.def("erase", &cpps_set::erase)
				.def("pop", &cpps_set::pop)
				.def("merge", &cpps_set::merge)
				.def("begin", &cpps_set::begin)
				.def("has", &cpps_set::has)
				.def("find", &cpps_set::has)
				.def("end", &cpps_set::end)
				.def("next", &cpps_set::next)
				.def("it", &cpps_set::it)
				.def("empty", &cpps_set::empty)
				.def("clear", &cpps_set::clear)
				.def("size", &cpps_set::size)
				.def_operator("[]", &cpps_set::has)
				.def_inside("where", &cpps_set::where)
				.def_inside("select", &cpps_set::select)
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

	cpps_set::~cpps_set()
	{
		_set.clear();
	}

	void cpps_set::constructor(object list)
	{
		if (list.isnull()) return;

		if(list.isvector())
		{
			for (auto it : object::vector(list)) {
				insert(it);
			}
			return;
		}

		insert(list.getval());
	}

	void cpps_set::insert(cpps_value k)
	{
		_set.insert(k);
	}

	void cpps_set::erase(cpps_value k)
	{
		_set.erase(k);
	}

	void cpps_set::begin()
	{
		_begin = _set.begin();
	}

	bool cpps_set::has(cpps_value k)
	{
		return _set.find(k) != _set.end();
	}

	bool cpps_set::end()
	{
		return _begin != _set.end();
	}

	bool cpps_set::empty()
	{
		return _set.empty();
	}

	void cpps_set::next()
	{
		++_begin;
	}

	cpps::cpps_value cpps_set::it()
	{
		return *_begin;
	}

	void cpps_set::pop()
	{
		_begin = _set.erase(_begin);
	}

	void cpps_set::clear()
	{
		_set.clear();
	}

	cpps_integer cpps_set::size()
	{
		return (cpps_integer)_set.size();
	}

	void cpps_set::merge(cpps_value right)
	{
		if (!cpps_base_issetable(right)) return;

		cpps_set* rightset = cpps_to_cpps_setable(right);
		for (auto it : rightset->realset()) {
			insert(it);
		}
	}


	cpps::cpps_value cpps_set::where(C* c, object o)
	{
		cpps_set* vec;
		cpps_value ret = newclass(c, &vec);
		if (o.isfunction()) {
			for (auto v : realset()) {
				bool b = object_cast<bool>(dofunction(c, o, v));
				if (b) vec->insert(v);
			}
		}
		return ret;
	}
	cpps::cpps_value cpps_set::select(C* c, object o)
	{
		cpps_set* vec;
		cpps_value ret = newclass(c, &vec);
		if (o.isfunction()) {
			for (auto v : realset()) {
				cpps_value b = object_cast<cpps_value>(dofunction(c, o, v));
				vec->insert(b);
			}
		}
		return ret;
	}

	cpps::cpps_hash_set& cpps_set::realset()
	{
		return _set;
	}

}

