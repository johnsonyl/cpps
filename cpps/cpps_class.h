#ifndef CPPS_CLASS_CPPS_HEAD_
#define CPPS_CLASS_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/12/10 (yy/mm/dd)
//@Module		:	CPPS_CLASS
//@Description	:	Cpps、C++类注册
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct cpps_cppsclassvar;
	template <class C>
	struct cpps_classvar;
	struct cpps_cppsclass : public cpps_domain
	{
		cpps_cppsclass(std::string _classname, Node *_o, cpps_domain* p, char type)
		:cpps_domain(p,type,_classname)
		{
			o = _o;
			classname = _classname;
		}
		virtual cpps_cppsclassvar *	create(bool alloc = true)
		{
			return (new cpps_cppsclassvar(getClassName(), this, cpps_domain_type_classvar, alloc));
		}
		std::string		getClassName()
		{
			return classname;
		}
		std::vector<cpps_cppsclass*>& parentClassList() { return _parentClassList; }
		Node		*o; //定义的变量
		std::string classname;
		std::vector<cpps_cppsclass*> _parentClassList;
	};

	template <class C>
	struct cpps_class : public cpps_cppsclass
	{
		cpps_class(std::string _classname,cpps_domain* p, char type)
			:cpps_cppsclass(_classname, NULL, p, type)
		{
		}
		virtual cpps_cppsclassvar *	create(bool alloc = true)
		{
			return new cpps_classvar<C>(getClassName(),this, cpps_domain_type_classvar, alloc);
		}
	};

	template <class T>
	struct cpps_class_singleton
	{
		cpps_class_singleton()
		{
			v = NULL;
		}
		void		setsls(cpps_cppsclass *_v)
		{
			v = _v;
		}
		cpps_cppsclass *getcls()
		{
			return v;
		}

		static cpps_class_singleton<T>* getSingletonPtr()
		{
			static cpps_class_singleton<T> t;
			return &t;
		}
		cpps_cppsclass	*v;
	};

}

#endif // CPPS_CLASS_CPPS_HEAD_