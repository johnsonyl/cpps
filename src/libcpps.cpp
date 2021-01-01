#include "cpps/cpps.h"
namespace cpps {
	/* 内部 */
	struct     cpps_value;
	struct     node;
	struct     cpps_domain;
	struct     cpps_value;
	struct     cpps_func_params;
	void cpps_gc_add_gen0(C* c, cpps_cppsclassvar* p);
	cpps_value cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* o, cpps_domain*& leftdomain);
	void cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d);
	cpps_value cpps_step_callfunction(C* c, cpps_domain* domain, cpps_domain* root, cpps_value func, node* d, cpps_domain* leftdomain);
	void cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o);
	void cpps_parse_dofunction(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	void cpps_parse_expression(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	int32 cpps_parse_expression_step(C* c, cpps_node_domain* domain, node* param, node*& lastOpNode, node* root, cppsbuffer& buffer);
	node* cpps_parse_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	node* cpps_parse_symbol(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer, bool leftsymbol = false);
	node* cpps_parse_string(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer, int8 endch);
	void cpps_parse_var(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit, node_var_type vartype);
	node* cpps_parse_var_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, bool findparent = true);
	node* cpps_parse_number(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer);
	void cpps_parse_def_function(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer);
	node* cpps_parse_line(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int32 limit = 0);
	void cpps_parse_builtin(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit);
	void cpps_parse_def(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit);
	node* cpps_parse_new_setv(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	void cpps_gc_add_barrier(C* c, cpps_regvar* v);
	void cpps_gc_check_gen0(C* c);
	void cpps_gc_check_gen1(C* c);
	bool cpps_loadlibrary(C* c, std::string libname);
	void cpps_symbol_handle(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_value& ret);
	bool cpps_io_file_exists(std::string path);
	std::string cpps_getcwd();
	std::string cpps_real_path();
	void		gc_cleanup(C* c);
	void cpps_detach_library(HMODULE module, const std::string& libname, C* c);
	void cpps_load_filebuffer(const char* path, std::string& fileSrc)
	{
#ifdef _WIN32
		FILE* file;
		fopen_s(&file, path, "rb+");
# else
		FILE* file = fopen(path, "rb+");
#endif
		if (file) {
			char s[4097];
			memset(s, 0, 4097);
			fseek(file, 0, SEEK_END);
			size_t size = (size_t)ftell(file);
			fseek(file, 0, SEEK_SET);
			while (size != 0) {
				size_t rs = size < 4096 ? size : 4096;
				rs = fread(s, 1, rs, file);
				s[rs] = '\0';
				size -= rs;
				fileSrc += s;
			}
			fclose(file);
		}
		else {
			throw cpps_error(cpps_error(path, 0, cpps_error_unknow, "ERROR: [%s] file load faild. ", path));
		}
	}
	void cpps_gc_check_step(C* c) {
		size_t	g0size = c->getgen0size();
		size_t	g1size = c->getgen1size();
		size_t	allsize = g0size + g1size;
		size_t	lastgensize = c->getlastgensize();
		size_t	checksize = CPPS_GEN1_CHECKSIZE < lastgensize ? (size_t)(lastgensize * 1.2) : CPPS_GEN1_CHECKSIZE;
		if (allsize > checksize) {
			cpps_gc_check_gen1(c);
		}
		else if ((g0size > CPPS_GEN0_CHECKSIZE
			/*|| c->getGen0()->size() > CPPS_GEN0_CHECKCOUNT*/
			)) {
			cpps_gc_check_gen0(c);
		}
	}
	bool cpps_parse_isnumber(char ch) {
		return(ch >= 48 && ch <= 57);
	}
	bool cpps_parse_isspace(char ch) {
		return(ch == ' ' || ch == '	' || ch == '\t');
	}
	bool cpps_parse_isenter(char ch) {
		return(ch == '\n' || ch == '\r');
	}
	void cpps_parse_rmspaceandenter(cppsbuffer& buffer) {
		while (cpps_parse_isenter(buffer.cur()) || cpps_parse_isspace(buffer.cur()))
			buffer.pop();
	}
	bool cpps_parse_issymbol(char ch) {
		return(ch == '+' || ch == '-' || ch == '*'
			|| ch == '/' || ch == '|' || ch == '&'
			|| ch == '%' || ch == '!' || ch == '['
			|| ch == ']' || ch == '.' || ch == '='
			|| ch == '<' || ch == '>' || ch == '?');
	}
	bool cpps_parse_isnotvarname(char ch) {
		return(ch == '~' || ch == '!' || ch == '@' || ch == '#' || ch == '$'
			|| ch == '%' || ch == '^' || ch == '&' || ch == '*' || ch == '('
			|| ch == ')' || ch == '+' || ch == '`' || ch == '{' || ch == '}'
			|| ch == ';' || ch == '"' || ch == ':' || ch == '\'' || ch == '\\'
			|| ch == '|' || ch == ',' || ch == '<' || ch == '.' || ch == '>'
			|| ch == '/' || ch == '?' || ch == '\0' || cpps_parse_isspace(ch)
			|| cpps_parse_issymbol(ch) || cpps_parse_isenter(ch) || ch == '\n');
	}
	bool cpps_parse_isint16(char ch) {
		ch = (char)tolower((int)ch);
		return(ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0');
	}
	std::string cpps_parse_varname(cppsbuffer& buffer) {
		std::string ret = "";
		while (!cpps_parse_isnotvarname(buffer.cur())) {
			ret.push_back(buffer.pop());
		}
		return(ret);
	}
	std::string cpps_parse_object_varname(cppsbuffer& buffer) {
		std::string ret = "";
		while (buffer.cur() != ':' && !cpps_parse_isspace(buffer.cur()) && !cpps_parse_isenter(buffer.cur())) {
			ret.push_back(buffer.pop());
		}
		return(ret);
	}
	std::string cpps_parse_include_varname(cppsbuffer& buffer) {
		std::string ret = "";
		while (buffer.cur() != '>') {
			ret.push_back(buffer.pop());
		}
		return(ret);
	}
	bool cpps_parse_isbuiltinname(std::string s) {
		return(s == "if" || s == "const" ||s == "async" || s == "yield" || s == "try" || s == "throw" || s == "namespace" || s == "var" || s == "else" || s == "for" || s == "foreach" || s == "do" || s == "while" || s == "class" || s == "module" || s == "struct" || s == "break" || s == "continue" || s == "case" || s == "switch" || s == "enum" || s == "return" || s == "dofile" || s == "import" || s == "include" || s == "dostring");
	}
	bool cpps_is_not_use_var_name(std::string s) {
		return(cpps_parse_isbuiltinname(s) || s == "true" || s == "catch" || s == "null" || s == "nil" || s == "NULL" || s == "false" || s == "map" || s == "vector" || s == "math" || s == "string" || s == "time" || s == "io" || s == "GC");
	}
	std::string cpps_parse_loadinterger16(cppsbuffer& buffer) {
		std::string ret = "";
		while (cpps_parse_isint16(buffer.cur())) {
			ret.push_back(buffer.pop());
		}
		return(ret);
	}
	void cpps_parse_loadnumber(cppsbuffer& buffer, node* str) {
		if (!cpps_parse_isnumber(buffer.realcur())) {
			throw(cpps_error(str->filename, buffer.line(), cpps_error_paramerror, "Not the expected number after the decimal point."));
		}
		while (cpps_parse_isnumber(buffer.realcur())) {
			char ch = buffer.pop();
			str->s.push_back(ch);
		}
	}
	char cpps_parse_transfer_character(char ch, cppsbuffer& buffer) {
		if (ch == '\\') {
			char ch2 = buffer.pop();
			switch (ch2) {
			case 'a': ch = 7;
				break;
			case 'b': ch = 8;
				break;
			case 'f': ch = 12;
				break;
			case 'n': ch = 10;
				break;
			case 'r': ch = 13;
				break;
			case 't': ch = 9;
				break;
			case 'v': ch = 11;
				break;
			case '\\': ch = 92;
				break;
			case '?': ch = 63;
				break;
			case '\'': ch = 39;
				break;
			case '"': ch = 34;
				break;
			case '0': ch = 0;
				break;
			case '{': ch = '{';
				break;
			case '}': ch = '}';
				break;
			default: ch = ch2;
				break;
			}
		}
		return(ch);
	}
	node* cpps_parse_string(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int8 endch) {
		node* str = new node(o->filename, buffer.line());
		str->type = CPPS_OSTR;
		str->value.str = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(c, true);
		buffer.pop();
		/* pop '"' */
		node* laststr = NULL;
		while (buffer.realcur() != endch && !buffer.isend()) {
			char lastch = buffer.realpop();
			/* todo 解析反斜杠 */
			char ch = cpps_parse_transfer_character(lastch, buffer);
			if (lastch != '\\' && ch == '{') {
				node* v = new node(str, o->filename, buffer.line());
				v->type = CPPS_VARNAME;
				cpps_parse_expression(c, domain, v, root, buffer);
				if (buffer.cur() != '}')
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varerror, "Unexpected end of '}' not detected when string defining variable value."));
				buffer.pop();
				/* pop } */
				laststr = NULL;
			}
			else {
				if (laststr == NULL) {
					laststr = new node(o->filename, buffer.line());
					laststr->type = CPPS_OSTR;
					str->add(laststr);
				}
				laststr->s.push_back(ch);
			}
		}
		buffer.pop();
		return(str);
	}
	
	node* cpps_get_root_node(node* n) {
		if (n->parent)
			return(cpps_get_root_node(n->parent)); else
			return(n);
	}
	void cpps_parse_def_function_param(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer) {
		node* typen = new node(right, right->filename, buffer.line());
		typen->type = CPPS_VARNAME;
		typen->s = cpps_parse_varname(buffer);
		if (typen->s != "var") throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "you need to enter a 'var' before defining the parameters"));
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		node* varname = new node(typen, right->filename, buffer.line());
		varname->s = cpps_parse_varname(buffer);
		varname->type = CPPS_VARNAME;
		/* right->parent is CPPS_ODEFVAR_FUNC */
		if (c->buildoffset) {
			if (right->parent &&( right->parent->type == CPPS_ODEFVAR_FUNC || right->parent->type == CPPS_ODEFVAR_LAMBDA_FUNC))
				/* 根节点.. */ {
				varname->offset = right->parent->size++;
				varname->offsettype = CPPS_OFFSET_TYPE_SELF;
				right->parent->regnode(varname->s, varname);
			}
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() == '='){
			/*有默认值 也就是可以不填... */ 
			buffer.pop();
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, varname, root, buffer);
		}
	}
	void cpps_parse_def_function(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer) {
		/* 定义了一个函数 */
		cpps_node_domain* funcdomain = new cpps_node_domain(right,domain, cpps_domain_type_func, right->parent->s);
		right->setdomain(funcdomain);
		node* params = new node(right, right->filename, buffer.line());
		node* context = new node(right, right->filename, buffer.line());
		char ch = 0;
		/* 获取类型 */
		while (!buffer.isend()) {
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			ch = buffer.cur();
			if (ch == ')') {
				buffer.pop();
				break;
			}
			if (ch == ',') {
				buffer.pop();
				cpps_parse_rmspaceandenter(buffer);
			}
			cpps_parse_def_function_param(c, funcdomain, params, root, buffer);
		}
		/* 解析{ } */
		cpps_parse_rmspaceandenter(buffer);
		ch = buffer.pop();
		if (ch == '{') {
			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					return;
				}
				cpps_parse_line(c, funcdomain, context, root, buffer);
			}
		}
		else {
			throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "Definition function did not detect '{'"));
		}
		throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "Definition function did not detect '}'"));
	}
	node* cpps_parse_var_right(C* c, cpps_node_domain* domain, node* var, node* root, cppsbuffer& buffer, int32 limit, node_var_type vartype) {
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		node* right = new node(var, var->filename, buffer.line());
		if (buffer.cur() == '=') {
			buffer.pop();
			if (limit & CPPS_NOT_DEFVAR) {
				throw("Prohibit defining variables");
			}
			if (vartype == node_var_type_asyncvar)
				throw(cpps_error(right->filename, buffer.line(), cpps_error_varerror, "do not define async var."));

			/* 是个变量 */
			right->type = CPPS_ODEFVAR_VAR;
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, right, root, buffer);
		}
		else if (buffer.cur() == '(')
			/* 定义了一个函数 */ {
			buffer.pop();
			if (limit & CPPS_NOT_DEFFUNCTION) {
				throw("Function definition prohibited");
			}
			if (vartype == node_var_type_constvar)
				throw(cpps_error(right->filename, buffer.line(), cpps_error_varerror, "do not define const function."));

			right->type = CPPS_ODEFVAR_FUNC;
			right->size = 0;
			right->varsize = 0;
			/*
			 * right->offset = 0;
			 * right->offsettype = 0;
			 * root 修改为 函数节点
			 */
			cpps_parse_def_function(c, domain, right, right, buffer);
			/* 定义了一个函数 */
		}
		else if (buffer.cur() == ';') {
			right->type = CPPS_ODEFVAR_NIL;
		}
		else {
			right->type = CPPS_ODEFVAR_NIL;
			/* throw(cpps_error(var->filename, buffer.line(), cpps_error_varerror, "Error in defining variable. There should be '=' or '(' after the variable!")); */
		}
		return right;
	}
	bool cpps_parse_checkdomain(cpps_node_domain* domain)
	{
		node* p = domain->nodedomain;
		if (!p) return false;
		return p->type == CPPS_OIF || p->type == CPPS_OFOR || p->type == CPPS_OFOREACH || p->type == CPPS_OWHILE || p->type == CPPS_OCATCH || p->type == CPPS_OASSEMBLE;
	}

	void cpps_parse_var_varname(cppsbuffer& buffer, node* str, C* c, node* root, cpps_node_domain* domain)
	{
		cpps_parse_rmspaceandenter(buffer);
		if (cpps_parse_isnumber(buffer.cur()))
			/* 首位是个字母 */
			throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Variable cannot start with a number."));
		/* 先找名字 */
		str->s = cpps_parse_varname(buffer);
		if (str->s.empty()) {
			throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Variable names cannot empty."));
		}

		cpps_parse_rmspaceandenter(buffer);
		
	}

	void cpps_parse_var_buildoffset(C* c, node* root, node* str, cpps_node_domain* domain)
	{
		if (c->buildoffset) {
			if (root->type == CPPS_ROOT)
				/* 根节点.. */ {
				str->offset = root->size++;
				str->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				if (cpps_parse_checkdomain(domain))
					domain->nodedomain->regnode(str->s, str);
				else
					root->regnode(str->s, str);
			}
			else if (root->type == CPPS_ODEFVAR_FUNC || root->type == CPPS_ODEFVAR_LAMBDA_FUNC) {
				str->offset = root->size++;
				str->offsettype = CPPS_OFFSET_TYPE_SELF;
				root->regnode(str->s, str);

				if (cpps_parse_checkdomain(domain))
					domain->nodedomain->regnode(str->s, str);
				else
					root->regnode(str->s, str);
			}
			else if (root->type == CPPS_OCLASS) {
				/* class { var func()} */
				if (str->l[0]->type == CPPS_ODEFVAR_FUNC || str->l[0]->type == CPPS_ODEFVAR_LAMBDA_FUNC) {
					node* r = cpps_get_root_node(root);
					/* namespace不注册到自己里面 */
					str->offset = r->size++;
					str->offsettype = CPPS_OFFSET_TYPE_LEFTCLASS;
					root->regnode(str->s, str);
				}
				else {
					str->offset = root->varsize++;
					str->offsettype = CPPS_OFFSET_TYPE_LEFTDOMAIN;
					root->regnode(str->s, str);
				}
			}
			else if (root->type == CPPS_ONAMESPACE) {
				node* r = cpps_get_root_node(root);
				/* namespace不注册到自己里面 */
				str->offset = r->size++;
				str->offsettype = CPPS_OFFSET_TYPE_LEFTCLASS;
				root->regnode(str->s, str);
			}
		}
	}

	void cpps_parse_var(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit, node_var_type vartype) {
		/* 剔除空格 */
		int8 vartypes[] = { CPPS_ODEFVAR , CPPS_ODEFCONSTVAR , CPPS_ODEFASYNCVAR };
		child->type = vartypes[vartype];
		while (true) {
			cpps_parse_rmspaceandenter(buffer);
			node* str = new node(child, child->filename, buffer.line());
			str->type = CPPS_VARNAME;

			//多返回值
			if (buffer.cur() == '[') {
				buffer.pop();//pop [
				str->type = CPPS_MULTIVAR;

				if (root->type == CPPS_OCLASS || root->type == CPPS_ONAMESPACE  )
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Multiple return value variables cannot be defined in a class or namespace."));

				node* childstr = new node(str, child->filename, buffer.line());
				childstr->type = CPPS_VARNAMES;
				while (!buffer.isend())
				{
					node* str2 = new node(childstr, child->filename, buffer.line());
					str2->type = CPPS_VARNAME;
					cpps_parse_var_varname(buffer, str2, c, root, domain);
					if (cpps_is_not_use_var_name(str2->s)) {
						throw(cpps_error(str2->filename, buffer.line(), cpps_error_varnotnumber, "Variable names cannot use reserved keywords."));
					}
					cpps_parse_rmspaceandenter(buffer);
					cpps_parse_var_buildoffset(c, root, str2, domain);
					if (buffer.cur() != ',')
						break;
					buffer.pop();
				}
				cpps_parse_rmspaceandenter(buffer);
				if(buffer.cur() != ']')
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Missing ']' when defining multiple return value variable."));

				buffer.pop(); //pop ]

				node *right = cpps_parse_var_right(c, domain, str, root, buffer, limit, vartype);
				if (right->type != CPPS_ODEFVAR_VAR)
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "When defining a variable with multiple return values, it must be assigned a function"));

			}
			else {
				cpps_parse_var_varname(buffer, str, c, root, domain);
				if (cpps_is_not_use_var_name(str->s)) {
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Variable names cannot use reserved keywords."));
				}
				cpps_parse_var_right(c, domain, str, root, buffer, limit, vartype);
				cpps_parse_var_buildoffset(c, root, str, domain);

			}

			
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ',') {
				return;
			}
			buffer.pop();
		}
	}
	void cpps_parse_module(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		child->type = CPPS_OMODULE;
		child->offset = 0;
		child->offsettype = 0;
		child->varsize = 0;
		child->size = 0;
		cpps_parse_rmspaceandenter(buffer);
		child->s = cpps_parse_varname(buffer);

		if (root->type != CPPS_ROOT)
			throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "module must be in a root node."));

		if (c->buildoffset) {
			if (root->type == CPPS_ROOT)
				/* 根节点.. */ {
				child->offset = root->size++;
				child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				root->regnode(child->s, child);
			}
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '{')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'{' was detected for module. Please check."));
		buffer.pop();
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				break;
			}
			if (buffer.isend()) {
				throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'}' was detected for module. Please check."));
			}
			cpps_parse_line(c, domain, child, child, buffer, CPPS_NOT_DEFASSEMBLE);
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '}') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'}' was detected for module. Please check."));
		}
		buffer.pop();
		/* pop } */
	}
	void cpps_parse_namespace(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		child->type = CPPS_ONAMESPACE;
		child->offset = 0;
		child->offsettype = 0;
		child->varsize = 0;
		child->size = 0;
		cpps_parse_rmspaceandenter(buffer);
		child->s = cpps_parse_varname(buffer);
		if (c->buildoffset) {
			if (root->type == CPPS_ROOT)
				/* 根节点.. */ {
				child->offset = root->size++;
				child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				root->regnode(child->s, child);
			}
			else if (root->type == CPPS_ONAMESPACE) {
				node* r = cpps_get_root_node(root);
				/* namespace不注册到自己里面 */
				child->offset = r->size++;
				child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				root->regnode(child->s, child);
			}
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '{')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_varnotnumber, "'{' was detected for namespace. Please check."));
		buffer.pop();
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				break;
			}
			if (buffer.isend()) {
				throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "'}' was detected for namespace. Please check."));
			}
			cpps_parse_line(c, domain, child, child, buffer, CPPS_NOT_DEFASSEMBLE);
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '}') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "'}' was detected for namespace. Please check."));
		}
		buffer.pop();
		/* pop } */
	}
	void cpps_parse_trycatch(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		cpps_node_domain* trycatchdomain = new cpps_node_domain(child,domain, cpps_domain_type_trycatch, "trycatch");
		/* 为这个集创建一个自己的域空间 */
		child->setdomain(trycatchdomain);
		child->type = CPPS_OTRYCATCH;
		node* func = new node(child, child->filename, buffer.line());
		/* 0 if的参数 */
		node* catchfun = new node(child, child->filename, buffer.line());
		catchfun->type = CPPS_OCATCH;
		/* true */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, trycatchdomain, func, root, buffer);
		cpps_parse_rmspaceandenter(buffer);
		std::string el = cpps_parse_varname(buffer);
		if (el == "catch") {
			node* catchvar = new node(catchfun, child->filename, buffer.line());
			/* true */
			node* catchstep = new node(catchfun, child->filename, buffer.line());
			/* true */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != '(')
				throw(cpps_error(catchfun->filename, buffer.line(), cpps_error_trycatherror, "Catch did not detect '('"));
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			std::string ifvar = cpps_parse_varname(buffer);
			if (ifvar != "var")
				throw(cpps_error(catchfun->filename, buffer.line(), cpps_error_trycatherror, "'var' needs to be defined to receive variables."));
			cpps_parse_rmspaceandenter(buffer);
			catchvar->s = cpps_parse_varname(buffer);
			catchvar->type = CPPS_VARNAME;
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ')') {
				throw(cpps_error(catchfun->filename, buffer.line(), cpps_error_trycatherror, "Catch did not detect ')'"));
			}
			buffer.pop();
			/* pop } */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_line(c, domain, catchstep, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
		}
		else {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_trycatherror, "Try needs corresponding catch."));
		}
	}
	void cpps_parse_char(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OINTEGER;

		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);

		//char('str')
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '(' after char"));
		}
		buffer.pop();

		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '\'') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing ' after char("));
		}
		buffer.pop();

		char lastch = buffer.realpop();
		/* todo 解析反斜杠 */
		char ch = cpps_parse_transfer_character(lastch, buffer);
		child->value.integer = (cpps_integer)ch;

		if (buffer.cur() != '\'') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing ' after char('"));
		}
		buffer.pop();

		cpps_parse_rmspaceandenter(buffer);

		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing ')' after char"));
		}
		buffer.pop();

		cpps_parse_rmspaceandenter(buffer);
	}
	node* cpps_parse_var_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, bool findparent) {
		node* param = new node(o->filename, buffer.line());
		param->type = CPPS_VARNAME;
		if (cpps_parse_isnumber(buffer.cur()))/* 首位是个字母 */
			throw(cpps_error(o->filename, buffer.line(), cpps_error_varnotnumber, "Variable cannot start with a number."));
		param->s = cpps_parse_varname(buffer);
		if (param->s == "new"){
			/* new 呀~ */ 
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			param->type = CPPS_ONEWVAR;
			param->s = cpps_parse_varname(buffer);
			if (param->s.empty()) {
				throw(cpps_error(param->filename, buffer.line(), cpps_error_varnotnumber, "The object of new must have a type."));
			}
			param->offset = 0;
			param->offsettype = 0;
			param->varsize = 0;
			param->size = 0;

			node* lastNode = param;
			/* 是否使用名空间 */
			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				if (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') {
					buffer.pop();
					buffer.pop();
					cpps_parse_rmspaceandenter(buffer);
					node* child = new node(lastNode, o->filename, buffer.line());
					child->s = cpps_parse_varname(buffer);
					child->type = CPPS_ONAMESPANCE_CHILD;
					lastNode = child;
				}
				else {
					break;
				}
			}
			cpps_parse_rmspaceandenter(buffer);
			if (param->s == "vector" && buffer.cur() == '[') {
				buffer.pop();
				node* count = new node(param, param->filename, buffer.line());
				cpps_parse_loadnumber(buffer, count);
				if (buffer.cur() != ']') {
					throw(cpps_error(param->filename, buffer.line(), cpps_error_varnotnumber, "']' was not detected when defining an array"));
				}
				buffer.pop();
			}
			else if (buffer.cur() == '(') {
				buffer.pop();
				node* child = new node(param->filename, buffer.line());
				child->type = CPPS_OCLASS_CONSTRUCTOR;
				cpps_node_domain* funcdomain = new cpps_node_domain(child,domain, cpps_domain_type_func, o->s);
				/* 为这个集创建一个自己的域空间 */
				child->setdomain(funcdomain);
				cpps_parse_dofunction(c, funcdomain, child, root, buffer);
				param->addtoright(child);
				/* var a = new A(){} */
				cpps_parse_rmspaceandenter(buffer);
				if (buffer.cur() == '{') {
					param->setparent(root);
					node* setv = cpps_parse_new_setv(c, domain, param, root, buffer); //root 修改成newvar node 但是这里取不到变量数量呀. 应该取类才对.
					param->add(setv);
				}
			}
		}
		else if (param->s == "this")
		{
			if (o->type == CPPS_ONIL)
				o->type = CPPS_OTHISPARAM;

			param->type = CPPS_OTHIS;
		}
		else if (param->s == "true" || param->s == "false") {
			/* 这说明什么？ 说明他是个bool.. */
			if (o->type == CPPS_ONIL)
				o->type = CPPS_OBOOLPARAM;
			param->type = CPPS_OBOOL;
		}
		else if (param->s == "nil" || param->s == "null" || param->s == "NULL") {
			param->type = CPPS_ONIL;
		}
		else if (param->s == "char")
		{
			cpps_parse_char(c, domain, param, root, buffer);
		}
		else if (param->s == "await")
		{
			/*协程等待..*/
			param->type = CPPS_OAWAIT;
			//node* awaitparam = new node(param, param->filename, buffer.line());
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, param, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
		}
		else {
			if (o->type == CPPS_ONIL)
				o->type = CPPS_OVARPARAM;
			if (findparent && c->buildoffset ) {
				node* t = o->getnode(param->s);
				bool closure = false; /*是否闭包*/
				if (!t && root->type == CPPS_ODEFVAR_LAMBDA_FUNC) {
					t = root->parent->getnode(param->s,true);
					closure = true;/*需要闭包*/
				}
				if (t) {
					param->type = CPPS_OOFFSET;
					param->offset = t->offset;
					param->offsettype = t->offsettype;
					if (closure && t->offsettype == CPPS_OFFSET_TYPE_SELF){ /*整理修改可能是闭包变量并且增加了计数,当没有使用的时候它就会被释放了..*/
						t->closure = true; //闭包创建永不删除
					}
				}
			}
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
		}
		return(param);
	}
	node* cpps_parse_number(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer) {
		node* str = new node(o->filename, buffer.line());
		str->type = CPPS_OINTEGER;
		cpps_parse_loadnumber(buffer, str);
		int32 off = buffer.offset();
		if (buffer.cur() == '.') {
			char ch = buffer.pop();
			if (!cpps_parse_isnumber(buffer.cur())) {
				/* 点后面不是数字说明不是小数点 */
				cpps_str2i64(str->s.c_str(), &str->value.integer);
				buffer.seek(off);
			}
			else {
				str->type = CPPS_ONUMBER;
				/* number呀. */
				str->s.push_back(ch);
				cpps_parse_loadnumber(buffer, str);
				cpps_str2d(str->s.c_str(), &str->value.number);
			}
		}
		else if (buffer.cur() == 'x') {
			str->type = CPPS_OINTEGER16;
			buffer.pop();
			str->s += "x";
			str->s += cpps_parse_loadinterger16(buffer);
			cpps_str2i64(str->s.c_str(), &str->value.integer);
		}
		else {
			cpps_str2i64(str->s.c_str(), &str->value.integer);
		}
		return(str);
	}
	node* cpps_parse_bracket(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		node* bracket = new node(o->filename, buffer.line());
		bracket->type = CPPS_OBRACKET;
		buffer.pop();
		/* pop ( */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, domain, bracket, root, buffer);
		cpps_parse_rmspaceandenter(buffer);
		buffer.pop();
		/* pop ) */
		return(bracket);
	}
	node* cpps_parse_object(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		node* bracket = new node(o->filename, buffer.line());
		bracket->type = CPPS_OOBJECT;
		buffer.pop();
		/* pop { */
		cpps_parse_rmspaceandenter(buffer);
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				buffer.pop();
				return(bracket);
			}
			node* n = new node(bracket, o->filename, buffer.line());
			node* k = new node(n, o->filename, buffer.line());
			k->s = cpps_parse_object_varname(buffer);
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':')
				throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "Definition object not detected '}'."));
			buffer.pop();
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			node* v = new node(n, o->filename, buffer.line());
			cpps_parse_expression(c, domain, v, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ',') {
				buffer.pop();
			}
		}
		throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "Definition object not detected '}'"));
		return(bracket);
	}
	node* cpps_parse_new_setv(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		node* bracket = new node(o->filename, buffer.line());
		bracket->setparent(o);
		bracket->type = CPPS_ONEW_SETV;
		buffer.pop();
		/* pop { */
		cpps_parse_rmspaceandenter(buffer);
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				buffer.pop();
				return(bracket);
			}
			node* n = new node(bracket, o->filename, buffer.line());
			node* k = new node(n, o->filename, buffer.line());
			k->s = cpps_parse_varname(buffer);
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != '=')
				throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "'=' was not detected when setting the variable"));
			buffer.pop();
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			node* v = new node(n, o->filename, buffer.line());
			cpps_parse_expression(c, domain, v, root, buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ',') {
				buffer.pop();
			}
		}
		throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "'}' was not detected when setting the variable"));
		return(bracket);
	}
	node* cpps_parse_array(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		node* bracket = new node(o->filename, buffer.line());
		bracket->type = CPPS_OARRAY;
		buffer.pop();
		/* pop [ */
		cpps_parse_rmspaceandenter(buffer);
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ']') {
				buffer.pop();
				return(bracket);
			}
			node* n = new node(bracket, o->filename, buffer.line());
			cpps_parse_expression(c, domain, n, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ',') {
				buffer.pop();
			}
		}
		throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "Definition array not detected '}'"));
		return(bracket);
	}
	node* cpps_parse_last_func(C* c, cppsbuffer& buffer, node* o, node* p, cpps_node_domain* domain, node* root) {
		/*有后续 */
		while (buffer.cur() == '[' || buffer.cur() == '.' || buffer.cur() == '(' || (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':')) {
			/*  */
			if (buffer.cur() == '.')
				if (cpps_parse_isnotvarname(buffer.at(buffer.offset() + 1)))
					break;
			char symblo = buffer.pop();
			node* geto = new node(o->filename, buffer.line());
			geto->setparent(o);
			geto->addtoleft(p);
			node* child = NULL;
			if (symblo == '[') {
				geto->type = CPPS_OGETCHIILD;
				child = new node(o->filename, buffer.line());
				child->setparent(geto);
				cpps_parse_expression(c, domain, child, root, buffer);
				if (!child)
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "'[' must be followed by a parameter"));
				if (buffer.cur() != ']')
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "']' parsing error"));
				buffer.pop();
			}
			else if (symblo == '.') {
				geto->type = CPPS_OGETOBJECT;

				//child = cpps_parse_var_param(c, domain, geto, root, buffer, false);
				child = new node(o->filename, buffer.line());
				child->type = CPPS_VARNAME;
				cpps_parse_var_varname(buffer, child, c, root, domain);
				child->setparent(geto);
				if (!child)
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "The variable name is required later on '.'"));
			}
			else if (symblo == '(')
				/* 是调用函数 */ {
				/* 获取里面 参数 */
				geto->type = CPPS_ODOFUNCTION;
				child = new node(o->filename, buffer.line());
				child->setparent(geto);
				cpps_node_domain* funcdomain = new cpps_node_domain(child,domain, cpps_domain_type_func, o->s);
				/* 为这个集创建一个自己的域空间 */
				child->setdomain(funcdomain);
				cpps_parse_dofunction(c, funcdomain, child, root, buffer);
			}
			else if (symblo == ':') {
				if (buffer.cur() != ':')
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "namespace requires double ':'"));
				buffer.pop();
				geto->type = CPPS_OGETOBJECT;
				//child = cpps_parse_var_param(c, domain, geto, root, buffer, false);
				child = new node(o->filename, buffer.line());
				child->type = CPPS_VARNAME;
				cpps_parse_var_varname(buffer, child, c, root, domain);
				if (!child)
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "namespace:: need a name is required later."));
			}
			geto->addtoright(child);
			/* 放到后续. */
			p = geto;
		}
		return(p);
	}
	node* cpps_parse_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		char ch = buffer.cur();
		node* p = NULL;
		if (ch == '"') {
			p = cpps_parse_string(c, domain, o, root, buffer, '"');
		}
		else if (ch == '\'') {
			p = cpps_parse_string(c, domain, o, root, buffer, '\'');
		}
		else if (ch == '{') {
			p = cpps_parse_object(c, domain, o, root, buffer);
		}
		else if (ch == '[') {
			p = cpps_parse_array(c, domain, o, root, buffer);
		}
		else if (cpps_parse_isnumber(ch))
			/* 数字参数   s */ {
			return cpps_parse_number(c, domain, o, buffer);
		}
		else if (!cpps_parse_isnotvarname(ch))
			/* 变量参数 */ {
			 p = cpps_parse_var_param(c, domain, o, root, buffer, true);
		}
		else if (ch == '(') {
			p = cpps_parse_bracket(c, domain, o, root, buffer);
		}

		if(p)
			p = cpps_parse_last_func(c, buffer, o, p, domain, root);

		return p;
	}
	node* cpps_parse_symbol(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer, bool leftsymbol) {
		std::string	symbolStr;
		std::string	tempStr;
		char		ch = buffer.cur();
		int32		curoffset = buffer.offset();
		while (cpps_parse_issymbol(ch)) {
			tempStr += ch;
			cpps_symbol* symbol = cpps_parse_getsymbol(tempStr, leftsymbol);
			if (!symbol) {
				if (symbolStr.empty() && !cpps_parse_issymbol(buffer.at(curoffset + 1))) {
					return(NULL);
				}
			}
			else {
				symbolStr = tempStr;
			}
			curoffset += 1;
			ch = buffer.at(curoffset);
		}
		cpps_symbol* symbol = cpps_parse_getsymbol(symbolStr, leftsymbol);
		if (!symbol) {
			return(NULL);
		}
		buffer.seek(buffer.offset() + (int32)symbolStr.size());
		node* pNode = new node(o->filename, buffer.line());
		pNode->type = CPPS_FUNCNAME;
		pNode->symbol = symbol;
		/* pNode->s = symbol->symbolfuncname; */
		return(pNode);
	}
	void cpps_parse_expression(C* c, cpps_node_domain* domain, node* param, node* root, cppsbuffer& buffer) {
		node* lastOpNode = param;
		while (cpps_parse_expression_step(c, domain, param, lastOpNode, root, buffer) == CPPS_NOERROR)
			;
	}
	std::string buildlambda() {
		std::string	ret = "lamdba@";
		int		i;
		int		len = 20;
		for (i = 0; i < len; ++i)
			ret += 'A' + rand() % 26;
		return(ret);
	}
	int32 cpps_parse_expression_step(C* c, cpps_node_domain* domain, node* param, node*& lastopnode, node* root, cppsbuffer& buffer) {
		/* 新增lambda函数参数 */
		if (buffer.cur() == '[' && buffer.at(buffer.offset() + 1) == ']' && buffer.at(buffer.offset() + 2) == '(') {
			buffer.seek(buffer.offset() + 3);
			node* r = cpps_get_root_node(root);
			node* lamdbavar = new node(param->filename, buffer.line());
			lamdbavar->setparent(lastopnode);
			lamdbavar->type = CPPS_ODEFVAR;
			r->l.insert(r->l.begin(), lamdbavar);
			node* str = new node(lamdbavar, param->filename, buffer.line());
			str->s = buildlambda();
			str->type = CPPS_VARNAME;

			//加快运行速度
			if (c->buildoffset) {
				node* r = cpps_get_root_node(root);
				str->offset = r->size++;
				str->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
			}

			node* lambda = new node(str, param->filename, buffer.line());
			lambda->type = CPPS_ODEFVAR_LAMBDA_FUNC;
			lambda->size = 0;
			if (root->type == CPPS_ODEFVAR_LAMBDA_FUNC || root->type == CPPS_ODEFVAR_FUNC){
				lambda->size = root->size;/*为了兼容闭包*/
			}
			usint16 takesize = lambda->size;
			cpps_parse_def_function(c, domain, lambda, lambda, buffer);
			/* 定义了一个函数 */
			node* lambdaparam = new node(param->filename, buffer.line());
			lambdaparam->type = CPPS_VARNAME_LAMBDA;
			lambdaparam->s = str->s;
			if (c->buildoffset) {
				lambdaparam->offset = str->offset;
				lambdaparam->offsettype = str->offsettype;
			}
			lambdaparam->size = takesize; //记录当时使用时它父类长度.
			lambdaparam->setparent(lastopnode);
			lambdaparam = cpps_parse_last_func(c, buffer, lastopnode, lambdaparam, domain, root);
			lastopnode->l.push_back(lambdaparam);
			return(1);
		}
		node* p = NULL;
		node* leftoproot = NULL;
		node* leftopparent = NULL;
		node* leftop = NULL;
		while ((leftop = cpps_parse_symbol(c, domain, param, buffer, true)) && leftop->symbol && leftop->symbol->getparamnum() == 1) {
			if (leftoproot == NULL) {
				leftoproot = leftop;
				leftopparent = leftop;
			}
			else {
				leftopparent->addtoleft(leftop);
				leftopparent = leftop;
			}
			cpps_parse_rmspaceandenter(buffer);
		}
		p = cpps_parse_param(c, domain, param, root, buffer);
		if (!p) {
			/* 没有参数 */
			throw(cpps_error(param->filename, buffer.line(), cpps_error_paramerror, "invalid syntax."));
		}
		cpps_parse_rmspaceandenter(buffer);
		node* op = cpps_parse_symbol(c, domain, param, buffer);
		if (op == NULL) {
			if (leftoproot != NULL) {
				leftopparent->addtoleft(p);
				p = leftoproot;
			}
			if (!lastopnode->symbol) {
				p->setparent(lastopnode);
				lastopnode->l.push_back(p);
			}
			else if (lastopnode->type == CPPS_FUNCNAME) {
				if (lastopnode->symbol && lastopnode->symbol->getparamnum() > 2)
					lastopnode->add(p); else
					lastopnode->addtoright(p);
			}
			else {
				lastopnode->addtoleft(p);
			}
			return(1);
		}
		/*三元表达式 */
		if (op->symbol && op->symbol->symboltype == CPPS_SYMBOL_TYPE_TERNARYOPERATOR) {
			if (lastopnode && lastopnode->type == CPPS_FUNCNAME && lastopnode->symbol && op->symbol->getprio() > lastopnode->symbol->getprio()) {
				if (leftoproot != NULL) {
					leftopparent->addtoleft(p);
					p = leftoproot;
				}
				if (!lastopnode->symbol) {
					p->setparent(lastopnode);
					lastopnode->l.push_back(p);
				}
				else if (lastopnode->type == CPPS_FUNCNAME) {
					if (lastopnode->symbol && lastopnode->symbol->getparamnum() > 2)
						lastopnode->add(p); else
						lastopnode->addtoright(p);
				}
				else {
					lastopnode->addtoleft(p);
				}
				p = lastopnode;
				op->setparent(lastopnode->parent);
				bool isLeft = (lastopnode == lastopnode->parent->getleft());
				if (isLeft)
					lastopnode->parent->addtoleft(op); else
					lastopnode->parent->addtoright(op);
				lastopnode->parent = op;
				lastopnode = op;
			}
			op->add(p);
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, op, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':') {
				throw(cpps_error(param->filename, buffer.line(), cpps_error_paramerror, "Ternary expression parsing error."));
			}
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, op, root, buffer);
			p = op;
			op = cpps_parse_symbol(c, domain, param, buffer);
			if (op == NULL) {
				if (lastopnode == p) {
					return(1);
				}
				if (leftoproot != NULL) {
					leftopparent->addtoleft(p);
					p = leftoproot;
				}
				if (!lastopnode->symbol) {
					p->setparent(lastopnode);
					lastopnode->l.push_back(p);
				}
				else if (lastopnode->type == CPPS_FUNCNAME) {
					if (lastopnode->symbol && lastopnode->symbol->getparamnum() > 2)
						lastopnode->add(p); else
						lastopnode->addtoright(p);
				}
				else {
					lastopnode->addtoleft(p);
				}
				return(1);
			}
		}
		if (op->symbol && op->symbol->getparamnum() == 1) {
			if (leftoproot != NULL) {

				if (leftoproot->symbol->getprio() <= op->symbol->getprio())
				{
					op->addtoleft(p);
					p = op;
					op = leftoproot;
				}
				else {
					leftopparent->addtoleft(p);
					p = leftoproot;
				}

			}
			op->addtoleft(p);
			p = op;
			op = cpps_parse_symbol(c, domain, param, buffer);
			if (op == NULL) {
				if (!lastopnode->symbol) {
					p->setparent(lastopnode);
					lastopnode->l.push_back(p);
				}
				else if (lastopnode->type == CPPS_FUNCNAME) {
					if (lastopnode->symbol->getparamnum() > 2)
						lastopnode->add(p); else
						lastopnode->addtoright(p);
				}
				else {
					lastopnode->addtoleft(p);
				}
				return(1);
			}
		}
		cpps_parse_rmspaceandenter(buffer);
		
		node* take = op;
		if (leftoproot != NULL) {
			if (leftoproot->symbol->getprio() <= op->symbol->getprio())
			{
				op->addtoleft(leftoproot);
				leftopparent->addtoleft(p);
			}
			else {
				op->addtoleft(p);
				leftopparent->addtoleft(op);
				op = leftoproot;
			}
		}
		else {
			op->addtoleft(p);
		}
		if (lastopnode && lastopnode->type == CPPS_FUNCNAME && lastopnode->symbol) {
			if (lastopnode->symbol->getparamnum() > 2)
				lastopnode->add(op); else
				lastopnode->addtoright(op);
		}
		else if (lastopnode) {
			p->setparent(lastopnode);
			lastopnode->l.push_back(op);
			if (lastopnode->symbol && lastopnode->symbol->getparamleftlimit()) {
				if (op->type != CPPS_VARNAME && op->type != CPPS_OOFFSET && op->type != CPPS_OGETOBJECT && op->type != CPPS_OGETCHIILD) {
					throw(cpps_error(op->filename, op->line, cpps_error_paramerror, "The left side of %s must be a variable", lastopnode->s.c_str()));
				}
				if (op->type == CPPS_VARNAME)
					op->type = CPPS_QUOTEVARNAME; else if (op->type == CPPS_OGETOBJECT)
					op->type = CPPS_QUOTEGETOBJECT; else if (op->type == CPPS_OGETCHIILD)
					op->type = CPPS_QUOTEGETCHIILD; else if (op->type == CPPS_OOFFSET)
					op->type = CPPS_QUOTEOFFSET;
			}
			if (lastopnode->symbol && lastopnode->symbol->getparamrightlimit()
				&& lastopnode->symbol->getparamnum() == 1) {
				if (op->type != CPPS_VARNAME && op->type != CPPS_OOFFSET && op->type != CPPS_OGETOBJECT && op->type != CPPS_OGETCHIILD) {
					throw(cpps_error(op->filename, op->line, cpps_error_paramerror, " The right side of %s must be a variable", lastopnode->s.c_str()));
				}
				if (op->type == CPPS_VARNAME)
					op->type = CPPS_QUOTEVARNAME; else if (op->type == CPPS_OGETOBJECT)
					op->type = CPPS_QUOTEGETOBJECT; else if (op->type == CPPS_OGETCHIILD)
					op->type = CPPS_QUOTEGETCHIILD; else if (op->type == CPPS_OOFFSET)
					op->type = CPPS_QUOTEOFFSET;
			}
			op->setparent(lastopnode);
		}
		/* 找到需要交换的地方，然后交换树 */
		node* compareop = op->parent;
		bool	needchange = false;
		bool	isleft = true;
		while (true) {
			if (compareop->type != CPPS_FUNCNAME) {
				break;
			}
			if (compareop->symbol == NULL) {
				break;
			}
			if (op->symbol == NULL) {
				break;
			}
			if (op->symbol->getprio() < compareop->symbol->getprio()) {
				break;
			}
			if (op->symbol->getprio() == compareop->symbol->getprio() && !cpps_parse_isleftasso(op->symbol->getprio())) {
				break;
			}
			isleft = (compareop == compareop->parent->getleft());
			compareop = compareop->parent;
			needchange = true;
		}
		if (needchange) {
			op->parent->addtoright(op->getleft());
			if (isleft) {
				op->addtoleft(compareop->getleft());
				compareop->addtoleft(op);
			}
			else {
				op->addtoleft(compareop->getright());
				compareop->addtoright(op);
			}
		}
		lastopnode = take;
		return(CPPS_NOERROR);
	}
	void cpps_parse_dofunction(C* c, cpps_node_domain* domain, node* param, node* root, cppsbuffer& buffer) {
		char ch = 0;

		size_t cc = 0;

		while (!buffer.isend()) {
			cpps_parse_rmspaceandenter(buffer);
			ch = buffer.cur();
			if (ch == ')') {
				buffer.pop();
				return;
			}

			

			if (cc > 0 && ch == ',')
				buffer.pop();
			else if (cc > 0)
				return;
			cc++;
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, param, root, buffer);
		}
		/* 未找到） 就返回了. 这是错的啊.. */
		throw(cpps_error(param->filename, buffer.line(), cpps_error_paramerror, "The calling function did not find ')'."));
	}
	void cpps_parse_assemble(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		cpps_node_domain* assdomain = new cpps_node_domain(o,domain, cpps_domain_type_ass, "assemble");
		/* 为这个集创建一个自己的域空间 */
		o->setdomain(assdomain);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		int holdline = buffer.line();
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				buffer.pop();
				return;
			}
			cpps_parse_line(c, assdomain, o, root, buffer);
		}
		throw(cpps_error(o->filename, holdline, cpps_error_assembleerror, "The '}' of the execution collection was not found."));
	}
	void cpps_parse_def(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		char ch = buffer.cur();
		if (!cpps_parse_isnotvarname(ch))
			/* 又是个名字. 那应该是定义变量 */ {
			if (child->s == "var") {
				child->type = CPPS_ODEFVAR;
			}
			cpps_parse_var(c, domain, child, root, buffer, limit, node_var_type::node_var_type_var);
			/* 其他类型的变量 */
		}
		else if (!child->s.empty()) {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_varerror, "Missing '=' in front of '%c'.", ch));
		}
	}
	void cpps_parse_if(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* ifdomain = new cpps_node_domain(child,domain, cpps_domain_type_if, "if");
		/* 为这个集创建一个自己的域空间 */
		child->setdomain(ifdomain);
		child->type = CPPS_OIF;
		node* param = new node(child, child->filename, buffer.line());
		/* 0 if的参数 */
		node* t = new node(child, child->filename, buffer.line());
		/* true */
		node* f = new node(child, child->filename, buffer.line());
		/* false */
		if (buffer.cur() != '(') {
			/* if 后面必须接() */
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing '('after if."));
		}
		buffer.pop();
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, ifdomain, param, root, buffer);
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing ')' after if."));
		}
		buffer.pop();
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, ifdomain, t, root, buffer);
		cpps_parse_rmspaceandenter(buffer);
		int32 offset = buffer.offset();
		/* 记住当前的字符， 因为下一句可能不是else */
		std::string el = cpps_parse_varname(buffer);
		if (el == "else") {
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_line(c, domain, f, root, buffer);
		}
		else {
			buffer.seek(offset);
			/* rollback 回滚... */
		}
	}
	bool cpps_parse_canreturn(cpps_node_domain* domain) {
		if (domain->domainType == cpps_domain_type_func) {
			return(true);
		}
		if (domain->parent && cpps_parse_canreturn(domain->parent)) {
			return(true);
		}
		return(false);
	}
	bool cpps_parse_canthrow(cpps_node_domain* domain) {
		if (domain->domainType == cpps_domain_type_trycatch) {
			return(true);
		}
		if (domain->parent && cpps_parse_canthrow(domain->parent)) {
			return(true);
		}
		return(false);
	}
	bool cpps_parse_canbreak(cpps_node_domain* domain) {
		if (domain->domainType == cpps_domain_type_for || domain->domainType == cpps_domain_type_foreach || domain->domainType == cpps_domain_type_while) {
			return(true);
		}
		if (domain->parent && cpps_parse_canbreak(domain->parent)) {
			return(true);
		}
		return(false);
	}
	bool cpps_parse_cancontinue(cpps_node_domain* domain) {
		if (domain->domainType == cpps_domain_type_for || domain->domainType == cpps_domain_type_foreach || domain->domainType == cpps_domain_type_while) {
			return(true);
		}
		if (domain->parent && cpps_parse_cancontinue(domain->parent)) {
			return(true);
		}
		return(false);
	}
	void cpps_parse_return(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		/* 先检测 这个return 是否合法. */
		if (!cpps_parse_canreturn(domain)) {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_deffuncrror, "Unknown return. Return must be defined in function."));
		}
		child->type = CPPS_ORETURN;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		/* 如果后续没有参数那就让他返回一个nil. */
		if (buffer.cur() == ';') {
			buffer.pop();
			new node(child, child->filename, child->line);
			return;
		}
		//支持multi return
		while (true) {

			/* 查找后续参数 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, child, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ',') break;
			
			buffer.pop(); // pop ,

		}
	}
	void cpps_parse_throw(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		/* 先检测 这个return 是否合法. */
		/*if (!cpps_parse_canthrow(domain)) {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_trycatherror, "Unknown throw. Throw must be defined after try!"));
		}*/
		child->type = CPPS_OTHROW;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		/* 查找后续参数 */
		cpps_parse_expression(c, domain, child, root, buffer);
	}
	void cpps_parse_for(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* fordomain = new cpps_node_domain(child,domain, cpps_domain_type_for, "for");
		child->setdomain(fordomain);
		child->type = CPPS_OFOR;
		node* for1 = new node(child, child->filename, buffer.line());
		node* for2 = new node(child, child->filename, buffer.line());
		node* for3 = new node(child, child->filename, buffer.line());
		node* for4 = new node(child, child->filename, buffer.line());
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing '(' after for"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ';')
			cpps_parse_line(c, fordomain, for1, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ';') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing first ';' after for"));
		}
		buffer.pop();
		/* pop ; */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ';')
			cpps_parse_expression(c, fordomain, for2, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ';') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing second ';' after for"));
		}
		buffer.pop();
		/* pop */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')')
			cpps_parse_line(c, fordomain, for3, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DEFVAR | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing ')'after for"));
		}
		buffer.pop();
		/* pop ) */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, fordomain, for4, root, buffer);
	}
	void cpps_parse_foreach(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* fordomain = new cpps_node_domain(child,domain, cpps_domain_type_for, "for");
		child->setdomain(fordomain);
		child->type = CPPS_OFOREACH;
		node* for1 = new node(child, child->filename, buffer.line());
		node* for2 = new node(child, child->filename, buffer.line());
		node* for4 = new node(child, child->filename, buffer.line());
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing '('after foreach"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, fordomain, for1, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS | CPPS_NOT_DEFVAR);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ':')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing ':'after foreach"));
		buffer.pop();
		/* for2 修改成获取表达式 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, fordomain, for2, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing ')'after foreach"));
		}
		buffer.pop();
		/* pop ) */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, fordomain, for4, root, buffer);
	}
	void cpps_parse_while(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* whiledomain = new cpps_node_domain(child,domain, cpps_domain_type_while, "while");
		child->setdomain(whiledomain);
		child->type = CPPS_OWHILE;
		node* while1 = new node(child, child->filename, buffer.line());
		node* while2 = new node(child, child->filename, buffer.line());
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_whileerror, "Missing '('after while"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, whiledomain, while1, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_whileerror, "Missing ')'after while"));
		}
		buffer.pop();
		/* pop ) */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_line(c, whiledomain, while2, root, buffer);
	}
	node* cpps_parse_getparent_node(node* root, node* parent) {
		node* ret = NULL;
		ret = root->getnode(parent->s);
		if (!parent->l.empty())
			ret = cpps_parse_getparent_node(ret, parent->l[0]);
		return(ret);
	}
	void cpps_parse_reg_base_class(C*c,std::set<node*>& parentset, node* srcclass, node* baseclass) {
		if (parentset.find(baseclass) != parentset.end())
			throw(cpps_error(srcclass->filename, srcclass->line, cpps_error_classerror, "[%s] base class is defined repeatedly", baseclass->s.c_str()));
		parentset.insert(baseclass);
		/* 父类的 */
		node* base_parents = baseclass->getleft();
		node* base_vars = baseclass->getright();
		/* 在注册父类的父类 */
		for (auto parent : base_parents->l) {
			node* parent_node = cpps_parse_getparent_node(baseclass, parent);
			if (!parent_node)
				throw(cpps_error(srcclass->filename, srcclass->line, cpps_error_classerror, "[%s] base class is not defined", parent->s.c_str()));
			cpps_parse_reg_base_class(c,parentset, srcclass, parent_node);
		}
		/*
		 * 注册父类函数因为函数都移动到了global.所以可以共用
		 * 先注册父类vars...
		 */
		for (auto var : base_vars->l) {
			node* var_node = var->l[0];
			if (!var_node->l.empty())
				/*有没有值.不是函数就要注册上. */ {
				node* cpyn = new node();
				cpyn->clone(var_node);
				if (c->buildoffset) {
					if (var_node->l[0]->type != CPPS_ODEFVAR_FUNC) {
						cpyn->offset = srcclass->varsize++;
						cpyn->offsettype = CPPS_OFFSET_TYPE_LEFTDOMAIN;
					}
					srcclass->regnode(var_node->s, cpyn);
				}
				
			}
		}
	}
	void cpps_parse_class(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OCLASS;
		/*
		 * child->offset = 0;
		 * child->offsettype = 0;
		 */
		child->varsize = 0;
		child->size = 0;
		node* parents = new node(child, child->filename, buffer.line());
		node* vars = new node(child, child->filename, buffer.line());
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		child->s = cpps_parse_varname(buffer);
		if (c->buildoffset) {
			if (root->type == CPPS_ROOT)
				/* 根节点.. */ {
				child->offset = root->size++;
				child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				root->regnode(child->s, child);
			}
			else if (root->type == CPPS_ONAMESPACE) {
				node* r = cpps_get_root_node(root);
				/* namespace不注册到自己里面 */
				child->offset = r->size++;
				child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				root->regnode(child->s, child);
			}
		}
		std::set<node*> parentset;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() == ':') {
			/* 说明有继承关系啊 */
			buffer.pop();
			while (!buffer.isend()) {
				/* 剔除空格 */
				cpps_parse_rmspaceandenter(buffer);
				node* parent = new node(parents, parents->filename, buffer.line());
				parent->s = cpps_parse_varname(buffer);
				/* 2020-11-10 增加支持继承namespace */
				node* lastNode = parent;
				/* 是否使用名空间 */
				while (!buffer.isend()) {
					/* 剔除回车. */
					cpps_parse_rmspaceandenter(buffer);
					if (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') {
						buffer.pop();
						buffer.pop();
						cpps_parse_rmspaceandenter(buffer);
						node* child = new node(lastNode, parents->filename, buffer.line());
						child->s = cpps_parse_varname(buffer);
						child->type = CPPS_ONAMESPANCE_CHILD;
						lastNode = child;
					}
					else {
						break;
					}
				}
				if (c->buildoffset) {
					node* parent_node = cpps_parse_getparent_node(root, parent);
					if (!parent_node)
						throw(cpps_error(child->filename, child->line, cpps_error_classerror, "[%s] base class is not defined", parent->s.c_str()));
					cpps_parse_reg_base_class(c, parentset, child, parent_node);
				}
				if (buffer.cur() != ',') {
					break;
				}
				buffer.pop();
			}
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '{') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '{' after class"));
		}
		buffer.pop();
		/* pop } */
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				break;
			}
			if (buffer.isend()) {
				throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '}'after class"));
			}
			cpps_parse_line(c, domain, vars, child, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_USEBUILTIN);
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '}') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '}'after class"));
		}
		buffer.pop();
		/* pop } */
	}
	void cpps_parse_import(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OIMPORT;
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, domain, child, root, buffer);
	}
	std::string cpps_rebuild_filepath(std::string path)
	{
		/*1.先查工作路径.*/
		std::string tmppath = cpps_getcwd() + "/" + path;
		if (cpps_io_file_exists(tmppath)) return tmppath;
		/*2.查执行文件所在目录.*/
		tmppath = cpps_real_path() + path;
		if (cpps_io_file_exists(tmppath)) return tmppath;
		/*3.绝对路径.*/
		tmppath = path;
		if (cpps_io_file_exists(tmppath)) return tmppath;
		return "";
	}
	
	
	void cpps_parse_include(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OINCLUDE;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		//#include <cpps/cpps.cpp> √
		if (buffer.cur() != '<') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '<' after include"));
		}
		buffer.pop();
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		std::string path = cpps_parse_include_varname(buffer);
		child->s = path;
		path = cpps_rebuild_filepath(path);
		if(path.empty()) throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "can't include file : %s", child->s.c_str()));
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '>') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '>'after include"));
		}
		buffer.pop();
		/*载入文件*/
		std::string fileSrc;
		cpps_load_filebuffer(path.c_str(), fileSrc);
		buffer.append(path,fileSrc.c_str(),(int32) fileSrc.size());

	}
	void cpps_parse_dofile(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_ODOFILE;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing '(' after dofile"));
		}
		buffer.pop();
		/* pop ( */
		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ')') {
				break;
			}
			if (buffer.isend()) {
				throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing ')' after dofile"));
			}
			cpps_parse_expression(c, domain, child, root, buffer);
			/* 是否到最后了. */
			if (buffer.cur() == ',') {
				buffer.pop();
				/* pop , */
			}
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing ')' after dofile"));
		}
		buffer.pop();
		/* pop ) */
	}
	void cpps_parse_dostring(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_ODOSTRING;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing '(' after dostring"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, domain, child, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing ')' after dostring"));
		}
		buffer.pop();
		/* pop ) */
	}
	void cpps_parse_break(C* c, cpps_node_domain* domain, node* child, cppsbuffer& buffer) {
		if (!cpps_parse_canbreak(domain)) {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_deffuncrror, "Unknown break must be defined in while or for."));
		}
		child->type = CPPS_OBREAK;
	}
	void cpps_parse_continue(C* c, cpps_node_domain* domain, node* child, cppsbuffer& buffer) {
		if (!cpps_parse_cancontinue(domain)) {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_deffuncrror, "Unknown continue must be defined in while or for"));
		}
		child->type = CPPS_OCONTINUE;
	}
	void cpps_parse_yield(C* c, cpps_node_domain* domain, node* child, cppsbuffer& buffer) {
		child->type = CPPS_OYIELD;
	}
	void cpps_parse_builtin(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		cpps_parse_rmspaceandenter(buffer);
		if (child->s == "if") {
			cpps_parse_if(c, domain, child, root, buffer);
		}
		else if (child->s == "return") {
			cpps_parse_return(c, domain, child, root, buffer);
		}
		else if (child->s == "throw") {
			cpps_parse_throw(c, domain, child, root, buffer);
		}
		else if (child->s == "for") {
			cpps_parse_for(c, domain, child, root, buffer);
		}
		else if (child->s == "foreach") {
			cpps_parse_foreach(c, domain, child, root, buffer);
		}
		else if (child->s == "while") {
			cpps_parse_while(c, domain, child, root, buffer);
		}
		else if (child->s == "class") {
			cpps_parse_class(c, domain, child, root, buffer);
		}
		else if (child->s == "dofile") {
			cpps_parse_dofile(c, domain, child, root, buffer);
		}
		else if (child->s == "dostring") {
			cpps_parse_dostring(c, domain, child, root, buffer);
		}
		else if (child->s == "break") {
			cpps_parse_break(c, domain, child, buffer);
		}
		else if (child->s == "continue") {
			cpps_parse_continue(c, domain, child, buffer);
		}
		else if (child->s == "yield")
		{
			cpps_parse_yield(c, domain, child, buffer);
		}
		else if (child->s == "const") {
			child->s = cpps_parse_varname(buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (child->s == "var")
				cpps_parse_var(c, domain, child, root, buffer, limit, node_var_type::node_var_type_constvar); else
				throw(cpps_error(child->filename, buffer.line(), cpps_error_normalerror, "const needs to use var later.", buffer.cur()));
		}
		else if (child->s == "async") {
			child->s = cpps_parse_varname(buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (child->s == "var")
				cpps_parse_var(c, domain, child, root, buffer, limit, node_var_type::node_var_type_asyncvar); else
				throw(cpps_error(child->filename, buffer.line(), cpps_error_normalerror, "async needs to use var later.", buffer.cur()));
		}
		else if (child->s == "var") {
			cpps_parse_var(c, domain, child, root, buffer, limit, node_var_type::node_var_type_var);
		}
		else if (child->s == "namespace") {
			cpps_parse_namespace(c, domain, child, root, buffer, limit);
		}
		else if (child->s == "module"){
			cpps_parse_module(c, domain, child, root, buffer, limit);
		}
		else if (child->s == "try") {
			cpps_parse_trycatch(c, domain, child, root, buffer, limit);
		}
	}
	node* cpps_parse_line(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int32 limit) {
		node* child = new node(o, buffer.getcurfile().filename, buffer.line());
		while (buffer.cur() == ';') buffer.pop(); /*pop空行*/
		if (buffer.cur() == '{') {
			if (limit & CPPS_NOT_DEFASSEMBLE) {
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "Definition assemble not allowed"));
			}
			buffer.pop();
			/* 这是个集~ */
			child->type = CPPS_OASSEMBLE;
			cpps_parse_assemble(c, domain, child, root, buffer);
			/* remove idxvar */
		}
		else if (buffer.cur() == '#'){
			/* 剔除空格 */
			if (limit & CPPS_NOT_DEFSYSTEM) {
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "Definition assemble not allowed"));
			}
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			child->s = cpps_parse_varname(buffer);
			if (child->s == "include") {
				cpps_parse_include(c, domain, child, root, buffer);
			}
			else if (child->s == "import") {
				cpps_parse_import(c, domain, child, root, buffer);
			}
			else {
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "parse error [%s] .", child->s.c_str()));
			}
		}
		else {
			if (cpps_parse_isnumber(buffer.cur())) {
				/* 首字母为 数字的话肯定有问题.想都别想.. */
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "The first letter of an expression cannot be a number '%c'", buffer.cur()));
			}
			int32 offset = buffer.offset();
			/* 记录一下 先检测是否为表达式 */
			child->s = cpps_parse_varname(buffer);
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			/* 检测表达式 */
			if (cpps_parse_isbuiltinname(child->s)) {
				if (limit & CPPS_NOT_USEBUILTIN && child->s != "var" && child->s != "const") {
					throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "builtin are not allowed"));
				}
				cpps_parse_builtin(c, domain, child, root, buffer, limit);
			}
			else if (o->parent && o->parent->type == CPPS_OCLASS && child->s == o->parent->s)
				/* 构造函数 */ {
				/* 剔除空格 */
				child->type = CPPS_ODEFVAR;
				cpps_parse_rmspaceandenter(buffer);
				node* str = new node(child, child->filename, buffer.line());
				str->type = CPPS_VARNAME;
				/* 先找名字 */
				str->s = "constructor";
				cpps_parse_var_right(c, domain, str, root, buffer, CPPS_NOT_DEFVAR,node_var_type::node_var_type_var);
			}
			else {
				/* 如果是表达式的话 */
				buffer.seek(offset);
				/* 重置回去 */
				child->type = CPPS_OEXPRESSION;
				cpps_parse_expression(c, domain, child, root, buffer);
			}
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			if ((limit & CPPS_NOT_DONTDELETEEND) == 0) {
				if (buffer.cur() != ';') {
					/* 未找到;号. 该不该报错呢？ */
				}
				else if (buffer.cur() == ';') {
					buffer.pop();
				}
			}
		}
		return(child);
	}
	node* loadbuffer(C* c, std::string& str, std::string filename) {
		/* 如果需要转译 */
		if (c->func)
			str = c->func(str);
		node* o = new node(filename);
		cpps_node_domain* domain = new cpps_node_domain(o, NULL, cpps_domain_type_root, "root");
		o->setdomain(domain);
		o->type = CPPS_ROOT;
		o->size = 0;
		o->varsize = 0;
		cppsbuffer buffer(filename.c_str(), str.c_str(), (int32)str.size());
		while (true) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.isend()) {
				break;
			}
			cpps_parse_line(c, domain, o, o, buffer);
		}
		return(o);
		/* 返回根节点 */
		return(NULL);
	}
	void cpps_create_root_G(C* c) {
		c->_G = new cpps_domain(NULL, cpps_domain_type_root, "root");
		/* 创建根节点域 */
		c->_G->setgcLevel(2);
		/* 永久存在 除非程序没了. */
		cpps_regvar* v = new cpps_regvar();
		/* _G 为根节点 */
		v->setvarname("_G");
		v->setval(cpps_value(c->_G));
		/* 域列表会copy进去 */
		c->_G->regvar(NULL, v);
		/* 将自己注册成_G.. */
	}

	void cpps_reglib(C* c)
	{
		std::string libmainpath = cpps_real_path() + "lib/main.cpp";
		if(cpps_io_file_exists(libmainpath))
			dofile(c, libmainpath.c_str());
	}

	cpps::C* create(int argc, char** argv) {
		C* c = new cpps::C(argc, argv);
		cpps_create_root_G(c);
		cpps_regstring(c);
		cpps_regbase(c);
		cpps_regsymbols(c);
		cpps_regmath(c);
		cpps_regmap(c);
		cpps_regio(c);
		cpps_regtime(c);
		cpps_regarray(c);
		cpps_reggc(c);
		cpps_regdebug(c);
		cpps_reglock(c);
		cpps_regrange(c);
		cpps_regconsole(c);
		cpps_reglambdafunction(c);
		cpps_regasyncio(c);
		cpps_reglib(c);

		return(c);
	}
	int32 dostring(C* c, std::string str) {
		_CPPS_TRY
			node* o = loadbuffer(c,  str, "");
		if (o)
			cpps_step_all(c, CPPS_MUNITRET, 0, 0, o);
		/* dostring pcall */
		_CPPS_CATCH
			cpps_gc_check_step(c);
		return(CPPS_NOERROR);
	}
	int32 loadfile(cpps::C* c, const char* path) {
		c->buildoffset = true;
		std::string fileSrc;
		cpps_load_filebuffer(path, fileSrc);
		node* o = loadbuffer(c, fileSrc, path);
		c->buildoffset = false;
		if (o) {
			c->push(o);
			return(CPPS_NOERROR);
		}
		return(1);
	}
	int32 dofile(cpps::C* c, const char* path) {
		return(loadfile(c, path) || pcall(c, CPPS_MUNITRET, c->_G, c->o));
	}
	void  cpps_destory_node(node* d) {
		if (!d) return;
		for (std::vector<node*>::iterator it = d->l.begin(); it != d->l.end(); ++it) {
			node* n = *it;
			if (n) {
				cpps_destory_node(n);
				delete n;
			}
		}
		d->l.clear();
	}

	void cpps_free_all_library(cpps::C*& c)
	{
		for (auto lib : c->modulelist) {
			if(lib.second != NULL) cpps_detach_library(lib.second, lib.first, c);
		}
		c->modulelist.clear();
	}

	int32 close(cpps::C*& c) {
		cpps_destory_node(c->o);
		delete c->o;
		c->o = NULL;
		/* 清理内存 */
		
		
		c->_G->destory(c, true);
		gc_cleanup(c);
		cpps_unregasyncio(c);
		cpps_free_all_library(c);
		c->barrierList.clear();
		c->_callstack->clear();
		c->_class_map_classvar.clear();
		//asyncio需要特殊处理
		delete c->_G;
		c->_G = NULL;
		delete c;
		c = NULL;
		return(0);
	}
	int32  pcall(C* c, int32 retType, cpps_domain* domain, node* o) {
		_CPPS_TRY
			domain->resize(o->size);
		cpps_step_all(c, retType, domain, domain, o);
		_CPPS_CATCH
			cpps_gc_check_step(c);
		return(0);
	}
	void  cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o) {
		if (domain == NULL)
			domain = c->_G;
		if (root == NULL)
			root = c->_G;
		if (o == NULL)
			o = c->o;
		domain->isbreak = false;
		/*有可能里面的让我退出 */
		size_t count = o->l.size();
		for (size_t i = 0; i < count && !domain->isbreak; i++) {
			node* d = o->l[i];
			cpps_step(c, domain, root, d);
		}
	}

	cpps_regvar* cpps_step_def_var_createvar( cpps_domain* domain, node* varName, C* c, cpps_domain* root, node_var_type isasync)
	{
		cpps_domain* leftdomain = NULL;
		cpps_regvar* v = domain->getvar(varName->s, leftdomain, false);
		if (!v) {
			v = new cpps_regvar();
			v->setvarname(varName->s);
			domain->regvar(c, v);
			/* 注册新的 */
			v->closeure = varName->closure;/*闭包*/
			v->offset = varName->offset;
			v->offsettype = varName->offsettype;
			if (varName->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
				c->_G->regidxvar(varName->offset, v);
			}
			else if (varName->offsettype == CPPS_OFFSET_TYPE_SELF) {
				root->regidxvar(varName->offset, v);
			}
			else if (varName->offsettype == CPPS_OFFSET_TYPE_LEFTDOMAIN) {
				domain->regidxvar((int32)domain->varList.size(), v);
			}
			if (isasync == node_var_type::node_var_type_constvar)
				v->setconst(true);
		}				
		return v;
	}

	void cpps_step_def_var_createvar_setval(cpps_value& value, cpps_regvar* v, C* c)
	{
		if (value.tt == CPPS_TSTRING) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)value.value.domain;
			std::string* tmpStr = (std::string*) cppsclassvar->getclsptr();
			v->setval(cpps_value(c, *tmpStr));
		}
		else {
			v->setval(value);
		}
	}

	void cpps_step_def_var(C* c, cpps_domain* domain, cpps_domain* root, node* d, node_var_type isasync) {
		/* d->s //变量名字 */
		for (size_t i = 0; i < d->l.size(); ++i) {
			node* varName = d->l[i];
			if (varName->type == CPPS_MULTIVAR) {
				node* varnames = varName->l[0];
				node* right = varName->l[1];
				std::vector< cpps_regvar*> multiresult;
				for (auto var : varnames->l) {
					cpps_regvar* v = NULL;
					v = cpps_step_def_var_createvar(domain, var, c, root, isasync);
					if (v) {
						multiresult.push_back(v);
					}
				}
				if (right && right->type == CPPS_ODEFVAR_VAR)
				{
					cpps_domain* leftdomain = NULL;
					cpps_value	value = cpps_calculate_expression(c, domain, root, right->l[0], leftdomain);
					if (value.tt == CPPS_TMULTIRETURN) {
						size_t ii = 0;
						cpps_vector* vec = cpps_to_cpps_vector(value);
						for (auto vv: vec->realvector())
						{
							if (ii < multiresult.size()) {
								cpps_regvar* takeregvar = multiresult[ii];
								cpps_step_def_var_createvar_setval(vv, takeregvar, c);
							}
							ii++;
						}
						//就用一下
						//delete value.value.multiv;
						value = nil;
					}
					else {
						//只返回一个..
						if (!multiresult.empty()) {
							cpps_regvar* firstregv = multiresult[0];
							cpps_step_def_var_createvar_setval(value, firstregv, c);
						}
					}
				}

			}
			else if (varName->type == CPPS_VARNAME) {
				node* var = varName->l[0];
				if (var && (var->type == CPPS_ODEFVAR_VAR || var->type == CPPS_ODEFVAR_NIL)) {
					/* 释放老的 */
					cpps_regvar* v = NULL;
					v = cpps_step_def_var_createvar( domain, varName, c, root, isasync);

					if (var->type == CPPS_ODEFVAR_VAR) {
						cpps_domain* leftdomain = NULL;
						cpps_value	value = cpps_calculate_expression(c, domain, root, var->l[0], leftdomain);
						if (value.tt == CPPS_TMULTIRETURN) {
							cpps_value firstvalue = cpps_to_cpps_vector(value)->realvector()[0];
							cpps_step_def_var_createvar_setval(firstvalue, v, c);
							//delete value.value.multiv;
							value = nil;
						}
						else
							cpps_step_def_var_createvar_setval(value, v, c);
					}
				}
				else if (var && (var->type == CPPS_ODEFVAR_FUNC || var->type == CPPS_ODEFVAR_LAMBDA_FUNC)) {
					/* cpps_regfunction regfunc = cpps_regfunction(varName->s, ); */
					cpps_domain* leftdomain = NULL;
					cpps_regvar* v = domain->getvar(varName->s, leftdomain, false);
					if (v) {
						cpps_cppsfunction* func = (cpps_cppsfunction*)v->getval().value.func;
						delete func;
					}
					else {
						v = new cpps_regvar();
						v->setvarname(varName->s);
						domain->regvar(c, v);
						v->setsource(true);
						v->setconst(true);
						v->offset = varName->offset;
						v->offsettype = varName->offsettype;
						if (varName->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
							c->_G->regidxvar(varName->offset, v);
						}
						else if (varName->offsettype == CPPS_OFFSET_TYPE_LEFTCLASS) {
							c->_G->regidxvar(varName->offset, v);
						}
					}
					/* domain->regFunc(&regfunc); */
					cpps_domain* funcdomain = new cpps_domain(NULL, cpps_domain_type_func, d->s + "::" + varName->s);
					cpps_cppsfunction* func = new cpps_cppsfunction(funcdomain, var->l[0], var->l[1], var->size);
					func->setfuncname(varName->s);
					func->setIsNeesC(false);
					if (isasync == node_var_type::node_var_type_asyncvar)
						func->setasync(true);
					v->setval(cpps_value(func));
				}
			}
		}
	}
	void cpps_step_throw(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		if (d->l.size() == 1) {
			cpps_domain* leftdomain = NULL;
			cpps_value	ret_value = cpps_calculate_expression(c, domain, root, d->l[0], leftdomain);
			cpps_domain* cpps_func_domain = domain;
			while (cpps_func_domain && cpps_func_domain->domainType != cpps_domain_type_trycatch) {
				cpps_func_domain->isbreak = true;
				cpps_func_domain = cpps_func_domain->parent[1];
			}
			if (cpps_func_domain && cpps_func_domain->domainType == cpps_domain_type_trycatch) {
				/* 设置回去 */
				cpps_func_domain->isbreak = true;
				cpps_func_domain->funcRet = ret_value;
			}
			else {
				throw(cpps_error(d->filename, d->line, cpps_error_trycatherror, "Illegal throw. please define in try."));
			}
		}
		else {
			/* 暂时不支持多个参数 */
		}
	}
	void cpps_step_return(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		
		cpps_domain* leftdomain = NULL;
		cpps_value	ret_value;
		if(d->l.size() == 1)
			ret_value = cpps_calculate_expression(c, domain, root, d->l[0], leftdomain);
		else if(d->l.size() > 1){
			cpps_vector* vec;
			ret_value = newclass(c, &vec);
			ret_value.tt = CPPS_TMULTIRETURN;
			//ret_value.value.multiv = new cpps_std_vector();
			for (auto nn : d->l) {
				cpps_value vv = cpps_calculate_expression(c, domain, root, nn, leftdomain);
				vec->push_back(vv);
			}
		}
		cpps_domain* cpps_func_domain = domain;
		while (cpps_func_domain && cpps_func_domain != c->_G && cpps_func_domain->parent[0]->domainType != cpps_domain_type_func) {
			cpps_func_domain->isbreak = true;
			cpps_func_domain = cpps_func_domain->parent[1];
		}
		if (cpps_func_domain && cpps_func_domain != c->_G && cpps_func_domain->parent[0]->domainType == cpps_domain_type_func) {
			/* 设置回去 */
			cpps_func_domain->isbreak = true;
			cpps_func_domain->funcRet = ret_value;
			cpps_regvar* v = new cpps_regvar();
			v->setvarname("return");
			v->setval(ret_value);
			cpps_func_domain->regvar(c, v);
		}
		else {
			/* 艹，没找到函数？？ 那怎么return */
			throw(cpps_error(d->filename, d->line, cpps_error_deffuncrror, "Illegal retrun. please define in function."));
		}
	
	}
	bool cpps_step_can_break(C* c, cpps_domain* domain) {
		if (domain == c->_G)
			return(false);
		if (domain->parent[0]->domainType == cpps_domain_type_while)
			return(true);
		if (domain->parent[0]->domainType == cpps_domain_type_for)
			return(true);
		if (domain->parent[0]->domainType == cpps_domain_type_foreach)
			return(true);
		return(false);
	}
	void cpps_step_break(C* c, cpps_domain* domain, node* d) {
		cpps_domain* cpps_func_domain = domain;
		while (cpps_func_domain && !cpps_step_can_break(c, cpps_func_domain)) {
			cpps_func_domain->isbreak = true;
			cpps_func_domain = cpps_func_domain->parent[1];
		}
		if (cpps_func_domain && cpps_step_can_break(c, cpps_func_domain)) {
			/* 设置回去 */
			cpps_func_domain->isbreak = true;
		}
		else {
			throw(cpps_error(d->filename, d->line, cpps_error_deffuncrror, "Illegal break. please define in while or for."));
		}
	}
	void cpps_step_continue(C* c, cpps_domain* domain, node* d) {
		cpps_domain* cpps_func_domain = domain;
		while (cpps_func_domain && cpps_func_domain->parent[0] && (cpps_func_domain->parent[0]->domainType != cpps_domain_type_while && cpps_func_domain->parent[0]->domainType != cpps_domain_type_foreach && cpps_func_domain->parent[0]->domainType != cpps_domain_type_for)) {
			cpps_func_domain->isbreak = true;
			cpps_func_domain = cpps_func_domain->parent[1];
		}
	}
	void cpps_step_yield(C* c, cpps_domain* domain, node* d) {
		cpps_async_loop* loop = (cpps_async_loop*)c->getmoduledata("asyncio");
		cpps::coroutine::yield(loop->ordinator);
	}
	void cpps_step_if(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain = NULL;
		cpps_value	if_value = cpps_calculate_expression(c, domain, root, d->l[0]->l[0], leftdomain);
		int32		cpp_bool_value = 1;
		cpp_bool_value = if_value.value.b;
		if (if_value.tt == CPPS_TSTRING)
			cpp_bool_value = 1;
		/* 要不要报错呢？？？ */ else if (if_value.tt == CPPS_TNIL)
			cpp_bool_value = 0; else if (if_value.tt == CPPS_TCLASSVAR)
			cpp_bool_value = 1;
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_exec);
		execdomain->setexecdomain(domain);
		if (cpp_bool_value)
			cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d->l[1]); else cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d->l[2]);
		execdomain->destory(c);
		c->domain_free(execdomain);
		cpps_gc_check_step(c);
	}
	void cpps_step_for(C* c, cpps_domain* domain, cpps_domain* root, node* d) {

		cpps_domain* fordomain = c->domain_alloc();
		fordomain->init(domain, cpps_domain_type_for);
		fordomain->setexecdomain(domain);
		node* for1 = d->l[0];
		node* for2 = d->l[1];
		node* for3 = d->l[2];
		node* for4 = d->l[3];
		fordomain->isbreak = false;
		if(!for1->l.empty()) cpps_step(c, fordomain, root, for1->l[0]);
		cpps_domain* execdomain = c->domain_alloc();
		while (true) {
			cpps_domain* leftdomain = NULL;
			cpps_value	canwhile = for2->l.size() == 0 ? true : cpps_calculate_expression(c, fordomain, root, for2->l[0], leftdomain);
			bool		b = cpps_converter<bool>::apply(canwhile);
			if (b == false)
				break;
			execdomain->init(fordomain, cpps_domain_type_exec);
			execdomain->setexecdomain(fordomain);
			if (!for4->l.empty())cpps_step(c, execdomain, root, for4->l[0]);
			bool isbreak = execdomain->isbreak;
			execdomain->destory(c);
			cpps_gc_check_step(c);
			if (isbreak)
				break;
			/* 需要跳出循环 */
			if (!for3->l.empty())
				cpps_step(c, fordomain, root, for3->l[0]);
		}
		c->domain_free(execdomain);
		fordomain->destory(c);
		c->domain_free(fordomain);
		cpps_gc_check_step(c);
	}
	void cpps_step_foreach(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* foreachdomain = c->domain_alloc();
		foreachdomain->init(domain, cpps_domain_type_foreach);
		foreachdomain->setexecdomain(domain);
		node* for1 = d->l[0];
		node* for2 = d->l[1];
		node* for4 = d->l[2];
		foreachdomain->isbreak = false;
		cpps_domain* leftdomain = NULL;
		cpps_step(c, foreachdomain, root, for1->l[0]);
		cpps_regvar* for1_v = foreachdomain->varList.empty() ? NULL : foreachdomain->varList.begin()->second;
		cpps_value v = cpps_calculate_expression(c, foreachdomain, root, for2->l[0], leftdomain);
		/* 它可能被释放,所以存在某个地方. */
		cpps_regvar* var = new cpps_regvar;
		var->setval(v);
		var->varName = buildlambda() + "foreach-list";
		foreachdomain->regvar(c, var);
		if (v.tt == CPPS_TNIL)
			return;
		if (v.isdomain() && v.value.domain->domainname == "vector") {
			cpps_vector* vec = cpps_converter<cpps_vector*>::apply(v);
			if (vec) {
				cpps_domain* execdomain = c->domain_alloc();
				cpps_std_vector& realvector = vec->realvector();
				for (size_t i = 0; i < realvector.size(); i++) {
					if (for1_v)
						for1_v->setval(realvector[i]);
					execdomain->init(foreachdomain, cpps_domain_type_exec);
					execdomain->setexecdomain(foreachdomain);
					cpps_step(c, execdomain, root, for4->l[0]);
					bool isbreak = execdomain->isbreak;
					execdomain->destory(c);
					cpps_gc_check_step(c);
					if (isbreak)
						break;
					/* 需要跳出循环 */
				}
				c->domain_free(execdomain);
			}
		}
		else if (v.isdomain() && v.value.domain->domainname == "map") {
			cpps_map_node* mapnode;
			cpps_value	ret = newclass<cpps_map_node>(c, &mapnode);
			if (for1_v)
				for1_v->setval(ret);
			cpps_map* vmap = cpps_converter<cpps_map*>::apply(v);
			if (vmap) {
				cpps_hash_map& realmap = vmap->realmap();
				cpps_hash_map::iterator	it = realmap.begin();
				cpps_hash_map::iterator	itend = realmap.end();
				cpps_domain* execdomain = c->domain_alloc();
				for (; it != itend; ++it) {
					mapnode->_first = it->first;
					mapnode->_second = it->second;
					execdomain->init(foreachdomain, cpps_domain_type_exec);
					execdomain->setexecdomain(foreachdomain);
					cpps_step(c, execdomain, root, for4->l[0]);
					bool isbreak = execdomain->isbreak;
					execdomain->destory(c);
					cpps_gc_check_step(c);
					if (isbreak)
						break;
					/* 需要跳出循环 */
				}
				c->domain_free(execdomain);
			}
		}
		else if (v.isdomain() && v.value.domain->domainname == "RANGE") {
			cpps_range* range = cpps_converter<cpps_range*>::apply(v);
			if (range) {
				if (for1_v)
					for1_v->getval().tt = CPPS_TINTEGER;
				cpps_domain* execdomain = c->domain_alloc();
				for (cpps_integer begin = range->begin; begin <= range->end; begin += range->inc) {
					if (for1_v)
						for1_v->getval().value.integer = begin;
					execdomain->init(foreachdomain, cpps_domain_type_exec);
					execdomain->setexecdomain(foreachdomain);
					cpps_step(c, execdomain, root, for4->l[0]);
					bool isbreak = execdomain->isbreak;
					execdomain->destory(c);
					cpps_gc_check_step(c);
					if (isbreak)
						break;
					/* 需要跳出循环 */
				}
				c->domain_free(execdomain);
			}
		}
		else {
			throw(cpps_error(d->filename, d->line, cpps_error_forerror, "Foreach only supports map, unordered_ Map, vector ,RANGE. Other formats are not supported at the moment."));
		}
		foreachdomain->destory(c);
		c->domain_free(foreachdomain);
		cpps_gc_check_step(c);
	}
	void cpps_step_while(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* whiledomain = c->domain_alloc();
		whiledomain->init(domain, cpps_domain_type_while);
		whiledomain->setexecdomain(domain);
		whiledomain->isbreak = false;
		node* while1 = d->l[0];
		node* while2 = d->l[1];
		cpps_domain* execdomain = c->domain_alloc();
		while (true) {
			cpps_domain* leftdomain = NULL;
			cpps_value	canwhile = cpps_calculate_expression(c, whiledomain, root, while1->l[0], leftdomain);
			bool		b = cpps_converter<bool>::apply(canwhile);
			if (b == false)
				break;
			execdomain->init(whiledomain, cpps_domain_type_exec);
			execdomain->setexecdomain(whiledomain);
			cpps_step_all(c, CPPS_MUNITRET, execdomain, root, while2);
			bool isbreak = execdomain->isbreak;
			execdomain->destory(c);
			cpps_gc_check_step(c);
			if (isbreak)
				break;
			/* 需要跳出循环 */
		}
		c->domain_free(execdomain);
		whiledomain->destory(c);
		c->domain_free(whiledomain);
		cpps_gc_check_step(c);
	}
	void cpps_step_namespace(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain;
		cpps_regvar* v = domain->getvar(d->s, leftdomain, false);
		cpps_domain* ns = NULL;

		if (v && (v->getval().isdomain() && v->getval().value.domain->domainType == cpps_domain_type_namespace))
		{
			ns = v->getval().value.domain;
			cpps_step_all(c, CPPS_MUNITRET, ns, root, d);
			return;
		}

		if(v)
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "%s is defined but this is not namespace.", d->s.c_str()));

		ns = new cpps_domain(domain, cpps_domain_type_namespace, d->s);
		v = new cpps_regvar();
		v->setvarname(d->s);
		v->setval(cpps_value(ns));
		domain->regvar(c, v);
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			c->_G->regidxvar(d->offset, v);
		}
		cpps_step_all(c, CPPS_MUNITRET, ns, root, d);
	}
	void cpps_step_module(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain;
		cpps_domain* _Gdomain = c->_G;
		cpps_regvar* v = c->_G->getvar(d->s, leftdomain, false);
		cpps_domain* ns = NULL;

		if (v && (v->getval().isdomain() && v->getval().value.domain->domainType == cpps_domain_type_module))
		{
			ns = v->getval().value.domain;

			if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
				c->_G->regidxvar(d->offset, v);
			}

			cpps_step_all(c, CPPS_MUNITRET, ns, root, d);
			return;
		}
		if (v) 
			throw(cpps_error(d->filename, d->line, cpps_error_moduleerror, "%s is defined but this is not module.", d->s.c_str()));

		ns = new cpps_domain(_Gdomain, cpps_domain_type_module, d->s);
		v = new cpps_regvar();
		v->setvarname(d->s);
		v->setval(cpps_value(ns));
		_Gdomain->regvar(c, v);
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			c->_G->regidxvar(d->offset, v);
		}
		cpps_step_all(c, CPPS_MUNITRET, ns, root, d);
	}
	void cpps_pop_stack_to_here(C* c, cpps_stack* here,bool cleanup) {
		while (!c->getcallstack()->empty()) {
			cpps_stack* takestack2 = c->getcallstack()->back();
			if (here != takestack2) {
				if(cleanup) delete  takestack2;
				c->getcallstack()->pop_back();
			}
			else break;
		}
	}
	void cpps_do_catch(C* c, cpps_domain* parent_domain, cpps_domain* root_domain, cpps_domain* root, node* catchfun, cpps_trycatch_error& e) {
		cpps_domain* execdomain2 = c->domain_alloc();
		execdomain2->init(parent_domain, cpps_domain_type_exec);
		execdomain2->setexecdomain(root_domain);
		node* varName = catchfun->l[0];
		cpps_regvar* v = new cpps_regvar();
		v->setvarname(varName->s);
		v->setval(cpps_cpp_to_cpps_converter<cpps_trycatch_error*>::apply(c, &e));
		execdomain2->regvar(c, v);
		cpps_step_all(c, CPPS_MUNITRET, execdomain2, root, catchfun->l[1]);
		execdomain2->destory(c);
		c->domain_free(execdomain2);
		cpps_gc_check_step(c);
	}

	void printcallstack(std::string& errmsg, C* c)
	{
		char errbuffer[1024];
		sprintf(errbuffer, "Error stack information:\n");
		errmsg.append(errbuffer);
		std::vector<cpps_stack*>* stacklist = c->getcallstack();
		for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)
		{
			cpps::cpps_stack* stack = *it;
			sprintf(errbuffer, "file:%s [%d] %s\n", stack->f, stack->l, stack->func);
			errmsg.append(errbuffer);
		}
	}

	void cpps_step_trycath(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		node* func = d->l[0];
		node* catchfun = d->l[1];
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_trycatch);
		execdomain->setexecdomain(domain);
		cpps_stack* takestack = c->getcallstack()->empty() ? NULL : c->getcallstack()->back();
		bool			hasCatch = false;
		cpps_trycatch_error	throwerr;
		try {
			cpps_step_all(c, CPPS_MUNITRET, execdomain, root, func);
			if (execdomain->funcRet.tt != CPPS_TNIL) {
				throwerr = cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "The exception thrown by throw.");
				throwerr.value = execdomain->funcRet;
				hasCatch = true;
			}
		}
		catch (cpps_trycatch_error e) {
			throwerr = e;

			std::string errmsg;
			printcallstack(errmsg, c);
			throwerr.callstackstr += errmsg;

			cpps_pop_stack_to_here(c, takestack);
			hasCatch = true;

		}
		catch (cpps_error e) {

			std::string errmsg;
			printcallstack(errmsg, c);


			cpps_pop_stack_to_here(c, takestack);
			/* 清栈 */
			throwerr = cpps_trycatch_error(e);
			throwerr.callstackstr = errmsg;
			hasCatch = true;
		}
		catch (const char* s) {

			std::string errmsg;
			printcallstack(errmsg, c);

			cpps_pop_stack_to_here(c, takestack);
			/* 清栈 */
			throwerr = cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "Unknown exception thrown by throw.");
			throwerr.value = cpps_value(c,s);
			throwerr.callstackstr = errmsg;
			hasCatch = true;
		}
		if (hasCatch)
			cpps_do_catch(c, domain, domain, root, catchfun, throwerr);
		execdomain->destory(c);
		c->domain_free(execdomain);
	}
	void cpps_step_class_reg_baseclass_idx_offset(cpps_cppsclass* cppsclass, cpps_cppsclass* parentclass, int32 takeoff) {
		/*复制父类所有的函数哟. */
		for (phmap::flat_hash_map<std::string, cpps_regvar*>::iterator it2 = parentclass->varList.begin(); it2 != parentclass->varList.end(); ++it2) {
			if (it2->first != "constructor")
				/*不复制构造函数.. */ {
				cppsclass->varList.erase(it2->first);
				cppsclass->hasVar = true;
				cppsclass->varList.insert(phmap::flat_hash_map<std::string, cpps_regvar*>::value_type(it2->first, it2->second));
			}
		}
		cppsclass->setidxoffset(parentclass, takeoff);
		for (auto grandfather : parentclass->parentclasslist()) {
			cpps_step_class_reg_baseclass_idx_offset(cppsclass, grandfather, takeoff);
			takeoff += grandfather->o->varsize;
		}
	}
	void cpps_step_class(C* c, cpps_domain* domain, node* d) {
		cpps_cppsclass* cppsclass = new cpps_cppsclass(d->s, d, domain, cpps_domain_type_class);
		cpps_regvar* v = new cpps_regvar();
		v->setvarname(d->s);
		v->setconst(true);
		v->setval(cpps_value(cppsclass));
		v->setsource(true);
		domain->regvar(NULL, v);
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			c->_G->regidxvar(d->offset, v);
		}
		node* parent = cppsclass->o->getleft();
		node* vars = cppsclass->o->getright();
		int32 off = 0;
		for (std::vector<node*>::iterator it = parent->l.begin(); it != parent->l.end(); ++it) {
			node* o = *it;
			cpps_domain* leftdomain = NULL;
			cpps_regvar* regvar = domain->getvar(o->s, leftdomain);
			node* lastNamespace = o;
			while (lastNamespace && lastNamespace->getleft() && lastNamespace->getleft()->type == CPPS_ONAMESPANCE_CHILD) {
				regvar = regvar->getval().value.domain->getvar(lastNamespace->getleft()->s, leftdomain, false);
				if (!regvar) {
					throw(cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "The parent class [%s] not found.", lastNamespace->getleft()->s.c_str()));
				}
				lastNamespace = lastNamespace->getleft();
			}
			if (!regvar || regvar->getval().tt != CPPS_TCLASS) {
				throw(cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "The parent class must be a class. Cannot be another type."));
			}
			cpps_cppsclass* parentclass = (cpps_cppsclass*)regvar->getval().value.domain;
			cppsclass->parentclasslist().push_back(parentclass);
			/* 添加父类 */
			cpps_step_class_reg_baseclass_idx_offset(cppsclass, parentclass, off);
			off += cppsclass->o->varsize;
		}
		/* 只注册函数到类里面  变量注册到 变量中 */
		for (std::vector<node*>::iterator it = vars->l.begin(); it != vars->l.end(); ) {
			node* o = *it;
			if (o->type == CPPS_ODEFVAR || o->type == CPPS_ODEFCONSTVAR || o->type == CPPS_ODEFASYNCVAR) {
				node* varName = o->l[0];
				if (varName->type == CPPS_VARNAME) {
					node* var = varName->l[0];
					if (var && var->type == CPPS_ODEFVAR_FUNC) {
						cpps_regvar* v2 = new cpps_regvar();
						v2->setvarname(varName->s);
						cppsclass->varList.erase(varName->s);
						/* 重写父类的函数 */
						cppsclass->regvar(c, v2);
						v2->setconst(true);
						v2->setsource(true);
						v2->offset = varName->offset;
						v2->offsettype = varName->offsettype;
						if (varName->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
							c->_G->regidxvar(varName->offset, v2);
						}
						else if (varName->offsettype == CPPS_OFFSET_TYPE_LEFTCLASS) {
							c->_G->regidxvar(varName->offset, v2);
						}
						cpps_domain* funcdomain = new cpps_domain(cppsclass, cpps_domain_type_func, d->s + "::" + varName->s);
						cpps_cppsfunction* func = new cpps_cppsfunction(funcdomain, var->l[0], var->l[1], var->size);
						func->setIsNeesC(false);
						if (o->type == CPPS_ODEFASYNCVAR) {
							func->setasync(true);
						}
						v2->setval(cpps_value(func));
						it = vars->l.erase(it);
					}
					else {
						++it;
					}
				}
			}
			else {
				it = vars->l.erase(it);
			}
		}
	}
	void cpps_step_import(C* c, cpps_domain* domain, cpps_domain* root, node* o) {
		cpps_domain* leftdomain = NULL;
		if (o->l.empty())
			return;
		cpps_value		list = cpps_calculate_expression(c, domain, root, o->l[0], leftdomain);
		cpps_cppsclass* cppsclass = (cpps_cppsclass*)list.value.domain->parent[0];
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)list.value.domain;
		if (list.tt == CPPS_TSTRING) {
			cpps_loadlibrary(c, cpps_to_string(list));
		}
		else if (cppsclass->getclassname() == "vector") {
			cpps_vector* vec = (cpps_vector*)cppsclassvar->getclsptr();
			for (auto& v : vec->realvector()) {
				if (v.tt == CPPS_TSTRING) {
					cpps_loadlibrary(c, cpps_to_string(v));
				}
			}
		}
	}
	void cpps_step_dofile(C* c, cpps_domain* domain, cpps_domain* root, node* o) {
		cpps_stack* takestack = c->getcallstack()->empty() ? NULL : c->getcallstack()->back();
		cpps_try
			for (std::vector<node*>::iterator it = o->l.begin(); it != o->l.end(); ++it) {
				cpps_domain* leftdomain = NULL;
				cpps_value	path = cpps_calculate_expression(c, domain, root, *it, leftdomain);
				std::string	fpath = cpps_to_string(path);
				std::string fileSrc;
				cpps_load_filebuffer(fpath.c_str(), fileSrc);
				node* o = loadbuffer(c, fileSrc, fpath);
				cpps_stack* stack = c->stack_alloc();
				stack->init((*it)->filename.c_str(), (*it)->line, "dofile");
				c->push_stack(stack);
				cpps_step_all(c, CPPS_MUNITRET, c->_G, c->_G, o);
				c->pop_stack();
				cpps_gc_check_step(c);
				c->stack_free(stack);
				cpps_destory_node(o);
				delete o;
				o = NULL;
			}
		cpps_catch
		cpps_pop_stack_to_here(c, takestack);
	}
	void cpps_step_dostring(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_stack* takestack = c->getcallstack()->empty() ? NULL : c->getcallstack()->back();
		cpps_try
			cpps_domain* leftdomain = NULL;
			cpps_value	path = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
			std::string* str = cpps_get_string(path);
			if (!str)
				return;
			node* o = loadbuffer(c, *str, "");
			cpps_stack* stack = c->stack_alloc();
			stack->init(d->filename.c_str(), d->line, "dostring");
			c->push_stack(stack);
			cpps_step_all(c, CPPS_MUNITRET, domain, root, o);
			c->pop_stack();
			c->stack_free(stack);
			cpps_gc_check_step(c);

			cpps_destory_node(o);
			delete o;
			o = NULL;
		cpps_catch
		cpps_pop_stack_to_here(c, takestack);
	}
	void cpps_step_newclassvar_reg_baselassvar(cpps_cppsclass* cppsclass, C* c, cpps_cppsclassvar* cppsclassvar, cpps_domain* root) {
		for (auto parentclass : cppsclass->parentclasslist()) {
			cpps_step_newclassvar_reg_baselassvar(parentclass, c, cppsclassvar, root);
			if (parentclass->o)
				cpps_step_all(c, CPPS_MUNITRET, cppsclassvar, root, parentclass->o->getright());
		}
	}
	void cpps_calculate_expression_newvar(cpps_domain* domain, node* d, C* c, cpps_domain* root, cpps_domain*& leftdomain, cpps_value& ret) {
		
		cpps_regvar* v = domain->getvar(d->s, leftdomain);
		if (!v)
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "[%s] not found or not defined", d->s.c_str()));
		/*看看有没有使用名空间 */
		node* lastNamespace = d;
		while (lastNamespace && lastNamespace->getleft() && lastNamespace->getleft()->type == CPPS_ONAMESPANCE_CHILD) {
			v = v->getval().value.domain->getvar(lastNamespace->getleft()->s, leftdomain);
			if (!v)
				throw(cpps_error(lastNamespace->getleft()->filename, lastNamespace->getleft()->line, cpps_error_normalerror, "[%s] not found or not defined", lastNamespace->getleft()->s.c_str()));
			lastNamespace = lastNamespace->getleft();
		}
		if (v && v->getval().tt == CPPS_TCLASS) {
			cpps_cppsclass* cppsclass = (cpps_cppsclass*)v->getval().value.domain;
			cpps_cppsclassvar* cppsclassvar = cppsclass->create(c);
			/* 将类对象里面的变量创建出来 */
			cpps_step_newclassvar_reg_baselassvar(cppsclass, c, cppsclassvar, root);
			if (cppsclass->o)
				cpps_step_all(c, CPPS_MUNITRET, cppsclassvar, root, cppsclass->o->getright());
			/* 将新创建出来的添加到新生区稍后检测要不要干掉 */
			cpps_gc_add_gen0(c, cppsclassvar);
			ret = cpps_value(cppsclassvar);
			cpps_domain* takedomain2 = leftdomain;
			leftdomain = cppsclassvar;

			

			/* 数组特殊处理. */
			if (d->s == "vector" && d->getleft()) {
				cpps_vector* array = static_cast<cpps_vector*>(cppsclassvar->getclsptr());
				cpps_integer	result;
				cpps_str2i64(d->getleft()->s.c_str(), &result);
				array->resize(result);
			}
			else if (d->getright())
				/* 构造函数 */ {
				if (d->l.size() == 3) {
					node* n = d->l[2];
					if (n && n->type == CPPS_ONEW_SETV) {
						node* rr = n;
						cpps_domain* execdomain = c->domain_alloc();
						for (size_t i = 0; i < rr->l.size(); i++) {
							node* k = rr->l[i]->getleft();

							//execdomain->init(cppsclassvar, cpps_domain_type_func);
						//	execdomain->setexecdomain(cppsclassvar);
							cpps_domain* takedomain3 = NULL;
							cpps_value v = cpps_calculate_expression(c, domain, root, rr->l[i]->getright()->getleft(), takedomain3);
							/* m->insert(cpps_value(c, k->s), v); */
							cpps_regvar* var = cppsclassvar->getvar(k->s, leftdomain);
							if (var)
								var->setval(v);
							//execdomain->destory(c);

						}

						c->domain_free(execdomain);
					}
				}
				node* n = d->getright();
				if (n && n->type == CPPS_OCLASS_CONSTRUCTOR) {
					/* 执行父类的默认构造函数（有参数就不执行） */
					for (size_t i = 0; i < cppsclass->parentclasslist().size(); ++i) {
						cpps_cppsclass* parent_cppsclass = cppsclass->parentclasslist()[i];
						cpps_call_parent_class_default_constructor(c, n, parent_cppsclass, domain, root, leftdomain);
					}
					cpps_regvar* var = cppsclassvar->getvar("constructor", leftdomain);
					if (var && var->getval().tt == CPPS_TFUNCTION) {
						/*cpps_function* f = var->getValue().value.func;*/
						cpps_domain* execdomain = c->domain_alloc();
						execdomain->init(domain, cpps_domain_type_func);
						execdomain->setexecdomain(domain);
						cpps_step_callfunction(c, execdomain, root, var->getval(), d, leftdomain);
						execdomain->destory(c);
						c->domain_free(execdomain);
					}
				}
			}
			leftdomain = takedomain2;
		}
		else {
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "The object out of new [%s] must be a class object", d->s.c_str()));
		}
	}

	void cpps_step_assemble(C* c, cpps_domain* domain, cpps_domain* root, node* d)
	{
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_exec);
		execdomain->setexecdomain(domain);
		cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d);
		execdomain->destory(c);
		c->domain_free(execdomain);
		cpps_gc_check_step(c);
	}

	void cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		/*记录当前执行的node*/
		c->curnode = d;

		if (d->type == CPPS_ODEFVAR) {
			cpps_step_def_var(c, domain, root, d , node_var_type::node_var_type_var);
		}
		else if (d->type == CPPS_ODEFCONSTVAR) {
			cpps_step_def_var(c, domain, root, d, node_var_type::node_var_type_constvar);
		}
		else if (d->type == CPPS_ODEFASYNCVAR) {
			cpps_step_def_var(c, domain, root, d, node_var_type::node_var_type_asyncvar);
		}
		else if (d->type == CPPS_OASSEMBLE) {
			cpps_step_assemble(c, domain, root, d);
		}
		else if (d->type == CPPS_OEXPRESSION) {
			cpps_domain* leftdomain = NULL;	cpps_calculate_expression(c, domain, root, d->l[0], leftdomain);
		}
		else if (d->type == CPPS_ORETURN) {
			cpps_step_return(c, domain, root, d);
		}
		else if (d->type == CPPS_OTHROW) {
			cpps_step_throw(c, domain, root, d);
		}
		else if (d->type == CPPS_OBREAK) {
			cpps_step_break(c, domain, d);
		}
		else if (d->type == CPPS_OCONTINUE) {
			cpps_step_continue(c, domain, d);
		}
		else if (d->type == CPPS_OYIELD) {
			cpps_step_yield(c, domain, d);
		}
		else if (d->type == CPPS_OIF) {
			cpps_step_if(c, domain, root, d);
		}
		else if (d->type == CPPS_OFOR) {
			cpps_step_for(c, domain, root, d);
		}
		else if (d->type == CPPS_OFOREACH) {
			cpps_step_foreach(c, domain, root, d);
		}
		else if (d->type == CPPS_OWHILE) {
			cpps_step_while(c, domain, root, d);
		}
		else if (d->type == CPPS_OCLASS) {
			cpps_step_class(c, domain, d);
		}
		else if (d->type == CPPS_ONAMESPACE) {
			cpps_step_namespace(c, domain, root, d);
		}
		else if (d->type == CPPS_OMODULE) {
			cpps_step_module(c, domain, root, d);
		}
		else if (d->type == CPPS_OTRYCATCH) {
			cpps_step_trycath(c, domain, root, d);
		}
		else if (d->type == CPPS_ODOFILE) {
			cpps_step_dofile(c, domain, root, d);
		}
		else if (d->type == CPPS_OINCLUDE) {
			//do nothing...
		}
		else if (d->type == CPPS_OIMPORT) {
			cpps_step_import(c, domain, root, d);
		}
		else if (d->type == CPPS_ODOSTRING) {
			cpps_step_dostring(c, domain, root, d);
		}
	}
	int32 cpps_str2d(const char* s, cpps_number* result) {
		char* endptr;
		*result = strtod(s, &endptr);
		if (endptr == s)
			return(0);
		/* conversion failed */
		if (*endptr == 'x' || *endptr == 'X')
			/* maybe an hexadecimal constant? */
			*result = cpps_number(strtoul(s, &endptr, 16));
		if (*endptr == '\0')
			return(1);
		/* most common case */
		while (cpps_parse_isspace(usint8(*endptr)))
			endptr++;
		if (*endptr != '\0')
			return(0);
		/* invalid trailing characters? */
		return(1);
	}
	int32 cpps_str2i64(const char* s, cpps_integer* result) {
		cpps_number n = 0;
		cpps_str2d(s, &n);
		*result = cpps_number2integer(n);
		return(1);
	}
	cpps_regvar* getregvar(cpps_domain* domain, node* o) {
		cpps_domain* leftdomain = NULL;
		cpps_regvar* v = domain->getvar(o->s, leftdomain);
		if (v) {
			if ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR) && !o->l.empty()) {
				return(getregvar(v->getval().value.domain, o->l[0]));
			}
		}
		return(v);
	}
	void make_values(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_std_vector& params) {
		char	index = 65;
		size_t	size = d->l.size();
		params.reserve(size);
		/* for (std::vector<Node*>::iterator it = d->l.begin(); it != d->l.end(); ++it) */
		for (size_t i = 0; i < size; i++) {
			cpps_domain* leftdomain = NULL;
			cpps_value	value = cpps_calculate_expression(c, domain, root, d->l[i], leftdomain);
			params.push_back(value);
			index++;
		}
	}
	cpps_regvar* cpps_node_to_regver(cpps_domain* domain, node* d, bool isgetRight = true) {
		if (d == NULL)
			return(NULL);
		/*d is null when it is a constructor*/
		cpps_regvar* v = NULL;
		if (d->type == CPPS_QUOTEGETCHIILD) {
			v = cpps_node_to_regver(domain, d->getleft());
			if (v && ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR)) && isgetRight) {
				if (v->getval().tt == CPPS_TCLASSVAR && v->getval().value.domain->getdomainname() != "map" && v->getval().value.domain->getdomainname() != "vector") {
					v = getregvar(v->getval().value.domain, d->getright());
				}
			}
		}
		else if (d->type == CPPS_QUOTEGETOBJECT) {
			v = cpps_node_to_regver(domain, d->getleft());
			if (v && ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR)) && isgetRight) {
				if (v->getval().tt == CPPS_TCLASSVAR && v->getval().value.domain->getdomainname() != "map" && v->getval().value.domain->getdomainname() != "vector") {
					v = getregvar(v->getval().value.domain, d->getright());
				}
			}
		}
		else if (d->type == CPPS_OGETOBJECT) {
			v = cpps_node_to_regver(domain, d->getleft());
			if (v && ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR)) && isgetRight) {
				if ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR) && v->getval().value.domain->getdomainname() != "map" && v->getval().value.domain->getdomainname() != "vector") {
					v = getregvar(v->getval().value.domain, d->getright());
				}
			}
		}
		else if (d->type == CPPS_OGETCHIILD) {
			v = cpps_node_to_regver(domain, d->getleft());
			if (v && ((v->getval().tt == CPPS_TDOMAIN || v->getval().tt == CPPS_TCLASSVAR)) && isgetRight) {
				if (v->getval().tt == CPPS_TCLASSVAR && v->getval().value.domain->getdomainname() != "map" && v->getval().value.domain->getdomainname() != "vector") {
					v = getregvar(v->getval().value.domain, d->getright());
				}
			}
		}
		else {
			cpps_domain* leftdomain = NULL;
			v = domain->getvar(d->s, leftdomain);
		}
		return(v);
	}
	bool cpps_check_parent_class(cpps_cppsclass* a, cpps_cppsclass* b) {
		if (a == NULL)
			return(false);
		if (b == NULL)
			return(false);
		for (size_t i = 0; i < a->parentclasslist().size(); i++) {
			cpps_cppsclass* c = a->parentclasslist()[i];
			if (c == b)
				return(true);
		}
		for (size_t i = 0; i < a->parentclasslist().size(); i++) {
			cpps_cppsclass* c = a->parentclasslist()[i];
			bool		ret = cpps_check_parent_class(c, b);
			if (ret)
				return(true);
		}
		return(false);
	}
	void cpps_call_parent_class_default_constructor(C* c, node* n, cpps_cppsclass* parent_cppsclass, cpps_domain* domain, cpps_domain* root, cpps_domain* leftdomain) {
		cpps_domain* tmpdomain = NULL;
		cpps_regvar* var = parent_cppsclass->getvar("constructor", tmpdomain, false);
		if (var && var->getval().tt == CPPS_TFUNCTION) {
			/* 在执行父类的父类的构造函数 */
			for (size_t i = 0; i < parent_cppsclass->parentclasslist().size(); i++) {
				cpps_cppsclass* tmp_parent_cppsclass = parent_cppsclass->parentclasslist()[i];
				cpps_call_parent_class_default_constructor(c, n, tmp_parent_cppsclass, domain, root, leftdomain);
			}
			cpps_function* f = var->getval().value.func;
			if (f->getparamcount() == 0) {
				cpps_domain* execdomain = c->domain_alloc();
				execdomain->init(domain, cpps_domain_type_func, "constructor");
				execdomain->setexecdomain(domain);
				node	tmp(n->filename, n->line);
				node	params(n->filename, n->line);
				tmp.addtoright(&params);
				/*  */
				cpps_step_callfunction(c, execdomain, root, var->getval(), &tmp, leftdomain);
				execdomain->destory(c);
				c->domain_free(execdomain);
			}
		}
	}
	cpps_domain* cpps_calculate_expression_offset_getclassvar(cpps_domain* root)
	{
		if (root == NULL) return NULL;

		if (root->parent[1] && root->parent[1]->domainType == cpps_domain_type_classvar) return root;

		return cpps_calculate_expression_offset_getclassvar(root->parent[1]);
	}
	void cpps_calculate_expression_offset(node* d, C* c, cpps_value& ret, cpps_domain* root, cpps_domain*& leftdomain) {
		/* 快速读取 */
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {
				ret = var->getval();
#ifndef NDEBUG
				//printf(" //global[%d]", d->offset);
#endif
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_SELF) {
			cpps_regvar* var = root->getregidxvar(d->offset);
			if (var) {
				ret = var->getval();
#ifndef NDEBUG
				//printf(" //self[%d]", d->offset);
#endif
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_LEFTCLASS) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {
				ret = var->getval();
#ifndef NDEBUG
				//printf(" //left-class[%d]", d->offset);
#endif
				cpps_domain* _classvarroot = cpps_calculate_expression_offset_getclassvar(root);
				assert(_classvarroot);
				leftdomain = _classvarroot;
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_LEFTDOMAIN) {
			cpps_domain* _classvarroot = cpps_calculate_expression_offset_getclassvar(root);
			assert(_classvarroot);

			cpps_regvar* var = _classvarroot->parent[1]->getregidxvar(_classvarroot->parent[1]->parent[0]->getidxoffset(_classvarroot->parent[0]) + d->offset);
			if (var) {
				ret = var->getval();
#ifndef NDEBUG
				//printf(" //left[%d]", d->offset);
#endif
			}
		}
	}
	cpps_domain*& cpps_calculate_expression_quoteoffset(node* d, C* c, cpps_value& ret, cpps_domain* root, cpps_domain*& leftdomain) {
		/* 快速读取 */
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {
				if (!var->isconst()) {
					ret.tt = CPPS_TREGVAR;
					ret.value.value = &var->getval();
				}
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_SELF) {
			cpps_regvar* var = root->getregidxvar(d->offset);
			if (var) {
				if (!var->isconst()) {
					ret.tt = CPPS_TREGVAR;
					ret.value.value = &var->getval();
				}
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_LEFTCLASS) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {
				if (!var->isconst()) {
					ret.tt = CPPS_TREGVAR;
					ret.value.value = &var->getval();
					cpps_domain* _classvarroot = cpps_calculate_expression_offset_getclassvar(root);
					assert(_classvarroot);
					leftdomain = _classvarroot;
				}
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_LEFTDOMAIN) {
			cpps_domain* _classvarroot = cpps_calculate_expression_offset_getclassvar(root);
			assert(_classvarroot);

			cpps_regvar* var = _classvarroot->parent[1]->getregidxvar(_classvarroot->parent[1]->parent[0]->getidxoffset(_classvarroot->parent[0]) + d->offset);
			if (var) {
				if (!var->isconst()) {
					ret.tt = CPPS_TREGVAR;
					ret.value.value = &var->getval();
				}
			}
		}
		return(leftdomain);
	}
	void cpps_calculate_expression_dofunction(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value var = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_func);
		execdomain->setexecdomain(domain);
		/* cpps_step_for(c, execdomain, d); */
		ret = cpps_step_callfunction(c, execdomain, root, var, d, leftdomain);
		execdomain->destory(c);
		c->domain_free(execdomain);
	}
	void cpps_calculate_expression_ostr(cpps_value& ret, node* d, cpps_domain*& leftdomain, C* c, cpps_domain* domain, cpps_domain* root) {
		std::string* str = (std::string*) d->value.str->getclsptr();
		str->clear();
		for (size_t i = 0; i < d->l.size(); i++) {
			node* s = d->l[i];
			if (s->type == CPPS_OSTR) {
				str->append(s->s);
			}
			else {
				cpps_domain* takedomian = leftdomain;
				leftdomain = NULL;
				cpps_value v = cpps_calculate_expression(c, domain, root, s->l[0], leftdomain);
				leftdomain = takedomian;
				str->append(v.tt == CPPS_TSTRING ? *cpps_get_string(v) : cpps_to_string(v));
			}
		}
		std::string* retstr = NULL;
		ret = newclass<std::string>(c, &retstr);
		retstr->append(*str);

	}
	void cpps_calculate_expression_array(cpps_value& ret, C* c, node* d, cpps_domain*& leftdomain, cpps_domain* domain, cpps_domain* root) {
		cpps_vector* vec = NULL;
		ret = newclass<cpps_vector>(c, &vec);
	
		for (size_t i = 0; i < d->l.size(); i++) {
			cpps_domain* takedomain = leftdomain;
			leftdomain = NULL;
			cpps_value v = cpps_calculate_expression(c, domain, root, d->l[i]->getleft(), leftdomain);
			vec->push_back(v);
			leftdomain = takedomain;
		}
	}
	void cpps_calculate_expression_object(cpps_value& ret, C* c, node* d, cpps_domain*& leftdomain, cpps_domain* domain, cpps_domain* root) {
		cpps_map* m = NULL;
		ret = newclass<cpps_map>(c, &m);
		
		for (size_t i = 0; i < d->l.size(); i++) {
			node* k = d->l[i]->getleft();
			cpps_domain* takedomain = leftdomain;
			leftdomain = NULL;
			cpps_value v = cpps_calculate_expression(c, domain, root, d->l[i]->getright()->getleft(), leftdomain);
			m->insert(cpps_value(c, k->s), v);
			leftdomain = takedomain;
		}
	}
	void cpps_calculate_expression_varname(C*c,cpps_domain*& leftdomain, cpps_domain* domain, node* d, cpps_value& ret) {

		cpps_regvar* v = NULL;
		if (leftdomain)
			v = leftdomain->getvar(d->s, leftdomain, true, true);
		else
			v = domain->getvar(d->s, leftdomain);

		if (v) {
			ret = v->getval();
		}
		else {
			if(!c->disabled_non_def_var)
				printf("Warning:  got a not existent variable of [%s].  line: %d file: %s\n", d->s.c_str(), d->line, d->filename.c_str());
		}
	}
	void cpps_calculate_expression_lambda(C*c,cpps_domain* domain,cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_regvar* v = NULL;
		if (d->offset != -1) {
			v = c->_G->getregidxvar(d->offset);
		}
		else {
			v = c->_G->getvar(d->s, leftdomain);
		}
		if (v) {
			/* printf(domain->domainName.c_str()); */
			cpps_lambda_function* func;
			ret = newclass< cpps_lambda_function >(c, &func);
			func->c = c;
			func->setrealfunc((cpps_cppsfunction*)v->getval().value.func);
			if (root->parent[0] && root->parent[0]->domainType == cpps_domain_type_func && root->stacklist) {
				size_t c = d->size;
				if (c > 0) {
					func->stacklist = new std::vector<cpps_regvar*>();

					//check
					assert(d->size <= root->stacklist->size());
					for (size_t i = 0; i < c; i++) {
						auto no = (*(root->stacklist))[i];
						if (no && no->closeure) {
							func->stacklist->push_back(no);
							no->closeureusecount++;//计数
						}
						else {
							func->stacklist->push_back(NULL);//占位
						}
					}
				}
			}
			ret.tt = CPPS_TLAMBDAFUNCTION;
		}
	}
	void cpps_calculate_expression_getchild(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value left = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0];
				if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "vector") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_vector* pVec = (cpps_vector*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right = cpps_calculate_expression(c,  domain, root, d->getright()->getleft(), leftdomain);
					if (right.tt != CPPS_TINTEGER) {
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array must contain a number as an index."));
					}
					leftdomain = takedomain;
					if (pVec->size() <= right.value.integer)
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array has crossed the current length: [%d]. You need to get the length: [%d]..", pVec->size(), right.value.integer));
					ret = pVec->at(right.value.integer);
				}
				else if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "map") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_map* pMap = (cpps_map*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right = cpps_calculate_expression(c, domain, root, d->getright()->getleft(), leftdomain);
					leftdomain = takedomain;
					ret = pMap->find(right);
				}

				else {
					/*cpps_domain* execdomain = c->domain_alloc();
					execdomain->init(left.value.domain, cpps_domain_type_exec);
					leftdomain = left.value.domain;
					ret = cpps_calculate_expression(c, execdomain, root, d->getright()->getleft(), leftdomain);
					execdomain->destory(c);
					c->domain_free(execdomain);*/
				}
			}
			else if (left.tt == CPPS_TSTRING)
			{
				std::string* str = cpps_get_string(left);
				cpps_domain* takedomain = leftdomain;
				leftdomain = NULL;
				cpps_value right = cpps_calculate_expression(c, domain, root, d->getright()->getleft(), leftdomain);
				if (right.tt != CPPS_TINTEGER) {
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String must contain a number as an index."));
				}
				leftdomain = takedomain;
				if ((cpps_integer)str->size() <= right.value.integer)
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String has crossed the current length: [%d]. You need to get the length: [%d]..", str->size(), right.value.integer));

				ret = (cpps_integer)str->at(right.value.integer);
			}
		/*	else if (left.tt == CPPS_TDOMAIN) {
				leftdomain = left.value.domain;
				ret = cpps_calculate_expression(c, left.value.domain, root, d->getright(), leftdomain);
			}*/
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}
	void cpps_calculate_expression_getobject(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value left = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR || left.tt == CPPS_TSTRING ) {
				/* cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0]; */
				cpps_domain* execdomain = c->domain_alloc();
				execdomain->init(c->_G, cpps_domain_type_exec);
				/* cpps_domain *takedomain = leftdomain; */
				leftdomain = left.value.domain;
				execdomain->setexecdomain(left.value.domain);
				ret = cpps_calculate_expression(c, execdomain, root, d->getright(), leftdomain);
				/* leftdomain = takedomain; */
				execdomain->destory(c);
				c->domain_free(execdomain);
			}
			else if (left.tt == CPPS_TCLASS && root->parent[1] && root->parent[1]->domainType == cpps_domain_type_classvar) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain;
				cpps_domain* takedomain = NULL;
				/* check this class is parent class.. */
				bool is_parent_class = cpps_check_parent_class((cpps_cppsclass*)root->parent[1]->parent[0], cppsclass);
				if (!is_parent_class)
					throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "Non parent function executed: [%s::%s].", d->getleft()->s.c_str(), d->getright()->s.c_str()));
				cpps_regvar* var = cppsclass->getvar(d->getright()->s, takedomain, false);
				if (var && var->getval().tt == CPPS_TFUNCTION) {
					ret = var->getval();
					leftdomain = root->parent[1];
				}
			}
			else if (left.tt == CPPS_TDOMAIN) {
				/* cpps_domain* takedomain = leftdomain; */
				leftdomain = left.value.domain;
				ret = cpps_calculate_expression(c, left.value.domain, root, d->getright(), leftdomain);
				/*
				 * if (left.value.domain->domainType == cpps_domain_type_namespace) //名空间不更改左域
				 * leftdomain = takedomain;
				 */
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}
	void cpps_calculate_expression_quotevarname(C* c, cpps_domain*& leftdomain, cpps_domain* domain, node* d, cpps_value& ret) {
		cpps_regvar* v = (leftdomain ? leftdomain : domain)->getvar(d->s, leftdomain);
		if (v) {
			cpps_value tmp = v->getval();
			ret = cpps_value(v->isconst() ? (&tmp) : (&v->getval()));
		}
		else {
			printf("cpps warring: [%s] is not define.. line : %d, file : %s\n", d->s.c_str(), d->line, d->filename.c_str());
		}
	}
	void cpps_calculate_expression_quotegetchild(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value left = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0];
				if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "vector") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_vector* pVec = (cpps_vector*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right = cpps_calculate_expression(c, domain, root, d->getright()->getleft(), leftdomain);
					if (right.tt != CPPS_TINTEGER) {
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array must contain a number as an index."));
					}
					leftdomain = takedomain;
					if (pVec->size() <= right.value.integer)
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array has crossed the current length: [%d]. You need to get the length: [%d].", pVec->size(), right.value.integer));
					ret = cpps_value(&pVec->cpps_at(right.value.integer));
				}
				else if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "map") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_map* pMap = (cpps_map*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right = cpps_calculate_expression(c, domain, root, d->getright()->getleft(), leftdomain);
					leftdomain = takedomain;
					ret = cpps_value(&pMap->cpps_find(right));
				}
				else {
					/*cpps_regvar* v = getregvar(left.value.domain, d->getright()->getleft());
					if (v) {
						cpps_value tmp = v->getval();
						ret = cpps_value(v->isconst() ? (&tmp) : (&v->getval()));
					}*/
				}
			}
			else if (left.tt == CPPS_TSTRING)
			{
				std::string* str = cpps_get_string(left);
				cpps_domain* takedomain = leftdomain;
				leftdomain = NULL;
				cpps_value right = cpps_calculate_expression(c, domain, root, d->getright()->getleft(), leftdomain);
				if (right.tt != CPPS_TINTEGER) {
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String must contain a number as an index."));
				}
				leftdomain = takedomain;
				if ((cpps_integer)str->size() <= right.value.integer)
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String has crossed the current length: [%d]. You need to get the length: [%d]..", str->size(), right.value.integer));

				ret = cpps_value(&str->at(right.value.integer));
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}
	void cpps_calculate_expression_quotegetobject(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value left = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR) {
				/* cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0]; */
				cpps_regvar* v = getregvar(left.value.domain, d->getright());
				if (v) {
					cpps_value tmp = v->getval();
					ret = cpps_value(v->isconst() ? (&tmp) : (&v->getval()));
				}
				else {
					printf("cpps warring: [%s] is not define.. line : %d, file : %s\n", d->getright()->s.c_str(), d->getright()->line, d->getright()->filename.c_str());
				}
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}

	void cpps_calculate_expression_await(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret)
	{
		cpps_value var = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		/*await 可能接受 cpps_async_object 或者 cpps_async_task*/
		cpps_value vtask = cpps_async_await(c, var);
		cpps_async_task* task = cpps_converter<cpps_async_task*>::apply(vtask);
		if (task) {
			if (task->state() == cpps_async_task_thorw) {
				auto throwerr = task->throwerr;
				throw throwerr;
			}
			ret = task->getresult();
		}
	}

	void cpps_calculate_expression_this(cpps_domain* root, cpps_value& ret)
	{
		if (root->parent[1] && root->parent[1]->domainType == cpps_domain_type_classvar)
		{
			ret = cpps_value((cpps_cppsclassvar*)root->parent[1]);
		}			
	}

	cpps_value cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain) {
		cpps_value ret;
		if (d->type == CPPS_OOFFSET) {
			cpps_calculate_expression_offset(d, c, ret, root, leftdomain);
		}
		else if (d->type == CPPS_QUOTEOFFSET) {
			leftdomain = cpps_calculate_expression_quoteoffset(d, c, ret, root, leftdomain);
		}
		else if (d->type == CPPS_ODOFUNCTION) {
			cpps_calculate_expression_dofunction(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_OSTR) {
			cpps_calculate_expression_ostr(ret, d, leftdomain, c, domain, root);
		}
		else if (d->type == CPPS_ONUMBER) {
			ret.tt = CPPS_TNUMBER;
			ret.value.number = d->value.number;
		}
		else if (d->type == CPPS_OINTEGER) {
			ret.tt = CPPS_TINTEGER;
			ret.value.integer = d->value.integer;
		}
		else if (d->type == CPPS_OINTEGER16) {
			ret.tt = CPPS_TINTEGER;
			ret.value.integer = d->value.integer;
		}
		else if (d->type == CPPS_ONEWVAR) {
			cpps_calculate_expression_newvar(domain, d, c, root, leftdomain, ret);
		}
		else if (d->type == CPPS_OBRACKET) {
			cpps_domain* leftdomain = NULL;
			/* 获取第一个节点 */
			if (d->getleft()) ret = cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain);
		}
		else if (d->type == CPPS_OARRAY) {
			cpps_calculate_expression_array(ret, c, d, leftdomain, domain, root);
		}
		else if (d->type == CPPS_OOBJECT) {
			cpps_calculate_expression_object(ret, c, d, leftdomain, domain, root);
		}
		else if (d->type == CPPS_OBOOL) {
			ret.tt = CPPS_TBOOLEAN;
			ret.value.b = (d->s[0] == 't');
			/* 首字母为t 就直接认为他是 true */
		}
		else if (d->type == CPPS_OTHIS)
		{
			cpps_calculate_expression_this(root, ret);
		}
		else if (d->type == CPPS_OAWAIT)
		{
			cpps_calculate_expression_await(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_VARNAME) {
			cpps_calculate_expression_varname(c,leftdomain, domain, d, ret);
		}
		else if (d->type == CPPS_VARNAME_LAMBDA) {
			cpps_calculate_expression_lambda(c,domain,root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_FUNCNAME) {
			cpps_symbol_handle(c, domain, root, d, ret);
		}
		else if (d->type == CPPS_OGETCHIILD) {
			cpps_calculate_expression_getchild(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_OGETOBJECT) {
			cpps_calculate_expression_getobject(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_QUOTEVARNAME) {
			cpps_calculate_expression_quotevarname(c, leftdomain, domain, d, ret);
		}
		else if (d->type == CPPS_QUOTEGETCHIILD) {
			cpps_calculate_expression_quotegetchild(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_QUOTEGETOBJECT) {
			cpps_calculate_expression_quotegetobject(c, domain, root, d, leftdomain, ret);
		}
		return(ret);
	}
	/* ///////////////////// */
	cpps_value cpps_execute_callfunction(C* c, cpps_function* f, cpps_domain* domain, std::string filename, int32 line, std::string funcname, cpps_std_vector& params) {
		cpps_value	ret;
		cpps_stack* stack = c->stack_alloc();
		stack->init(filename.c_str(), line, funcname.c_str());
		c->push_stack(stack);
		f->callfunction(c, &ret, domain, &params, stack);
		c->pop_stack();
		c->stack_free(stack);
		return(ret);
	}
	void cpps_debug_breakpoint(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		std::vector<cpps_stack*>* stacklist = c->getcallstack();
		int			i = 1;
		for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it) {
			cpps::cpps_stack* stack = *it;
			std::cout << "#f" << i << " " << stack->f << "	The" << stack->l << "line	function:" << stack->func << std::endl;
			i++;
		}
		std::cout << "quit debug command : quit" << std::endl;
		while (true) {
			std::cout << "debug>";
			std::string str ;
			getline(std::cin, str);
			if (str == "quit")
				break;
			node* o = loadbuffer(c, str, "");
			cpps_stack* stack = c->stack_alloc();
			stack->init(d->filename.c_str(), d->line, "dostring");
			c->push_stack(stack);
			cpps_step_all(c, CPPS_MUNITRET, domain, root, o);
			c->pop_stack();
			c->stack_free(stack);
		}
	}
	void cpps_debug_trace_domain(int kg,std::string parent,cpps_domain* root) {
		for (auto var : root->varList)
		{
			cpps_value& value = var.second->getval();
			if (!cpps_base_isfunction(value)) {
				for (int i = 0; i < kg; i++)
					printf(" ");
				printf("%s%s = ", parent.c_str(), var.second->varName.c_str());
				cpps_base_printf(value);
				if (value.isdomain() && !value.value.domain->varList.empty())
				{
					for (int i = 0; i < kg; i++)
						printf(" ");
					printf("\r\n{\r\n");
					cpps_domain* value_domain = value.value.domain;
					cpps_debug_trace_domain(kg + 4, (parent.empty() ? std::string("") : (parent + ".")) + var.second->varName + ".", value_domain);
					for (int i = 0; i < kg; i++)
						printf(" ");
					printf("}\r\n");

				}
				printf("\r\n");
			}
		}
	}
	void cpps_debug_trace(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_debug_trace_domain(0,"",root);
	}
	cpps_value cpps_step_callfunction(C* c, cpps_domain* domain, cpps_domain* root, cpps_value func, node* d, cpps_domain* leftdomain) {
		cpps_value ret;
		if (func.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func = pfunc;
			func.tt = CPPS_TFUNCTION;
		}
		if (func.tt == CPPS_TFUNCTION) {
			cpps_function* f = func.value.func;
			if (f->funcname == "isset") c->disabled_non_def_var = true;
			cpps_std_vector params;
			cpps_value		isNeedC;
			if (f->getIsNeedC()) {
				isNeedC = cpps_cpp_to_cpps_converter<C*>::apply(c, c);
				params.push_back(isNeedC);
			}
			make_values(c, domain, root, d->getright(), params);
			std::string	filename = d->getright()->filename;
			std::string	funcname = f->getfuncname();
			int32		line = d->getright()->line;
			if (f) {
				/*协程异步函数返回一个协程对象,不执行函数*/
				if (f->isasync()) {
					/*要记录的有 leftdomain 函数, 参数列表..cpps_function本体 其他开发过程在补.*/
					cpps_async_object* obj ; 
					ret = newclass< cpps_async_object>(c, &obj);
					obj->f = f;
					obj->leftdomain = leftdomain ? leftdomain : c->_G;
					obj->params = params;
					obj->filename = filename;
					obj->funcname = funcname;
					obj->line = line;
				}
				else {

					cpps_domain* execdomain = leftdomain ? leftdomain : c->_G;
					
					ret = cpps_execute_callfunction(c, f, execdomain, filename, line, funcname, params);

					c->disabled_non_def_var = false;
				}
			}
			cpps_regvar* v = cpps_node_to_regver(domain, d->getleft(), false);
			if (v && v->varName == "debug" && d->getleft()->getright()->s == "breakpoint") {
				cpps_debug_breakpoint(c, domain, root, d);
			}
			if (v && v->varName == "debug" && d->getleft()->getright()->s == "trace") {
				cpps_debug_trace(c, domain, root, d);
			}
		}
		else {
			/* 需要一个函数，但是他不是！！！！ */
			if (d->getleft()->l.size() == 2)
				printf("cpps warring: [%s] is not function  line:%d , file:%s \n", d->getleft()->getright()->s.c_str(), d->line, d->filename.c_str()); else
				printf("cpps warring: [%s] is not function  line:%d , file:%s \n", d->getleft()->s.c_str(), d->line, d->filename.c_str());
		}
		return(ret);
	}
	cpps::object _G(C* c) {
		return(cpps_value(c->_G));
	}
	void collect(C* c) {
		cpps_gc_check_gen0(c);
	}
	void collectall(C* c) {
		cpps_gc_check_gen1(c);
	}
	void setchartrans(C* c, std::string(*func)(std::string&)) {
		c->func = func;
	}

	


}