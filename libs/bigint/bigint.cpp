
#include "cpps_uint256.h"
#include <cpps/cpps.h>
using namespace cpps;
using namespace std;




#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic pop
#endif
cpps_export_void cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "bigint")[
		_class<cpps_uint256>("uint256")
			.def_inside("constructor",&cpps_uint256::_constructor)
			.def_operator_inside("+",&cpps_uint256::_add)
			.def_operator_inside("-",&cpps_uint256::_sub)
			.def_operator_inside("*",&cpps_uint256::_mul)
			.def_operator_inside("/",&cpps_uint256::_div)
			.def_operator_inside("&",&cpps_uint256::_add2)
			.def_operator_inside("&=",&cpps_uint256::_add2andassignment)
			.def_operator_inside("|=",&cpps_uint256::_add3andassignment)
			.def_operator_inside("|",&cpps_uint256::_add3)
			.def_operator_inside("%",&cpps_uint256::_quyu)
			.def_operator_inside("=",&cpps_uint256::_assignment)
			.def_operator_inside("<",&cpps_uint256::_less)
			.def_operator_inside(">",&cpps_uint256::_bigger)
			.def_operator_inside(">=",&cpps_uint256::_biggerorequel)
			.def_operator_inside("==",&cpps_uint256::_equel)
			.def_operator_inside("<=",&cpps_uint256::_lessandequel)
			.def_operator_inside("++",&cpps_uint256::_rightautoincrease)
			.def_operator_inside("--",&cpps_uint256::_rightautodecrease)
			.def_operator_inside("++left",&cpps_uint256::_leftautoincrease)
			.def_operator_inside("--left",&cpps_uint256::_leftautodecrease)
			.def_operator_inside("^",&cpps_uint256::_add4)
			.def_operator_inside("^=",&cpps_uint256::_add4andassignment)
			.def_operator_inside("!=",&cpps_uint256::_notequel)
			.def_operator_inside("<<=",&cpps_uint256::_add5andassignment)
			.def_operator_inside(">>=",&cpps_uint256::_add6andassignment)
			.def_operator_inside("*=",&cpps_uint256::_mulandassignment)
			.def_operator_inside("/=",&cpps_uint256::_divandassignment)
			.def_operator_inside("+=",&cpps_uint256::_addandassignment)
			.def_operator_inside("-=",&cpps_uint256::_subandassignment)
			.def_operator_inside("<<",&cpps_uint256::_add5)
			.def_operator_inside(">>",&cpps_uint256::_add6)
			.def_operator_inside("~/",&cpps_uint256::_div)
			.def_operator_inside("~/=",&cpps_uint256::_divandassignment)
			.def_operator_inside("-left",&cpps_uint256::_minus)
			.def_operator_inside("+left",&cpps_uint256::_plus)
			.def_operator_inside("!left",&cpps_uint256::_not)
			.def_operator_inside("~left",&cpps_uint256::_not2)
			.def("tostring",&cpps_uint256::_tostring)
			.def("tohex",&cpps_uint256::_tohex)
			.def("todec",&cpps_uint256::_todec)
			.def("toint",&cpps_uint256::_toint)
			.def("hash_value",&cpps_uint256::_hash_value)
	];



   
}
cpps_export_void cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "bigint");
}

cpps_export_finish