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
		size = 0;
		varlist = NULL;
		value.uinteger = 0;
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
		size = 0;
		varlist = NULL;
		value.uinteger = 0;
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
		size = 0;
		varlist = NULL;
		value.uinteger = 0;
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
		size = 0;
		varlist = NULL;
		value.uinteger = 0;
		closure = false;
		quote = false;
		needdelete = false;
	}

	node::~node()
	{
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
		cpps_destory_node(this);
	}

	void node::release()
	{
		CPPSDELETE(this);
	}

	void node::add(node* o)
	{
		o->setparent(this);
		l.emplace_back(o);
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
				l.emplace_back(cpyn);
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
		needdelete = v->needdelete;
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

	void node::cpps_release()
	{
		cpps_destory_node(this);
	}

	void node::write(cpps::Buffer* _buffer)
	{

		_buffer->writeuint8((usint8)s.size());
		if (!s.empty())
			_buffer->writestring(s);

		_buffer->writeint32(type);


		_buffer->writeuint16((usint8)filename.size());
		if (!filename.empty())
			_buffer->writestring(filename);

		_buffer->writeint32(line);
		_buffer->writebool(closure);
		_buffer->writebool(quote);

		_buffer->writeuint32((usint32)l.size());
		for (auto& _n : l) {
			if (_n == NULL) {
				_buffer->writeuint8(0); //占位
			}
			else {
				_buffer->writeuint8(1);
				_n->write(_buffer);
			}
		}
	}

	void node::read(cpps::Buffer* _buffer)
	{
		cpps_integer _size = _buffer->readuint8();
		if (_size > 0)
			s = _buffer->readstring(_size);

		type = (int32)_buffer->readint32();

		_size = _buffer->readuint16();
		if (_size > 0)
			filename = _buffer->readstring(_size);

		line = (int32)_buffer->readint32();
		closure = _buffer->readbool();
		quote = _buffer->readbool();

		_size = _buffer->readuint32();
		for (cpps_integer i = 0; i < _size; i++) {
			cpps_integer _ = _buffer->readuint8();
			if (_ == 0)
				l.push_back(NULL);
			else {
				node* _n = CPPSNEW(node)();
				add(_n);
				_n->read(_buffer);
			}
		}
	}
	void node::swap(node* _n)
	{
		for (auto& _cn : l) {
			_n->add(_cn);
		}
	}
}