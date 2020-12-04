#include "cpps.h"

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
				std::vector<cpps_value> paramlist;


				cpps_domain *execdomain = c->_G;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free(stack);
				//¼ì²âgc
				//if ( c->getcallstack()->size() == 0)
			/*	{
					cpps_gc_check_step(c);
				}*/

			}
		}
		return ret;
	}
	object doclassfunction(C *c,cpps_domain *leftdomain, object func)
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
				std::vector<cpps_value> paramlist;


				cpps_domain *execdomain = leftdomain;

				cpps_stack* stack = c->stack_alloc();
				stack->init("", 0, f->funcname.c_str());
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				c->stack_free( stack);
				//¼ì²âgc
				//if ( c->getcallstack()->size() == 0)
			/*	{
					cpps_gc_check_step(c);
				}*/

			}
		}
		return ret;
	}
}
