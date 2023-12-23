#include <cpps/cpps.h> //Single Header

using namespace cpps;
namespace cpps { std::string cpps_io_getfilepath(std::string str); std::string cpps_rebuild_filepath(std::string path); std::string cpps_real_path(); }

class csharp_class
{

};

extern "C" {

	void* tcmalloc_func(size_t _size) {
		return malloc(_size);
	}
	void	tcfree_func(void* p) {
		free(p);
	}
	//basic
	__declspec(dllexport) cpps::C* cpps_create()
	{
		return cpps::create(0,NULL, tcmalloc_func, tcfree_func);
	}

	__declspec(dllexport) void cpps_close(cpps::C * c)
	{
		cpps::close(c);
	}

	__declspec(dllexport) void cpps_dofile(cpps::C * c, char* path)
	{
		std::string path2 = cpps_rebuild_filepath(path);
		cpps_try
			cpps::dofile(c, path2.c_str());
		cpps_catch
	}
	//module
	__declspec(dllexport) cpps_domain* cpps_defmodule(cpps::C* c, char* name)
	{
		cpps::cpps_module m = _module(c, name);
		return m.domain;
	}


	__declspec(dllexport) void cpps_module_def(cpps::C* c, cpps_domain* domain, char* name, cpps_func _f)
	{
		regxmodule r = def(name, _f);
		domain->regfunc(r.f, c);
	}
	//def class
	__declspec(dllexport) _class2* cpps_def_class(cpps::C* c, cpps_domain* domain, char* name, cpps_class_alloc __alloc, cpps_class_free __free)
	{
		_class2 *r = new _class2(name, __alloc, __free);
		domain->regfunc(r->f, c);
		return r;
	}
	__declspec(dllexport) void cpps_def_class_free(_class2 *_r)
	{
		delete _r;
	}
	__declspec(dllexport) void cpps_class_def(cpps::C* c, _class2* r, char* name, cpps_class_func _f)
	{
		r->def_inside(name, _f);
	}
	__declspec(dllexport) void cpps_class_def_operator(cpps::C* c, _class2* r, char* name, cpps_class_func _f)
	{
		r->def_operator_inside(name, _f);
	}
	//def enum
	__declspec(dllexport) _enum* cpps_def_enum(cpps::C* c, cpps_domain* domain, char* name)
	{
		_enum* r = new _enum(c,name);
		domain->regfunc(r->f, c);
		return r;
	}
	__declspec(dllexport) void cpps_def_enum_free(_enum * _r)
	{
		delete _r;
	}
	__declspec(dllexport) void cpps_enum_def(cpps::C* c, _enum* r, char* name, cpps_integer i)
	{
		r->value(name, i);
	}
	//cpps_value api function
	__declspec(dllexport) bool cpps_value_ispair(cpps_value* v)
	{
		return cpps_ispair(*v);
	}
	__declspec(dllexport) bool cpps_value_isset(cpps_value* v)
	{
		return cpps_isset(*v);
	}
	__declspec(dllexport) bool cpps_value_ismap(cpps_value* v)
	{
		return cpps_ismap(*v);
	}
	__declspec(dllexport) bool cpps_value_isbasevar(cpps_value* v)
	{
		return cpps_isbasevar(*v);
	}
	__declspec(dllexport) bool cpps_value_isstring(cpps_value* v)
	{
		return cpps_isstring(*v);
	}
	__declspec(dllexport) bool cpps_value_isvector(cpps_value* v)
	{
		return cpps_isvector(*v);
	}
	__declspec(dllexport) bool cpps_value_isrange(cpps_value* v)
	{
		return cpps_isrange(*v);
	}
	__declspec(dllexport) bool cpps_value_isint(cpps_value* v)
	{
		return cpps_isint(*v);
	}
	__declspec(dllexport) bool cpps_value_ischar(cpps_value* v)
	{
		return cpps_ischar(*v);
	}
	__declspec(dllexport) bool cpps_value_isuint(cpps_value* v)
	{
		return cpps_isuint(*v);
	}
	__declspec(dllexport) bool cpps_value_isbool(cpps_value* v)
	{
		return cpps_isbool(*v);
	}
	__declspec(dllexport) bool cpps_value_isnumber(cpps_value* v)
	{
		return cpps_isnumber(*v);
	}
	__declspec(dllexport) bool cpps_value_isnull(cpps_value* v)
	{
		return cpps_isnull(*v);
	}
	__declspec(dllexport) bool cpps_value_isfunction(cpps_value* v)
	{
		return cpps_isfunction(*v);
	}	
	__declspec(dllexport) bool cpps_value_isclassvar(cpps_value* v)
	{
		return cpps_isclassvar(*v);
	}
	__declspec(dllexport) bool cpps_value_isclass(cpps_value* v)
	{
		return cpps_isclass(*v);
	}
	__declspec(dllexport) bool cpps_value_isellipsis(cpps_value* v)
	{
		return cpps_isellipsis(*v);
	}
	__declspec(dllexport) bool cpps_value_istuple(cpps_value* v)
	{
		return cpps_istuple(*v);
	}
	__declspec(dllexport) void cpps_value_getclassname(cpps_value* v,char *_out)
	{
		strcpy(_out,cpps_getclassname(*v).c_str());
	}
	__declspec(dllexport) void cpps_value_global(C*c,cpps_value *out)
	{
		*out = cpps_value(c->_G);
	}
	__declspec(dllexport) void cpps_value_get_by_name(cpps_value *obj,char *name,cpps_value *out)
	{
		*out = object(*obj)[name].realval();
	}
	__declspec(dllexport) void cpps_value_get_by_idx(cpps_value *obj,int idx,cpps_value *out)
	{
		*out = object(*obj)[idx].realval();
	}
	__declspec(dllexport) void cpps_value_incruse(cpps_value *obj)
	{
		obj->incruse();
	}
	__declspec(dllexport) void cpps_value_decruse(cpps_value *obj)
	{
		obj->decruse();
	}
	__declspec(dllexport) void* cpps_value_cast(cpps_value *obj)
	{
		if (!cpps_isclassvar(*obj)) return NULL;
		cpps_class_handler* handler = object_cast<cpps_class_handler*>(*obj);
		return handler->get_ptr();
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
	__declspec(dllexport) void cpps_vector_push_back(cpps_value* _vct,cpps_value* _v)
	{
		cpps_vector* vct = cpps_to_cpps_vector(*_vct);
		vct->push_back(*_v);
	}
	__declspec(dllexport) int cpps_vector_size(cpps_value* _vct)
	{
		cpps_vector* vct = cpps_to_cpps_vector(*_vct);
		return (int)vct->size();
	}
	__declspec(dllexport) void cpps_vector_erase(cpps_value* _vct,int idx)
	{
		cpps_vector* vct = cpps_to_cpps_vector(*_vct);
		vct->erase(idx);
	}
	__declspec(dllexport) void cpps_vector_at(cpps_value* _vct,int idx,cpps_value* out)
	{
		cpps_vector* vct = cpps_to_cpps_vector(*_vct);
		*out = vct->at(idx);
		printf("idx:%d type:%d\n",idx,out->tt);
	}
	__declspec(dllexport) void cpps_vector_clear(cpps_value* _vct)
	{
		cpps_vector* vct = cpps_to_cpps_vector(*_vct);
		vct->clear();
	}
	__declspec(dllexport) void cpps_vector_create(C*c,cpps_value *out)
	{
		cpps_create_class_var(cpps_vector, c, vct_value, vct_ptr);
		*out = vct_value;
	}
	//map
	__declspec(dllexport) void cpps_map_insert(cpps_value* _map, cpps_value* _key,cpps_value* _value)
	{
		cpps_map* m = cpps_to_cpps_map(*_map);
		m->insert(*_key, *_value);
	}
	__declspec(dllexport) int cpps_map_size(cpps_value* _map)
	{
		cpps_map* m = cpps_to_cpps_map(*_map);
		return (int)m->size();
	}
	__declspec(dllexport) void cpps_map_clear(cpps_value* _map)
	{
		cpps_map* m = cpps_to_cpps_map(*_map);
		m->clear();
	}
	__declspec(dllexport) void cpps_map_erase(cpps_value* _map,cpps_value* _key)
	{
		cpps_map* m = cpps_to_cpps_map(*_map);
		m->erase(*_key);
	}
	__declspec(dllexport) void cpps_map_find(cpps_value* _map,cpps_value* _key,cpps_value *out)
	{
		cpps_map* m = cpps_to_cpps_map(*_map);
		*out = m->find(*_key);
	}
	__declspec(dllexport) void cpps_map_create(C*c, cpps_value* out)
	{
		cpps_create_class_var(cpps_map, c, map_value, map_ptr);
		*out = map_value;
	}
	//function
	__declspec(dllexport) void cpps_function_call(C* c, cpps_value* func,cpps_value *ps,cpps_value *out)
	{
		object ret = function_caller(c, *func, cpps_to_cpps_vector(*ps)->realvector());
		*out = ret.realval();
	}
	__declspec(dllexport) void cpps_function_class_call(C* c, cpps_value* _class, cpps_value* func,cpps_value *ps,cpps_value *out)
	{
		object ret = function_caller(c,*_class, *func, cpps_to_cpps_vector(*ps)->realvector());
		*out = ret.realval();
	}
	//string

	__declspec(dllexport) int cpps_string_size( cpps_value* _str)
	{
		std::string* s = cpps_get_string(*_str);
		return (int)s->size();
	}
	__declspec(dllexport) void cpps_string_append( cpps_value* _str, char* out, int count)
	{
		std::string* s = cpps_get_string(*_str);
		s->append(out, (size_t)count);
	}
	__declspec(dllexport) void cpps_string_strcopy( cpps_value* _str,char *out)
	{
		std::string* s = cpps_get_string(*_str);
		memcpy(out, s->c_str(), s->size());
	}
	__declspec(dllexport) void cpps_string_assignment(cpps_value* _str,char *out,int count)
	{
		std::string* s = cpps_get_string(*_str);
		s->assign(out, (size_t)count);
	}
	__declspec(dllexport) void cpps_string_create(C*c,cpps_value* _out)
	{
		*_out = cpps_value(c, "");
	}
	//classvar
	__declspec(dllexport) void cpps_classvar_create(C* c, cpps_value* _class,cpps_value* _out)
	{
		if (!cpps_isclass(*_class)) return;
		cpps_cppsclass* _cppsclass = cpps_to_cpps_cppsclass(*_class);
		newcppsclasvar(c, _cppsclass, _out);
	}
}

//cpps_export_finish	//unix export.