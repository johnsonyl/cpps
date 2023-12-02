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
	struct cpps_function;
	struct cpps_cppsclassvar;
	template <class C>
	struct cpps_classvar;
	void  cpps_destory_node(node* d);
	struct cpps_cppsclass : public cpps_domain
	{
		cpps_cppsclass(std::string _classname, node *_o, cpps_domain* p, char type);
		virtual ~cpps_cppsclass();
		virtual cpps_cppsclassvar *			create(C* c, bool alloc = true);
		virtual bool						iscppsclass();
		std::string							getclassname();
		std::vector<cpps_cppsclass*>&		parentclasslist();
		virtual void						release();
		virtual void						operatorreg(std::string symbolname, cpps_function* func);
		virtual cpps_function*				getoperator(std::string symbolname);
		node*								getvars_node();
		C*									getDefaultCState(){return _c;}
		void								setDefaultCState(C* c) { _c = c; }

		node* o; //定义的变量
		std::string classname;
		std::vector<cpps_cppsclass*>								_parentclasslist;
		phmap::flat_hash_map<std::string, cpps_function*>			operatorlist; //牺牲内存提高运行效率.
		C* _c; //放一个C的指针会不会更方便操作。
	};

	template <class T>
	struct cpps_class : public cpps_cppsclass
	{
		cpps_class(std::string _classname,cpps_domain* p, char type)
			:cpps_cppsclass(_classname, NULL, p, type){	}
		virtual ~cpps_class(){}
		virtual bool					iscppsclass() { return false; }
		virtual void					release() { CPPSDELETE( this); }
		virtual cpps_cppsclassvar* create(C* c, bool alloc = true)
		{
			cpps_classvar<T>* v = CPPSNEW( cpps_classvar<T>)(getclassname(), this, cpps_domain_type_classvar, alloc);
			if (alloc){
				cpps_cppsclassvar* class_var = (cpps_cppsclassvar* )v;
				
#ifdef _DEBUG
				if(c) c->_class_map_classvar.insert(std::map<void*, cpps_cppsclassvar*>::value_type(v->__class, class_var));
#else
				if (c) c->_class_map_classvar.insert(phmap::flat_hash_map<void*, cpps_cppsclassvar*>::value_type(v->__class, class_var));
#endif

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