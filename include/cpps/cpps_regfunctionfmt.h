#ifndef CPPS_REG_FUNCTIONFMT_CPPS_HEAD_
#define CPPS_REG_FUNCTIONFMT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_CALL_FUNCTION
//@Description	:	ע�ắ��
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	template<typename p1>
	class dofunc;
	template<class retType, typename context, typename F, typename IsVoid>
	retType call_function(cpps_value &ret, cpps_domain *domain, node*o, context &c, F &f, IsVoid&);
	template<class T>
	class cpps_is_void;
	template <class C>
	struct cpps_class;
	struct cpps_cppsclass;
	struct cpps_cpps_value;
	struct Buffer;
#define cpps_def_regfunction	1
#define cpps_def_regclass		2
#define cpps_def_regvar			3


	std::string transformat(std::string fmt, cpps_value v);
	size_t formatcheck(C *c,std::string &fmt, size_t j, size_t i, std::string &take, cpps_value &v);

	template<class R>
	struct cpps_functionfmt : public cpps_function
	{
		cpps_functionfmt(R(*f)(C* c, cpps::cpps_value args, ...))
			:f(f)
		{
		}

		virtual int8 getparamcount() { return 1; }

		template<typename IsVoid>
		void call_function(C* c, cpps_value& vct_value, cpps_value* ret, IsVoid) {
			R r = f(c, vct_value);
			if (!cpps_cpp_to_cpps_converter<R>::match(c, r))
			{
				throw(cpps_error("0", 0, 0, "%s is not defined to script, conversion failed.", typeid(R).name()));
			}
			*ret = cpps_cpp_to_cpps_converter<R>::apply(c, r); //c++�ķ���ֵ ֻ�п���ת���Ĳſ���
		}

		void call_function(C* c, cpps_value& vct_value, cpps_value* ret, cpps_is_void<void>) {
			f(c, vct_value);
		}

		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			cpps_vector* vct = NULL; cpps_value vct_value;
			newclass<cpps_vector>(c, &vct, &vct_value);
			vct->realvector().assign(o->begin(),o->end());
			call_function(c, vct_value, ret, cpps_is_void<R>());
		}
		R(*f)(C* c, cpps::cpps_value args, ...);
	};
	template<>
	struct cpps_functionfmt<cpps_value*> : public cpps_function
	{
		cpps_functionfmt(cpps_value* (*f)(C* c, cpps::cpps_value args, ...))
			:f(f)
		{
		}
		void call_function(C* c, cpps_value& vct_value, cpps_value* ret) {
			cpps_value* r = f(c, vct_value);
			ret->tt = r->tt;
			ret->value = r->value;
			ret->incruse();
		}
		void  callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			cpps_vector* vct = NULL; cpps_value vct_value;
			newclass<cpps_vector>(c, &vct, &vct_value);
			vct->realvector().assign(o->begin(), o->end());
			call_function(c, vct_value, ret);
		}
		cpps_value* (*f)(C* c, cpps::cpps_value args, ...);
	};

	template<class R,typename CLS>
	struct cpps_functionfmt_class : public cpps_function
	{
		cpps_functionfmt_class(R(CLS::*f)(C* c, cpps::cpps_value args, ...))
			:f(f)
		{
		}

		virtual int8 getparamcount() { return 1; }

		template<typename IsVoid>
		void call_function(C* c, cpps_domain *domain,cpps_value& vct_value, cpps_value* ret, IsVoid) {
			if (domain && domain->domainType == cpps_domain_type_classvar)
			{
				cpps_classvar<CLS>* cls = (cpps_classvar<CLS> *)domain;
				R r = (cls->__class->*f)(c, vct_value);
				if (!cpps_cpp_to_cpps_converter<R>::match(c, r))
				{
					throw(cpps_error("0", 0, 0, "%s is not defined to script, conversion failed.", typeid(R).name()));
				}
				*ret = cpps_cpp_to_cpps_converter<R>::apply(c, r); //c++�ķ���ֵ ֻ�п���ת���Ĳſ���
			}
		}
		void call_function(C* c, cpps_domain* domain, cpps_value& vct_value, cpps_value* ret, cpps_is_void<void>) {
			if (domain && domain->domainType == cpps_domain_type_classvar)
			{
				cpps_classvar<CLS>* cls = (cpps_classvar<CLS> *)domain;
				(cls->__class->*f)(c, vct_value);
			}
		}

		virtual void  callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			cpps_vector* vct = NULL; cpps_value vct_value;
			newclass<cpps_vector>(c, &vct, &vct_value);
			vct->realvector().assign(o->begin(), o->end());
			call_function(c,domain, vct_value, ret, cpps_is_void<R>());
		}
		R(CLS::*f)(C* c, cpps::cpps_value args, ...);
	};

	template<class R>
	cpps_regfunction* make_regfunction(std::string func, R(*f)(C*c,cpps::cpps_value args , ...), bool isasync ,bool isoperator )
	{
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW( cpps_functionfmt<R>)(f),isasync, isoperator);
	}
	template<class R,typename CLS>
	cpps_regfunction* make_regfunction2(std::string func, R(CLS::*f)(C*c,cpps::cpps_value args , ...), bool isasync ,bool isoperator )
	{
		typedef cpps_functionfmt_class<R, CLS> _Type;
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW(_Type)(f),isasync, isoperator);
	}
}
#endif