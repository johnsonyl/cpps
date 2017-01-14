#include "cpps.h"

namespace cpps
{
	void cpps_gc_add_barrier(C*c, cpps_regvar *v)
	{
		if (!v->getIsJoinBarrier())
		{
			v->setIsJoinBarrier(true);
			c->barrierList.insert(v);
		}
	}

	void cpps_gc_remove_barrier(C*c, cpps_regvar *v)
	{
		if (v->getIsJoinBarrier())
		{
			v->setIsJoinBarrier(false);
			c->barrierList.erase(v);
		}
	}

	void cpps_gc_add_gen0(C*c, cpps_cppsclassvar *p)
	{
		c->gen0size += p->size();
		//新增到新生代
		c->gen0.insert(p);
	}
	void cpps_gc_add_gen1(C*c, cpps_cppsclassvar *p)
	{
		c->gen1size += p->size();
		//新增到老生代
		c->gen1.insert(p);
	}
	void cpps_gc_check_gen_value(C*c, cpps_value &v, bool checkchild, std::unordered_set<cpps_cppsclassvar *> &oldgen, std::unordered_set<cpps_cppsclassvar *> &newgen, size_t &size);

	void cpps_gc_check_child(cpps_value &v, C* c, bool checkchild, std::unordered_set<cpps_cppsclassvar *> & oldgen, std::unordered_set<cpps_cppsclassvar *> & newgen, size_t &size)
	{
		for (std::unordered_map<std::string, cpps_regvar*>::iterator it = v.value.domain->varList.begin(); it != v.value.domain->varList.end(); ++it)
		{
			cpps_regvar *var = it->second;
			if (var->getValue().tt == CPPS_TCLASSVAR)
			{
				if(c->debug) printf("%s\n", var->varName.c_str());
				cpps_gc_check_gen_value(c, var->getValue(), checkchild, oldgen, newgen,size);
			}
		}
	}
	//
	void cpps_gc_check_gen_value(C*c, cpps_value &v, bool checkchild, std::unordered_set<cpps_cppsclassvar *> &oldgen, std::unordered_set<cpps_cppsclassvar *> &newgen, size_t &size)
	{
		if (v.tt == CPPS_TCLASSVAR)
		{
			if (v.value.domain->getDomainName() == "vector")
			{
				cpps_cppsclassvar *pClsVar = (cpps_cppsclassvar *)v.value.domain;
				cpps_vector *pVector = (cpps_vector*)pClsVar->getclsptr();
				for (pVector->begin(); pVector->end(); pVector->next())
				{
					cpps_value value = pVector->it();
					cpps_gc_check_gen_value(c, value, checkchild, oldgen, newgen, size);
				}

				std::unordered_set<cpps_cppsclassvar *>::iterator it = oldgen.find(pClsVar);
				if (it != oldgen.end())
				{
					oldgen.erase(it);
					newgen.insert(pClsVar);
					size += pClsVar->size();
					pClsVar->setGCLevel(1);//设置成老生代 哪怕也是老生代了 也设置一下
				}

			}
			else if (v.value.domain->getDomainName() == "map" )
			{
				cpps_cppsclassvar *pClsVar = (cpps_cppsclassvar *)v.value.domain;
				cpps_map *pMap = (cpps_map*)pClsVar->getclsptr();
				for (pMap->begin(); pMap->end(); pMap->next())
				{
					cpps_value value0 = pMap->key();
					cpps_value value1 = pMap->it();

					cpps_gc_check_gen_value(c, value0, checkchild, oldgen, newgen, size);
					cpps_gc_check_gen_value(c, value1, checkchild, oldgen, newgen, size);

				}

				std::unordered_set<cpps_cppsclassvar *>::iterator it = oldgen.find(pClsVar);
				if (it != oldgen.end())
				{
					oldgen.erase(it);
					newgen.insert(pClsVar);
					size += pClsVar->size();
					pClsVar->setGCLevel(1);//设置成老生代 哪怕也是老生代了 也设置一下
				}
			}
			else if (v.value.domain->getDomainName() == "unordered_map")
			{
				cpps_cppsclassvar *pClsVar = (cpps_cppsclassvar *)v.value.domain;
				cpps_unordered_map *pMap = (cpps_unordered_map*)pClsVar->getclsptr();
				for (pMap->begin(); pMap->end(); pMap->next())
				{
					cpps_value value0 = pMap->key();
					cpps_value value1 = pMap->it();

					cpps_gc_check_gen_value(c, value0, checkchild, oldgen, newgen, size);
					cpps_gc_check_gen_value(c, value1, checkchild, oldgen, newgen, size);

				}

				std::unordered_set<cpps_cppsclassvar *>::iterator it = oldgen.find(pClsVar);
				if (it != oldgen.end())
				{
					oldgen.erase(it);
					newgen.insert(pClsVar);
					size += pClsVar->size();
					pClsVar->setGCLevel(1);//设置成老生代 哪怕也是老生代了 也设置一下
				}
			}
			else if (v.tt == CPPS_TCLASSVAR)
			{
				cpps_cppsclassvar *pClsVar = (cpps_cppsclassvar *)v.value.domain;
				std::unordered_set<cpps_cppsclassvar *>::iterator it = oldgen.find(pClsVar);
				if (it != oldgen.end())
				{
					oldgen.erase(it);
					newgen.insert(pClsVar);
					size += pClsVar->size();
					pClsVar->setGCLevel(1);//设置成老生代 哪怕也是老生代了 也设置一下
					if (checkchild)
					{
						cpps_gc_check_child(v, c, checkchild, oldgen, newgen, size);
					}
				}
			}
		}
	}
	//检测新生代
	void cpps_gc_check_gen0(C *c)
	{
		if (c->debug) printf("gc gen0\n");

		for (std::unordered_set<cpps_regvar*>::iterator it = c->barrierList.begin();
			it != c->barrierList.end(); ++it)
		{
			cpps_regvar *v = *it;
			cpps_gc_check_gen_value(c, v->getValue(), true, c->gen0, c->gen1,c->gen1size);
			v->setIsJoinBarrier(false);
		}

		//释放gen0里面的内存
		for (std::unordered_set<cpps_cppsclassvar *>::iterator it = c->gen0.begin();
			it != c->gen0.end(); ++it)
		{
			cpps_cppsclassvar * pClsVar = *it;
			pClsVar->destory(c);
			if (c->debug) printf("=======================================================gc to %s\n", pClsVar->getDomainName().c_str());
			delete pClsVar;
		}
		c->barrierList.clear();
		c->gen0.clear();
		c->gen0size = 0;
	}

