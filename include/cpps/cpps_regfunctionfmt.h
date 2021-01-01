#ifndef CPPS_REG_FUNCTIONFMT_CPPS_HEAD_
#define CPPS_REG_FUNCTIONFMT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_CALL_FUNCTION
//@Description	:	注册函数
//@website		:	http://cpps.wiki
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
		cpps_functionfmt(R(*f)(const char* fmt, ...))
			:f(f)
		{
		}

		virtual int8 getparamcount() { return 1; }

		void  callfunction(C *c, cpps_value *ret, cpps_domain *domain, cpps_std_vector *o, cpps_stack *stack = NULL, std::vector< cpps_regvar*>* lambdastacklist = NULL)
		{

			cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)o->at(0).value.domain;
			std::string * tmpStr= (std::string *)cppsclassvar->getclsptr();
			std::string fmt = *(tmpStr);
			size_t idx = 1;


			std::string take;
			for (size_t i = 0; i < fmt.size(); i++)
			{
				if (fmt[i] == '%')
				{
					if (fmt[i + 1] == '%')
					{
						fmt.replace(i, 2, "%");
						continue;
					}
					take = "";
					for (size_t j = i; j < fmt.size(); j++)
					{
						take.push_back(fmt[j]);
						size_t newsize = formatcheck(c,fmt, j, i, take, o->at(idx));
						if (newsize != 0)
						{
							idx++;
							i += newsize - 1;
							break;
						}

					}
				}
			}



			cpps_is_void<R> isvoid = cpps_is_void<R>();
			if (isvoid.b)
			{
				f(fmt.c_str());
			}
			else
			{
				R r = f(fmt.c_str());
				if (!cpps_cpp_to_cpps_converter<R>::match(c, r))
				{
					throw(cpps_error("0", 0, 0, "%s is not defined to script, conversion failed.", typeid(R).name()));
				}
				*ret = cpps_cpp_to_cpps_converter<R>::apply(c, r); //c++的返回值 只有可以转换的才可以
			}

		}
		R(*f)(const char* fmt, ...);
	};


	template<class R>
	cpps_regfunction* make_regfunction(std::string func, R(*f)(const char* fmt, ...), bool isasync)
	{
		return new cpps_regfunction(func, new cpps_functionfmt<R>(f),isasync);
	}
}
#endif