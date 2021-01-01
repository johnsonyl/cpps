#include "cpps/cpps.h"

namespace cpps {
	
	void	cpps_reglambdafunction(C* c)
	{
		cpps::_module(c)[
		_class<cpps_lambda_function>("CPPS_LAMBDA_FUNCTION")
		];
	}

	cpps_lambda_function::cpps_lambda_function()
	{
		c = NULL;
		stacklist = NULL;
		realfunc = NULL;
	}

	cpps_lambda_function::~cpps_lambda_function()
	{
		if (stacklist) {
			for (auto no : *(stacklist))
			{
				if (no && no->closeure) {
					no->closeureusecount--;
					if (no->closeureusecount == 0) {
						cpps_gc_remove_barrier(c, no);
						delete no;
					}
				}
			}
			stacklist->clear();
		}
	}

	void cpps_lambda_function::setrealfunc(cpps_cppsfunction* func)
	{
		realfunc = func;
	}

	void cpps_lambda_function::callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack /*= NULL*/, std::vector< cpps_regvar*>* lambdastacklist /*= NULL*/)
	{
		realfunc->callfunction(c, ret, domain, o, stack, stacklist);
	}

	std::string cpps_lambda_function::getfuncname()
	{
		return realfunc->getfuncname();
	}

	bool cpps_lambda_function::getIsNeedC()
	{
		return realfunc->getIsNeedC();
	}

	cpps::int8 cpps_lambda_function::getparamcount()
	{
		return realfunc->getparamcount();
	}

}