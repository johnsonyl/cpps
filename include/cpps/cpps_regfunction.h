#ifndef CPPS_REG_FUNCTION_CPPS_HEAD_
#define CPPS_REG_FUNCTION_CPPS_HEAD_

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
	template< typename p1 >
	dofunc< p1 > make_function(p1 param1);
	template<class retType, typename context, typename F, typename IsVoid>
	retType call_function(cpps_value &ret, cpps_domain *domain, node*o, context &c, F &f, IsVoid&);
	template<class T>
	class cpps_is_void;
	template <class C>
	struct cpps_class;
	template <class C>
	struct cpps_classvar;
	struct cpps_cppsclass;
	struct cpps_cpps_value;
	struct Buffer;
	class cpps_class_handler;


	struct cpps_function
	{
		cpps_function();
		virtual ~cpps_function();
		virtual void	callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL);
		virtual void			setfuncname(std::string name);
		virtual std::string		getfuncname();
		virtual void release();
		virtual void setIsNeedC(bool b);
		virtual bool getIsNeedC();
		virtual int8 getparamcount();
		virtual bool isasync();
		virtual bool isoperator();
		virtual void setasync(bool b);
		virtual void setoperator(bool b);
		virtual bool iscppsfunc();
		virtual cpps_cppsclass* getclass();
		virtual void setclass(cpps_cppsclass* cls);
		bool			isNeedC;
		std::string		funcname;
		bool			nasync;
		bool			noperator;
		cpps_cppsclass*	_cls;
	};


	struct cpps_reg
	{
		cpps_reg();
		virtual ~cpps_reg();
		virtual void release();
		cpps_reg* next;
		int8	type;
		std::string varname;
		cpps_value value;
		bool		isneedC;
	};

	struct cpps_regfunction : public cpps_reg
	{
		cpps_regfunction(std::string f, cpps_function* func, bool isasync = false, bool isoperator = false);
		virtual ~cpps_regfunction();
		virtual void release();

		cpps_function* func;
	};
	struct cpps_reggvar : public cpps_reg
	{
		cpps_reggvar(std::string n, cpps_value v);
		virtual ~cpps_reggvar();
		virtual void release();

	};
	

	
	struct cpps_reg_class_var : public cpps_reg
	{
		cpps_reg_class_var(std::string n, cpps_classvar_quato* v);
		virtual ~cpps_reg_class_var();
		virtual void release();
		cpps_classvar_quato* _v;
	};
	struct cpps_regparentclass : public cpps_reg
	{
		cpps_regparentclass(cpps_cppsclass* _cppsclass);
		virtual ~cpps_regparentclass();
		virtual void release();
		cpps_cppsclass* __cppsclass;
	};
	struct cpps_regclass : public cpps_reg
	{
		cpps_regclass(std::string name, cpps_cppsclass* c);
		virtual ~cpps_regclass();
		virtual void release();

		cpps_cppsclass* cls;
	};

	template<class C>
	struct cpps_regclass_template : public cpps_regclass
	{
		cpps_regclass_template(std::string name, cpps_class<C>* c);
		virtual ~cpps_regclass_template();
		virtual void release();
	};

	struct cpps_regenum : public cpps_reg
	{
		cpps_regenum(std::string name, cpps_domain* c);
		virtual ~cpps_regenum();
		virtual void release();
		cpps_domain* _enum_domain;
	};


	
	template<class R>
	struct cpps_function1 : public cpps_function
	{
		cpps_function1( R(*f)())
		:f(f)
		{
			param = make_vector(f);
		}
		virtual ~cpps_function1() {}

		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			dofunc< vector1<R> > func = make_function(param);

			call_function<R>(c,*ret,domain, *o, func, f, cpps_is_void<R>());
		}
		virtual void release() { CPPSDELETE( this); }
		R(*f)();
		vector1<R> param;
	};

	

	template<class R>
	cpps_regfunction* make_regfunction(std::string func, R(*f)(), bool isasync = false, bool isoperator = false)
	{
		return CPPSNEW(cpps_regfunction)(func, CPPSNEW(cpps_function1<R>)(f), isasync, isoperator);
	}


	template<class R, class CLS>
	struct cpps_cpp_function1 : public cpps_function
	{
		cpps_cpp_function1(R(CLS::*f)())
		:f(f)
		{
			param = make_vector(f);
		}
		virtual ~cpps_cpp_function1() {}

		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			dofunc< vector1<R> > func = make_function(param);

			call_function< R>(c,*ret,domain, *o, func, f, cpps_is_void<R>());
		}
		virtual void release() { CPPSDELETE( this ); }
		R(CLS::* f)();
		vector1<R> param;
	};

	template<class R, class C>
	cpps_regfunction* make_regfunction(std::string func, R(C::*f)(), bool isasync = false, bool isoperator = false)
	{
		typedef cpps_cpp_function1<R, C> _Type;
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW(_Type)(f), isasync,isoperator);
	}
	template<class R, class C>
	cpps_regfunction* make_regfunction(std::string func, R(C::*f)()const, bool isasync = false, bool isoperator = false)
	{
		typedef cpps_cpp_function1<R, C> _Type;
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW(_Type)((R(C::*)())f), isasync,isoperator);
	}

	template<class F>
	cpps_reggvar* make_regvar(std::string name, F v)
	{
		return CPPSNEW( cpps_reggvar)(name, v);
	}
	
	cpps_regparentclass* make_parentclass(cpps_cppsclass* _cppsclass);
	
	template<class C>
	inline cpps_regclass_template<C>::cpps_regclass_template(std::string name, cpps_class<C>* c)
		:cpps_regclass(name, c)
	{

	}


	template<class C>
	inline cpps_regclass_template<C>::~cpps_regclass_template() {}

	template<class C>
	inline void cpps_regclass_template<C>::release() { CPPSDELETE(this); }

}


