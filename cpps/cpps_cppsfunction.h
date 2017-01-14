#ifndef CPPS_CPPSFUNCTION_CPPS_HEAD_
#define CPPS_CPPSFUNCTION_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/26 (yy/mm/dd)
//@Module		:	CPPS_CPPSFUNCTION
//@Description	:	Cpps函数处理
//@website		:	http://cpps.wiki
//==================================
#ifdef WIN32
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

	//内部
	cpps_value				cpps_calculate_expression(C *c, cpps_domain *domain, Node *o, cpps_domain *&leftdomain);
	void					cpps_step(C * c, cpps_domain *domain, Node* d);
	void					cpps_step_all(C * c, int32 retType, cpps_domain* domain, Node *o, bool isCheckGen1 );

	void					make_values(C *c, cpps_domain *domain, Node *d, std::vector<cpps_value> &params);

	

	struct cpps_cppsfunction : public cpps_function
	{
		cpps_cppsfunction(cpps_domain *d,Node *p,Node *c)
		{
			params = new Node(p->filename, p->line);
			params->clone(p); //克隆出所有列表

			context = new Node(c->filename, c->line);
			context->clone(c); //克隆出所有列表

			jitbuffer = NULL;
			jitbufferSize = 0;

			domain = d;
		}



		virtual void  callfunction(C *c, cpps_value *ret, cpps_domain *prevdomain, std::vector<cpps_value> *o, cpps_stack *stack, bool isCheckGen1 )
		{
#ifdef CPPS_JIT_COMPILER

			if (jitbuffer == NULL)
			{
				cpps_jit_compiler compiler;
				compiler.compiler(c, domain, context);
				compiler.build(jitbuffer, jitbufferSize);
			}
			
#endif // CPPS_JIT_COMPILER

			//todo 先delete指针
			cpps_domain *funcdomain = new cpps_domain(domain, cpps_domain_type_exec,"");
			funcdomain->setexecdomain(prevdomain);

			//l 的0 代表返回值列表 1.代表参数列表

			if (o->size() > params->l.size())
			{
				throw("参数不对");
			}

			for (size_t i = 0; i < params->l.size(); i++)
			{

				Node* d = params->l[i];
				Node* varName = d->l[0];

				if (varName->type == CPPS_VARNAME)
				{
					cpps_regvar * v = new cpps_regvar();
					v->setVarName(varName->s);

					if (i >= o->size())
					{
						if (i < varName->l.size())
						{
							Node *var = varName->l[0]; //默认参数。。。 如果穿进来则不执行默认参数
							cpps_domain *leftdomain = NULL;
							cpps_value value = cpps_calculate_expression(c, prevdomain, var, leftdomain);
							v->setValue(value);
						}
					}
					else
					{
						cpps_value value = (*o)[i];
						v->setValue(value);
					}
					funcdomain->regVar(v);
				}
			}
		
#ifdef CPPS_JIT_COMPILER

			call_native_func((usint64)jitbuffer, c, domain, stack);
// 			if (ret)
// 				*ret = stack->funcRet;//return的值反馈回去

#else

			cpps_step_all(c,CPPS_SINGLERET,funcdomain, context,isCheckGen1);
			
 			if (ret)
 				*ret = funcdomain->funcRet;//return的值反馈回去
#endif

			funcdomain->destory(c);
			delete funcdomain;
		}
	public:
		Node		*params;
		Node		*context;
		cpps_domain	*domain; //在每个函数中临时创建一个域。 提供临时变量
		char		*jitbuffer; //JIT缓存
		size_t		jitbufferSize;
	};

}
#endif // CPPS_CPPSFUNCTION_CPPS_HEAD_