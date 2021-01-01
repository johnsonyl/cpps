#ifndef CPPS_CPPSFUNCTION_CPPS_HEAD_
#define CPPS_CPPSFUNCTION_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/26 (yy/mm/dd)
//@Module		:	CPPS_CPPSFUNCTION
//@Description	:	Cpps函数处理
//@website		:	http://cpps.wiki
//==================================
#ifdef _WIN32
#if defined _M_X64 || defined _M_IX86
#ifdef _M_X64
extern "C" void __stdcall		call_native_func(cpps::usint64 a, cpps::C *c, cpps::cpps_domain *domain, cpps::cpps_stack *stack);
#endif
#else
#error	"This code works only for x86 and x64!"
#endif
#endif

namespace cpps
{

	

	struct cpps_cppsfunction : public cpps_function
	{
		cpps_cppsfunction(cpps_domain *d,node *p,node *c, usint32 count);
		virtual ~cpps_cppsfunction();
		virtual int8	getparamcount();
		virtual void	callfunction(C *c, cpps_value *ret, cpps_domain *prevdomain, cpps_std_vector *o, cpps_stack *stack, std::vector< cpps_regvar*>* lambdastacklist);
		void			setasync(bool b);
		virtual bool	isasync();
	public:
		usint32		varcount;
		node		*params;
		node		*context;
		cpps_domain	*domain; //在每个函数中临时创建一个域。 提供临时变量
#ifdef CPPS_JIT_COMPILER
		char*		jitbuffer; //JIT缓存
		size_t		jitbufferSize;
#endif
	};

}
#endif // CPPS_CPPSFUNCTION_CPPS_HEAD_