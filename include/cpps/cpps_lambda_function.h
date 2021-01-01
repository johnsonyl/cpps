#ifndef CPPS_LAMBDAFUNCTION_CPPS_HEAD_
#define CPPS_LAMBDAFUNCTION_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/4 (yy/mm/dd)
//@Module		:	CPPS_CPPSFUNCTION
//@Description	:	Cppsº¯Êý´¦Àí
//@website		:	http://cpps.wiki
//==================================
namespace cpps
{
	struct cpps_lambda_function : public cpps_function
	{
		cpps_lambda_function();
		virtual ~cpps_lambda_function();
		void							setrealfunc(cpps_cppsfunction* func);
		virtual void					callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL);
		virtual std::string				getfuncname();
		virtual bool					getIsNeedC();
		virtual int8					getparamcount();
		cpps_cppsfunction* realfunc;
		std::vector< cpps_regvar*>* stacklist;
		C* c;
	};

	void	cpps_reglambdafunction(C* c);

}

#endif //CPPS_LAMBDAFUNCTION_CPPS_HEAD_