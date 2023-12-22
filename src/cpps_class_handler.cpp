#include "cpps/cpps.h"

namespace cpps {
	cpps_class_handler::cpps_class_handler()
	{
		__ptr = NULL;
	}
	cpps_class_handler::~cpps_class_handler()
	{
		if(__ptr != NULL)
			__free(__ptr);
		__ptr = NULL;
	}
	cpps_value cpps_class_handler::def_call(C* c, cpps::cpps_value ps, ...)
	{
		cpps_value _this = cpps_cpp_to_cpps_converter<cpps_class_handler*>::apply(c, this);
		FUNCMAP::iterator it = __func->find(c->getcallstack()->back()->func);
		if (it != __func->end()) {
			cpps_value * ret = it->second(_this, c, ps);
			return *ret;
		}
		return nil;
	}
	cpps_value cpps_class_handler::def_operator_call(C* c, cpps_value right)
	{
		cpps_value _this = cpps_cpp_to_cpps_converter<cpps_class_handler*>::apply(c, this);

		FUNCMAP::iterator it = __func->find(c->getcallstack()->back()->func);
		if (it != __func->end()) {
			cpps_value * ret = it->second(_this, c, right);
			return *ret;
		}
		return nil;
	}
	void* cpps_class_handler::get_ptr()
	{
		return __ptr;
	}
}
