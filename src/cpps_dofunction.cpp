#include "cpps.h"

namespace cpps
{

	object dofunction(C *c, object func)
	{
		cpps_value ret;
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function *f = func.value.value.func;
			if (f)
			{
				std::vector<cpps_value> paramlist;


				cpps_domain *execdomain = c->_G;
				if (func.value.parentlambdavar)
					execdomain = func.value.parentlambdavar;

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
		if (func.value.tt == CPPS_TFUNCTION)
		{
			cpps_function *f = func.value.value.func;
			if (f)
			{
				std::vector<cpps_value> paramlist;


				cpps_domain *execdomain = leftdomain;
				if (func.value.parentlambdavar)
					execdomain = func.value.parentlambdavar;

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
