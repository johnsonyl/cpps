#include "cpps/cpps.h"

namespace cpps
{

	object dofunction(C *c, object func)
	{
		cpps_value ret;

		if (func.value.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func.value = pfunc;
			func.value.tt = CPPS_TFUNCTION;
		}
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function *f = func.value.value.func;
			if (f)
			{
				cpps_std_vector paramlist;


				cpps_domain *execdomain = c->_G;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free(stack);

			}
		}
		return ret;
	}
	object doclassfunction(C *c, object domain, object func)
	{
		cpps_value ret;

		if (func.value.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func.value = pfunc;
			func.value.tt = CPPS_TFUNCTION;
		}
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function *f = func.value.value.func;
			if (f)
			{
				cpps_std_vector paramlist;


				cpps_domain *execdomain = domain.value.value.domain;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free( stack);

			}
		}
		return ret;
	}

	cpps_value cpps_new_tmp_string(const std::string& tmp) {
		cpps_cppsclassvar* var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(NULL, false);
		var->setclsptr((void*)&tmp);
		cpps_value retv(var);
		retv.tt = CPPS_TSTRING;
		return retv;
	}

	void cpps_delete_tmp_string(cpps_value& v) {
		cpps_cppsclassvar* var = (cpps_cppsclassvar*)v.value.domain;
		v = nil;
		var->release();
		var = NULL;
	}

	void newcppsclasvar(C* c, cpps::cpps_cppsclass* cppsclass, cpps_value* ret_value)
	{
		cpps_cppsclassvar* cppsclassvar = cppsclass->create(c);
		/* 将类对象里面的变量创建出来 */
		cpps_step_newclassvar_reg_baselassvar(cppsclass, c, cppsclassvar, cppsclassvar);
		//将类对象里面的变量创建出来
		if (cppsclass->o)
			cpps_step_all(c, CPPS_MUNITRET, cppsclassvar, cppsclassvar, cppsclass->o->getright(), false);
		//将新创建出来的添加到新生区稍后检测要不要干掉
		cpps_gc_add_gen0(c, cppsclassvar);
		//执行0参数构造函数
		cpps_domain* leftdomain = NULL;
		cpps_regvar* var = cppsclassvar->getvar("constructor", leftdomain);

		ret_value->tt = CPPS_TCLASSVAR;
		ret_value->value.domain = (cpps_domain*)cppsclassvar;
		ret_value->incruse();

		if (var && var->getval().tt == CPPS_TFUNCTION) {
			node n("", 0);
			leftdomain = cppsclassvar;
			for (auto parent_cppsclass : cppsclass->parentclasslist()) {
				cpps_call_parent_class_default_constructor(c, &n, parent_cppsclass, c->_G, c->_G, leftdomain);
			}
			cpps_function* f = var->getval().value.func;
			if (f->getparamcount() == 0) {
				doclassfunction(c, *ret_value, var->getval());
			}
		}
	}

	object function_caller(C* c, object func, cpps_std_vector& paramlist)
	{
		cpps_value ret;

		if (func.value.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func.value = pfunc;
			func.value.tt = CPPS_TFUNCTION;
		}
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function* f = func.value.value.func;
			if (f)
			{
				cpps_domain* execdomain = c->_G;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free(stack);

			}
		}
		return ret;
	}
	object function_caller(C* c, object domain, object func, cpps_std_vector& paramlist)
	{
		cpps_value ret;

		if (func.value.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func.value = pfunc;
			func.value.tt = CPPS_TFUNCTION;
		}
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function* f = func.value.value.func;
			if (f)
			{
				cpps_domain* execdomain = domain.value.value.domain;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free(stack);
			}
		}
		return ret;
	}
}
