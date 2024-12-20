#ifndef CPPS_MODULE_CPPS_HEAD_
#define CPPS_MODULE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_MODULE
//@Description	:	Cpp注册到Cpps接口
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	template<class R, class CLS>
	cpps_regfunction* make_regfunction2(std::string func, R(CLS::* f)(C* c, cpps::cpps_value args, ...), bool isasync, bool isoperator);
	void cpps_debug_trace_domain(C* c, int kg, std::string parent, cpps_domain* root);
	struct cpps_reg_class
	{
		cpps_reg* f;
	};

	struct regxmodule
	{

		regxmodule(cpps_reg_class c)
			:f(c.f)
		{
			f->isneedC = false;
		}
		regxmodule( cpps_regfunction* f,bool b )
			:f(f)
		{
			f->isneedC = b;
		}

		regxmodule(cpps_reggvar* f, bool b)
			:f(f)
		{
			f->isneedC = b;
		}
		regxmodule 	operator ,(regxmodule c)
		{
			if (!f->next)
			{
				f->next = c.f;
			}
			else
			{
				cpps_reg* next = f->next;
				while (next->next)
					next = next->next;
			
				next->next = c.f;
			}
			return *this;
		}
		cpps_reg* f;
	};
	struct _enum : public cpps_reg_class 
	{
		_enum(C*cstate,std::string name)
		{
			c = cstate;
			_enum_domain = CPPSNEW(cpps_domain)(NULL, cpps_domain_type_enum, name.c_str());
			f = CPPSNEW(cpps_regenum)(name, _enum_domain);
		}
		_enum& value(std::string varname, cpps_integer v);
		cpps_domain* _enum_domain;
		C* c;
	};
	template< class CLS>
	struct _class : public cpps_reg_class
	{
		_class(std::string name)
		{
			//assert(!cpps_class_singleton<CLS*>::instance()->getcls());

			_cls = CPPSNEW( cpps_class<CLS>)(name,NULL, cpps_domain_type_class);
			f = CPPSNEW( cpps_regclass_template<CLS>)(name, _cls);

			cpps_class_singleton<CLS*>::instance()->setsls(_cls);
		}
		template<class F>
		_class<CLS>& base()
		{
			assert(cpps_class_singleton<F*>::instance()->getcls());

			cpps_reg* r = make_parentclass(cpps_class_singleton<F*>::instance()->getcls());
			r->isneedC = false;
			_cls->regfunc(r);
			return *this;
		}
		template<class F>
		_class<CLS>& 	def(std::string func, F _f, bool isasync = false)
		{
			cpps_reg* r = make_regfunction(func, _f,isasync);
			r->isneedC = false;
			_cls->regfunc( r );
			return *this;
		}

		template<class F>
		_class<CLS>&  defvar(C *c, std::string name, F v)
		{
			cpps_reg* r = make_regvar(name, cpps_cpp_to_cpps_converter<F>::apply(c, v));
			r->isneedC = false;
			_cls->regfunc(r);
			return *this;
		}
		_class<CLS>& def_classvar( std::string name, object CLS::* v)
		{
			cpps_reg* r = make_regclassvar(name,v);
			_cls->regfunc(r);
			return *this;
		}

		template<class F>
		_class<CLS>& 	def_inside(std::string func, F _f, bool isasync = false)
		{
			cpps_reg* r = make_regfunction(func, _f,isasync);
			r->isneedC = true;
			_cls->regfunc(r);
			return *this;
		}

		template<class F>
		_class<CLS>& def_operator(std::string func, F _f)
		{
			cpps_reg* r = make_regfunction(func, _f, false,true);
			r->isneedC = false;
			_cls->regfunc(r);
			return *this;
		}
		template<class F>
		_class<CLS>& def_operator_inside(std::string func, F _f)
		{
			cpps_reg* r = make_regfunction(func, _f, false, true);
			r->isneedC = true;
			_cls->regfunc(r);
			return *this;
		}

		regxmodule 	operator ,(regxmodule c)
		{
			regxmodule(*this).operator,(c);
			return *this;
		}
		cpps_class<CLS> *_cls;
	};

	struct _class2 : public cpps_reg_class
	{
		_class2(std::string name, cpps_class_alloc __alloc, cpps_class_free __free)
		{
			_cls = CPPSNEW(cpps_class2)(name, NULL, cpps_domain_type_class, __alloc, __free);
			f = CPPSNEW(cpps_regclass)(name, _cls);
			cpps_class_singleton<cpps_class_handler*>::instance()->setsls(_cls);
		}

		_class2& def_inside(std::string func,  cpps_class_func _f2, bool isasync = false)
		{
			cpps_reg* r = make_regfunction2(func, &cpps_class_handler::def_call, isasync,false);
			r->isneedC = true;
			_cls->regfunc(r);
			_cls->bind_func(func, _f2);
			return *this;
		}

		_class2& def_operator_inside(std::string func, cpps_class_func _f2)
		{
			cpps_reg* r = make_regfunction(func, &cpps_class_handler::def_operator_call, false, true);
			r->isneedC = true;
			_cls->regfunc(r);
			_cls->bind_func(func, _f2);
			return *this;
		}

		regxmodule 	operator ,(regxmodule c)
		{
			regxmodule(*this).operator,(c);
			return *this;
		}
		cpps_class2* _cls;
	};

	template<class F>
	regxmodule def(std::string func, F f, bool isasync = false)
	{
		return regxmodule(make_regfunction(func, f, isasync,false),false);
	}


	template<class F>
	regxmodule defvar(C *c,std::string name, F v)
	{
		return regxmodule(make_regvar(name, cpps_cpp_to_cpps_converter<F>::apply(c,v)), false);
	}




	template<class F>
	regxmodule def_inside(std::string func, F f, bool isasync = false)
	{
		return regxmodule(make_regfunction(func, f,isasync), true);
	}
	


	struct cpps_module
	{
		cpps_module(C* c,std::string _domain,bool isinit) //默认注册给_G
		{
			cState = c;
			domain = cState->_G;//默认是根节点
			if (isinit) {
				if (!_domain.empty())//如果定义名字了 那就是自己的一个。。。
				{
					cpps_domain* leftdomain = NULL;

					cpps_regvar* v = domain->getvar(_domain, leftdomain);
					if (!v)
					{
						cpps_domain* temp_domain = CPPSNEW(cpps_domain)(c->_G, cpps_domain_type_module, _domain.c_str());//创建根节点域


						v = CPPSNEW(cpps_regvar)();//_G 为根节点
						v->setvarname(_domain);
						auto temp = cpps_value(temp_domain);
						v->setval(temp); //域列表会copy进去
						domain->regvar(NULL, v); //将自己注册成_G..
						v->setsource(true);
					}

					domain = v->getval().value.domain;
				}
			}
			else {
				if (!_domain.empty())//如果定义名字了 那就是自己的一个。。。
				{
					cpps_domain* leftdomain = NULL;

					cpps_regvar* v = domain->getvar(_domain, leftdomain);
					if (v)
					{
						cpps_domain* temp_domain = v->getval().value.domain;
						if (v->getval().isdomain()) {
							temp_domain->destory(c, true);
						}
						temp_domain->release();
						domain->unregvar(c, v);
						v->release();
					}
				}
			}
			
		}

		void	operator [](regxmodule m)
		{
			domain->regfunc(m.f,cState);
		}

	public:
		C					*cState;
		cpps_domain			*domain;	//根节点
	};


	cpps_module _module(C* c, std::string _domain = "");
	void		_unmodule(C* c, std::string _domain);


}

#endif // CPPS_MODULE_CPPS_HEAD_