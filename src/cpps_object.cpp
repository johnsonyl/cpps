#include "cpps/cpps.h"

namespace cpps
{
	void cpps_base_printf(C* c, object b);
	void cpps_calculate_expression_quote_real(cpps_domain* left, cpps_value& src, cpps_value& tar, bool isconst);
	cpps::int32 type(object&& o) {
		return type(o);
	}
	cpps::int32 type(const object& o)
	{
		if (o.realval().tt == CPPS_TLAMBDAFUNCTION) return CPPS_TFUNCTION;
		return o.realval().tt;
	}

	std::string type_s(const object& o)
	{
		switch (type(o))
		{
		case CPPS_TNIL:
			return "nil";
		case CPPS_TINTEGER:
			return "integer";
		case CPPS_TUINTEGER:
			return "unsigned integer";
		case CPPS_TNUMBER:
			return "number";
		case CPPS_TBOOLEAN:
			return "boolean";
		case CPPS_TSTRING:
			return "string";
		case CPPS_TCLASSVAR: 
		{
			return o.realval().value.domain->getdomainname();
		}
		case CPPS_TFUNCTION:
			return "function";
		}
		return "unknow";
	}
	std::string type_s(object&& o)
	{
		return type_s(o);
	}
	void print(C*c,const object& o)
	{
		cpps_base_printf(c, o);
	}
	void println(C*c,const object& o)
	{
		print(c,o);
		printf("\n");
	}
//#define value.real() (value.tt == CPPS_TREF ? *(value.value.value) : value)

	cpps_integer object::size()
	{
		cpps_integer ret = 0;

		if (cpps_ismap(value.real())) {
			cpps_map* m = cpps_to_cpps_map(value.real());
			ret = m->size();
		}
		else if (cpps_isvector(value.real())) {
			cpps_vector* vct = cpps_to_cpps_vector(value.real());
			ret = vct->size();
		}
		else if (cpps_isset(value.real())) {
			cpps_set* vct = cpps_to_cpps_set(value.real());
			ret = vct->size();
		}
		else if (cpps_isstring(value.real())) {
			std::string* str = cpps_get_string(value.real());
			ret = (cpps_integer)str->size();
		}
		return ret;
	}

	bool object::ispair()
	{
		return cpps_ispair(value.real());
	}

	bool object::ismap()
	{
		return cpps_ismap(value.real());
	}

	bool object::isset()
	{
		return cpps_isset(value.real());
	}

	bool object::isstring()
	{
		return cpps_isstring(value.real());
	}

	bool object::isvector()
	{
		return cpps_isvector(value.real());
	}

	bool object::isrange()
	{
		return cpps_isrange(value.real());
	}

	bool object::istuple()
	{
		return cpps_istuple(value.real());
	}
	bool object::isellipsis()
	{
		return cpps_isellipsis(value.real());
	}

	bool object::isint()
	{
		return cpps_isint(value.real());
	}

	bool object::ischar()
	{
		return cpps_ischar(value.real());
	}

	bool object::isnumber()
	{
		return cpps_isnumber(value.real());
	}

	bool object::isnull()
	{
		return cpps_isnull(value.real());
	}

	bool object::isfunction()
	{
		return cpps_isfunction(value.real());
	}

	bool object::isclass()
	{
		return cpps_isclass(value.real());
	}

	bool object::isclassvar()
	{
		return cpps_isclassvar(value.real());
	}

	bool object::isref()
	{
		return value.isref();
	}
	std::string	object::getclassname() {

		return cpps_getclassname(value.real());
	}

	std::string object::tostring()
	{
		return object_cast<std::string>(*this);
	}

	cpps_integer object::toint()
	{
		return object_cast<cpps_integer>(*this);
	}
	cpps_uinteger object::touint()
	{
		return object_cast<cpps_uinteger>(*this);
	}

	char& object::tochar()
	{
		return *value.real().value.strv;
	}

	cpps_number object::tonumber()
	{
		return object_cast<cpps_number>(*this);
	}

	bool object::tobool()
	{
		return object_cast<bool>(*this);
	}


	object object::toreal()
	{
		return value.real();
	}
	cpps_value object::ref()
	{
		return value.real().ref();
	}

	cpps::cpps_value& object::realval()
	{
		return value.real();
	}

	const cpps::cpps_value& object::realval() const
	{
		return value.real();
	}

