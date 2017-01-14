#include "cpps.h"

///(ㄒoㄒ)/~~  这个没有能坚持写下来，有机会再来写吧。

namespace cpps
{
	//函数体有几个参数
	//参数1 C *c   rsp + 30h
	//参数2 cpps_domain *domain  rsp + 38h  //域里包含 此函数其他参数变量
	//参数3 cpps_stack *stack rsp + 40h

	//编译为机器码
	void cpps_jit_compiler::compiler(C *c,cpps_domain *domain, Node *p)
	{
		printf("开始编译函数.\n");

		for (std::vector<Node*>::iterator it = p->l.begin(); it != p->l.end(); ++it)
		{
			Node *o = *it;
			switch (o->type)
			{
			case CPPS_ODOFUNCTION:
				compiler_dofunction(c, domain, o);
				break;
			default:
				break;
			}
		}
	}
	std::vector<cpps_value>* cpps_jit_createparams(cpps_stack *stack)
	{
		return stack->createparams();
	}
	cpps_value* cpps_jit_create_cpps_value(cpps_stack *stack)
	{
		return stack->create_cpps_value();
	}
	void					cpps_jit_callfunction(cpps_function *f, C *c, cpps_value *ret, cpps_domain *domain, std::vector<cpps_value> *o, cpps_stack *stack = NULL)
	{
		f->callfunction(c, ret, domain, o, stack);
	}
	//已经失效了，因为解析层已经修改了 此函数需要重写了。
	void cpps_jit_compiler::compiler_dofunction(C * c,cpps_domain * domain, Node * o)
	{
		//检测编译异常
		cpps_regvar *reg = domain->getVar(o->s);
		if (!reg)
		{
			throw("JIT ERROR：要执行的函数变量不存在");
		}
		if (reg->getValue().tt != CPPS_TFUNCTION)
		{
			throw("JIT ERROR：需要执行的变量不是函数，不能被执行");
		}
		mov_rcx_rsp_off(0x40); //获取 stack参数
		call_func((usint64)cpps_jit_create_cpps_value); //返回值暂时存到rax里

		usint32 ret_offset = reg_stack_size(sizeof(usint64));
		mov_rsp_off_rax(ret_offset);


		cpps_function *f = reg->getValue().value.func;
		mov_rcx_rsp_off(0x40); //获取 stack参数
		call_func((usint64)cpps_jit_createparams); //返回值暂时存到rax里

		usint32 vector_offset = reg_stack_size(sizeof(usint64));
		mov_rsp_off_rax(vector_offset);

		//编译参数
		for (std::vector<Node*>::iterator it2 = o->l.begin(); it2 != o->l.end(); ++it2)
		{
			Node *v = *it2;
			compiler_params(c, domain, v);
			compiler_vector_pushback(c, domain, v, vector_offset);
		}


		//传参数进执行函数的函数
		mov_rax_rsp_off(0x40);   
		mov_rsp_off_rax(0x28);
		mov_rax_rsp_off(vector_offset);
		mov_rsp_off_rax(0x20);
		mov_r9_rsp_off(0x38);
		mov_r8_rsp_off(ret_offset);
		mov_rdx_rsp_off(0x30);
		mov_rcx_ll((usint64)f);
		call_func((usint64)cpps_jit_callfunction);

		//设置反馈值
		mov_rax_rsp_off(ret_offset);
	}

	void cpps_jit_vector_push_back(std::vector<cpps_value>* vector, cpps_value* value)
	{
		vector->push_back(*value);
	}




	cpps_value* cpps_jit_interger_to_value(cpps_value* value, cpps_integer i)
	{
		value->tt = CPPS_TINTEGER;
		value->value.integer = i;
		return value;
	}
	cpps_value* cpps_jit_number_to_value(cpps_value* value, cpps_number* n)
	{
		value->tt = CPPS_TNUMBER;
		value->value.number = *n;
		return value;
	}
	cpps_value* cpps_jit_string_to_value(cpps_value* value, const char* s)
	{
		value->tt = CPPS_TSTRING;
		value->str = s;
		return value;
	}
	cpps_value* cpps_jit_bool_to_value(cpps_value* value, bool b)
	{
		value->tt = CPPS_TBOOLEAN;
		value->value.b = b;
		return value;
	}
	
