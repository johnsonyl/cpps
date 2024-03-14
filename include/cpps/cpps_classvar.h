#ifndef CPPS_CLASSVAR_CPPS_HEAD_
#define CPPS_CLASSVAR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/10 (yy/mm/dd)
//@Module		:	CPPS_CLASSVAR
//@Description	:	Cpps类对象
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	struct cpps_cppsclassvar : public cpps_domain , public cpps_gcobject
	{
		cpps_cppsclassvar(const char* clsname, cpps_domain* p, char type, bool alloc);
		virtual ~cpps_cppsclassvar(){}
		virtual void* getclsptr();
		virtual void setclsptr(void* p);
		virtual bool isallocclass();
		virtual void release();
		virtual size_t size();
		virtual cpps_cppsclass* getcppsclass();
		bool isalloc;
		int usecount;
	};
	template<class CLS>
	struct cpps_classvar : public cpps_cppsclassvar
	{
		cpps_classvar(const char* clsname,cpps_domain* p, char type, bool alloc)
			:cpps_cppsclassvar(clsname,p, type, alloc)
		{
			__class = NULL;
			if (alloc)	__class = CPPSNEW( CLS)();//暂时不支持带参数的构造函数
		}
		virtual ~cpps_classvar(){}
		virtual void *getclsptr(){ return (void *)__class; }
		virtual void release() {
			if (__class && isallocclass()) CPPSDELETE( __class );
			__class = NULL; 
			CPPSDELETE( this);
		}
		virtual void setclsptr(void* p) { __class = (CLS*)p; }
		virtual size_t size() { return sizeof(*this) + ((__class && isallocclass()) ? sizeof(*__class) : 0); }
		virtual void destory(C* c, bool isclose = false) {
			cpps_domain::destory(c);
			if (__class && isallocclass()) {
				c->_classvarlock->lock();
				c->_class_map_classvar.erase(__class);
				c->_classvarlock->unlock();
			}
		}
		CLS *__class;
	};

}

#endif // CPPS_CLASSVAR_CPPS_HEAD_