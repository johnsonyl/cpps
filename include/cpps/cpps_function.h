#ifndef CPPS_FUNCTION_CPPS_HEAD_
#define CPPS_FUNCTION_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_FUNCTION
//@Description	:	Cppº¯Êýµ÷ÓÃ
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	struct cpps_domain;
	template<class C>
	struct cpps_classvar;
	template<class R>
	struct cpps_converter;


	template<typename p1>
	class dofunc
	{
	public:
		dofunc()
		{}
		
		dofunc(p1 dofunc_param_p1)
			: vec(dofunc_param_p1)
		{

		}
	public:
		p1 vec;
	};
	template<class context>
	void call_normal(C *c_state, cpps_domain *domain, cpps_std_vector &o, context &c, void(*f)())
	{
		f();
	}
	template <class Ret, class context>
	Ret call_member(C *c_state, cpps_domain *domain, cpps_std_vector &o, context &c, Ret(*f)())
	{
		return f();
	}


	template<class context,class CLS >
	void call_normal(C *c_state, cpps_domain *domain, cpps_std_vector &o, context &c, void(CLS::*f)())
	{
		if (domain&& domain->domainType == cpps_domain_type_classvar)
		{
			cpps_classvar<CLS> *cls = (cpps_classvar<CLS> *)domain;
			(cls->_class->*f)();
		}
	}
	template < class Ret, class CLS, class context>
	Ret call_member(C *c_state, cpps_domain *domain, cpps_std_vector &o, context &c, Ret(CLS::*f)())
	{
		if (domain&& domain->domainType == cpps_domain_type_classvar)
		{
			cpps_classvar<CLS> *cls = (cpps_classvar<CLS> *)domain;
			return (cls->_class->*f)();
		}
		return Ret();
	}

}

#define CPPS_FUNCTION_ITER_C 1
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C

#define CPPS_FUNCTION_ITER_C 2
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C

#define CPPS_FUNCTION_ITER_C 3
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C

#define CPPS_FUNCTION_ITER_C 4
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C


#define CPPS_FUNCTION_ITER_C 5
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C


#define CPPS_FUNCTION_ITER_C 6
#include "cpps_function.h"
#undef CPPS_FUNCTION_ITER_C
#endif

#ifdef CPPS_FUNCTION_ITER_C
namespace cpps
{
	template<class context, CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, class A) >
	void call_normal(C *c_state, cpps_domain *domain, cpps_std_vector &o, context c, void(*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, A)))
	{
		CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(CPPS_FUNCTION_ITER_C, c.vec.param, o);
		f(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, c.vec.param));
	}
	template <class Ret, class context, CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, class A) >
	Ret call_member(C *c_state, cpps_domain *domain, cpps_std_vector &o, context c, Ret(*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, A)))
	{
		CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(CPPS_FUNCTION_ITER_C, c.vec.param, o);
		return f(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, c.vec.param));
	}

	template<class context, class CLS, CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, class A) >
	void call_normal(C *c_state, cpps_domain *domain, cpps_std_vector &o, context c, void(CLS::*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, A)))
	{
		if (domain && domain->domainType == cpps_domain_type_classvar)
		{
			cpps_classvar<CLS> *cls = (cpps_classvar<CLS> *)domain;
			CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(CPPS_FUNCTION_ITER_C, c.vec.param, o);
			(cls->_class->*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, c.vec.param));
		}
	}
	template < class Ret, class context, class CLS, CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, class A) >
	Ret call_member(C *c_state, cpps_domain *domain, cpps_std_vector &o, context c, Ret(CLS::*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, A)))
	{
		if (domain&& domain->domainType == cpps_domain_type_classvar)
		{
			cpps_classvar<CLS> *cls = (cpps_classvar<CLS> *)domain;
			CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(CPPS_FUNCTION_ITER_C, c.vec.param, o);
			return (cls->_class->*f)(CPPS_PP_ENUM_PARAMS(CPPS_FUNCTION_ITER_C, c.vec.param));
		}
		return Ret();
	}
}
#endif // CPPS_FUNCTION_CPPS_HEAD_