#define CPPS_MAKE_REGFUNCTION_ITER_C 1
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C



#define CPPS_MAKE_REGFUNCTION_ITER_C 2
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C



#define CPPS_MAKE_REGFUNCTION_ITER_C 3
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C



#define CPPS_MAKE_REGFUNCTION_ITER_C 4
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C



#define CPPS_MAKE_REGFUNCTION_ITER_C 5
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 6
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 7
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 8
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 9
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 10
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 11
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 12
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 13
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 14
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 15
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#define CPPS_MAKE_REGFUNCTION_ITER_C 16
#include "cpps_regfunction.h"
#undef  CPPS_MAKE_REGFUNCTION_ITER_C

#endif //CPPS_REG_FUNCTION_CPPS_HEAD_

#ifdef CPPS_MAKE_REGFUNCTION_ITER_C

namespace cpps
{
	template<class R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, class A) >
	CPPS_PP_CAT(struct cpps_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) : public cpps_function
	{
		CPPS_PP_CAT(cpps_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C))( R(*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)))
		: f(f)
		{
			param = make_vector(f);
		}
		virtual ~CPPS_PP_CAT(cpps_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C))(){}
		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			dofunc<CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > > func = make_function(param);

			call_function<R>(c,*ret,domain, *o, func, f, cpps_is_void<R>());
		}
		virtual void release() { CPPSDELETE( this); }
		virtual int8 getparamcount() { return CPPS_MAKE_REGFUNCTION_ITER_C; }
		R(*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A));
		CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > param;
	};
	template<class R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, class A) >
	cpps_regfunction* make_regfunction(std::string func, R(*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)), bool isasync = false, bool isoperator = false)
	{
		typedef  CPPS_PP_CAT(cpps_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > _Type;
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW(_Type)(f), isasync,isoperator);
	}


	template<class R, class CLS, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, class A) >
	CPPS_PP_CAT(struct cpps_cpp_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) : public cpps_function
	{
		CPPS_PP_CAT(cpps_cpp_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C))(R(CLS::*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)))
		: f(f)
		{
			param = make_vector(f);
		}
		virtual ~CPPS_PP_CAT(cpps_cpp_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C))(){}
		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			dofunc<CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > > func = make_function(param);

			call_function<R>(c,*ret, domain, *o, func, f, cpps_is_void<R>());
		}
		virtual void release() { CPPSDELETE( this); }
		virtual int8 getparamcount() { return CPPS_MAKE_REGFUNCTION_ITER_C; }
		R(CLS::*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A));
		CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > param;
	};
	template<class R, class C, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, class A) >
	cpps_regfunction* make_regfunction(std::string func, R(C::*f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)), bool isasync = false, bool isoperator = false)
	{
		typedef CPPS_PP_CAT(cpps_cpp_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, C, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > _Type;
		return CPPSNEW( cpps_regfunction)(func, CPPSNEW(_Type)(f),isasync,isoperator);
	}
	template<class R, class C, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, class A) >
	cpps_regfunction* make_regfunction(std::string func, R(C::* f)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)) const, bool isasync = false, bool isoperator = false)
	{
		typedef CPPS_PP_CAT(cpps_cpp_function, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_REGFUNCTION_ITER_C)) < R, C, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A) > _Type;
		return CPPSNEW(cpps_regfunction)(func, CPPSNEW(_Type)((R(C::*)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_REGFUNCTION_ITER_C, A)))f), isasync, isoperator);
	}



}
#endif