#ifndef CPPS_DOMAIN_CPPS_HEAD_
#define CPPS_DOMAIN_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	域空间
//@website		:	http://cppscript.org
//==================================



namespace cpps
{

	void					cpps_gc_remove_barrier(C*c, cpps_regvar *v);
	void					cpps_gc_add_barrier(C*c, cpps_regvar *v);
	typedef					phmap::flat_hash_map<std::string, cpps_regvar*>	VARLIST;

	struct cpps_domain : public cpps_gcobject
	{
		cpps_domain();
		cpps_domain(cpps_domain* p, char type, const char* name);
		virtual ~cpps_domain();
		virtual cpps_cppsclassvar *							create(C* c, bool alloc = true);
		void												init(cpps_domain* p, char type, const char* name);
		void												init(cpps_domain* p, char type);
		std::string											getdomainname();
		void												regfunc( cpps_reg* f, cpps::C* c = NULL);
		cpps_regvar*										getvar(std::string s, cpps_domain *&leftdomain, bool findparent = true,bool filterroot = false);
		void												regvar(C *c,cpps_regvar * v);
		void												unregvar(C *c, cpps_regvar * v);
		void												setexecdomain(cpps_domain *exec);
		void												cleanup();
		void												clear_var_real(C* c, bool isclose);
		inline void											clear_var(C* c, bool isclose = false)
		{
			if (hasVar || (isclose && varList)) {
				clear_var_real(c, isclose);
			}
			if (stacklist != NULL)
			{
				stacklist->clear();
				CPPSDELETE(stacklist);
				stacklist = NULL;
			}
			isbreak = cpps_step_check_none;
		}
		virtual void										destory(C* c, bool isclose = false);
		void												regidxvar(int32 offset, cpps_regvar* v);
		void												removeidxvar(int32 offset);
		inline cpps_regvar*									getregidxvar(int32 offset)
		{
			cpps_regvar* it = NULL;
			if (stacklist != NULL) {
				 it = (* stacklist)[offset];
			}
			return it;
		}
		int32												getidxoffset(cpps_domain* parentclass);
		void												setidxoffset(cpps_domain* parentclass, int32 off);
		void												resize(usint16 size);
		virtual void										release();
		void												lock();
		void												unlock();
		void												lock_shared();
		void												unlock_shared();
		inline size_t										getvarcount() { return hasVar ? varList->size() : 0; }

		cpps_domain											*parent[2]; // 0为父域， 1为执行域
		char												domainType;
		VARLIST												*varList;
		cpps_value											funcRet;//当他是一个func域的时候作为返回值用的 将来或许还有别的用途 暂时起名为funcRet
		int8												isbreak;//有可能其他地方让我这个执行集退出执行
		char												domainname[255]; //域名字
		bool												hasVar;
		std::vector< cpps_regvar*>							*stacklist;
		int32												offset;
		int32												offsettype;
		phmap::flat_hash_map<cpps_domain*, int32>*			parentclassoffset; //基类偏移
		cpps_lock											_lock;
		void clone(cpps_domain* clone_domain);
	};

}

#endif // CPPS_DOMAIN_CPPS_HEAD_