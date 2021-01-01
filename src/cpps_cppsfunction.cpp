#include "cpps/cpps.h"

namespace cpps
{

	//内部
	cpps_value				cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* o, cpps_domain*& leftdomain);
	void					cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d);
	void					cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o);
	void					make_values(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_std_vector& params);
	void					cpps_gc_add_barrier(C* c, cpps_regvar* v);
	void					cpps_gc_remove_barrier(C* c, cpps_regvar* v);
	void					cpps_gc_check_step(C* c);
	void					cpps_destory_node(node* d);

	
	cpps_cppsfunction::cpps_cppsfunction(cpps_domain* d, node* p, node* c, usint32 count)
	{
		params = new node(p->filename, p->line);
		params->clone(p); //克隆出所有列表

		context = new node(c->filename, c->line);
		context->clone(c); //克隆出所有列表

		varcount = count;


#ifdef CPPS_JIT_COMPILER
		jitbuffer = NULL;
		jitbufferSize = 0;
#endif // CPPS_JIT_COMPILER

		domain = d;
	}

	cpps_cppsfunction::~cpps_cppsfunction()
	{
		cpps_destory_node(params); delete params; params = NULL;
		cpps_destory_node(context); delete context; context = NULL;
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

		//l 的0 代表返回值列表 1.代表参数列表

		//if (o->size() > params->l.size())
		//{
			//throw("The number of parameters does not match");
		//}

		for (size_t i = 0; i < params->l.size(); i++)
		{

			node* d = params->l[i];
			node* varname = d->l[0];

			if (varname->type == CPPS_VARNAME)
			{
				cpps_regvar* v = new cpps_regvar();
				v->setvarname(varname->s);

				if (i >= o->size())
				{
					if (i < varname->l.size())
					{
						node* var = varname->l[0]; //默认参数。。。 如果穿进来则不执行默认参数
						cpps_domain* leftdomain = NULL;
						cpps_value value = cpps_calculate_expression(c, prevdomain, prevdomain, var, leftdomain);
						v->setval(value);
					}
				}
				else
				{
					cpps_value value = (*o)[i];
					v->setval(value);
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
		cpps_step_all(c, CPPS_MUNITRET, funcdomain, funcdomain, context);

		if (ret)
			*ret = funcdomain->funcRet;//return的值反馈回去
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

}
