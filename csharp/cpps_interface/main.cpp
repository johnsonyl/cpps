#include <cpps/cpps.h> //Single Header

using namespace cpps;
namespace cpps { std::string cpps_io_getfilepath(std::string str); std::string cpps_rebuild_filepath(std::string path); std::string cpps_real_path(); }

#define ptr_cast(a) cpps_value _##a; _##a.tt = a->tt;_##a.value = a->value; _##a.incruse();


extern "C" {

	void* tcmalloc_func(size_t _size) {
		return malloc(_size);
	}
	void	tcfree_func(void* p) {
		free(p);
	}
	//basic
	cpps_export_type( cpps::C*) cpps_create()
	{
		return cpps::create(0,NULL, tcmalloc_func, tcfree_func);
	}

	cpps_export_type( void )cpps_close(cpps::C * c)
	{
		cpps::close(c);
	}

	cpps_export_type( void )cpps_dofile(cpps::C * c, char* path)
	{
		std::string path2 = cpps_rebuild_filepath(path);
		cpps_try
			cpps::dofile(c, path2.c_str());
		cpps_catch
	}
	//module
	cpps_export_type( cpps_domain* )cpps_defmodule(cpps::C* c, char* name)
	{
		cpps::cpps_init_cpps_class(c);
		cpps::cpps_module m = _module(c, name);
		return m.domain;
	}
	cpps_export_type( void )cpps_unmodule(cpps::C* c, char* name)
	{
		_unmodule(c, name);
	}


	cpps_export_type( void )cpps_module_def(cpps::C* c, cpps_domain* domain, char* name, cpps_func _f)
	{
		regxmodule r = def(name, _f);
		domain->regfunc(r.f, c);
	}
	//def class
	cpps_export_type( _class2* )cpps_def_class(cpps::C* c, cpps_domain* domain, char* name, cpps_class_alloc __alloc, cpps_class_free __free)
	{
		_class2 *r = new _class2(name, __alloc, __free);
		domain->regfunc(r->f, c);
		return r;
	}
	cpps_export_type( void )cpps_def_class_free(_class2 *_r)
	{
		delete _r;
	}
	cpps_export_type( void )cpps_class_def(cpps::C* c, _class2* r, char* name, cpps_class_func _f)
	{
		r->def_inside(name, _f);
	}
	cpps_export_type( void )cpps_class_def_operator(cpps::C* c, _class2* r, char* name, cpps_class_func _f)
	{
		r->def_operator_inside(name, _f);
	}
	//def enum
	cpps_export_type( _enum* )cpps_def_enum(cpps::C* c, cpps_domain* domain, char* name)
	{
		_enum* r = new _enum(c,name);
		domain->regfunc(r->f, c);
		return r;
	}
	cpps_export_type( void )cpps_def_enum_free(_enum * _r)
	{
		delete _r;
	}
	cpps_export_type( void )cpps_enum_def(cpps::C* c, _enum* r, char* name, cpps_integer i)
	{
		r->value(name, i);
	}
	//cpps_value api function
	cpps_export_type( bool )cpps_value_ispair(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_ispair(_v);
	}
	cpps_export_type( bool )cpps_value_isset(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isset(_v);
	}
	cpps_export_type( bool )cpps_value_ismap(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_ismap(_v);
	}
	cpps_export_type( bool )cpps_value_isbasevar(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isbasevar(_v);
	}
	cpps_export_type( bool )cpps_value_isstring(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isstring(_v);
	}
	cpps_export_type( bool )cpps_value_isvector(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isvector(_v);
	}
	cpps_export_type( bool )cpps_value_isrange(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isrange(_v);
	}
	cpps_export_type( bool )cpps_value_isint(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isint(_v);
	}
	cpps_export_type( bool )cpps_value_ischar(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_ischar(_v);
	}
	cpps_export_type( bool )cpps_value_isuint(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isuint(_v);
	}
	cpps_export_type( bool )cpps_value_isbool(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isbool(_v);
	}
	cpps_export_type( bool )cpps_value_isnumber(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isnumber(_v);
	}
	cpps_export_type( bool )cpps_value_isnull(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isnull(_v);
	}
	cpps_export_type( bool )cpps_value_isfunction(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isfunction(_v);
	}	
	cpps_export_type( bool )cpps_value_isclassvar(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isclassvar(_v);
	}
	cpps_export_type( bool )cpps_value_isclass(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isclass(_v);
	}
	cpps_export_type( bool )cpps_value_isellipsis(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_isellipsis(_v);
	}
	cpps_export_type( bool )cpps_value_istuple(cpps_value* v)
	{
		ptr_cast(v);
		return cpps_istuple(_v);
	}
	cpps_export_type( void )cpps_value_getclassname(cpps_value* v,char *_out)
	{
		ptr_cast(v);
		strcpy(_out,cpps_getclassname(_v).c_str());
	}
	cpps_export_type( void )cpps_value_global(C*c,cpps_value *out)
	{
		*out = cpps_value(c->_G);
	}
	cpps_export_type(void) cpps_value_get_by_name(cpps_value *obj,char *name,cpps_value *out)
	{
		*out = object(*obj)[name].getval();
	}
	cpps_export_type( void )cpps_value_get_by_idx(cpps_value *obj,int idx,cpps_value *out)
	{
		*out = object(*obj)[idx].getval();
	}
	cpps_export_type(void)cpps_value_incruse(cpps_value *obj)
	{
		obj->incruse();
	}
	cpps_export_type( void )cpps_value_decruse(cpps_value *obj)
	{
		obj->decruse();
	}
	cpps_export_type( void* )cpps_value_cast(cpps_value *obj)
	{
		ptr_cast(obj);
		if (!cpps_isclassvar(_obj)) return NULL;
		cpps_class_handler* handler = object_cast<cpps_class_handler*>(_obj);
		return handler->get_ptr();
	}
	cpps_export_type( void )cpps_value_real(cpps_value* obj, cpps_value* out)
	{
		ptr_cast(obj);
		*out = _obj.real();
	}
	cpps_export_type(void )cpps_value_ref(cpps_value* obj, cpps_value* out)
	{
		*out = obj->ref();
	}
	cpps_export_type( void )cpps_value_ref_assign(cpps_value* _ref, cpps_value *v)
	{
		ptr_cast(_ref);
		ptr_cast(v);
		object _ref_object = object(_ref);
		_ref_object = object(_v);
	}

	//cpps_value* cpps_value_create(const cpps_value& v) {
	//	cpps_value* ret = CPPSNEW( cpps_value)();
	//	ret->tt = v.tt;
	//	ret->value = v.value;
	//	return ret;
	//}
	//__declspec(dllexport) void cpps_value_release(cpps_value* v)
	//{
	//	CPPSDELETE( v);
	//}
	//vector
	cpps_export_type( void )cpps_vector_push_back(cpps_value* _vct,cpps_value* _v)
	{
		ptr_cast(_vct);
		ptr_cast(_v);
		cpps_vector* vct = cpps_to_cpps_vector(__vct);
		vct->push_back(__v);
	}
	cpps_export_type( int )cpps_vector_size(cpps_value* _vct)
	{
		ptr_cast(_vct);
		cpps_vector* vct = cpps_to_cpps_vector(+_vct);
		return (int)vct->size();
	}
	cpps_export_type( void )cpps_vector_erase(cpps_value* _vct,int idx)
	{
		ptr_cast(_vct);
		cpps_vector* vct = cpps_to_cpps_vector(+_vct);
		vct->erase(idx);
	}
	cpps_export_type( void )cpps_vector_at(cpps_value* _vct,int idx,cpps_value* out)
	{
		ptr_cast(_vct);
		cpps_vector* vct = cpps_to_cpps_vector(__vct);
		*out = vct->at(idx);
	}
	cpps_export_type( void )cpps_vector_clear(cpps_value* _vct)
	{
		ptr_cast(_vct);
		cpps_vector* vct = cpps_to_cpps_vector(__vct);
		vct->clear();
	}
	cpps_export_type( void )cpps_vector_create(C*c,cpps_value *out)
	{
		cpps_create_class_var(cpps_vector, c, vct_value, vct_ptr);
		*out = vct_value;
	}
	//map
	cpps_export_type( void )cpps_map_insert(cpps_value* _map, cpps_value* _key,cpps_value* _value)
	{
		ptr_cast(_map);
		ptr_cast(_key);
		ptr_cast(_value);
		cpps_map* m = cpps_to_cpps_map(__map);
		m->insert(__key, __value);
	}
	cpps_export_type( int )cpps_map_size(cpps_value* _map)
	{
		ptr_cast(_map);
		cpps_map* m = cpps_to_cpps_map(__map);
		return (int)m->size();
	}
	cpps_export_type( void )cpps_map_clear(cpps_value* _map)
	{
		ptr_cast(_map);
		cpps_map* m = cpps_to_cpps_map(__map);
		m->clear();
	}
	cpps_export_type( void )cpps_map_erase(cpps_value* _map,cpps_value* _key)
	{
		ptr_cast(_map);
		cpps_map* m = cpps_to_cpps_map(__map);
		m->erase(*_key);
	}
	cpps_export_type( void )cpps_map_find(cpps_value* _map,cpps_value* _key,cpps_value *out)
	{
		ptr_cast(_map);
		ptr_cast(_key);
		cpps_map* m = cpps_to_cpps_map(__map);
		*out = m->find(__key);
	}
	cpps_export_type( void )cpps_map_create(C*c, cpps_value* out)
	{
		cpps_create_class_var(cpps_map, c, map_value, map_ptr);
		*out = map_value;
	}
	//function
	cpps_export_type( void )cpps_function_call(C* c, cpps_value* func,cpps_value *ps,cpps_value *out)
	{
		ptr_cast(func);
		ptr_cast(ps);
		object ret = function_caller(c, _func, cpps_to_cpps_vector(_ps)->realvector());
		*out = ret.realval();
	}
	cpps_export_type( void )cpps_function_class_call(C* c, cpps_value* _class, cpps_value* func,cpps_value *ps,cpps_value *out)
	{
		ptr_cast(_class);
		ptr_cast(func);
		object ret = function_caller(c,__class, _func, cpps_to_cpps_vector(*ps)->realvector());
		*out = ret.realval();
	}
	//string

	cpps_export_type( int )cpps_string_size( cpps_value* _str)
	{
		ptr_cast(_str);
		std::string* s = cpps_get_string(__str);
		return (int)s->size();
	}
	cpps_export_type( void )cpps_string_append( cpps_value* _str, char* out, int count)
	{
		ptr_cast(_str);
		std::string* s = cpps_get_string(__str);
		s->append(out, (size_t)count);
	}
	cpps_export_type( void )cpps_string_strcopy( cpps_value* _str,char *out)
	{
		ptr_cast(_str);
		std::string* s = cpps_get_string(__str);
		memcpy(out, s->c_str(), s->size());
	}
	cpps_export_type( void )cpps_string_assignment(cpps_value* _str,char *out,int count)
	{
		ptr_cast(_str);
		std::string* s = cpps_get_string(__str);
		s->assign(out, (size_t)count);
	}
	cpps_export_type( void )cpps_string_create(C*c,cpps_value* _out)
	{
		*_out = cpps_value(c, "");
	}
	//classvar
	cpps_export_type( void )cpps_classvar_create(C* c, cpps_value* _class,cpps_value* _out)
	{
		ptr_cast(_class);
		if (!cpps_isclass(__class)) return;
		cpps_cppsclass* _cppsclass = cpps_to_cpps_cppsclass(*_class);
		newcppsclasvar(c, _cppsclass, _out);
	}
}

//cpps_export_finish	//unix export.