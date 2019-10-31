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
				if (func.value.parentLambdaVar)
					execdomain = func.value.parentLambdaVar;

				cpps_stack *stack = new cpps_stack("", 0, f->funcname);
				c->push_stack(stack);

				f->callfunction(c, &ret, execdomain, &paramlist);

				c->pop_stack();
				delete stack;
				//¼ì²âgc
				if ( c->getcallstack()->size() == 0)
				{
					cpps_gc_check_step(c);
				}

			}
		}
		return ret;
	}
}
