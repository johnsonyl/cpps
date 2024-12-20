#include "cpps/cpps.h"

namespace cpps
{

	cpps_domain::cpps_domain()
	{
		//do nothing...
		domainType = 0;
		hasVar = false;
		isbreak = cpps_step_check_none;
		parent[0] = NULL;
		parent[1] = NULL;
		funcRet.tt = CPPS_TNIL;
		stacklist = NULL;
		varList = NULL;
		offset = -1;
		offsettype = -1;
		parentclassoffset = NULL;
	}

	cpps_domain::cpps_domain(cpps_domain* p, char type, const char* name) 
	{
		init(p, type, name);
	}

	cpps_domain::~cpps_domain()
	{
		if (stacklist) {
			CPPSDELETE( stacklist);
		}
		if (parentclassoffset) {
			CPPSDELETE( parentclassoffset);
		}
		if (varList) {
			CPPSDELETE(varList);
		}
	}

	cpps::cpps_cppsclassvar* cpps_domain::create(C* c, bool alloc /*= true*/)
	{
		return NULL;
	}

	void cpps_domain::init(cpps_domain* p, char type, const char* name)
	{
		parent[0] = p;
		domainType = type;
		isbreak = cpps_step_check_none;
		parent[1] = NULL;
		strcpy(domainname, name);
		funcRet.tt = CPPS_TNIL;
		hasVar = false;
		stacklist = NULL;
		offset = -1;
		offsettype = -1;
		parentclassoffset = NULL;
		varList = NULL;
	}

	void cpps_domain::init(cpps_domain* p, char type)
	{
		parent[0] = p;
		domainType = type;
		isbreak = cpps_step_check_none;
		parent[1] = NULL;
		funcRet.tt = CPPS_TNIL;
		hasVar = false;
		stacklist = NULL;
		offset = -1;
		offsettype = -1;
		varList = NULL;
		parentclassoffset = NULL;
	}

	std::string cpps_domain::getdomainname()
	{
		return domainname;
	}

