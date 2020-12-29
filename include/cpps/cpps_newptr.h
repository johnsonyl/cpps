#ifndef CPPS_NEWPTR_HEAD_
#define CPPS_NEWPTR_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	LIBCPPS
//@Description	:	Cpps处理接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	template <class T>
	struct cpps_class_singleton;
	void cpps_gc_add_gen0(C*c, cpps_cppsclassvar *p);
	void cpps_call_parent_class_default_constructor(C* c, node* n, cpps_cppsclass* parent_cppsclass, cpps_domain* domain, cpps_domain* root, cpps_domain* leftdomain);
	void cpps_step_newclassvar_reg_baselassvar(cpps_cppsclass* cppsclass, C* c, cpps_cppsclassvar* cppsclassvar, cpps_domain* root);
	void					cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o);
	object					doclassfunction(C* c, object leftdomain, object func);
	//可以增加到GC的PTR
	struct string;
	

	template<class T>
	inline cpps_value		newclass(C *c, T ** ret)
	{
		bool isstr = cpps_is_string<T>().b;
		cpps_cppsclassvar* var;
		if (isstr)
			var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(c, true);
		else
			var = cpps_class_singleton<T*>::instance()->getcls()->create(c, true);
		//将新创建出来的添加到新生区稍后检测要不要干掉
		cpps_gc_add_gen0(c, var);
		*ret = (T *)var->getclsptr();
		cpps_value retv( var);
		if (isstr) retv.tt = CPPS_TSTRING;
		return retv;
	}

	inline cpps_value cpps_new_tmp_string(const std::string& tmp) {
		cpps_cppsclassvar* var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(NULL, false);
		var->setclsptr((void*)&tmp);
		cpps_value retv(var);
		retv.tt = CPPS_TSTRING;
		return retv;
	}
	inline	void	cpps_delete_tmp_string(cpps_value& v) {
		cpps_cppsclassvar* var = (cpps_cppsclassvar*)v.value.domain;
		v = nil;
		delete var;
		var = NULL;
	}
	
	inline cpps_value newcppsclasvar(C* c, cpps::cpps_cppsclass* cppsclass)
	{
		cpps_cppsclassvar* cppsclassvar = cppsclass->create(c);
		/* 将类对象里面的变量创建出来 */
		cpps_step_newclassvar_reg_baselassvar(cppsclass, c, cppsclassvar, cppsclassvar);
		//将类对象里面的变量创建出来
		if (cppsclass->o)
			cpps_step_all(c, CPPS_MUNITRET, cppsclassvar,cppsclassvar, cppsclass->o->getright());
		//将新创建出来的添加到新生区稍后检测要不要干掉
		cpps_gc_add_gen0(c, cppsclassvar);
		//执行0参数构造函数
		cpps_domain* leftdomain = NULL;
		cpps_regvar* var = cppsclassvar->getvar("constructor", leftdomain);
		cpps_value cppsclassvar_val(cppsclassvar);
		if (var && var->getval().tt == CPPS_TFUNCTION){
			node n("",0);
			leftdomain = cppsclassvar;
			for (auto parent_cppsclass : cppsclass->parentclasslist())			{
				cpps_call_parent_class_default_constructor(c, &n, parent_cppsclass, c->_G,c->_G, leftdomain);
			}
			cpps_function* f = var->getval().value.func;
			if (f->getparamcount() == 0){
				doclassfunction(c, cppsclassvar_val, var->getval());
			}
		}
		return cppsclassvar_val;
	}
}

#define cpps_create_class_var(t,c,v,p) t *p = NULL; cpps_value v = newclass<t>(c, &p);

#endif // CPPS_NEWPTR_HEAD_