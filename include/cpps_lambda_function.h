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
		cpps_lambda_function() {
			stacklist = NULL;
			realfunc = NULL;
		}
		virtual ~cpps_lambda_function() {
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
			}
			stacklist->clear();
		}
		void setrealfunc(cpps_cppsfunction* func)
		{
			realfunc = func;
		}
		virtual void	callfunction(C* c, cpps_value* ret, cpps_domain* domain, std::vector<cpps_value>* o, cpps_stack* stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{
			realfunc->callfunction(c, ret, domain, o, stack,stacklist);
		}
		virtual bool getIsNeedC()
		{
			return realfunc->getIsNeedC();
		}
		virtual int8 getparamcount() { return realfunc->getparamcount(); }
		cpps_cppsfunction* realfunc;
		std::vector< cpps_regvar*>* stacklist;
		C* c;
	};

	void	cpps_reglambdafunction(C* c);

}

#endif //CPPS_LAMBDAFUNCTION_CPPS_HEAD_