	void object::clear()
	{

		if (cpps_ismap(value.real())) {
			cpps_map* m = cpps_to_cpps_map(value.real());
			m->clear();
		}
		else if (cpps_isvector(value.real())) {
			cpps_vector* vct = cpps_to_cpps_vector(value.real());
			vct->clear();
		}
		else if (cpps_isstring(value.real())) {
			std::string* str = cpps_get_string(value.real());
			str->clear();
		}
	}

	bool object::empty()
	{

		if (cpps_ismap(value.real())) {
			cpps_map* m = cpps_to_cpps_map(value.real());
			return m->empty();
		}
		else if (cpps_isvector(value.real())) {
			cpps_vector* vct = cpps_to_cpps_vector(value.real());
			return vct->empty();
		}
		else if (cpps_isstring(value.real())) {
			std::string* str = cpps_get_string(value.real());
			return str->empty();
		}
		return true;
	}

	void object::insert(object key, object val)
	{
	if (cpps_ismap(value.real())) {
			cpps_map* m = cpps_to_cpps_map(value.real());
			m->insert(key.value, val.value);
		}
	}


	
	void object::push_back(object& val)
	{
		if (cpps_isvector(value.real())) {
			cpps_vector* vct = cpps_to_cpps_vector(value.real());
			vct->push_back(val.value);
		}
	}

	cpps::object object::real(object& o)
	{
		return o.toreal();
	}

	object::object(const cpps_value& v) :value(v)
	{
		
	}

	object::object()
	{

	}

	object::object(const object& k)
	{
		if (k.value.tt == CPPS_TREF)
			value = *k.value.value.value;
		else
			value = k.value;
	}

	void object::define(C* c, std::string varname, object val /*= object()*/)
	{
		cpps_domain* leftdomain = NULL;
		cpps_regvar* v = c->_G->getvar(varname, leftdomain, false);
		if (!v) {
			v = CPPSNEW (cpps_regvar)();
			v->setvarname(varname);
			c->_G->regvar(c, v);
		}
		if(!v->isconst())
			v->setval(val.value);
	}

	
	object& object::operator=(const object& k)
	{
		if (value.tt == CPPS_TREF) {
			if (k.value.tt == CPPS_TREF)
				*value.value.value = *k.value.value.value;
			else
				*value.value.value = k.value;
		}
		else {
			if (k.value.tt == CPPS_TREF)
				value = *k.value.value.value;
			else
				value = k.value;
		}
		return *this;
	}


	object object::globals(C* c)
	{
		return object(cpps_value(c->_G));
	}

	object		object::operator[](const std::string k)
	{

		cpps_value ret;
		if (value.real().isdomain()) {

			if (cpps_ismap(value.real())) {
				cpps_map* m = cpps_to_cpps_map(value.real());
				cpps_value key = cpps_new_tmp_string(k);
				ret = m->find(key);
				cpps_delete_tmp_string(key);
			}
			else {
				cpps_domain* leftdomain = NULL;
				cpps_regvar* var = value.real().value.domain->getvar(k, leftdomain, true,true);
				if (var) {
					cpps_calculate_expression_quote_real(value.real().value.domain, var->getval(), ret, false);
				}
			}
		}
		return static_cast<object>(ret);
	}
	object		object::operator[](const cpps_integer k)
	{

		cpps_value ret;
		if (value.real().isdomain()) {

			if (cpps_isvector(value.real())) {
				cpps_vector* vct = cpps_to_cpps_vector(value.real());
				ret = vct->at(k);
			}
			else if (cpps_ismap(value.real())) {
				cpps_map* m = cpps_to_cpps_map(value.real());
				ret = m->find(cpps_value(k));
			}
		}
		return static_cast<object>(ret);
	}
	
	cpps::cpps_value& object::getval()
	{
		return value;
	}

	cpps::object& object::operator=(const cpps_value k)
	{
		value = k;
		return *this;
	}
	object object::create_with_pair(C* c)
	{
		cpps_create_class_var(cpps_pair, c, cpps_value_map, cpps_map_ptr);
		return  static_cast<object>(cpps_value_map);
	}

