#ifndef CPPS_CLASS_CPPS_HEAD_
#define CPPS_CLASS_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
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
		cpps_cppsclass(std::string _classname, node *_o, cpps_domain* p, char type)
		:cpps_domain(p,type,_classname){ o = _o;classname = _classname;	}
		virtual cpps_cppsclassvar *			create(C* c, bool alloc = true){ return (new cpps_cppsclassvar(getclassname(), this, cpps_domain_type_classvar, alloc)); }
		virtual bool						iscppsclass() { return true; }
		std::string							getclassname(){	return classname; }
		std::vector<cpps_cppsclass*>&		parentclasslist() { return _parentclasslist; }
		node		*o; //定义的变量
		std::string classname;
		std::vector<cpps_cppsclass*> _parentclasslist;
	};

	template <class T>
	struct cpps_class : public cpps_cppsclass
	{
		cpps_class(std::string _classname,cpps_domain* p, char type)
			:cpps_cppsclass(_classname, NULL, p, type){	}
		virtual bool					iscppsclass() { return false; }
		virtual cpps_cppsclassvar*		create(C* c, bool alloc = true)
		{
			cpps_classvar<T>* v = new cpps_classvar<T>(getclassname(), this, cpps_domain_type_classvar, alloc);
			if (alloc){
				cpps_cppsclassvar* class_var = (cpps_cppsclassvar* )v;
				if(c) c->_class_map_classvar.insert(phmap::flat_hash_map<void*, cpps_cppsclassvar*>::value_type(v->_class, class_var));
			}
			return v;
		}
	};
	template <class T>
	struct cpps_class_singleton
	{
		cpps_class_singleton(){	v = NULL; }
		void							setsls(cpps_cppsclass *_v){	v = _v;	}
		cpps_cppsclass *				getcls(){ return v; }
		static cpps_class_singleton<T>* instance(){	static cpps_class_singleton<T> t; return &t;}
		cpps_cppsclass	*v;
	};

	
}

#endif // CPPS_CLASS_CPPS_HEAD_