	//检测老生代  这样检测估计后面就卡死了。  还得想办法优化！！！
	void cpps_gc_check_gen1(C *c)
	{
		if (c->debug) printf("=======================================================gc all\n");

		std::unordered_set<cpps_cppsclassvar *> newgen;
		size_t newgensize = 0;

		//先把新生代的检测了
		for (std::unordered_set<cpps_regvar*>::iterator it = c->barrierList.begin();
			it != c->barrierList.end(); ++it)
		{
			cpps_regvar *v = *it;
			cpps_gc_check_gen_value(c, v->getValue(), true, c->gen0, newgen, newgensize);
			cpps_gc_check_gen_value(c, v->getValue(), true, c->gen1, newgen, newgensize);
			v->setIsJoinBarrier(false);
		}

		//释放gen0里面的内存
		for (std::unordered_set<cpps_cppsclassvar *>::iterator it = c->gen0.begin();
			it != c->gen0.end(); ++it)
		{
			cpps_cppsclassvar * pClsVar = *it;
			pClsVar->destory(c);
			if (c->debug) printf("=======================================================gc to %s\n",pClsVar->getDomainName().c_str());
			delete pClsVar;
		}
		c->gen0.clear();
		c->gen0size = 0;

		cpps_value value = c->_G;
		cpps_gc_check_child(value, c, true, c->gen1, newgen, newgensize);

		//释放gen1里面的内存
		for (std::unordered_set<cpps_cppsclassvar *>::iterator it = c->gen1.begin();
			it != c->gen1.end(); ++it)
		{
			cpps_cppsclassvar * pClsVar = *it;
			pClsVar->destory(c);
			if (c->debug) printf("=======================================================gc to %s\n", pClsVar->getDomainName().c_str());
			delete pClsVar;
		}
		c->barrierList.clear();
		c->gen1.clear();
		c->gen1 = newgen;
		c->gen1size = newgensize;
		c->lastgensize = c->gen1size;
	}


	void cpps_reggc(C *c)
	{
		module(c,"GC")[
			def_inside("Collect", cpps_gc_check_gen0),
			def_inside("CollectAll", cpps_gc_check_gen1)
		];
	}

}
