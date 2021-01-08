#ifndef CPPS_MODULE_CPPS_HEAD_
#define CPPS_MODULE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_MODULE
//@Description	:	Cppע�ᵽCpps�ӿ�
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	template<class R>
	cpps_regfunction* make_regfunction(std::string func, R(*f)(const char* fmt, ...), bool isasync = false);
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

	template< class C>
	struct _class : public cpps_reg_class
	{
		_class(std::string name)
		{
			_cls = new cpps_class<C>(name,NULL, cpps_domain_type_class);
			f = new cpps_regclass_template<C>(name, _cls);
			cpps_class_singleton<C*>::instance()->setsls(_cls);
		}
		template<class F>
		_class<C>& 	def(std::string func, F _f, bool isasync = false)
		{
			cpps_reg* r = make_regfunction(func, _f,isasync);
			r->isneedC = false;
			_cls->regfunc( r );
			return *this;
		}

		template<class F>
		_class<C>&  defvar(C *c, std::string name, F v)
		{
			cpps_reg* r = make_regvar(name, cpps_cpp_to_cpps_converter<F>::apply(c, v));
			r->isneedC = false;
			_cls->regfunc(r);
			return *this;
		}

		template<class F>
		_class<C>& 	def_inside(std::string func, F _f, bool isasync = false)
		{
			cpps_reg* r = make_regfunction(func, _f,isasync);
			r->isneedC = true;
			_cls->regfunc(r);
			return *this;
		}

		regxmodule 	operator ,(regxmodule c)
		{
			regxmodule(*this).operator,(c);
			return *this;
		}
		cpps_class<C> *_cls;
	};

	template<class F>
	regxmodule def(std::string func, F f, bool isasync = false)
	{
		return regxmodule(make_regfunction(func, f, isasync),false);
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
		cpps_module(C* c,std::string _domain) //Ĭ��ע���_G
		{
			cState = c;
			domain = cState->_G;//Ĭ���Ǹ��ڵ�
			if (!_domain.empty())//������������� �Ǿ����Լ���һ��������
			{
				cpps_domain* leftdomain = NULL;

				cpps_regvar * v = domain->getvar(_domain,leftdomain);
				if (!v)
				{
					cpps_domain *temp_domain = new cpps_domain(c->_G, cpps_domain_type_module, _domain.c_str());//�������ڵ���


					v = new cpps_regvar();//_G Ϊ���ڵ�
					v->setvarname(_domain);
					v->setval(cpps_value(temp_domain)); //���б��copy��ȥ
					domain->regvar(NULL,v); //���Լ�ע���_G..
				}
				domain = v->getval().value.domain;
			}
		}

		void	operator [](regxmodule m)
		{
			domain->regfunc(m.f);
		}

	public:
		C					*cState;
		cpps_domain			*domain;	//���ڵ�
	};


	cpps_module _module(C* c, std::string _domain = "");


}

#endif // CPPS_MODULE_CPPS_HEAD_