	object object::create_with_map(C* c)
	{
		cpps_create_class_var(cpps_map, c, cpps_value_map, cpps_map_ptr);
		return  static_cast<object>(cpps_value_map);
	}
	object object::create_with_set(C* c)
	{
		cpps_create_class_var(cpps_set, c, cpps_value_map, cpps_map_ptr);
		return  static_cast<object>(cpps_value_map);
	}
	object object::create_with_vector(C* c)
	{
		cpps_create_class_var(cpps_vector, c, cpps_value_vector, cpps_vector_ptr);
		return  static_cast<object>(cpps_value_vector);
	}

	object object::create_with_cppsclassvar(C* c, object __classobject)
	{
		cpps::cpps_cppsclass* cppsclass = cpps_to_cpps_cppsclass(__classobject.value);
		if (cppsclass->iscppsclass())
		{
			object ret;
			newcppsclasvar(c, cppsclass,&ret.value);
			return ret;
		}
		return nil;
	}

	object::vector::vector(object& obj)
	{
		_vec = cpps_to_cpps_vector(obj.realval());
		_src_value = obj.value;
	}

	cpps_std_vector::iterator object::vector::begin()
	{
		return _vec->realvector().begin();
	}

	cpps_std_vector::iterator object::vector::end()
	{
		return _vec->realvector().end();
	}

	void object::vector::push_back(object v)
	{
		_vec->push_back(object::real(v).value);
	}

	void object::vector::erase(cpps_integer idx)
	{
		_vec->erase(idx);
	}

	size_t object::vector::size()
	{
		return realvector().size();
	}

	cpps::object object::vector::toobject()
	{
		return _src_value;
	}

	cpps::object object::vector::operator[](const cpps_integer k)
	{
		cpps_value& v = _vec->realvector()[size_t(k)];
		return cpps_value(&v);
	}

	cpps::cpps_std_vector& object::vector::realvector()
	{
		return _vec->realvector();
	}

	object::vector object::vector::create(C* c)
	{
		auto obj = object::create_with_vector(c);
		return object::vector(obj);
	}

	object::map::map(C* cstate, object& obj)
	{
		_map = cpps_to_cpps_map(obj.realval());
		c = cstate;
		_src_value = obj.value;
	}

	cpps_hash_map::iterator object::map::begin()
	{
		return _map->realmap().begin();
	}

	cpps_hash_map::iterator object::map::end()
	{
		return _map->realmap().end();
	}

	


	void object::map::insert(const object& key, const object& value)
	{
		_map->realmap().insert(cpps_hash_map::value_type(key.realval(), value.realval()));
	}

	cpps::object object::map::toobject()
	{
		return _src_value;
	}

	size_t object::map::size()
	{
		return realmap().size();
	}

	cpps::cpps_hash_map& object::map::realmap()
	{
		return _map->realmap();
	}

	cpps::object::map object::map::create(C* c)
	{
		auto obj = cpps::object::create_with_map(c);
		return cpps::object::map(c,obj);
	}

	object::set object::set::create(C* c)
	{
		auto obj = cpps::object::create_with_set(c);
		return cpps::object::set(c, obj);

	}

	object::set::set(C* cstate, object& obj)
	{
		_set = cpps_to_cpps_set(obj.realval());
		c = cstate;
		_src_value = obj.value;
	}

	cpps::cpps_hash_set::iterator object::set::begin()
	{
		return _set->realset().begin();
	}

	cpps::cpps_hash_set::iterator object::set::end()
	{
		return _set->realset().end();
	}

	void object::set::insert(object& key)
	{
		_set->insert(key.realval());
	}

	cpps::object object::set::toobject()
	{
		return _src_value;
	}

	size_t object::set::size()
	{
		return realset().size();
	}

	cpps::cpps_hash_set& object::set::realset()
	{
		return _set->realset();
	}

	object::pair object::pair::create(C* c, object& first, object &second)
	{
		auto ret = cpps::object::pair(c, cpps::object::create_with_pair(c));
		ret._pair->_first = first.getval();
		ret._pair->_second = second.getval();
		return ret;
	}
	object::pair object::pair::create(C* c, object&& first, object&& second)
	{
		return object::pair::create(c, first, second);
	}

	object::pair::pair(C* cstate, object obj)
	{
		_pair = cpps_to_cpps_pair(obj.value);
		c = cstate;
		_src_value = obj.value;
	}

	cpps::object object::pair::first()
	{
		return _pair->first();
	}

	cpps::object object::pair::second()
	{
		return _pair->second();
	}

	cpps::object object::pair::toobject()
	{
		return _src_value;
	}

}
