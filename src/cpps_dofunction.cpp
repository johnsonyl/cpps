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
}
