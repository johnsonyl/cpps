#include "cpps/cpps.h"

namespace cpps
{


	cpps::int32 type(object o)
	{
		if (object::real(o).value.tt == CPPS_TLAMBDAFUNCTION) return CPPS_TFUNCTION;
		return object::real(o).value.tt;
	}

	std::string type_s(object o)
	{
		switch (type(o))
		{
		case CPPS_TNIL:
			return "nil";
		case CPPS_TINTEGER:
			return "integer";
		case CPPS_TNUMBER:
			return "number";
		case CPPS_TBOOLEAN:
			return "boolean";
		case CPPS_TSTRING:
			return "string";
		case CPPS_TCLASSVAR: 
		{
			return object::real(o).value.value.domain->domainname; 
		}
		case CPPS_TFUNCTION:
			return "function";
		}
		return "unknow";
	}
#define SAFE_VALUE (value.tt == CPPS_TREGVAR ? *(value.value.value) : value)

	cpps_integer object::size()
	{
		cpps_integer ret = 0;

		if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			ret = m->size();
		}
		else if (cpps_base_isvector(SAFE_VALUE)) {
			cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
			ret = vct->size();
		}
		else if (cpps_base_isstring(SAFE_VALUE)) {
			std::string* str = cpps_get_string(SAFE_VALUE);
			ret = (cpps_integer)str->size();
		}
		return ret;
	}

	bool object::ismap()
	{
		return cpps_base_ismap(SAFE_VALUE);
	}

	bool object::isstring()
	{
		return cpps_base_isstring(SAFE_VALUE);
	}

	bool object::isvector()
	{
		return cpps_base_isvector(SAFE_VALUE);
	}

	bool object::isint()
	{
		return cpps_base_isint(SAFE_VALUE);
	}

	bool object::isnumber()
	{
		return cpps_base_isnumber(SAFE_VALUE);
	}

	bool object::isnull()
	{
		return cpps_base_isnull(SAFE_VALUE);
	}

	bool object::isfunction()
	{
		return cpps_base_isfunction(SAFE_VALUE);
	}

	bool object::isclass()
	{
		return cpps_base_isclass(SAFE_VALUE);
	}

	bool object::isclassvar()
	{
		return cpps_base_isclassvar(SAFE_VALUE);
	}

	std::string object::tostring()
	{
		return object_cast<std::string>(*this);
	}

	cpps_integer object::toint()
	{
		return object_cast<cpps_integer>(*this);
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
		return SAFE_VALUE;
	}

	void object::clear()
	{

		if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			m->clear();
		}
		else if (cpps_base_isvector(SAFE_VALUE)) {
			cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
			vct->clear();
		}
		else if (cpps_base_isstring(SAFE_VALUE)) {
			std::string* str = cpps_get_string(SAFE_VALUE);
			str->clear();
		}
	}

	bool object::empty()
	{

		if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			return m->empty();
		}
		else if (cpps_base_isvector(SAFE_VALUE)) {
			cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
			return vct->empty();
		}
		else if (cpps_base_isstring(SAFE_VALUE)) {
			std::string* str = cpps_get_string(SAFE_VALUE);
			return str->empty();
		}
		return true;
	}

	void object::insert(object key, object val)
	{
	if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			m->insert(key.value, val.value);
		}
	}


	void object::set(object key, object val)
	{
		if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			cpps_value& takeval = m->cpps_find(key.value);
			takeval = val.value;;
		}
	}

	void object::set(std::string key, object val)
	{
		if (cpps_base_ismap(SAFE_VALUE)) {
			cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
			cpps_value key_val = cpps_new_tmp_string(key);
			m->set(key_val, val.value);
			cpps_delete_tmp_string(key_val);
		}
		else if (value.isdomain()) {
			cpps_domain* leftdomain = NULL;
			cpps_regvar* var = SAFE_VALUE.value.domain->getvar(key, leftdomain, true, true);
			if (var) {
				var->setval(val.value);
			}
		}
	}

	void object::set(cpps_integer key, object val)
	{
		if (SAFE_VALUE.isdomain()) {
			if (cpps_base_isvector(SAFE_VALUE)) {
				cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
				vct->realvector()[(size_t)key] = val.value;
			}
			else if (cpps_base_ismap(SAFE_VALUE)) {
				cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
				cpps_value key_val(key);
				cpps_value& takeval = m->cpps_find(key_val);
				takeval = val.value;
			}
		}
	}

	void object::push_back(object& val)
	{
		if (cpps_base_isvector(SAFE_VALUE)) {
			cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
			vct->push_back(val.value);
		}
	}

	cpps::object object::real(object o)
	{
		return o.toreal();
	}

	object::object(cpps_value v) :value(v)
	{
		
	}

	object::object()
	{

	}

	object::object(const object& k)
	{
		if (k.value.tt == CPPS_TREGVAR)
			value = *k.value.value.value;
		else
			value = k.value;
	}

	void object::define(C* c, std::string varname, object val /*= object()*/)
	{
		cpps_domain* leftdomain = NULL;
		cpps_regvar* v = c->_G->getvar(varname, leftdomain, false);
		if (!v) {
			v = new cpps_regvar();
			v->setvarname(varname);
			c->_G->regvar(c, v);
		}
		if(!v->isconst())
			v->setval(val.value);
	}

	object& object::operator=(const object& k)
	{
		if (value.tt == CPPS_TREGVAR) {
			if (k.value.tt == CPPS_TREGVAR)
				*value.value.value = *k.value.value.value;
			else
				*value.value.value = k.value;
		}
		else {
			if (k.value.tt == CPPS_TREGVAR)
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
		if (SAFE_VALUE.isdomain()) {

			if (cpps_base_ismap(SAFE_VALUE)) {
				cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
				cpps_value key = cpps_new_tmp_string(k);
				ret = m->find(key);
				cpps_delete_tmp_string(key);
			}
			else {
				cpps_domain* leftdomain = NULL;
				cpps_regvar* var = SAFE_VALUE.value.domain->getvar(k, leftdomain, true,true);
				if (var) {
					ret = cpps_value(&var->getval());
				}
			}
		}
		return static_cast<object>(ret);
	}
	object		object::operator[](const cpps_integer k)
	{

		cpps_value ret;
		if (SAFE_VALUE.isdomain()) {

			if (cpps_base_isvector(SAFE_VALUE)) {
				cpps_vector* vct = cpps_to_cpps_vector(SAFE_VALUE);
				ret = vct->at(k);
			}
			else if (cpps_base_ismap(SAFE_VALUE)) {
				cpps_map* m = cpps_to_cpps_map(SAFE_VALUE);
				ret = m->find(cpps_value(k));
			}
		}
		return static_cast<object>(ret);
	}
	
	cpps::object& object::operator=(const cpps_value k)
	{
		value = k;
		return *this;
	}

	object object::create_with_map(C* c)
	{
		cpps_create_class_var(cpps_map, c, cpps_value_map, cpps_map_ptr);
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
		if(cppsclass->iscppsclass())
			return newcppsclasvar(c, cppsclass);
		return nil;
	}

	object::vector::vector(object obj)
	{
		_vec = cpps_to_cpps_vector(obj.value);
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

	cpps::object object::vector::toobject()
	{
		return _src_value;
	}

	cpps::object object::vector::operator[](const cpps_integer k)
	{
		cpps_value& v = _vec->realvector()[k];
		return cpps_value(&v);
	}

	cpps::cpps_std_vector& object::vector::realvector()
	{
		return _vec->realvector();
	}

	object::vector object::vector::create(C* c)
	{
		return object::vector(object::create_with_vector(c));
	}

	object::map::map(C* cstate, object obj)
	{
		_map = cpps_to_cpps_map(obj.value);
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

	


	void object::map::insert(object key, object value)
	{
		_map->insert(object::real(key).value, object::real(value).value);
	}

	cpps::object object::map::toobject()
	{
		return _src_value;
	}

	cpps::cpps_hash_map& object::map::realmap()
	{
		return _map->realmap();
	}

	cpps::object::map object::map::create(C* c)
	{
		return cpps::object::map(c,cpps::object::create_with_map(c));
	}

}