	void cpps_domain::regfunc(cpps_reg* f,cpps::C* c)
	{
		if (varList == NULL) varList = CPPSNEW(VARLIST)();

		hasVar = true;
		do
		{
			cpps_domain* leftdomain = NULL;
			cpps_regvar* var = NULL;
			if (!f->varname.empty()) {
				var = getvar(f->varname, leftdomain, false, true);;
				if (!var) {
					var = CPPSNEW (cpps_regvar)();
					var->setvarname(f->varname);
					var->setconst(true);

					varList->insert(VARLIST::value_type(var->varName, var));
				}
			}
			if (f->type == cpps_def_regfunction)
			{
				cpps_regfunction* func = (cpps_regfunction*)f;
				if (func->func->isoperator()) {
					/*cpps_symbol* symbol = cpps_parse_getsymbol(c ,f->varname, false);
					if (!symbol){
						throw cpps_error(__FILE__, __LINE__, cpps_error_normalerror, "%s operator symbol is not support.", f->varname.c_str());
					}*/
					cpps_cppsclass* cppsclass = (cpps_cppsclass*)this;
					cppsclass->operatorreg(f->varname, func->func);
				}
				auto tmp = cpps_value(func->func);
				var->setval(tmp);
				var->setconst(true);
				var->setsource(true);
				func->func->setIsNeedC(f->isneedC);
			}
			else if (f->type == cpps_def_regclass)
			{
				cpps_regclass* cls = (cpps_regclass*)f;
				cls->cls->setDefaultCState(c);
				auto tmp = cpps_value(cls->cls);
				var->setval(tmp);
				var->setsource(true);
				var->setconst(true);
			}
			else if (f->type == cpps_def_regvar)
			{
				var->setval(f->value);
			}
			else if (f->type == cpps_def_regparentclass)
			{
				leftdomain = NULL;
				cpps_regparentclass* _regparentclass = (cpps_regparentclass*)f;
				cpps_cppsclass* _parent = _regparentclass->__cppsclass;

				if (_parent->varList != NULL) {
					for (auto _var : *_parent->varList) {
						auto __var = _var.second;

						leftdomain = NULL;
						auto __self_var = getvar(__var->getvarname(), leftdomain, true, false);
						if (!__self_var) {
							__self_var = CPPSNEW(cpps_regvar)();
							__self_var->setvarname(__var->getvarname());
							__self_var->setsource(false);
							__self_var->setconst(true);
							varList->insert(VARLIST::value_type(__var->getvarname(), __self_var));
						}
						__self_var->setsource(false);
						__self_var->setval(__var->getval());
					}
				}
				
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)this;

				//注册父类的operator.
				for (auto v : _parent->operatorlist) {
					cppsclass->operatorlist[v.first] = v.second;
				}
			}
			else if (f->type == cpps_def_regenum)
			{
				cpps_regenum* _regenum = (cpps_regenum*)f;
				auto tmp = cpps_value(_regenum->_enum_domain);
				var->setval(tmp);
				var->setsource(true);
				var->setconst(true);
			}
			else if (f->type == cpps_def_regclassvar) {
				cpps_reg_class_var* _clsvar = (cpps_reg_class_var*)f;
				_clsvar->_v->setclass((cpps_cppsclass*)this);
				auto tmp = cpps_value(_clsvar->_v);
				var->setval(tmp);
				var->setsource(true);
				var->setconst(false);
			}
			cpps_reg* take = f;
			f = f->next;
			take->release();
		} while (f);
	}

	cpps::cpps_regvar* cpps_domain::getvar(std::string s, cpps_domain*& leftdomain, bool findparent /*= true*/, bool filterroot)
	{

		cpps_regvar* ret = NULL;
		if (hasVar) {
			VARLIST::iterator it = varList->find(s);
			if (it != varList->end())
			{
				ret = it->second;
				return ret;
			}
		}
		if (findparent)
		{
			if (parent[1] )
			{
				if (parent[1]->domainType != cpps_domain_type_root || !filterroot) {
					ret = parent[1]->getvar(s, leftdomain, findparent, filterroot);
					if (ret) return ret;
				}
			}
			if (parent[0])
			{
				if (parent[0]->domainType != cpps_domain_type_root || !filterroot) {
					ret = parent[0]->getvar(s, leftdomain,findparent,filterroot);
					if (leftdomain == NULL && parent[0]->domainType == cpps_domain_type_class)
					{
						leftdomain = this;
					}

					if (ret) return ret;
				}
				
			}
		}



		return ret;
	}

	void cpps_domain::regvar(C* c, cpps_regvar* v)
	{
		if (varList == NULL) varList = CPPSNEW(VARLIST)();
		hasVar = true;
		lock();
		varList->insert(VARLIST::value_type(v->varName, v));
		unlock();
		if (c != NULL && this != c->_G) cpps_gc_add_barrier(c, v);
	}

	void cpps_domain::unregvar(C* c, cpps_regvar* v)
	{
		lock();
		varList->erase(v->varName);
		unlock();
		cpps_gc_remove_barrier(c, v);
	}

	void cpps_domain::setexecdomain(cpps_domain* exec)
	{
		parent[1] = exec;
	}
	void cpps_domain::cleanup()
	{
		if (hasVar) {
			for (VARLIST::iterator it = varList->begin(); it != varList->end(); ++it)
			{
				cpps_regvar* v = it->second;
				v->release();
			}
			hasVar = false;
			varList->clear();
		}
		
		if (stacklist != NULL)
		{
			stacklist->clear();
			CPPSDELETE(stacklist);
			stacklist = NULL;
		}
	}
	void cpps_domain::clear_var_real(C* c, bool isclose)
	{
		for (VARLIST::iterator it = varList->begin(); it != varList->end(); ++it)
		{
			cpps_regvar* v = it->second;
			if ((!v->closeure || v->closeureusecount <= 0) || isclose) { /*闭包不删除,但是必须有人使用*/
				cpps_gc_remove_barrier(c, v);
				if (!isclose && v->stackdomain && v->offset != -1) {
					v->stackdomain->removeidxvar(v->offset);
				}
				if (v->issource() && v->getval().tt == CPPS_TCLASS) {
					cpps_cppsclass* _cls = cpps_to_cpps_cppsclass(v->getval());
					_cls->destory(c, isclose);
					_cls->release();//class 会随着变量消失而删除.
				}
				else if (v->issource() && v->getval().tt == CPPS_TFUNCTION)
				{
					cpps_function* func = (cpps_function*)v->getval().value.func;
					if (domainType == cpps_domain_type_class && func->getclass()) { //判断是否是继承来的 cpp 注册进来的类不需要判断 删就完了.
						cpps_cppsclass* pclsthis = (cpps_cppsclass*)this;
						if (pclsthis == func->getclass()) {
							func->release();//类里面的函数也要被清理
						}
					}
					else {
						func->release();
					}
				}
				else if (isclose && v->issource() && v->getval().tt == CPPS_TDOMAIN && v->getval().value.domain != this && v->getval().value.domain != c->_G)
				{
					cpps_domain* domain = v->getval().value.domain;
					domain->destory(c, isclose);
					domain->release();
				}
				v->release();
			}
		}
		varList->clear();
		hasVar = false;
	}
	void cpps_domain::destory(C* c,bool isclose)
	{
		funcRet.decruse();
		funcRet.tt = CPPS_TNIL;
		clear_var(c, isclose);
		isbreak = cpps_step_check_none;
		parent[0] = NULL;
		parent[1] = NULL;
	}
	void cpps_domain::regidxvar(int32 offset, cpps_regvar* v)
	{
		if (stacklist == NULL)
		{
			stacklist = CPPSNEW( std::vector< cpps_regvar*>)();
		}
		if (offset < (int32)stacklist->size())
			(*stacklist)[offset] = v;
		else
			stacklist->push_back(v);

		v->stackdomain = this;
	}

	void cpps_domain::removeidxvar(int32 offset)
	{
		if (stacklist == NULL)
		{
			return;
		}
		(*stacklist)[offset] = NULL;
	}

	cpps::int32 cpps_domain::getidxoffset(cpps_domain* parentclass)
	{
		if (this == parentclass) return 0;
		if (parentclassoffset == NULL)
		{
			return 0;
		}
		auto it = parentclassoffset->find(parentclass);
		if (it == parentclassoffset->end())
		{
			return 0;
		}
		return it->second;
	}

	void cpps_domain::setidxoffset(cpps_domain* parentclass, int32 off)
	{
		if (parentclassoffset == NULL)
		{
			typedef phmap::flat_hash_map<cpps_domain*, int32> _Type;
			parentclassoffset = CPPSNEW(_Type )();
		}
		parentclassoffset->insert(phmap::flat_hash_map<cpps_domain*, int32>::value_type(parentclass, off));
	}

	void cpps_domain::resize(usint16 size)
	{
		if (size == 0) return;

		if (size == 65535) return;

		if (stacklist == NULL)
		{
			typedef std::vector< cpps_regvar*> _Type;
			stacklist = CPPSNEW(_Type)();
		}
		stacklist->resize(size);
	}

	void cpps_domain::release() { CPPSDELETE(this); }

	void cpps_domain::lock() { _lock.lock(); }

	void cpps_domain::unlock() { _lock.unlock(); }

	void cpps_domain::lock_shared() { _lock.lock_shared(); }

	void cpps_domain::unlock_shared() { _lock.unlock_shared(); }

	void cpps_domain::clone(cpps_domain* clone_domain)
	{
		if (clone_domain->stacklist && !clone_domain->stacklist->empty()) {
			resize((usint16)clone_domain->stacklist->size());
			for (size_t i = 0; i < clone_domain->stacklist->size(); i++)
				(* stacklist)[i] = (*clone_domain->stacklist)[i];
		}
		clone_domain->lock_shared();
		if (clone_domain->varList != NULL) {
			if (varList == NULL) varList = CPPSNEW(VARLIST)();

			for (VARLIST::iterator it = clone_domain->varList->begin(); it != clone_domain->varList->end(); ++it)
			{
				cpps_regvar* v = it->second;
				const std::string& name = it->first;
				if (name != "_G") {
					cpps_regvar* v2 = CPPSNEW(cpps_regvar)();
					v2->ref(v);
					if (v2->offset != -1) regidxvar(v2->offset, v2);
					varList->insert(VARLIST::value_type(name, v2));
					hasVar = true;
				}
			}
		}
		
		clone_domain->unlock_shared();
	}
}
