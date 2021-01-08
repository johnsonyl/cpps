#ifndef CPPS_CLASSVAR_CPPS_HEAD_
#define CPPS_CLASSVAR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/10 (yy/mm/dd)
//@Module		:	CPPS_CLASSVAR
//@Description	:	Cpps�����
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	struct cpps_cppsclassvar : public cpps_domain
	{
		cpps_cppsclassvar(std::string clsname,cpps_domain* p, char type, bool alloc)
			:cpps_domain(p, type, clsname), isalloc(alloc){	usecount = 0;}
		virtual ~cpps_cppsclassvar(){}
		virtual void *getclsptr(){ return NULL; }
		virtual void setclsptr(void *p){ }
		virtual bool isallocclass() { return isalloc; }
		virtual size_t size() { return sizeof(*this); }
		bool isalloc;
		int usecount;
	};
	template<class CLS>
	struct cpps_classvar : public cpps_cppsclassvar
	{
		cpps_classvar(std::string clsname,cpps_domain* p, char type, bool alloc)
			:cpps_cppsclassvar(clsname,p, type, alloc)
		{
			_class = NULL;
			if (alloc)	_class = new CLS();//��ʱ��֧�ִ������Ĺ��캯��
		}
		virtual ~cpps_classvar()
		{
			if (_class && isallocclass()) delete _class;
		}
		virtual void *getclsptr(){ return (void *)_class; }
		virtual void setclsptr(void *p){ _class = (CLS*)p; }
		virtual size_t size() { return sizeof(*this) + (_class ? sizeof(*_class) : 0 ); }
		virtual void destory(C* c, bool isclose = false) {
			cpps_domain::destory(c);
			if (_class && isallocclass())
				c->_class_map_classvar.erase(_class);
		}
		CLS *_class;
	};

}

#endif // CPPS_CLASSVAR_CPPS_HEAD_