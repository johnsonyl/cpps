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

	cpps_vector::cpps_vector()
	{
		_begin = _vec.begin();
	}

	cpps_vector::~cpps_vector()
	{
		_vec.clear();
	}

	void cpps_vector::push_back(cpps_value v)
	{
		_vec.push_back(v);
	}

	void cpps_vector::assign(cpps_vector* v)
	{
		_vec.assign(v->realvector().begin(), v->realvector().end());
	}

	void cpps_vector::pop_back()
	{
		_vec.pop_back();
	}

	void cpps_vector::push_front(cpps_value v)
	{
		_vec.insert(_vec.begin(), v);
	}

	void cpps_vector::pop_front()
	{
		_vec.erase(_vec.begin());
	}

	void cpps_vector::insert(cpps_integer idx, cpps_value v)
	{
		_vec.insert(_vec.begin() + idx, v);
	}

	cpps::cpps_value cpps_vector::at(cpps_integer idx)
	{
		if (idx < 0 || idx >= size())
		{
			throw(cpps_error("0", 0, 0, "Vector is out of bounds!"));
			return cpps::nil;
		}
		return _vec[((size_t)idx)];
	}

	cpps::cpps_value& cpps_vector::cpps_at(cpps_integer idx)
	{
		if (idx < 0 || idx >= size())
		{
			throw(cpps_error("0", 0, 0, "Vector is out of bounds!"));
			return cpps::nil;
		}
		return _vec[(size_t)idx];
	}

	void cpps_vector::erase(cpps_integer idx)
	{
		_vec.erase(_vec.begin() + idx);
	}

	void cpps_vector::pop()
	{
		_begin = _vec.erase(_begin);
	}

	void cpps_vector::begin()
	{
		_begin = _vec.begin();
	}

	bool cpps_vector::end()
	{
		return _begin != _vec.end();
	}

	bool cpps_vector::empty()
	{
		return _vec.empty();
	}

	void cpps_vector::next()
	{
		if (_begin != _vec.end())
			++_begin;
	}

	cpps::cpps_value cpps_vector::it()
	{
		cpps_value ret;
		if (_begin != _vec.end())
		{
			ret = *_begin;
		}
		return ret;
	}

	void cpps_vector::clear()
	{
		_vec.clear();
	}

	bool cpps_vector::has(cpps_value v)
	{
		for (auto& v2 : _vec)
		{
			if (v2 == v) return true;
		}
		return false;
	}

	cpps_integer cpps_vector::size()
	{
		return (cpps_integer)_vec.size();
	}

	void cpps_vector::resize(cpps_integer s)
	{
		_vec.resize((size_t)s);
	}

	std::vector<cpps::cpps_value>& cpps_vector::realvector()
	{
		return _vec;
	}

}