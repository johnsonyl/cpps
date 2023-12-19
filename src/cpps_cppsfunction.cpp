#include "cpps/cpps.h"

namespace cpps
{

	//内部
	void					cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d);
	void					cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o, bool);
	void					make_values(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_std_vector& params);
	void					cpps_gc_add_barrier(C* c, cpps_regvar* v);
	void					cpps_gc_remove_barrier(C* c, cpps_regvar* v);
	void					cpps_gc_check_step(C* c);
	void					cpps_destory_node(node* d);

	
	cpps_cppsfunction::cpps_cppsfunction(cpps_domain* d, node* p, node* c, usint32 count)
	{
		params = CPPSNEW( node)(p->filename, p->line);
		params->clone(p); //克隆出所有列表

		context = CPPSNEW( node)(c->filename, c->line);
		context->clone(c); //克隆出所有列表

		varcount = count;
		quatoreturn = false;
		mulitparams = false;

#ifdef CPPS_JIT_COMPILER
		jitbuffer = NULL;
		jitbufferSize = 0;
#endif // CPPS_JIT_COMPILER

		domain = d;
	}

	cpps_cppsfunction::~cpps_cppsfunction()
	{
		cpps_destory_node(params); CPPSDELETE( params); params = NULL;
		cpps_destory_node(context); CPPSDELETE( context); context = NULL;
		if (domain) CPPSDELETE(domain);
		domain = NULL;
	}

	cpps::int8 cpps_cppsfunction::getparamcount()
	{
		return static_cast<int8>(params->l.size());
	}

	void cpps_cppsfunction::callfunction(C* c, cpps_value* ret, cpps_domain* prevdomain, cpps_std_vector* o, cpps_stack* stack, std::vector< cpps_regvar*>* lambdastacklist)
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
		cpps_domain* funcdomain = c->domain_alloc();
		funcdomain->init(domain, cpps_domain_type_exec);
		funcdomain->setexecdomain(prevdomain);
		funcdomain->resize((usint16)varcount);
		if (lambdastacklist) {
			assert(funcdomain->stacklist->size() >= lambdastacklist->size());
			if (funcdomain->stacklist->size() >= lambdastacklist->size()) {
				for (size_t i = 0; i < lambdastacklist->size(); i++) {
					cpps_regvar* regvar = (*(lambdastacklist))[i];
					(*(funcdomain->stacklist))[i] = (*(lambdastacklist))[i];
					if (regvar)
					{
						funcdomain->regvar(c, regvar);
					}
				}
			}
		}

		for (size_t i = 0; i < params->l.size(); i++)
		{

			node* d = params->l[i];
			node* varname = d->l[0];

			if (varname->type == CPPS_VARNAME)
			{
				cpps_regvar* v = CPPSNEW(cpps_regvar)();
				v->setvarname(varname->s);
				//如果是多参数并且是最后一个参数
				if (ismulitparams() && i == params->l.size()-1) {
					
					cpps_vector* vct = NULL; cpps_value vct_v;
					newclass<cpps_vector>(c, &vct, &vct_v);
					for (size_t j = i; j < o->size(); j++) {

						cpps_value value = (*o)[j];
						if (value.tt == CPPS_TREF && !varname->quote)
							vct->push_back(*value.value.value);
						else
							vct->push_back(value);
					}
					v->setval(vct_v);
				}
				else {
					if (i >= o->size())
					{
						if (varname->l.size() > 0)
						{
							node* var = varname->l[0]; //默认参数。。。 如果穿进来则不执行默认参数
							cpps_domain* leftdomain = NULL;
							cpps_value value;
							cpps_calculate_expression(c, prevdomain, prevdomain, var, leftdomain, value);
							v->setval(value.real());
						}
					}
					else
					{
						cpps_value value = (*o)[i];
						if (value.tt == CPPS_TREF && !varname->quote)
							v->setval(*value.value.value);
						else
							v->setval(value);
					}
				}
				
				funcdomain->regvar(c, v);
				if (varname->offsettype == CPPS_OFFSET_TYPE_SELF)
				{
					v->offset = varname->offset;
					v->offsettype = varname->offsettype;
					v->closeure = varname->closure;
					funcdomain->regidxvar(varname->offset, v);
				}
			}
		}

#ifdef CPPS_JIT_COMPILER

		call_native_func((usint64)jitbuffer, c, domain, stack);
		// 			if (ret)
		// 				*ret = stack->funcRet;//return的值反馈回去

#else
		if (quatoreturn) {
			funcdomain->funcRet.tt = CPPS_TREF;
		}
		cpps_step_all(c, CPPS_MUNITRET, funcdomain, funcdomain, context, false);

		if (ret) {
			*ret = funcdomain->funcRet;//return的值反馈回去
		}
#endif

			/*是否闭包*/
		funcdomain->destory(c);
		c->domain_free(funcdomain);




		cpps_gc_check_step(c);
	}

	void cpps_cppsfunction::setasync(bool b)
	{
		nasync = b;
	}

	bool cpps_cppsfunction::isasync()
	{
		return nasync;
	}

	void cpps_cppsfunction::setmulitparams(bool b)
	{
		mulitparams = b;
	}

	bool cpps_cppsfunction::ismulitparams()
	{
		return mulitparams;
	}

	void cpps_cppsfunction::rebuildfunc(node* p, node* c, usint32 count)
	{
		cpps_destory_node(params); CPPSDELETE( params); params = NULL;
		params = CPPSNEW( node)(p->filename, p->line);
		params->clone(p); //克隆出所有列表

		cpps_destory_node(context); CPPSDELETE( context); context = NULL;
		context = CPPSNEW( node)(c->filename, c->line);
		context->clone(c); //克隆出所有列表

		varcount = count;
	}

	void cpps_cppsfunction::setquatoreturn(bool b)
	{
		quatoreturn = b;
	}

	void cpps_cppsfunction::release() { CPPSDELETE(this); }

}