	cpps_value* cpps_jit_classvar_to_value(cpps_value* value, cpps_cppsclassvar* s)
	{
		value->tt = CPPS_TCLASSVAR;
		value->value.domain = s;
		return value;
	}
	cpps_value* cpps_jit_get_regvar_value(cpps_value* value, cpps_regvar* var)
	{
		*value = var->getValue();
		return value;
	}
	char* cpps_jit_create_string(const char *str)
	{
		char *newstr = new char[strlen(str)+1];
		strcpy(newstr,  str);
		return newstr;
	}
	cpps_number* cpps_jit_create_number(cpps_number number)
	{
		cpps_number *pNumber = new cpps_number(number);
		return pNumber;
	}
	void cpps_jit_compiler::compiler_vector_pushback(C * c, cpps_domain * domain, Node * v, usint32 vector_offset)
	{
		mov_rdx_rax();
		mov_rcx_rsp_off(vector_offset);
		call_func((usint64)cpps_jit_vector_push_back);
	}
	cpps_cppsclassvar * cpps_jit_new_var(C *c,cpps_cppsclass *cppsclass)
	{
		cpps_cppsclassvar *cppsclassvar = cppsclass->create();
			
		if (cppsclass->o)
			cpps_step_all(c, CPPS_SINGLERET, cppsclassvar, cppsclass->o->getright(),false);

		cpps_regvar * v = new cpps_regvar();//_G 为根节点
		v->setVarName("this");
		v->setValue(cpps_value(cppsclassvar)); //域列表会copy进去
		cppsclassvar->regVar(v);

		return cppsclassvar;
	}
	void	cpps_jit_resize_vector(cpps_vector *vector,usint64 size)
	{
		vector->resize(size);
	}
	void cpps_jit_compiler::compiler_params(C * c, cpps_domain * domain, Node * v)
	{
		mov_rcx_rsp_off(0x40); //获取 stack参数
		call_func((usint64)cpps_jit_create_cpps_value); //返回值暂时存到rax里

		if (v->type == CPPS_OINTEGER)
		{
 			cpps_integer value;
 			cpps_str2i64(v->s.c_str(), &value);
			
			mov_rdx_ll(value);
			mov_rcx_rax();
			call_func((usint64)cpps_jit_interger_to_value);
		}
		else if (v->type == CPPS_OSTR)
		{
			char *ptr = cpps_jit_create_string(v->s.c_str());

			mov_rdx_ll((usint64)ptr);
			mov_rcx_rax();
			call_func((usint64)cpps_jit_string_to_value);
		}
		else if (v->type == CPPS_ONUMBER)
		{
			cpps_number value;
			cpps_str2d(v->s.c_str(), &value);
			cpps_number* pNumber = cpps_jit_create_number(value);

			mov_rdx_ll((usint64)pNumber);
			mov_rcx_rax();
			call_func((usint64)cpps_jit_number_to_value);
		}
		else if (v->type == CPPS_ONEWVAR)
		{
			cpps_regvar *var = domain->getVar(v->s);
			if (!var || var->getValue().tt != CPPS_TCLASS)
			{
				throw(cpps_error(v->filename, v->line, cpps_error_classerror, " 【%s】 new 出的对象必须为类对象", v->s.c_str()));
			}

			usint32 cpps_value_offset = reg_stack_size(sizeof(usint64));
			mov_rsp_off_rax(cpps_value_offset);


			cpps_cppsclass *cppsclass = (cpps_cppsclass*)var->getValue().value.domain;
			mov_rdx_ll((usint64)cppsclass);
			mov_rcx_rsp_off(0x30);
			call_func((usint64)cpps_jit_new_var);

			usint32 classvar_offset = reg_stack_size(sizeof(usint64));
			mov_rsp_off_rax(classvar_offset);

			
			if (v->s == "vector" && v->getleft())
			{
				cpps_integer result;
				cpps_str2i64(v->getleft()->s.c_str(), &result);

				mov_rdx_ll(result);
				mov_rcx_rsp_off(classvar_offset);
				call_func((usint64)cpps_jit_resize_vector);
			}

			mov_rdx_rsp_off(classvar_offset); 
			mov_rcx_rsp_off(cpps_value_offset);
			call_func((usint64)cpps_jit_classvar_to_value);
		}
		else if (v->type == CPPS_OBRACKET)
		{
			if (v->getleft())
			{
				compiler_params(c, domain, v->getleft());
			}
		}
		else if (v->type == CPPS_OBOOL)
		{
			bool value = v->s.at(0) == 't';

			mov_rdx_ll(value);
			mov_rcx_rax();
			call_func((usint64)cpps_jit_bool_to_value);
		}
		else if (v->type == CPPS_VARNAME)
		{
			cpps_regvar *var = domain->getVar(v->s);
			if (!var)
			{
				return;
			}
			mov_rdx_ll((usint64)var);
			mov_rcx_rax();
			call_func((usint64)cpps_jit_get_regvar_value);
		}
		else if (v->type == CPPS_FUNCNAME)
		{
			compiler_dofunction(c, domain, v);
		}
		else if (v->type == CPPS_OGETOBJECT)
		{
			usint32 cpps_value_offset = reg_stack_size(sizeof(usint64));
			mov_rsp_off_rax(cpps_value_offset);

			compiler_params(c, domain, v->getleft());

			usint32 left_offset = reg_stack_size(sizeof(usint64));
			mov_rsp_off_rax(left_offset);

			switch (v->getright()->type )
			{
			case CPPS_OINTEGER:
				break;
			case CPPS_OSTR:
				break;
			case CPPS_ONUMBER:
				break;
			default:
				break;
			}

		}
	}
}