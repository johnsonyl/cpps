#include "cpps/cpps.h"


namespace cpps {

	
	node::node()
	{
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
		needdelete = false;
		closure = false;
		quote = false;
	}

	node::node(std::string f)
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
		needdelete = false;
		quote = false;
	}

	node::node(node* n, std::string f, int32 ln)
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
		needdelete = false;
		quote = false;
	}

	node::node(std::string f, int32 ln)
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
		needdelete = false;
	}

	node::~node()
	{
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
			CPPSDELETE(varlist);
			varlist = NULL;
		}
		if (domain) {
			CPPSDELETE(domain);
			domain = NULL;
		}
		if (needdelete) {
			CPPSDELETE(value.val);
			value.val = NULL;
			needdelete = false;
		}
	}

	void node::release()
	{
		CPPSDELETE(this);
	}

	void node::add(node* o)
	{
		o->setparent(this);
		l.push_back(o);
	}

	void node::clone(node* v)
	{
		s = v->s;
		type = v->type;
		for (auto n : v->l)
		{
			if (n != NULL) {
				node* cpyn = CPPSNEW(node)();
				cpyn->clone(n);
				l.push_back(cpyn);
			}
			else {
				l.push_back(NULL);//占位
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
		needdelete = needdelete;
		if (needdelete)
		{
			value.val = CPPSNEW(cpps_value)();
			*value.val = *v->value.val;
		}
		else
			value = v->value;
		closure = v->closure;
		quote = v->quote;
	}

	void node::setparent(node* p)
	{
		parent = p;
	}

	void node::addtoleft(node* p)
	{
		if (l.size() < 1)
		{
			l.resize(1);
		}
		if (symbol && symbol->getparamleftlimit())
		{
			if (p->type != CPPS_VARNAME && p->type != CPPS_OGETOBJECT && p->type != CPPS_OGETCHIILD && p->type != CPPS_OSLICE && p->type != CPPS_OOFFSET)
			{
				throw(cpps_error(p->filename, p->line, cpps_error_paramerror, " The left side of %s must be a variable.", s.c_str()));
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

	cpps::node* node::getleft()
	{
		if (l.size() >= 1)
		{
			return l[0];
		}
		return NULL;
	}

	void node::addtoright(node* p)
	{
		if (l.size() < 2)
		{
			l.resize(2);
		}
		if (symbol && symbol->getparamrightlimit()
			&& symbol->getparamnum() >= 2)
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

	cpps::node* node::getright()
	{
		if (l.size() >= 2)
		{
			return l[1];
		}
		return NULL;
	}

	cpps::node* node::getthird()
	{
		if (l.size() >= 3)
		{
			return l[2];
		}
		return NULL;
	}

	void node::setdomain(cpps_node_domain* d)
	{
		domain = d;
	}

	void node::regnode(std::string& s, node* n)
	{
		if (varlist == NULL) //需要在添加
		{
			varlist = CPPSNEW(var_list_type)();
		}
		varlist->insert(phmap::flat_hash_map<std::string, node*>::value_type(s, n));
	}

	void node::unregnode(std::string& s)
	{
		if (varlist != NULL) //需要在添加
		{
			varlist->erase(s);
		}
	}

	cpps::node* node::getnode(std::string& s, bool b /*= false*/)
	{
		if (varlist != NULL) //需要在添加
		{
			phmap::flat_hash_map<std::string, node*>::iterator it = varlist->find(s);
			if (it != varlist->end())
			{
				return it->second;
			}
		}
		if ((type != CPPS_ODEFVAR_LAMBDA_FUNC || b == true) && parent != NULL)
		{
			return parent->getnode(s, b);
		}
		return NULL;
	}

}