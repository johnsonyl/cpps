
#ifndef CPPS_OBJ_CPPS_HEAD_
#define CPPS_OBJ_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_NODE
//@Description	:	Cpps�ڵ�
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	struct C;
	struct cpps_node_domain;
	typedef phmap::flat_hash_map<std::string, node*> var_list_type;

	enum node_var_type
	{
		node_var_type_var,
		node_var_type_constvar,
		node_var_type_asyncvar,
		node_var_type_externvar,
	};
	struct node
	{
		node() {
			type = 0;
			parent = NULL;
			symbol = NULL;
			domain = NULL;
			line = 0;
			varsize = -1;
			offset = -1;
			offsettype = -1;
			size = -1;
			varlist = NULL;
			value.integer = 0;
			closure = false;
			quote = false;
		}
		node(std::string f)
		{
			type = 0;
			filename = f;
			parent = NULL; 
			symbol = NULL;
			domain = NULL;
			line = 1;
			varsize = -1;
			offset = -1;
			offsettype = -1;
			size = -1;
			varlist = NULL;
			value.integer = 0;
			closure = false;
			quote = false;
		}
		node(node *n,std::string f,int32 ln)
		{
			type = 0;
			parent = n;
			symbol = NULL;
			filename = f;
			n->add(this);
			domain = NULL;
			line = ln;
			varsize = -1;
			offset = -1;
			offsettype = -1;
			size = -1;
			varlist = NULL;
			value.integer = 0;
			closure = false;
			quote = false;
		}
		node(std::string f, int32 ln)
		{
			type = 0;
			line = 0;
			symbol = NULL;
			parent = NULL;
			domain = NULL;
			filename = f;
			line = ln;
			varsize = -1;
			offset = -1;
			offsettype = -1;
			size = -1;
			varlist = NULL;
			value.integer = 0;
			closure = false;
			quote = false;
		}
		~node() {
#ifdef _DEBUG
			static bool b = false;
			if (b) {
				FILE* file = fopen("node.txt", "ab+");
				char buff[4096];
				sprintf(buff, "file:%s line:%d", filename.c_str(), line);
				fclose(file);
			}
#endif
			if (varlist) {
				CPPSDELETE( varlist);
				varlist = NULL;
			}
			if (domain) {
				CPPSDELETE(domain);
				domain = NULL;
			}
		}
		void release()
		{
			CPPSDELETE(this);
		}
		void add(node *o)
		{
			o->setparent(this);
			l.push_back(o);
		}

		void clone(node * v)
		{
			s = v->s;
			type = v->type;
			for (auto n : v->l)
			{
				if (n != NULL){
					node* cpyn = CPPSNEW( node)();
					cpyn->clone(n);
					l.push_back(cpyn);
				}
				else {
					l.push_back(NULL);//ռλ
				}
			}
			filename = v->filename;
			symbol = v->symbol;
			line = v->line;
			parent = v->parent;
			varsize = v->varsize;
			offset = v->offset;
			offsettype = v->offsettype;
			size = v->size;
			value = v->value;
			closure = v->closure;
			quote = v->quote;
		}

		void setparent(node* p)
		{
			parent = p;
		}

		void addtoleft(node *p)
		{
			if (l.size() < 1)
			{
				l.resize(1);
			}
			if (symbol && symbol->getparamleftlimit())
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD && p->type != CPPS_OSLICE && p->type != CPPS_OOFFSET)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " The left side of %s must be a variable.",s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;
				else if (p->type == CPPS_OOFFSET)
					p->type = CPPS_QUOTEOFFSET;

			}
			if (symbol && symbol->getparamrightlimit() 
				&& symbol->getparamnum() == 1)
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD && p->type != CPPS_OSLICE && p->type != CPPS_OOFFSET)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, "The right side of %s must be a variable.", s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;
				else if (p->type == CPPS_OOFFSET)
					p->type = CPPS_QUOTEOFFSET;
			}
			l[0] = p;
			p->setparent(this);
		}

		node* getleft()
		{
			if (l.size() >= 1)
			{
				return l[0];
			}
			return NULL;
		}

		void addtoright(node *p)
		{
			if (l.size() < 2)
			{
				l.resize(2);
			}
			if (symbol && symbol->getparamrightlimit()
				&& symbol->getparamnum() >=2)
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD && p->type != CPPS_OSLICE && p->type != CPPS_OOFFSET)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " The left side of %s must be a variable", s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;
				else if (p->type == CPPS_OOFFSET)
					p->type = CPPS_QUOTEOFFSET;
			}
			l[1] = p;
			p->setparent(this);
		}


		node* getright()
		{
			if (l.size() >= 2)
			{
				return l[1];
			}
			return NULL;
		}
		node* getthird()
		{
			if (l.size() >= 3)
			{
				return l[2];
			}
			return NULL;
		}

		void setdomain(cpps_node_domain* d)
		{
			domain = d;
		}


		std::string s;
		std::vector<node*> l;
		int32 type;
		std::string filename;
		cpps_symbol* symbol;
		node *parent;
		cpps_node_domain* domain;
		int32 line;
		bool closure;
		bool quote;

		//����ת��
		union Value
		{
			cpps_number			number;		// double float 
			cpps_integer		integer;	// int
			int32				b;			// bool
		};
		Value					value;		//ֵ��

		//���Ͳ㶯̬�����ڴ��ַ.=========
		int8 varsize;	//��������� (ֻ��������) ǰ������
		int16 offset;	//����ƫ��
		int8 offsettype; //ƫ������ 0 global 1 left 2 self
		int16 size;	//�ӽڵ����� ( ֻ������ ���ռ� ���� , ��������°�������) 65535���ڵ㻹������?

		var_list_type *varlist; //Ϊ����������,���������ٶ�.
		void regnode(std::string& s, node* n)
		{
			if (varlist == NULL) //��Ҫ�����
			{
				varlist = CPPSNEW(var_list_type)();
			}
			varlist->insert(phmap::flat_hash_map<std::string, node*>::value_type(s, n));
		}
		void unregnode(std::string& s)
		{
			if (varlist != NULL) //��Ҫ�����
			{
				varlist->erase(s);
			}
		}
		node* getnode(std::string &s ,bool b = false)
		{
			if (varlist != NULL) //��Ҫ�����
			{
				phmap::flat_hash_map<std::string, node*>::iterator it = varlist->find(s);
				if (it != varlist->end())
				{
					return it->second;
				}
			}
			if ((type != CPPS_ODEFVAR_LAMBDA_FUNC || b == true)&& parent != NULL)
			{
				return parent->getnode(s,b);
			}
			return NULL;
		}

	};









}
#endif // CPPS_OBJ_CPPS_HEAD_
