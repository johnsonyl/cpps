#include "cpps/cpps.h"



namespace cpps
{



	void cpps_regarray(C *c)
	{
		cpps::_module(c)[
			_class<cpps_std_vector::iterator>("vector_iterator"),
			_class<cpps_vector>("vector")
				.def("constructor",&cpps_vector::constructor)
				.def("push_back", &cpps_vector::push_back)
				.def("add", &cpps_vector::push_back)
				.def("append", &cpps_vector::push_back)
				.def("assign", &cpps_vector::assign)
				.def("extend", &cpps_vector::extend)
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
				.def("indexof", &cpps_vector::indexof)
				.def_operator_inside("*",&cpps_vector::multiplication)
				.def_operator_inside("&",&cpps_vector::andfunc)
				.def_operator_inside("+",&cpps_vector::andfunc)
				.def_operator("+=",&cpps_vector::extend)
				.def_operator("-=",&cpps_vector::remove_vec)
				.def_inside("reverse", &cpps_vector::reverse)
				.def_inside("where", &cpps_vector::where)
				.def_inside("remove", &cpps_vector::remove)
				.def_inside("select", &cpps_vector::select)
				.def_inside("orderby", &cpps_vector::orderby)
				.def("swap", &cpps_vector::swap)
				.def("shrink_to_fit", &cpps_vector::shrink_to_fit)
		];
	}
	void	cpps_vector::constructor(object v) {
		if (v.isrange()) {
			cpps_range* range = object_cast<cpps_range*>(v);
			for (cpps_integer i = range->begin;i < range->end; i += range->inc)
			{
				realvector().push_back(cpps_value(i));
			}
		}
		else if (v.isvector())
		{
			auto vec = object::vector(v);
			realvector().reserve(vec.realvector().size());
			for (auto&& vv : vec) {
				realvector().emplace_back(vv);
			}
		}
		else if (v.isset())
		{
			auto vec = object::set(NULL,v);
			realvector().reserve(vec.realset().size());
			for (auto&& vv : vec) {
				realvector().emplace_back(vv);
			}
		}
	}
	cpps_vector::cpps_vector()
	{
		_begin = _vec.begin();
	}

	cpps_vector::~cpps_vector()
	{
		_vec.clear();
	}

	void cpps_vector::emplace_back(object v)
	{
		_vec.emplace_back(v.realval());
	}

	void cpps_vector::push_back(object v)
	{
		_vec.emplace_back(v.realval());
	}

	void cpps_vector::assign(cpps_vector* v)
	{
		_vec.assign(v->realvector().begin(), v->realvector().end());
	}

	void cpps_vector::extend(cpps_vector* v)
	{
		_vec.insert(_vec.end(),v->realvector().begin(), v->realvector().end());
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
		_vec.insert(_vec.begin() + (size_t)idx, v);
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
		_vec.erase(_vec.begin() + (size_t)idx);
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
	cpps_value cpps_vector::where(C*c,object o)
	{
		cpps_vector* vec;
		cpps_value ret;
		newclass(c, &vec,&ret);
		if (o.isfunction()) {
			for (auto& v : realvector()) {
				bool b = object_cast<bool>(dofunction(c, o, v));
				if (b) vec->push_back(v);
			}
		}
		return ret;
	}
	void cpps_vector::remove(C* c, object o)
	{
		if (o.isfunction()) {
			auto it = realvector().begin();
			for (;it != realvector().end();) 
			{
				bool b = object_cast<bool>(dofunction(c, o, *it));
				if (b)
					it = realvector().erase(it);
				else
					++it;
			}
		}
	}
	cpps_value cpps_vector::reverse(C* c)
	{
		cpps_newclass(c, cpps_vector, vec, cpps_value, ret);
		vec->realvector() = realvector();
		std::reverse(vec->realvector().begin(), vec->realvector().end());
		return ret;
	}
	cpps_value cpps_vector::select(C* c, object o)
	{
		cpps_vector* vec;
		cpps_value ret;
		newclass(c, &vec,&ret);
		if (o.isfunction()) {
			for (auto& v : realvector()) {
				cpps_value b = object_cast<cpps_value>(dofunction(c, o, v));
				vec->push_back(b);
			}
		}
		return ret;
	}

	cpps_value cpps_vector:: andfunc(C* c, object o) {

		cpps_value ret;
		if (o.isvector())
		{
			cpps_vector* copyvct;
			newclass(c, &copyvct,&ret);

			cpps_vector* right = cpps_to_cpps_vector(o.getval());
			size_t newsize = right->realvector().size() + realvector().size();

			copyvct->realvector().reserve(newsize);
			for (auto&& v : realvector()) {
				copyvct->realvector().emplace_back(v);
			}
			for (auto&& v : right->realvector()) {
				copyvct->realvector().emplace_back(v);
			}
		}
		return ret;
	}
	cpps_value cpps_vector::multiplication(C* c, object o)
	{
		cpps_value ret;
		if (o.isint()) {
			cpps_vector* copyvct;
			newclass(c, &copyvct,&ret);
			size_t on = (size_t)o.toint();
			copyvct->realvector().reserve(realvector().size() * on);

			for (size_t i = 0; i < on; i++) {
				for (auto&& v : realvector()) {
					copyvct->realvector().emplace_back(v);
				}
			}
		}
		return ret;
	}

	cpps_integer cpps_vector::indexof(cpps_value o)
	{
		cpps_integer i = 0;
		for (auto& v : realvector()) {
			if (v == o) return i;
			i++;
		}
		return -1;
	}

	void cpps_vector::swap(cpps_vector* v)
	{
		realvector().swap(v->realvector());
	}

	void cpps_vector::shrink_to_fit()
	{
		realvector().shrink_to_fit();
	}

	std::vector<cpps::cpps_value>& cpps_vector::realvector()
	{
		return _vec;
	}

	void cpps_vector::remove_vec(cpps_vector* v)
	{
		auto it = realvector().begin();
		for (; it != realvector().end();)
		{
			cpps_integer b = v->indexof(*it);
			if (b != -1)
				it = realvector().erase(it);
			else
				++it;
		}
	}
	cpps_value cpps_vector::orderby_call(C* c, object& func, cpps_value& v) {
		return dofunction(c, func, v).realval();
	}
	size_t cpps_vector::partition(C* c, cpps_std_vector& v, size_t begin, size_t end, object& func)
	{
		cpps_value pivot = orderby_call(c,func,v[begin]);
		size_t left = begin + 1;
		size_t right = end;
		while (true)
		{
			while (left < right && orderby_call(c,func,v[right]) >= pivot)
				right--;
			while (left < right && orderby_call(c, func, v[left]) < pivot)
				left++;
			if (left == right)
				break;
			std::swap(v[left], v[right]);
		}
		if (orderby_call(c, func, v[left]) >= pivot)
			return begin;

		v[begin] = v[left];
		v[left] = pivot;
		return left;
	}
	void cpps_vector::quick_sort(C* c, cpps_std_vector& v, size_t begin, size_t end, object& func)
	{
		if (begin >= end)
			return;
		size_t boundary = partition(c,v, begin, end,func);
		quick_sort(c,v, begin, boundary - 1, func);
		quick_sort(c,v, boundary + 1, end, func);
	}
	cpps_vector* cpps_vector::orderby(C* c, object o)
	{
		quick_sort(c, realvector(), 0, realvector().size() - 1,o);
		return this;
	}

}