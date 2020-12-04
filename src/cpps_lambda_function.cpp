#include "cpps.h"

namespace cpps {
	
	void	cpps_reglambdafunction(C* c)
	{
		module(c)[
		_class<cpps_lambda_function>("CPPS_LAMBDA_FUNCTION")
		];
	}

}