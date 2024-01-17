#ifndef CPPS_NEWPTR_HEAD_
#define CPPS_NEWPTR_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	LIBCPPS
//@Description	:	Cpps处理接口
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	template <class T>
	struct cpps_class_singleton;
	void cpps_gc_add_gen0(C*c, cpps_cppsclassvar *p);
	void cpps_call_parent_class_default_constructor(C* c, node* n, cpps_cppsclass* parent_cppsclass, cpps_domain* domain, cpps_domain* root, cpps_domain* leftdomain);
	void cpps_step_newclassvar_reg_baselassvar(cpps_cppsclass* cppsclass, C* c, cpps_cppsclassvar* cppsclassvar, cpps_domain* root);
	object					doclassfunction(C* c, object leftdomain, object func);
	//可以增加到GC的PTR
	
	

	template<class T>
	inline cpps_cppsclassvar* newclass(C* c, T** ret)
	{
		bool isstr = cpps_is_string<T>().b;
		cpps_cppsclassvar* var;
		if (isstr)
			var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(c, true);
		else
			var = cpps_class_singleton<T*>::instance()->getcls()->create(c, true);
		//将新创建出来的添加到新生区稍后检测要不要干掉
		cpps_gc_add_gen0(c, var);
		if (typeid(std::string) == typeid(T)) {
			cpps::string* _cppstr = (cpps::string*)var->getclsptr();
			*ret = (T*)&(_cppstr->real());
		}
		else {
			*ret = (T*)var->getclsptr();
		}
		return var;
	}
	template<class T>
	inline void		newclass(C *c, T ** ret,cpps_value *ret_value)
	{
		bool isstr = cpps_is_string<T>().b;
		cpps_cppsclassvar* var = newclass(c, ret);
		
		ret_value->tt = CPPS_TCLASSVAR;
		ret_value->value.domain = (cpps_domain *)var;
		if (isstr) ret_value->tt = CPPS_TSTRING;
		ret_value->incruse();
	}
	template<class T>
	inline void		newclass(C *c, cpps_value *ret_value)
	{
		bool isstr = cpps_is_string<T>().b;
		T* ret = NULL;
		cpps_cppsclassvar* var = newclass(c, ret);
		
		ret_value->tt = CPPS_TCLASSVAR;
		ret_value->value.domain = (cpps_domain *)var;
		if (isstr) ret_value->tt = CPPS_TSTRING;
		ret_value->incruse();
	}

	cpps_value cpps_new_tmp_string(const std::string& tmp);
	void	cpps_delete_tmp_string(cpps_value& v);
	void newcppsclasvar(C* c, cpps::cpps_cppsclass* cppsclass, cpps_value* ret_value);
}

#define cpps_create_class_var(t,c,v,p) t *p = NULL; cpps_value v; newclass<t>(c, &p,&v);

#endif // CPPS_NEWPTR_HEAD_