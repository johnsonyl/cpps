#ifndef CPPS_DOMAIN_CPPS_HEAD_
#define CPPS_DOMAIN_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	域空间
//@website		:	http://cpps.wiki
//==================================



namespace cpps
{

	void					cpps_gc_remove_barrier(C*c, cpps_regvar *v);

	

	struct cpps_domain : public cpps_gcobject
	{
		cpps_domain(cpps_domain* p, char type, std::string domainname)
			:cpps_gcobject()
		{
			parent[0] = p;
			domainType = type;
			isbreak = false;
			parent[1] = NULL;
			domainName = domainname;
		}

		virtual cpps_cppsclassvar *	create()
		{
			return NULL;
		}
		std::string								getDomainName()
		{
			return domainName;
		}
		void									regFunc(cpps_reg* f)
		{
			do
			{
				cpps_regvar* var = new cpps_regvar();
				var->setVarName(f->varname);
				if (f->type == cpps_def_regfunction)
				{
					cpps_regfunction *func = (cpps_regfunction *)f;
					var->setValue(cpps_value(func->func));
					func->func->setIsNeesC(f->isneedC);
				}
				else if (f->type == cpps_def_regclass)
				{
					cpps_regclass *cls = (cpps_regclass *)f;
					cpps_domain *domain = (cpps_domain *)cls->cls;
					//domain->parent[0] = this;
					var->setValue(cpps_value(cls->cls));
				}
				varList.insert(std::unordered_map<std::string, cpps_regvar*>::value_type(var->varName, var));
				f = f->next;
			} while (f);

		}
		cpps_regvar*							getVar(std::string s,bool findparent = true)
		{
			cpps_regvar *ret = NULL;
			std::unordered_map<std::string, cpps_regvar*>::iterator it = varList.find(s);
			if (it != varList.end())
			{
				ret = it->second;
			}
			else if (findparent)
			{
				

				if (parent[1])
				{
					cpps_regvar* ret = parent[1]->getVar(s);
					if (ret) return ret;
				}
				if (parent[0])
				{
					cpps_regvar* ret = parent[0]->getVar(s);
					if (ret) return ret;
				}
			}

			return ret;
		}

		void									regVar(cpps_regvar * v)
		{
			
			varList.insert(std::unordered_map<std::string, cpps_regvar*>::value_type(v->varName, v));
		}
		void									unregVar(cpps_regvar * v)
		{

			varList.erase(v->varName);
		}
		void									setexecdomain(cpps_domain *exec)
		{
			parent[1] = exec;
		}

		void destory(C *c)
		{
			for (std::unordered_map<std::string, cpps_regvar*>::iterator it = varList.begin(); it != varList.end();++it)
			{
				cpps_regvar* v = it->second;
				cpps_gc_remove_barrier(c, v);
				if(c->debug) printf("destory %s \n", v->varName.c_str());
				delete v;
			}
			varList.clear();
		}



		cpps_domain											*parent[2]; // 0为父域， 1为执行域
		char												domainType;
		std::unordered_map<std::string, cpps_regvar*>		varList;
		cpps_value											funcRet;//当他是一个func域的时候作为返回值用的 将来或许还有别的用途 暂时起名为funcRet
		bool												isbreak;//有可能其他地方让我这个执行集退出执行
		std::string											domainName; //域名字
	};

}

#endif // CPPS_DOMAIN_CPPS_HEAD_