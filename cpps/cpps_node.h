
#ifndef CPPS_OBJ_CPPS_HEAD_
#define CPPS_OBJ_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_NODE
//@Description	:	Cpps节点
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct C;
	struct Node
	{
		Node(std::string f)
		{
			type = 0;
			filename = f;
			parent = NULL; 
			symbol = NULL;
			domain = NULL;
			line = 1;
		}
		Node(Node *n,std::string f,int32 ln)
		{
			type = 0;
			parent = n;
			symbol = NULL;
			filename = f;
			n->add(this);
			domain = NULL;
			line = ln;
		}
		Node(std::string f, int32 ln)
		{
			type = 0;
			line = 0;
			symbol = NULL;
			parent = NULL;
			domain = NULL;
			filename = f;
			line = ln;
		}
		void release()
		{
			delete this;
		}
		void add(Node *o)
		{
			o->setParent(this);
			l.push_back(o);
		}

		void clone(Node * v)
		{
			s = v->s;
			type = v->type;
			l = v->l;
			filename = v->filename;
			symbol = v->symbol;
			line = v->line;
			parent = v->parent;
		}

		void setParent(Node* p)
		{
			parent = p;
		}

		void addtoleft(Node *p)
		{
			if (l.size() < 1)
			{
				l.resize(1);
			}
			if (symbol && symbol->getparamleftlimit())
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " '%s' 的左边必须为变量",s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;

			}
			if (symbol && symbol->getparamrightlimit() 
				&& symbol->getparamnum() == 1)
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " '%s' 的右边必须为变量", s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;
			}
			l[0] = p;
			p->setParent(this);
		}

		Node* getleft()
		{
			if (l.size() >= 1)
			{
				return l[0];
			}
			return NULL;
		}

		void addtoright(Node *p)
		{
			if (l.size() < 2)
			{
				l.resize(2);
			}
			if (symbol && symbol->getparamrightlimit()
				&& symbol->getparamnum() >=2)
			{
				if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD)
				{
					throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " '%s' 的左边必须为变量", s.c_str()));
				}
				if (p->type == CPPS_VARNAME)
					p->type = CPPS_QUOTEVARNAME;
				else if (p->type == CPPS_OGETOBJECT)
					p->type = CPPS_QUOTEGETOBJECT;
				else if (p->type == CPPS_OGETCHIILD)
					p->type = CPPS_QUOTEGETCHIILD;
			}
			l[1] = p;
			p->setParent(this);
		}

		Node* getright()
		{
			if (l.size() >= 2)
			{
				return l[1];
			}
			return NULL;
		}

		void setdomain(cpps_domain * d)
		{
			domain = d;
		}


		std::string s;
		std::vector<Node*> l;
		int32 type;
		std::string filename;
		cpps_symbol* symbol;
		Node *parent;
		cpps_domain * domain;
		int32 line;
	};









}
#endif // CPPS_OBJ_CPPS_HEAD_