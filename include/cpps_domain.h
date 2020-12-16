#ifndef CPPS_DOMAIN_CPPS_HEAD_
#define CPPS_DOMAIN_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	��ռ�
//@website		:	http://cpps.wiki
//==================================



namespace cpps
{

	void					cpps_gc_remove_barrier(C*c, cpps_regvar *v);
	void					cpps_gc_add_barrier(C*c, cpps_regvar *v);
	

	struct cpps_domain : public cpps_gcobject
	{
		cpps_domain()
		{
			//do nothing...
			domainType = 0;
			hasVar = false;
			isbreak = false;
			parent[0] = NULL;
			parent[1] = NULL;
			funcRet.tt = CPPS_TNIL;
			stacklist = NULL;
			offset = -1;
			offsettype = -1;
			parentclassoffset = NULL;
		}
		cpps_domain(cpps_domain* p, char type, std::string name)
			:cpps_gcobject()
		{
			init(p, type, name);
		}
		virtual ~cpps_domain()
		{

		}
		virtual cpps_cppsclassvar *	create(C* c, bool alloc = true)
		{
			return NULL;
		}
		void init(cpps_domain* p, char type, std::string name)
		{
			parent[0] = p;
			domainType = type;
			isbreak = false;
			parent[1] = NULL;
			domainname = name;
			funcRet.tt = CPPS_TNIL;
			hasVar = false;
			stacklist = NULL;
			offset = -1;
			offsettype = -1;
			parentclassoffset = NULL;
		}
		void init(cpps_domain* p, char type)
		{
			parent[0] = p;
			domainType = type;
			isbreak = false;
			parent[1] = NULL;
			funcRet.tt = CPPS_TNIL;
			hasVar = false;
			stacklist = NULL;
			offset = -1;
			offsettype = -1;
			parentclassoffset = NULL;
		}
		std::string								getdomainname()
		{
			return domainname;
		}
		void									regfunc(cpps_reg* f)
		{
			hasVar = true;
			do
			{
				cpps_regvar* var = new cpps_regvar();
				var->setvarname(f->varname);
				var->setconst(true);
				if (f->type == cpps_def_regfunction)
				{
					cpps_regfunction *func = (cpps_regfunction *)f;
					var->setval(cpps_value(func->func));
					var->setconst(true);
					func->func->setIsNeesC(f->isneedC);
				}
				else if (f->type == cpps_def_regclass)
				{
					cpps_regclass *cls = (cpps_regclass *)f;
					//cpps_domain *domain = (cpps_domain *)cls->cls;
					//domain->parent[0] = this;
					var->setval(cpps_value(cls->cls));
					var->setconst(true);
				}
				else if (f->type == cpps_def_regvar)
				{
					var->setval(f->value);
				}
				varList.erase(var->varName);
				varList.insert(std::unordered_map<std::string, cpps_regvar*>::value_type(var->varName, var));
				f = f->next;
			} while (f);

		}
		cpps_regvar*							getvar(std::string s, cpps_domain *&leftdomain, bool findparent = true)
		{
			cpps_regvar *ret = NULL;
			if (hasVar) {
				std::unordered_map<std::string, cpps_regvar*>::iterator it = varList.find(s);
				if (it != varList.end())
				{
					ret = it->second;
					return ret;
				}
			}
			if (findparent)
			{
				if (parent[1])
				{
					cpps_regvar* ret = parent[1]->getvar(s, leftdomain);
					if (ret) return ret;
				}
				if (parent[0])
				{
					cpps_regvar* ret = parent[0]->getvar(s, leftdomain);
					if (leftdomain == NULL && parent[0]->domainType == cpps_domain_type_class)
					{
						leftdomain = this;
					}

					if (ret) return ret;
				}
			}

			

			return ret;
		}

		void									regvar(C *c,cpps_regvar * v)
		{
			hasVar = true;
			varList.insert(std::unordered_map<std::string, cpps_regvar*>::value_type(v->varName, v));
			if(c != NULL && this != c->_G) cpps_gc_add_barrier(c, v);
		}
		void									unregvar(C *c, cpps_regvar * v)
		{
			varList.erase(v->varName);
			cpps_gc_remove_barrier(c, v);
		}
		void									setexecdomain(cpps_domain *exec)
		{
			parent[1] = exec;
		}

		virtual void destory(C *c)
		{
			if (hasVar) {
				for (std::unordered_map<std::string, cpps_regvar*>::iterator it = varList.begin(); it != varList.end(); ++it)
				{
					cpps_regvar* v = it->second;
					if (!v->closeure || v->closeureusecount <= 0) { /*�հ���ɾ��,���Ǳ�������ʹ��*/
						cpps_gc_remove_barrier(c, v);
						if (v->stackdomain) {
							v->stackdomain->removeidxvar(v->offset);
						}
						delete v;
					}
				}
				varList.clear();
				hasVar = false;
			}
			if (stacklist != NULL)
			{
				stacklist->clear();
				stacklist = NULL;
			}
			funcRet.decruse();
			funcRet.tt = CPPS_TNIL;
			isbreak = false;
			parent[0] = NULL;
			parent[1] = NULL;
		}

		void									regidxvar(int32 offset, cpps_regvar* v)
		{
			if (stacklist == NULL)
			{
				stacklist = new std::vector< cpps_regvar*>();
			}
			if (offset < (int32)stacklist->size())
				(*stacklist)[offset] = v;
			else
				stacklist->push_back(v);

			v->stackdomain = this;
		}
		void									removeidxvar(int32 offset)
		{
			if (stacklist == NULL)
			{
				return;
			}
			(*stacklist)[offset] = NULL;

		}

		cpps_regvar* 						getregidxvar(int32 offset)
		{
			if (stacklist == NULL)
			{
				return NULL;
			}
			return (*stacklist)[offset];
		}

		int32						getidxoffset(cpps_domain* parentclass)
		{
			if (this == parentclass) return 0;
			if (parentclassoffset == NULL)
			{
				return 0;
			}
			return (*parentclassoffset)[parentclass];
		}
		void						setidxoffset(cpps_domain* parentclass, int32 off)
		{
			if (parentclassoffset == NULL)
			{
				parentclassoffset = new std::map<cpps_domain*, int32>();
			}
			parentclassoffset->insert(std::map<cpps_domain*, int32>::value_type(parentclass, off));
		}
		void resize(usint16 size)
		{
			if (size == 0) return;

			if (size == 65535) return;

			if (stacklist == NULL)
			{
				stacklist = new std::vector< cpps_regvar*>();
			}
			stacklist->resize(size);
		}


		cpps_domain											*parent[2]; // 0Ϊ���� 1Ϊִ����
		char												domainType;
		std::unordered_map<std::string, cpps_regvar*>		varList;
		cpps_value											funcRet;//������һ��func���ʱ����Ϊ����ֵ�õ� ���������б����; ��ʱ����ΪfuncRet
		bool												isbreak;//�п��������ط��������ִ�м��˳�ִ��
		std::string											domainname; //������
		bool												hasVar;
		std::vector< cpps_regvar*>							*stacklist;
		int32												offset;
		int32												offsettype;
		std::map<cpps_domain*, int32>*						parentclassoffset; //����ƫ��
	};

}

#endif // CPPS_DOMAIN_CPPS_HEAD_