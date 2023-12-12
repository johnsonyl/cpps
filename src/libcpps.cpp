#include "cpps/cpps.h"
namespace cpps {
	/* 内部 */
	struct     cpps_value;
	struct     node;
	struct     cpps_domain;
	struct     cpps_value;
	struct     cpps_func_params;
	void cpps_gc_add_gen0(C* c, cpps_cppsclassvar* p);
	void cpps_console_color(cpps_integer color);
	void cpps_console_clearcolor();
	void cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* o, cpps_domain*& leftdomain, cpps_value& ret);
	void cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d);
	cpps_value cpps_step_callfunction(C* c, cpps_domain* domain, cpps_domain* root, cpps_value func, node* d, cpps_domain* leftdomain);
	void cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o, bool);
	void cpps_parse_dofunction(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	void cpps_parse_expression(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	int32 cpps_parse_expression_step(C* c, cpps_node_domain* domain, node* param, node*& lastOpNode, node* root, cppsbuffer& buffer);
	node* cpps_parse_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	node* cpps_parse_symbol(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer, bool leftsymbol = false);
	node* cpps_parse_string(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int8 endch);
	void cpps_parse_var(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit, node_var_type vartype);
	node* cpps_parse_var_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, bool findparent = true);
	node* cpps_parse_number(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer);
	void cpps_parse_def_function(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer);
	node* cpps_parse_line(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int32 limit = 0);
	void cpps_parse_builtin(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit);
	void cpps_parse_def(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit);
	node* cpps_parse_new_setv(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer);
	void cpps_parse_foreach2(C* c, cpps_node_domain* fordomain, node* child, node* root, cppsbuffer& buffer);
	void cpps_gc_add_barrier(C* c, cpps_regvar* v);
	void cpps_gc_check_gen0(C* c);
	void cpps_gc_check_gen1(C* c);
	bool cpps_loadlibrary(C* c, std::string libname,cppsbuffer& buffer);
	void cpps_symbol_handle(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_value& ret);
	bool cpps_io_file_exists(std::string path);
	std::string cpps_getcwd();
	std::string cpps_real_path();
	void gc_cleanup(C* c,bool cleanall = false);
	void gc_swap(C* src, C* dest);
	void cpps_detach_library(HMODULE module, const std::string& libname, C* c);
	int32 cpps_str2ui64(const char* s, cpps_uinteger* result);
	void cpps_base_printfln_new(C* c, cpps::cpps_value args, ...);
	bool cpps_io_isdir(std::string p);
	bool cpps_io_isfile(std::string p);
	void cpps_cpp_real_walk(std::vector<std::string>& vct, std::string path, bool bfindchildren);
	node* cpps_parse_getsymbol2node(C* c, node* o, std::string  symbolStr, cppsbuffer& buffer, bool leftsymbol);


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
			|| c->getgen0()->size() > CPPS_GEN0_CHECKCOUNT
			)) {
			cpps_gc_check_gen0(c);
		}
	}
	bool cpps_parse_isnumber(char ch) {
		return(ch >= 48 && ch <= 57);
	}
	bool cpps_parse_isspace(char ch) {
		return(ch == ' ' || ch == '	');
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
			|| ch == '<' || ch == '>' || ch == '?'
			|| ch == '^' || ch == '~' || ch == '^' );
	}
	bool cpps_parse_isnotvarname(char ch) {
		return(ch == '~' || ch == '!' || ch == '@' || ch == '#' || ch == '$'
			|| ch == '%' || ch == '^' || ch == '&' || ch == '*' || ch == '('
			|| ch == ')' || ch == '+' || ch == '-' || ch == '`' || ch == '{'
			|| ch == '}' || ch == ';' || ch == '"' || ch == ':' || ch == '\'' 
			|| ch == '\\'|| ch == '|' || ch == ',' || ch == '<' || ch == '.' 
			|| ch == '>' || ch == '/' || ch == '?' || ch == '\0'|| cpps_parse_isspace(ch)
			|| cpps_parse_issymbol(ch)|| cpps_parse_isenter(ch) || ch == '\n');
	}
	bool cpps_parse_isint16(char ch) {
		ch = (char)tolower((int)ch);
		return(ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0');
	}
	std::string cpps_parse_varname(cppsbuffer& buffer) {
		std::string ret = "";
		while (!cpps_parse_isnotvarname(buffer.cur())) {
			ret.push_back(buffer.pop());
			if (buffer.isend())
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_varerror, "Unexpected end"));
		}
		return(ret);
	}

	std::string cpps_parse_other_varname(cppsbuffer& buffer, char endc) {
		std::string ret = "";
		while (buffer.cur() != endc) {
			ret.push_back(buffer.pop());
			if (buffer.isend())
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_varerror, "Unexpected end"));

		}
		return(ret);
	}
	std::string cpps_parse_object_varname(cppsbuffer& buffer) {
		std::string ret = "";
		char hold = 0;
		if (buffer.cur() == '\'') {
			hold = '\'';
			buffer.pop();
		}
		else if (buffer.cur() == '\"') {
			hold = '\"';
			buffer.pop();
		}
		if (hold != 0) {
			ret = cpps_parse_other_varname(buffer, hold);
		}
		else {
			ret = cpps_parse_varname(buffer);
			if (ret.empty())
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_varerror, "error object name"));
		}
		if (hold != 0) {
			if(buffer.cur() != hold)
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_varerror, "Unexpected end"));
			buffer.pop();
		}

		return(ret);
	}
	bool cpps_parse_isbuiltinname(std::string s) {
		return(s == "if" || s == "echo" || s == "const" ||
			s == "async" || s == "yield" || s == "try" || 
			s == "throw" || s == "namespace" || s == "var" || 
			s == "else" || s == "for" || s == "foreach" ||
			s == "do" || s == "while" || s == "class" ||
			s == "module" || s == "struct" || s == "break" ||
			s == "continue" || s == "case" || s == "switch" ||
			s == "enum" || s == "return" || s == "dofile" ||
			s == "import" || s == "include" || s == "dostring"||
			s == "parse"|| s == "assert"|| s == "donode" );
	}
	bool cpps_is_not_use_var_name(std::string s) {
		return(cpps_parse_isbuiltinname(s) ||
			s == "true" ||
			s == "catch" ||
			s == "null" ||
			s == "nil" ||
			s == "NULL" || 
			s == "false" || 
			s == "map" || 
			s == "vector" ||
			s == "math" ||
			s == "string" || 
			s == "time" ||
			s == "io" ||
			s == "GC");
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
		node* str = CPPSNEW (node)(o->filename, buffer.line());
		str->type = CPPS_OSTR;
		buffer.pop();
		/* pop '"' */
		node* laststr = NULL;
		while (buffer.realcur() != endch && !buffer.isend()) {
			char lastch = buffer.realpop();
			/* todo 解析反斜杠 */
			char ch = cpps_parse_transfer_character(lastch, buffer);
			if (lastch != '\\' && ch == '{') {
				node* v = CPPSNEW( node)(str, o->filename, buffer.line());
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
					laststr = CPPSNEW( node)(o->filename, buffer.line());
					laststr->type = CPPS_OSTR;
					str->add(laststr);
				}
				laststr->s.push_back(ch);
			}
		}
		buffer.pop();

		/*const string*/
		if (str->l.size() == 1 || str->l.size() == 0 ) {
			std::string* tmpstr;
			str->value.val = CPPSNEW(cpps_value)();
			newclass(c, &tmpstr, str->value.val);
			str->needdelete = true;
			if(str->l.size() == 1) tmpstr->append(str->l[0]->s);
		}

		return(str);
	}
	node* cpps_parse_regx(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int8 endch) {
		node* str = CPPSNEW( node)(o->filename, buffer.line());
		str->type = CPPS_OSTR;
		buffer.pop();
		/* pop '"' */
		node* laststr = NULL;
		while (buffer.realcur() != endch && !buffer.isend()) {
			char lastch = buffer.realpop();
			if (laststr == NULL) {
				laststr = CPPSNEW( node)(o->filename, buffer.line());
				laststr->type = CPPS_OSTR;
				str->add(laststr);
			}
			laststr->s.push_back(lastch);
		}
		buffer.pop();

		/*const string*/
		if (str->l.size() == 1 || str->l.size() == 0) {
			std::string* tmpstr;
			str->value.val = CPPSNEW(cpps_value)();
			newclass(c, &tmpstr, str->value.val);
			str->needdelete = true;
			if (str->l.size() == 1)tmpstr->append(str->l[0]->s);
		}

		return(str);
	}
	
	node* cpps_get_root_node(node* n) {
		if (n->parent)
			return(cpps_get_root_node(n->parent)); else
			return(n);
	}
	void cpps_parse_def_function_param(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer) {
		node* typen = CPPSNEW( node)(right, right->filename, buffer.line());
		node* varname = NULL;
		if (buffer.cur() == '.' && buffer.at(buffer.offset() + 1) == '.' && buffer.at(buffer.offset() + 2) == '.') {
			// var func(var a,... ps){}
			typen->type = CPPS_VARNAME;
			buffer.pop();//.
			buffer.pop();//.
			buffer.pop();//.
			right->quote = true; //quote true 代表是多参数
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
		}
		else {
			typen->type = CPPS_VARNAME;
			typen->s = cpps_parse_varname(buffer);
			if (typen->s != "var") throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "you need to put an 'var' before defining the parameters"));
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
		}
		
		varname = CPPSNEW( node)(typen, right->filename, buffer.line());
		if (buffer.cur() == '&' && typen->type == CPPS_VARNAME) {
			buffer.pop();
			varname->quote = true;
			cpps_parse_rmspaceandenter(buffer);
		}
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
		if (buffer.cur() == '=' && typen->type == CPPS_VARNAME){
			/*有默认值 也就是可以不填... */ 
			buffer.pop();
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, varname, root, buffer);
		}
	}
	void cpps_parse_def_function(C* c, cpps_node_domain* domain, node* right, node* root, cppsbuffer& buffer) {
		/* 定义了一个函数 */
		cpps_node_domain* funcdomain = CPPSNEW( cpps_node_domain)(right,domain, cpps_domain_type_func, right->parent->s);
		right->setdomain(funcdomain);
		node* params = CPPSNEW( node)(right, right->filename, buffer.line());
		node* context = CPPSNEW( node)(right, right->filename, buffer.line());
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
			if (params->quote) {
				//多参数,必须结束
				cpps_parse_rmspaceandenter(buffer);
				ch = buffer.cur();
				if (ch != ')') {
					throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "mulitparams func parse error.."));
				}
				buffer.pop();
				break;
			}
		}

		cpps_parse_rmspaceandenter(buffer);
		/*尝试看看有没有async?*/
		if (buffer.cur() != '{' && buffer.cur() != '=') {
			std::string tag = cpps_parse_varname(buffer);
			if (tag == "async") {
				/*..怎么告诉外部呢?*/
				if(right && right->parent && right->parent->parent && right->parent->parent->type == CPPS_ODEFVAR)
					right->parent->parent->type = CPPS_ODEFASYNCVAR;//修改成异步函数
				else
					throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "async lambda parse error.."));
			}
			else {
				throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "async lambda parse error.."));
			}
			cpps_parse_rmspaceandenter(buffer);
		}
		/* 解析{ } */
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
		else if (ch == '=' && buffer.at(buffer.offset()) == '>') {
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_line(c, funcdomain, context, root, buffer);
			return;
		}
		else if (ch == ';')
		{
			/*ok.!*/
			return;
		}
		else {
			throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "Definition function did not detect '{'"));
		}
		throw(cpps_error(right->filename, buffer.line(), cpps_error_deffuncrror, "Definition function did not detect '}'"));
	}
	node* cpps_parse_var_right(C* c, cpps_node_domain* domain, node* var, node* root, cppsbuffer& buffer, int32 limit, node_var_type vartype) {
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		node* right = CPPSNEW( node)(var, var->filename, buffer.line());
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
		else if (buffer.cur() == ':' && buffer.at(buffer.offset()+1) == ':')
		{
			/* var namespace::namespace::class::func()*/
			node* lastNode = right;
			/* 是否使用名空间 */
			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				if (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') {
					buffer.pop();
					buffer.pop();
					cpps_parse_rmspaceandenter(buffer);
					node* child = CPPSNEW (node)(lastNode, var->filename, buffer.line());
					child->s = cpps_parse_varname(buffer);
					child->type = CPPS_ONAMESPANCE_CHILD;
					lastNode = child;
				}
				else {
					break;
				}
			}
			if (buffer.cur() == '(')
				/* 定义了一个函数 */ {
				buffer.pop();
				if (limit & CPPS_NOT_DEFFUNCTION) {
					throw("Function definition prohibited");
				}
				if (vartype == node_var_type_constvar)
					throw(cpps_error(right->filename, buffer.line(), cpps_error_varerror, "do not define const function."));

				lastNode->type = CPPS_ODEFVAR_FUNC;
				lastNode->size = 0;
				lastNode->varsize = 0;
				/*
				 * right->offset = 0;
				 * right->offsettype = 0;
				 * root 修改为 函数节点
				 */
				cpps_parse_def_function(c, domain, lastNode, lastNode, buffer);
				/* 定义了一个函数 */
				right->type = CPPS_ODEFVAR_HOTUPDATE;

			}
			else
			{
				throw(cpps_error(right->filename, buffer.line(), cpps_error_varerror, "invalid syntax.hotupdate child function need ("));
			}
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
		if (buffer.cur() == '&') {
			buffer.pop();
			str->quote = true;
			cpps_parse_rmspaceandenter(buffer);
		}

		cpps_parse_rmspaceandenter(buffer);
		if (cpps_parse_isnumber(buffer.cur()))
			/* 首位是个字母 */
			throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Variable cannot start with a number."));
		/* 先找名字 */
		

		str->s = cpps_parse_varname(buffer);
		if (str->s.empty()) {
			throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Variable names cannot empty."));
		}
		if (str->s == "operator") {
			cpps_parse_rmspaceandenter(buffer);
			node* op = NULL;
			if (buffer.cur() == '(' && buffer.at(buffer.offset() + 1) == ')') {
				//operator () 特殊处理
				op = cpps_parse_getsymbol2node(c, str, "()", buffer, false);
			}
			else {
				op = cpps_parse_symbol(c, domain, str, buffer);
			}
			
			if (!op) throw(cpps_error(str->filename, buffer.line(), cpps_error_varerror, "invalid operator symbol."));
			str->s = op->symbol->symbolfuncname;
			str->symbol = op->symbol;
			op->release();
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
			node* str = CPPSNEW( node)(child, child->filename, buffer.line());
			str->type = CPPS_VARNAME;

			/*多返回值*/
			if (buffer.cur() == '[') {
				buffer.pop();/*pop [*/
				str->type = CPPS_MULTIVAR;

				if (root->type == CPPS_OCLASS || root->type == CPPS_ONAMESPACE  )
					throw(cpps_error(str->filename, buffer.line(), cpps_error_varnotnumber, "Multiple return value variables cannot be defined in a class or namespace."));

				node* childstr = CPPSNEW( node)(str, child->filename, buffer.line());
				childstr->type = CPPS_VARNAMES;
				while (!buffer.isend())
				{
					node* str2 = CPPSNEW( node)(childstr, child->filename, buffer.line());
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

				buffer.pop(); /*pop ]*/

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
	void cpps_parse_enum(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer, int32 limit) {
		child->type = CPPS_OENUM;
		cpps_parse_rmspaceandenter(buffer);
		child->offset = 0;
		child->offsettype = 0;
		child->varsize = 0;
		child->size = 0;

		cpps_parse_rmspaceandenter(buffer);
		child->s = cpps_parse_varname(buffer);
		if (child->s.empty()) {
			child->s = "_G"; /*全局*/
		}
		cpps_parse_rmspaceandenter(buffer);

		cpps_integer enum_idx = 0;
		if (buffer.cur() != '{')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'{' was detected for enum. Please check."));
		buffer.pop();

		

		while (!buffer.isend()) {
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			/* 是否到最后了. */
			if (buffer.cur() == '}') {
				break;
			}
			if (buffer.isend()) {
				throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'}' was detected for enum. Please check."));
			}

			node* enum_value = CPPSNEW( node)(child, child->filename, buffer.line());
			enum_value->s = cpps_parse_varname(buffer);
			cpps_parse_rmspaceandenter(buffer);
			
			if (buffer.cur() == '=') {
				buffer.pop();
				cpps_parse_rmspaceandenter(buffer);

				node *value = cpps_parse_number(c, domain, enum_value, buffer);
				if (value->type != CPPS_OINTEGER && value->type != CPPS_OINTEGER16) 
					throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "enum only defined integer value."));

				enum_value->value.integer = value->value.integer;
				if(enum_value->value.integer < enum_idx )
					throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "enum the value defined must be greater than the previous value ."));
				enum_idx = enum_value->value.integer + 1;
				value->release();
			}
			else {
				enum_value->value.integer = enum_idx++;
			}
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() == ',')
				buffer.pop();
			else
				break;
		}

		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '}')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_moduleerror, "'}' was detected for enum. Please check."));
		buffer.pop();

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

		//if (c->buildoffset) {
		//	if (root->type == CPPS_ROOT)
		//		/* 根节点.. */ {
		//		child->offset = root->size++;
		//		child->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
		//		root->regnode(child->s, child);
		//	}
		//}
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
		cpps_node_domain* trycatchdomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_trycatch, "trycatch");
		/* 为这个集创建一个自己的域空间 */
		child->setdomain(trycatchdomain);
		child->type = CPPS_OTRYCATCH;
		node* func = CPPSNEW( node)(child, child->filename, buffer.line());
		/* 0 if的参数 */
		node* catchfun = CPPSNEW( node)(child, child->filename, buffer.line());
		catchfun->type = CPPS_OCATCH;
		/* true */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() == '{') {
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);

			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					break;
				}
				cpps_parse_line(c, trycatchdomain, func, root, buffer);
			}
		}
		else {
			cpps_parse_line(c, trycatchdomain, func, root, buffer);
		}
		cpps_parse_rmspaceandenter(buffer);
		std::string el = cpps_parse_varname(buffer);
		if (el == "catch") {
			node* catchvar = CPPSNEW( node)(catchfun, child->filename, buffer.line());
			/* true */
			node* catchstep = CPPSNEW( node)(catchfun, child->filename, buffer.line());
			/* true */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != '(')
				throw(cpps_error(catchfun->filename, buffer.line(), cpps_error_trycatherror, "Catch did not detect '('"));
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			std::string ifvar = cpps_parse_varname(buffer);
			if (ifvar != "var")
				throw(cpps_error(catchfun->filename, buffer.line(), cpps_error_trycatherror, "'var' needs to be defined to variables."));
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
			if (buffer.cur() == '{') {
				buffer.pop();

				cpps_parse_rmspaceandenter(buffer);

				while (!buffer.isend()) {
					/* 剔除回车. */
					cpps_parse_rmspaceandenter(buffer);
					/* 是否到最后了. */
					if (buffer.cur() == '}') {
						buffer.pop();
						break;
					}
					cpps_parse_line(c, domain, catchstep, root, buffer);
				}
			}
			else {
				cpps_parse_line(c, domain, catchstep, root, buffer);
			}
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

		/*char('str')*/
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

	void cpps_parse___file__(C* c, cpps_node_domain* domain, node* child, cppsbuffer& buffer) {
		child->type = CPPS_OSTR;
		child->s = buffer.getcurfile().filename;

		auto laststr = CPPSNEW(node)(child->filename, buffer.line());
		laststr->type = CPPS_OSTR;
		child->add(laststr);

		std::string* tmpstr;
		child->value.val = CPPSNEW(cpps_value)();
		newclass(c, &tmpstr, child->value.val);
		child->needdelete = true;
		if (child->l.size() == 1)tmpstr->append(buffer.getcurfile().filename);
	}
	void cpps_parse___line__(C* c, cpps_node_domain* domain, node* child, cppsbuffer& buffer) {
		child->type = CPPS_OINTEGER;
		child->value.integer = buffer.line();
	}
	void cpps_parse___func__(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OSTR;
		//child->s = root;
		if (root && root->type == CPPS_ODEFVAR_FUNC && root->parent && root->parent->type == CPPS_VARNAME) {
			child->s = root->parent->s;
		}
		else {
			child->s = "__func__";
		}

		auto laststr = CPPSNEW(node)(child->filename, buffer.line());
		laststr->type = CPPS_OSTR;
		child->add(laststr);

		std::string* tmpstr;
		child->value.val = CPPSNEW(cpps_value)();
		newclass(c, &tmpstr, child->value.val);
		child->needdelete = true;
		if (child->l.size() == 1)tmpstr->append(child->s);
	}
	node* cpps_parse_var_param(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, bool findparent) {
		node* param = CPPSNEW( node)(o->filename, buffer.line());
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
					node* child = CPPSNEW( node)(lastNode, o->filename, buffer.line());
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
				node* count = CPPSNEW( node)(param, param->filename, buffer.line());
				count->type = CPPS_OVECTORSIZE;
				cpps_parse_loadnumber(buffer, count);
				if (buffer.cur() != ']') {
					throw(cpps_error(param->filename, buffer.line(), cpps_error_varnotnumber, "']' was not detected when defining an array"));
				}
				buffer.pop();
			}
			else if (buffer.cur() == '(') {
				buffer.pop();
				node* child = CPPSNEW( node)(param->filename, buffer.line());
				child->type = CPPS_OCLASS_CONSTRUCTOR;
				cpps_node_domain* funcdomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_func, o->s);
				/* 为这个集创建一个自己的域空间 */
				child->setdomain(funcdomain);
				cpps_parse_dofunction(c, funcdomain, child, root, buffer);
				param->addtoright(child);
				cpps_parse_rmspaceandenter(buffer);
				if (buffer.cur() == '{') {
					param->setparent(root);
					node* setv = cpps_parse_new_setv(c, domain, param, root, buffer); /*root 修改成newvar node 但是这里取不到变量数量呀. 应该取类才对.*/
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
		else if (param->s == "__FILE__") {
			cpps_parse___file__(c, domain, param, buffer);
		}
		else if (param->s == "__LINE__") {
			cpps_parse___line__(c, domain, param, buffer);
		}
		else if (param->s == "__func__") {
			cpps_parse___func__(c, domain, param, root, buffer);
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
						t->closure = true; /*闭包创建永不删除*/
					}
					//这里可以优化如果目标是一个常量值，则可以直接取值不需要在调用它了。类似  a+b+c b为常量10 则优化a+10+c 
					//下个版本优化
				}
			}
			/* 剔除空格 */
			cpps_parse_rmspaceandenter(buffer);
		}
		return(param);
	}
	void cpps_checknumber_right(C* c, cpps_node_domain* domain, node* number, cppsbuffer& buffer) {
		char v = buffer.cur();
		switch (v) {
		case 'u':
		case 'U':
			buffer.pop();
			number->type = CPPS_OUINTEGER;
			cpps_str2ui64(number->s.c_str(), &number->value.uinteger);
			break;
		case 'L':
			buffer.pop();
		default:
			cpps_str2i64(number->s.c_str(), &number->value.integer);
			break;
		}
	}
	node* cpps_parse_number(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer) {
		node* str = CPPSNEW( node)(o->filename, buffer.line());
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
		else if (str->s == "0" && buffer.cur() == 'x') {
			str->type = CPPS_OINTEGER16;
			buffer.pop();
			str->s += "x";
			str->s += cpps_parse_loadinterger16(buffer);
			cpps_str2i64(str->s.c_str(), &str->value.integer);
		}
		else {
			//检测后面又没有U或者u 为unsigned
			cpps_parse_rmspaceandenter(buffer);
			cpps_checknumber_right(c, domain, str, buffer);
		}
		return(str);
	}
	node* cpps_parse_bracket(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer) {
		node* bracket = CPPSNEW( node)(o->filename, buffer.line());
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
		node* bracket = CPPSNEW( node)(o->filename, buffer.line());
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
			node* n = CPPSNEW( node)(bracket, o->filename, buffer.line());
			node* k = CPPSNEW( node)(n, o->filename, buffer.line());
			k->s = cpps_parse_object_varname(buffer);
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':')
				throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "Definition object not detected '}'."));
			buffer.pop();
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			node* v = CPPSNEW( node)(n, o->filename, buffer.line());
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
		node* bracket = CPPSNEW( node)(o->filename, buffer.line());
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
			node* n = CPPSNEW(node)(bracket, o->filename, buffer.line());
			node* k = CPPSNEW( node)(n, o->filename, buffer.line());
			k->s = cpps_parse_varname(buffer);
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != '=')
				throw(cpps_error(bracket->filename, buffer.line(), cpps_error_arrayeerror, "'=' was not detected when setting the variable"));
			buffer.pop();
			/* 剔除回车. */
			cpps_parse_rmspaceandenter(buffer);
			node* v = CPPSNEW( node)(n, o->filename, buffer.line());
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
		node* bracket = CPPSNEW( node)(o->filename, buffer.line());
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
			node* n = CPPSNEW( node)(bracket, o->filename, buffer.line());
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
	void cpps_parse_getchild_or_slice(C* c, cpps_node_domain* domain, node* root, node* geto,node* parent, cppsbuffer& buffer)
	{
		while (!buffer.isend()) {

			node* child = CPPSNEW( node)(parent, parent->filename, buffer.line());
			node* first = CPPSNEW( node)(child, child->filename, buffer.line());
			node* second = CPPSNEW (node)(child, child->filename, buffer.line());
			node* third = CPPSNEW (node)(child, child->filename, buffer.line());

			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':') {
				cpps_parse_expression(c, domain, first, root, buffer);
			}

			cpps_parse_rmspaceandenter(buffer);
			/*fix ellpsis.*/
			if (first->getleft() && first->getleft()->type == CPPS_OELLIPSIS){
				geto->type = CPPS_OSLICE;
			}
			if (buffer.cur() == ']') {
				buffer.pop();
				return;
			}

			if (buffer.cur() == ',') {
				geto->type = CPPS_OSLICE;
				buffer.pop();
				continue;
			}
			if (buffer.cur() != ':')
				throw(cpps_error(child->filename, buffer.line(), cpps_error_unknow, "';'slice parsing error"));
			buffer.pop();

			geto->type = CPPS_OSLICE;


			if (buffer.cur() == ']') {
				buffer.pop();
				return;
			}

			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':') {
				cpps_parse_expression(c, domain, second, root, buffer);
			}

			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() == ',') {
				buffer.pop();
				continue;
			}
			if (buffer.cur() == ']') {
				buffer.pop();
				return;
			}
			if (buffer.cur() != ':')
				throw(cpps_error(child->filename, buffer.line(), cpps_error_unknow, "';'slice parsing error"));
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() == ',') {
				buffer.pop();
				continue;
			}
			if (buffer.cur() != ']') {
				cpps_parse_expression(c, domain, third, root, buffer);
			}

			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() == ',') {
				buffer.pop();
				continue;
			}
			if (buffer.cur() != ']')
				throw(cpps_error(child->filename, buffer.line(), cpps_error_unknow, "']' parsing error"));

			buffer.pop();
			return;
		}
	}
	node* cpps_parse_last_func(C* c, cppsbuffer& buffer, node* o, node* p, cpps_node_domain* domain, node* root) {
		/*有后续 */
		cpps_parse_rmspaceandenter(buffer);
		while (buffer.cur() == '[' || buffer.cur() == '.' || buffer.cur() == '(' || (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') || (buffer.cur() == '-' && buffer.at(buffer.offset() + 1) == '>')) {
			/*  fix  var    .     func();  */
			if (buffer.cur() == '.') {
				auto take = buffer.offset();
				buffer.pop();

				cpps_parse_rmspaceandenter(buffer);
				if (cpps_parse_isnotvarname(buffer.cur())) {
					buffer.seek(take);
					break;
				}
				buffer.seek(take);
			}
			/*fix slice[var::] but slice[var::var] don't work..*/
			if (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') {

				auto take = buffer.offset();
				buffer.pop();
				buffer.pop();
				cpps_parse_rmspaceandenter(buffer);

				if (cpps_parse_isnotvarname(buffer.cur())) {
					buffer.seek(take);

					break;
				}
				buffer.seek(take);
			}
			/*fix slice[var::] but slice[var::var] don't work..*/
			if (buffer.cur() == '-' && buffer.at(buffer.offset() + 1) == '>') {

				auto take = buffer.offset();
				buffer.pop();
				buffer.pop();
				cpps_parse_rmspaceandenter(buffer);

				if (cpps_parse_isnotvarname(buffer.cur())) {
					buffer.seek(take);

					break;
				}
				buffer.seek(take);
			}
				

			char symblo = buffer.pop();
			node* geto = CPPSNEW( node)(o->filename, buffer.line());
			geto->setparent(o);
			geto->addtoleft(p);
			node* child = NULL;
			if (symblo == '[') {
				geto->type = CPPS_OGETCHIILD;
				child = CPPSNEW( node)(o->filename, buffer.line());
				child->setparent(geto);
				cpps_parse_getchild_or_slice(c, domain, root, geto,child, buffer);
			}
			else if (symblo == '.' || symblo == '-') {
				if (symblo == '-' && buffer.cur() != '>')
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "invalid syntax"));
				if (symblo == '-') {
					buffer.pop(); /* pop >*/
					geto->quote = true; /* -> getsubobject*/
				}


				geto->type = CPPS_OGETOBJECT;
				child = CPPSNEW( node)(o->filename, buffer.line());
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
				child = CPPSNEW( node)(o->filename, buffer.line());
				child->setparent(geto);
				cpps_node_domain* funcdomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_func, o->s);
				/* 为这个集创建一个自己的域空间 */
				child->setdomain(funcdomain);
				cpps_parse_dofunction(c, funcdomain, child, root, buffer);
			}
			else if (symblo == ':') {
				if (buffer.cur() != ':')
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "namespace requires double ':'"));
				buffer.pop();
				geto->type = CPPS_OGETOBJECT;
				
				child = CPPSNEW( node)(o->filename, buffer.line());
				child->type = CPPS_VARNAME;
				cpps_parse_var_varname(buffer, child, c, root, domain);
				if (!child)
					throw(cpps_error(o->filename, buffer.line(), cpps_error_unknow, "namespace:: need a name is required later."));
			}
			geto->addtoright(child);
			/* 放到后续. */
			p = geto;
			cpps_parse_rmspaceandenter(buffer);
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
		else if (ch == 'r' && (buffer.at(buffer.offset() + 1) == '"' || buffer.at(buffer.offset() + 1) == '\''))
		{
			buffer.pop();
			char ch2 = buffer.cur();
			if (ch2 == '"') {
				p = cpps_parse_regx(c, domain, o, root, buffer, '"');
			}
			else if(ch2 == '\''){
				p = cpps_parse_regx(c, domain, o, root, buffer, '\'');
			}
		}
		else if (ch == ':' && buffer.at(buffer.offset() + 1) == ':') {
			p = CPPSNEW(node)(o->filename, buffer.line());
			p->type = CPPS_OGLOBAL;
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
	node* cpps_parse_getsymbol2node(C*c, node* o, std::string  symbolStr, cppsbuffer& buffer, bool leftsymbol) {
		cpps_symbol* symbol = cpps_parse_getsymbol(c, symbolStr, leftsymbol);
		if (!symbol) {
			return(NULL);
		}
		buffer.seek(buffer.offset() + (int32)symbolStr.size());
		node* pNode = CPPSNEW(node)(o->filename, buffer.line());
		pNode->type = CPPS_FUNCNAME;
		pNode->symbol = symbol;
		pNode->s = symbolStr;
		return pNode;
	}
	node* cpps_parse_symbol(C* c, cpps_node_domain* domain, node* o, cppsbuffer& buffer, bool leftsymbol) {
		std::string	symbolStr;
		std::string	tempStr;
		char		ch = buffer.cur();
		int32		curoffset = buffer.offset();
		while (cpps_parse_issymbol(ch)) {
			tempStr += ch;
			cpps_symbol* symbol = cpps_parse_getsymbol(c,tempStr, leftsymbol);
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
		return cpps_parse_getsymbol2node(c,o,symbolStr,buffer,leftsymbol);
	}
	void cpps_parse_expression(C* c, cpps_node_domain* domain, node* param, node* root, cppsbuffer& buffer) {
		node* lastOpNode = param;
		while (cpps_parse_expression_step(c, domain, param, lastOpNode, root, buffer) == CPPS_NOERROR)
			;
	}
	std::string buildlambda(C*c) {
		std::string	ret = "lamdba@" + std::to_string(c->_lambdanum++) + "_";
		int		i;
		int		len = 20;
		for (i = 0; i < len; ++i)
			ret += 'A' + rand() % 26;
		return(ret);
	}
	int32 cpps_parse_expression_step(C* c, cpps_node_domain* domain, node* param, node*& lastopnode, node* root, cppsbuffer& buffer) {
		/* ellipsis*/
		if (buffer.cur() == '.' && buffer.at(buffer.offset() + 1) == '.' && buffer.at(buffer.offset() + 2) == '.') {
			buffer.seek(buffer.offset() + 3);
			node* ellipsis = CPPSNEW( node)(param->filename, buffer.line());
			ellipsis->type = CPPS_OELLIPSIS;
			lastopnode->l.emplace_back(ellipsis);
			return 1;
		}
		/* 新增lambda函数参数 */
		if (buffer.cur() == '[' && buffer.at(buffer.offset() + 1) == ']' && buffer.at(buffer.offset() + 2) == '(') {
			buffer.seek(buffer.offset() + 3);
			node* r = cpps_get_root_node(root);
			node* lamdbavar = CPPSNEW( node)(param->filename, buffer.line());
			lamdbavar->setparent(lastopnode);
			lamdbavar->type = CPPS_ODEFVAR;
			r->l.insert(r->l.begin(), lamdbavar);
			node* str = CPPSNEW( node)(lamdbavar, param->filename, buffer.line());
			str->s = buildlambda(c);
			str->type = CPPS_VARNAME;

			/*加快运行速度*/
			if (c->buildoffset) {
				node* r2 = cpps_get_root_node(root);
				str->offset = r2->size++;
				str->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
			}

			node* lambda = CPPSNEW( node)(str, param->filename, buffer.line());
			lambda->type = CPPS_ODEFVAR_LAMBDA_FUNC;
			lambda->size = 0;
			if (root->type == CPPS_ODEFVAR_LAMBDA_FUNC || root->type == CPPS_ODEFVAR_FUNC){
				lambda->size = root->size;/*为了兼容闭包*/
			}
			usint16 takesize = lambda->size;
			cpps_parse_def_function(c, domain, lambda, lambda, buffer);
			/* 定义了一个函数 */
			node* lambdaparam = CPPSNEW( node)(param->filename, buffer.line());
			lambdaparam->type = CPPS_VARNAME_LAMBDA;
			lambdaparam->s = str->s;
			if (c->buildoffset) {
				lambdaparam->offset = str->offset;
				lambdaparam->offsettype = str->offsettype;
			}
			lambdaparam->size = takesize; /*记录当时使用时它父类长度.*/
			lambdaparam->setparent(lastopnode);
			lambdaparam = cpps_parse_last_func(c, buffer, lastopnode, lambdaparam, domain, root);
			lastopnode->l.emplace_back(lambdaparam);
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
				lastopnode->l.emplace_back(p);
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
					lastopnode->l.emplace_back(p);
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
			node* takeparent = op->parent;
			op->parent = NULL;
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, op, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ':') {
				throw(cpps_error(param->filename, buffer.line(), cpps_error_paramerror, "Ternary expression parsing error."));
			}
			buffer.pop();
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, op, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			op->parent = takeparent;
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
					lastopnode->l.emplace_back(p);
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
					lastopnode->l.emplace_back(p);
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
			lastopnode->l.emplace_back(op);
			if (lastopnode->symbol && lastopnode->symbol->getparamleftlimit()) {
				if (op->type != CPPS_VARNAME && op->type != CPPS_OOFFSET && op->type != CPPS_OGETOBJECT && op->type != CPPS_OSLICE && op->type != CPPS_OGETCHIILD) {
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
				if (op->type != CPPS_VARNAME && op->type != CPPS_OOFFSET && op->type != CPPS_OGETOBJECT && op->type != CPPS_OSLICE && op->type != CPPS_OGETCHIILD) {
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
			if (op->symbol->getprio() == compareop->symbol->getprio() && !cpps_parse_isleftasso(c,op->symbol->getprio())) {
				break;
			}
			if (!compareop->parent)
				break;
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
		cpps_node_domain* assdomain = CPPSNEW (cpps_node_domain)(o,domain, cpps_domain_type_ass, "assemble");
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
		cpps_node_domain* ifdomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_if, "if");
		/* 为这个集创建一个自己的域空间 */
		child->setdomain(ifdomain);
		child->type = CPPS_OIF;
		node* param = CPPSNEW( node)(child, child->filename, buffer.line());
		/* 0 if的参数 */
		node* t = CPPSNEW( node)(child, child->filename, buffer.line());
		/* true */
		node* f = CPPSNEW (node)(child, child->filename, buffer.line());
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
		if (buffer.cur() == ';') {
			buffer.pop();
			return;
		}
		//优化{} 因为{}效率非常低。
		if (buffer.cur() == '{') {
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);

			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					break;
				}
				cpps_parse_line(c, ifdomain, t, root, buffer);
			}
		}
		else {
			cpps_parse_line(c, ifdomain, t, root, buffer);
		}
		cpps_parse_rmspaceandenter(buffer);
		int32 offset = buffer.offset();
		/* 记住当前的字符， 因为下一句可能不是else */
		std::string el = cpps_parse_varname(buffer);
		if (el == "else") {
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() == '{') {
				buffer.pop();

				cpps_parse_rmspaceandenter(buffer);

				while (!buffer.isend()) {
					/* 剔除回车. */
					cpps_parse_rmspaceandenter(buffer);
					/* 是否到最后了. */
					if (buffer.cur() == '}') {
						buffer.pop();
						break;
					}
					cpps_parse_line(c, ifdomain, f, root, buffer);
				}
			}
			else {
				cpps_parse_line(c, ifdomain, f, root, buffer);
			}
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
	void cpps_parse_echo(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OECHO;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		/* 查找后续参数 */
		//cpps_parse_expression(c, domain, child, root, buffer);
		while (true) {

			/* 查找后续参数 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, child, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ',') break;

			buffer.pop(); /* pop ,*/

		}

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
			CPPSNEW( node)(child, child->filename, child->line);
			return;
		}
		/*支持multi return*/
		while (true) {

			/* 查找后续参数 */
			cpps_parse_rmspaceandenter(buffer);
			cpps_parse_expression(c, domain, child, root, buffer);
			cpps_parse_rmspaceandenter(buffer);
			if (buffer.cur() != ',') break;
			
			buffer.pop(); /* pop ,*/

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
		if (buffer.cur() == ';') {
			buffer.pop();
			return;
		}
		/* 查找后续参数 */
		cpps_parse_expression(c, domain, child, root, buffer);
	}
	void cpps_parse_for(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* fordomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_for, "for");
		child->setdomain(fordomain);
		child->type = CPPS_OFOR;
		node* for1 = CPPSNEW(node)(child, child->filename, buffer.line());
		
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing '(' after for"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ';') {
			do
			{
				if (buffer.cur() == ',')
					buffer.pop();
				cpps_parse_line(c, fordomain, for1, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS);
				cpps_parse_rmspaceandenter(buffer);

			} while (buffer.cur() == ',' && !buffer.isend());
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		

		if (buffer.cur() == ':')
		{
			//foreach 优化
			child->type = CPPS_OFOREACH;
			cpps_parse_foreach2(c, fordomain, child, root, buffer);
			return;
		}
		if (buffer.cur() != ';') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing first ';' after for"));
		}
		buffer.pop();

		node* for2 = CPPSNEW(node)(child, child->filename, buffer.line());
		node* for3 = CPPSNEW(node)(child, child->filename, buffer.line());
		node* for4 = CPPSNEW(node)(child, child->filename, buffer.line());

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
		if (buffer.cur() != ')') {
			do 
			{
				if (buffer.cur() == ',')
					buffer.pop();
				cpps_parse_line(c, fordomain, for3, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DEFVAR | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS);
				cpps_parse_rmspaceandenter(buffer);

			} while (buffer.cur() == ',' && !buffer.isend());
		}
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing ')'after for"));
		}
		buffer.pop();
		/* pop ) */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() == ';') {
			buffer.pop();
			return;
		}
		if (buffer.cur() == '{') {
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);

			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					return;
				}
				cpps_parse_line(c, fordomain, for4, root, buffer);
			}
		}
		else {
			cpps_parse_line(c, fordomain, for4, root, buffer);
		}
	}
	void cpps_parse_foreach(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* fordomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_for, "for");
		child->setdomain(fordomain);
		child->type = CPPS_OFOREACH;
		node* for1 = CPPSNEW(node)(child, child->filename, buffer.line());
		
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing '('after foreach"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		do
		{
			if (buffer.cur() == ',')
				buffer.pop();
			cpps_parse_line(c, fordomain, for1, root, buffer, CPPS_NOT_DEFASSEMBLE | CPPS_NOT_DEFFUNCTION | CPPS_NOT_USEBUILTIN | CPPS_NOT_DONTDELETEEND | CPPS_NOT_DEFCLASS | CPPS_NOT_DEFVAR);
			cpps_parse_rmspaceandenter(buffer);

		} while (buffer.cur() == ',' && !buffer.isend());
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		

		cpps_parse_foreach2(c, fordomain, child, root, buffer);
		
	}
	void cpps_parse_foreach2(C*c, cpps_node_domain* fordomain,node * child, node* root, cppsbuffer& buffer) {

		if (buffer.cur() != ':')
			throw(cpps_error(child->filename, buffer.line(), cpps_error_forerror, "Missing ':'after foreach"));
		buffer.pop();

		node* for2 = CPPSNEW(node)(child, child->filename, buffer.line());
		node* for4 = CPPSNEW(node)(child, child->filename, buffer.line());
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
		if (buffer.cur() == ';') {
			buffer.pop();
			return;
		}
		if (buffer.cur() == '{') {
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);

			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					return;
				}
				cpps_parse_line(c, fordomain, for4, root, buffer);
			}
		}
		else {
			cpps_parse_line(c, fordomain, for4, root, buffer);
		}
	}
	void cpps_parse_while(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		cpps_node_domain* whiledomain = CPPSNEW( cpps_node_domain)(child,domain, cpps_domain_type_while, "while");
		child->setdomain(whiledomain);
		child->type = CPPS_OWHILE;
		node* while1 = CPPSNEW( node)(child, child->filename, buffer.line());
		node* while2 = CPPSNEW(node)(child, child->filename, buffer.line());
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
		if (buffer.cur() == ';') {
			buffer.pop();
			return;
		}
		if (buffer.cur() == '{') {
			buffer.pop();

			cpps_parse_rmspaceandenter(buffer);

			while (!buffer.isend()) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.cur() == '}') {
					buffer.pop();
					return;
				}
				cpps_parse_line(c, whiledomain, while2, root, buffer);
			}
		}
		else {
			cpps_parse_line(c, whiledomain, while2, root, buffer);
		}
	}
	node* cpps_parse_getparent_node(node* root, node* parent) {
		if (!root) return NULL;
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
				if (c->buildoffset) {
					node* cpyn = CPPSNEW (node)();
					cpyn->clone(var_node);
					srcclass->getthird()->add(cpyn); /*为了移除用*/
					if (var_node->l[0]->type != CPPS_ODEFVAR_FUNC) {
						cpyn->offset = srcclass->varsize++;
						cpyn->offsettype = CPPS_OFFSET_TYPE_LEFTDOMAIN;
					}
					srcclass->regnode(var_node->s, cpyn);
					
				}
				
			}
		}
	}
	void cpps_parse_reg_base_class2(C*c,std::set<cpps_cppsclass*>& parentset, node* srcclass, cpps_cppsclass* baseclass) {
		if (parentset.find(baseclass) != parentset.end())
			throw(cpps_error(srcclass->filename, srcclass->line, cpps_error_classerror, "[%s] base class is defined repeatedly", baseclass->getclassname().c_str()));

		parentset.insert(baseclass);
		/* 父类的 */
		auto base_parents = baseclass->parentclasslist();
		node* base_vars = baseclass->getvars_node()->getright();
		/* 在注册父类的父类 */
		for (auto parent : base_parents) {
			cpps_parse_reg_base_class2(c,parentset, srcclass, parent);
		}
		/*
		 * 注册父类函数因为函数都移动到了global.所以可以共用
		 * 先注册父类vars...
		 */
		for (auto var : base_vars->l) {
			node* var_node = var->l[0];
			if (!var_node->l.empty())
				/*有没有值.不是函数就要注册上. */ {
				if (c->buildoffset) {
					node* cpyn = CPPSNEW (node)();
					cpyn->clone(var_node);
					srcclass->getthird()->add(cpyn); /*为了移除用*/
					if (var_node->l[0]->type != CPPS_ODEFVAR_FUNC) {
						cpyn->offset = srcclass->varsize++;
						cpyn->offsettype = CPPS_OFFSET_TYPE_LEFTDOMAIN;
					}
					srcclass->regnode(var_node->s, cpyn);
					
				}
				
			}
		}
	}
	cpps_cppsclass* cpps_parse_getparent_cppsclass(cpps_domain* _G, node* parent)
	{
		if (!_G) return NULL;
		cpps_regvar* ret = NULL;
		cpps_domain* leftdomain = NULL;
		ret = _G->getvar(parent->s, leftdomain,false,false);
		if (!ret->getval().isdomain()) return NULL;

		if (!parent->l.empty()) {
			cpps_domain* next = cpps_to_domain(ret->getval());
			return cpps_parse_getparent_cppsclass(next, parent->l[0]);
		}
		if (!cpps_isclass(ret->getval())) return NULL;
		return cpps_to_cpps_cppsclass(ret->getval());
	}

	void cpps_parse_class(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OCLASS;
		/*
		 * child->offset = 0;
		 * child->offsettype = 0;
		 */
		child->varsize = 0;
		child->size = 0;
		node* parents = CPPSNEW (node)(child, child->filename, buffer.line());
		node* vars = CPPSNEW (node)(child, child->filename, buffer.line());
		CPPSNEW (node)(child, child->filename, buffer.line());
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
		std::set<cpps_cppsclass*> parentset_class;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() == ':') {
			/* 说明有继承关系啊 */
			buffer.pop();
			while (!buffer.isend()) {
				/* 剔除空格 */
				cpps_parse_rmspaceandenter(buffer);
				node* parent = CPPSNEW( node)(parents, parents->filename, buffer.line());
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
						node* child2 = CPPSNEW (node)(lastNode, parents->filename, buffer.line());
						child2->s = cpps_parse_varname(buffer);
						child2->type = CPPS_ONAMESPANCE_CHILD;
						lastNode = child2;
					}
					else {
						break;
					}
				}
				if (c->buildoffset) {
					node* parent_node = cpps_parse_getparent_node(root, parent);
					if (parent_node)
						cpps_parse_reg_base_class(c, parentset, child, parent_node);
					else {
						cpps_cppsclass* parent_class = cpps_parse_getparent_cppsclass(c->_G, parent);
						if (parent_class)
							cpps_parse_reg_base_class2(c, parentset_class, child, parent_class);
						else
							throw(cpps_error(parent->filename, parent->line, cpps_error_classerror, "[%s] base class is not defined", parent->s.c_str()));

					}
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
		//cpps_parse_rmspaceandenter(buffer);
		//cpps_parse_expression(c, domain, child, root, buffer);

		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		/*#import "base64" √*/
		if (buffer.cur() != '\"') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '\"' after include"));
		}
		buffer.pop();
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		std::string path = cpps_parse_other_varname(buffer,'\"');
		child->s = path;
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '\"') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '\"'after include"));
		}
		buffer.pop();
		/*载入文件*/
		cpps_loadlibrary(c, path,buffer);
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
	
	void cpps_parse_include_file(C* c, std::string path,cppsbuffer &buffer) {
		if (cpps_io_isdir(path)) {
			std::vector<std::string> files;
			cpps_cpp_real_walk(files, path, false);
			for (std::string& _path : files) {
				cpps_parse_include_file(c, _path, buffer);
			}
		}
		else if (cpps_io_isfile(path)) {
			/*载入文件*/
			std::string fileSrc;
			cpps_load_filebuffer(path.c_str(), fileSrc);
			buffer.append(path, fileSrc.c_str(), (int32)fileSrc.size());
		}
	}
	void cpps_parse_include(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OINCLUDE;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		/*#include <cpps/cpps.cpp> √*/
		if (buffer.cur() != '<') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '<' after include"));
		}
		buffer.pop();
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		std::string path = cpps_parse_other_varname(buffer,'>');
		child->s = path;
		path = cpps_rebuild_filepath(path);
		if(path.empty()) throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "can't include file : %s", child->s.c_str()));
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '>') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_classerror, "Missing '>'after include"));
		}
		buffer.pop();
		cpps_parse_include_file(c, path, buffer);
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
		child->s = buffer.getcurfile().filename;
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
	void cpps_parse_donode(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_ODONODE;
		child->s = buffer.getcurfile().filename;
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing '(' after donode"));
		}
		buffer.pop();
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, domain, child, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing ')' after donode"));
		}
		buffer.pop();
		/* pop ) */
	}
	void cpps_parse_assert(C* c, cpps_node_domain* domain, node* child, node* root, cppsbuffer& buffer) {
		child->type = CPPS_OASSERT;
		
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != '(') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing '(' after assert"));
		}
		buffer.pop();

		auto take = buffer.offset();
		child->s = cpps_parse_other_varname(buffer,')');
		buffer.seek(take);
		/* pop ( */
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		cpps_parse_expression(c, domain, child, root, buffer);
		/* 剔除空格 */
		cpps_parse_rmspaceandenter(buffer);
		if (buffer.cur() != ')') {
			throw(cpps_error(child->filename, buffer.line(), cpps_error_iferror, "Missing ')' after assert"));
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
		else if (child->s == "donode") {
			cpps_parse_donode(c, domain, child, root, buffer);
		}
		else if (child->s == "assert") {
			cpps_parse_assert(c, domain, child, root, buffer);
		}
		else if (child->s == "break") {
			cpps_parse_break(c, domain, child, buffer);
		}
		else if (child->s == "continue") {
			cpps_parse_continue(c, domain, child, buffer);
		}
		else if (child->s == "echo")
		{
			cpps_parse_echo(c, domain, child, root, buffer);
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
		else if (child->s == "enum"){
			cpps_parse_enum(c, domain, child, root, buffer, limit);
		}
		else if (child->s == "try") {
			cpps_parse_trycatch(c, domain, child, root, buffer, limit);
		}
	}
	node* cpps_parse_line(C* c, cpps_node_domain* domain, node* o, node* root, cppsbuffer& buffer, int32 limit) {
		node* child = CPPSNEW( node)(o, buffer.getcurfile().filename, buffer.line());
		cpps_parse_rmspaceandenter(buffer);
		while (buffer.cur() == ';') {
			buffer.pop(); cpps_parse_rmspaceandenter(buffer);
		}/*pop空行*/
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
				throw(cpps_error(buffer.getcurfile().filename, buffer.line(), cpps_error_normalerror, "Definition system not allowed"));
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
		else if (buffer.cur() == ':' && buffer.at(buffer.offset() + 1) == ':') {
			// ::println("1") => _G.println("1"); ::代表是根节点的println
			child->type = CPPS_OEXPRESSION;
			cpps_parse_expression(c, domain, child, root, buffer);
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
				node* str = CPPSNEW( node)(child, child->filename, buffer.line());
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
	void cpps_regbase(C* c, cppsbuffer& buffer)
	{
		if (!c->isloadbase) {
			cpps_loadlibrary(c, "base", buffer);
			c->isloadbase = true;
		}
	}
	node* loadbuffer(C* c,cpps_domain *parent_domain, std::string& str, std::string filename) {
		/* 如果需要转译 */
		if (c->func)
			str = c->func(str);
		node* o = CPPSNEW( node)(filename);
		cpps_node_domain* domain = CPPSNEW (cpps_node_domain)(o, NULL, cpps_domain_type_root, "root");
		o->setdomain(domain);
		o->type = CPPS_ROOT;
		if(c->buildoffset && parent_domain) o->size = parent_domain->stacklist ? (int16)parent_domain->stacklist->size() : 0;
		o->varsize = 0;
		cppsbuffer buffer(filename.c_str(), str.c_str(), (int32)str.size());
		cpps_regbase(c,buffer); // only first dofile reg base module.
		try {
			while (true) {
				/* 剔除回车. */
				cpps_parse_rmspaceandenter(buffer);
				/* 是否到最后了. */
				if (buffer.isend()) {
					break;
				}
				cpps_parse_line(c, domain, o, o, buffer);
			}
		}
		catch (...){
			cpps_destory_node(o);
			CPPSDELETE(o);
			throw;
		}
	
		
		if (c->buildoffset && parent_domain && parent_domain->stacklist) parent_domain->stacklist->resize(o->size);
		return(o);
	}
	void cpps_create_root_G(C* c) {
		c->_G = CPPSNEW( cpps_domain)(NULL, cpps_domain_type_root, "root");
		/* 创建根节点域 */
		c->_G->setgcLevel(2);
		/* 永久存在 除非程序没了. */
		cpps_regvar* v = CPPSNEW( cpps_regvar)();
		/* _G 为根节点 */
		v->setvarname("_G");
		auto tmp = cpps_value(c->_G);
		v->setval(tmp);
		/* 域列表会copy进去 */
		c->_G->regvar(NULL, v);
		/* 将自己注册成_G.. */
	}

	
	void cpps_init_memory(C*c) {
		c->memory_handler = CPPSMEMORYHANDLER();
	}
	void cpps_init_c(C* c) {
		c->_classvarlock = new cpps_lock();
		c->_gen0lock = new cpps_lock();
	}
	cpps::C* create(C* _parent) {
		C* c = new cpps::C();
		cpps_create_root_G(c);
		c->clone(_parent->_parentCState ? _parent->_parentCState : _parent);
		return c;
	}
	cpps::C* create(int argc, char** argv, cpps_alloc_f alloc_func, cpps_free_f free_func) {
		CPPSMEMORYINIT(alloc_func, free_func);
		C* c = new cpps::C(argc, argv);
		cpps_init_c(c);
		cpps_init_memory(c);
		cpps_create_root_G(c);
		cpps_regsymbols(c);
		cpps_regstring(c);
		cpps_regbase(c);
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
		cpps_regthread(c);

		return(c);
	}
	int32 dostring(C* c, std::string str) {
		_CPPS_TRY
			node* o = loadbuffer(c, c->_G, str, "");
		if (o)
			cpps_step_all(c, CPPS_MUNITRET, 0, 0, o, false);
			/* dostring pcall */
		if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }
		_CPPS_CATCH
		cpps_gc_check_step(c);
		return(CPPS_NOERROR);
	}
	int32 loadfile(cpps::C* c, const char* path) {
		c->buildoffset = true;
		std::string fileSrc;
		cpps_load_filebuffer(path, fileSrc);
		node* o = loadbuffer(c,c->_G, fileSrc, path);
		
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
				n->release();
			}
		}
		d->l.clear();
	}

	void cpps_free_all_library(cpps::C*& c)
	{
		for (auto lib : c->modulelist) {
			if (lib.second != NULL) {
				cpps_detach_library(lib.second, lib.first, c);
			}
		}
		c->modulelist.clear();
	}

	int32 close(cpps::C*& c,cpps::C* parent_c) {
		if(c->_parentCState != NULL)
			c->_G->cleanup();

		c->barrierList.clear();
		c->resume();

		gc_cleanup(c);
		/* 清理内存 */

		
		c->_G->destory(c, true);
		c->domain_pool.freeall();
		c->stack_pool.freeall();
		cpps_unregasyncio(c);
		if(c->isloadbase == false)
			gc_cleanup(c, true);
		else {
			gc_cleanup(c);
			if(parent_c) gc_swap(c,parent_c);
		}

		cpps_free_all_library(c);
		c->_class_map_classvar.clear();

		c->_G->release();
		c->_G = NULL;
		delete c;
		c = NULL;
		return(0);
	}
	int32  pcall(C* c, int32 retType, cpps_domain* domain, node* o) {
		_CPPS_TRY
		domain->resize(o->size);
		cpps_step_all(c, retType, domain, domain, o, true);
		_CPPS_CATCH
		cpps_gc_check_step(c);
		cpps_destory_node(c->o); c->o->release(); c->o = NULL;
		return(0);
	}
	void  cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o,bool releasenode) {
		/*有可能里面的让我退出 */
		size_t count = o->l.size();
		if (count == 0) return;
		if (domain == NULL)
			domain = c->_G;
		if (root == NULL)
			root = c->_G;
		if (o == NULL)
			o = c->o;
		domain->isbreak = false;
		for (size_t i = 0; i < count && !domain->isbreak && !c->isterminate; i++) {
			node* d = o->l[i];
			cpps_step(c, domain, root, d);
			if (releasenode) {
				cpps_destory_node(d);
				d->release();
				o->l[i] = NULL;
			}
		}
		if (releasenode)
			o->l.clear();
	}

	cpps_regvar* cpps_step_def_var_createvar( cpps_domain* domain, node* varName, C* c, cpps_domain* root, node_var_type isasync)
	{
		cpps_domain* leftdomain = NULL;
		cpps_regvar* v = domain->getvar(varName->s, leftdomain, false);
		if (!v) {
			v = CPPSNEW( cpps_regvar)();
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

	void cpps_step_def_var_createvar_setval(cpps_value& value, cpps_regvar* v, C* c,bool quato = false)
	{
		if (value.tt == CPPS_TREF && !quato) {
			CPPS_TO_REAL_VALUE(value);
		}

		if (value.tt == CPPS_TSTRING) {
			
			std::string* tmpStr = cpps_get_string(value);
			std::string *newstr;
			newclass(c, &newstr, &v->getval());
			newstr->append(tmpStr->begin(), tmpStr->end());
		}
		else {
			v->setval(value);
		}
		
	}

	void cpps_step_def_var(C* c, cpps_domain* domain, cpps_domain* root, node* d, node_var_type isasync) {
		/* d->s 变量名字 */
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
						multiresult.emplace_back(v);
					}
				}
				if (right && right->type == CPPS_ODEFVAR_VAR)
				{
					cpps_domain* leftdomain = NULL;
					cpps_value	value;
					cpps_calculate_expression(c, domain, root, right->l[0], leftdomain,value);
					CPPS_TO_REAL_VALUE(value);

					if (value.tt == CPPS_TTUPLE) {
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
						/*就用一下*/
						value = nil;
					}
					else {
						/*只返回一个..*/
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
						cpps_value	value;
						cpps_calculate_expression(c, domain, root, var->l[0], leftdomain,value);
						if (varName->quote) {
							cpps_step_def_var_createvar_setval(value, v, c, varName->quote);
						}
						else {
							CPPS_TO_REAL_VALUE(value);
							if (value.tt == CPPS_TTUPLE) {
								cpps_value firstvalue = cpps_to_cpps_vector(value)->realvector()[0];
								cpps_step_def_var_createvar_setval(firstvalue, v, c);
								value = nil;
							}
							else
								cpps_step_def_var_createvar_setval(value, v, c);
						}
					}
				}
				else if (var && (var->type == CPPS_ODEFVAR_FUNC || var->type == CPPS_ODEFVAR_LAMBDA_FUNC)) {
					/*全局函数,不存在重载问题*/
					
					cpps_domain* leftdomain = NULL;
					cpps_regvar* v = domain->getvar(varName->s, leftdomain, false);
					cpps_cppsfunction* func = NULL;
					if (v) {
						func = (cpps_cppsfunction*)v->getval().value.func;
					}
					else {
						v = CPPSNEW( cpps_regvar)();
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
					
					if (func == NULL){
						cpps_domain* funcdomain = CPPSNEW (cpps_domain)(NULL, cpps_domain_type_func, d->s + "::" + varName->s);
						func = CPPSNEW (cpps_cppsfunction)(funcdomain, var->l[0], var->l[1], var->size);
					}
					else
						func->rebuildfunc(var->l[0], var->l[1], var->size);

					func->setfuncname(varName->s);
					func->setIsNeedC(false);
					if (isasync == node_var_type::node_var_type_asyncvar)
						func->setasync(true);
					func->setquatoreturn(varName->quote);
					if (var->l[0]->quote)
						func->setmulitparams(true);

					auto tmp = cpps_value(func);
					v->setval(tmp);
				}
				else if(var && var->type == CPPS_ODEFVAR_HOTUPDATE) {
					cpps_domain* leftdomain = NULL;
					cpps_regvar* v = domain->getvar(varName->s, leftdomain, false);
					cpps_regvar* subv = v;
					if (v) {
						node* takenode = var->getleft();
						while(takenode && (takenode->type == CPPS_ONAMESPANCE_CHILD || takenode->type == CPPS_ODEFVAR_FUNC)){
							v = subv;
							subv = subv->getval().value.domain->getvar(takenode->s, leftdomain);
							if (!subv) throw cpps_error(d->filename, d->line, cpps_error_trycatherror, "hotupdate classfunc [%s] not found.", takenode->s.c_str());
							if (takenode->getleft() && (takenode->getleft()->type == CPPS_ONAMESPANCE_CHILD || takenode->getleft()->type == CPPS_ODEFVAR_FUNC)) takenode = takenode->getleft();
							else break;
						}
						if (subv && subv->getval().tt == CPPS_TFUNCTION)
						{
							cpps_function* func = subv->getval().value.func;
							if (func->iscppsfunc()) {
								cpps_cppsfunction* cppsfunc = (cpps_cppsfunction*)func;
								/*如果是类函数*/
								if (v->getval().tt == CPPS_TCLASS) {
									cpps_cppsclass* pclsthis = cpps_to_cpps_cppsclass(v->getval());
									if (pclsthis == cppsfunc->getclass()) {
										cppsfunc->rebuildfunc(takenode->getleft(), takenode->getright(), takenode->size); /*不是继承来的替换.*/
									}
									/*继承来的暂时不可以修改. 并且也不能创建.*/
								}
								else {
									/*不是类的函数直接替换,没有继承问题*/
									cppsfunc->rebuildfunc(takenode->getleft(), takenode->getright(), takenode->size);
								}
							}
							else {
								throw cpps_error(d->filename, d->line, cpps_error_trycatherror, "hotupdate classfunc [%s] must be a cppsfunction.", takenode->s.c_str());
							}
						}
						else {
							throw cpps_error(d->filename, d->line, cpps_error_trycatherror, "hotupdate classfunc [%s] must be a cppsfunction.", takenode->s.c_str());
						}
					}
				}
			}
		}
	}
	void cpps_step_echo(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		if (d->l.size() == 1) {
			cpps_domain* leftdomain = NULL;
			cpps_value	ret_value;
			if (d->l.size() == 1)
				cpps_calculate_expression(c, domain, root, d->l[0], leftdomain,ret_value);
			else if (d->l.size() > 1) {
				cpps_vector* vec;
				newclass(c, &vec, &ret_value);
				ret_value.tt = CPPS_TTUPLE;
				for (auto nn : d->l) {
					cpps_value vv;
					cpps_calculate_expression(c, domain, root, nn, leftdomain, vv);
					vec->realvector().emplace_back(vv.real());
				}
			}
			CPPS_TO_REAL_VALUE(ret_value);


			object echofunc = cpps::object::globals(c)["__echofunc"];
			object echoleft = cpps::object::globals(c)["__echoleft"];
			if (echofunc.isfunction()) {
				if (echoleft.isclassvar()) {
					doclassfunction(c, echoleft, echofunc, ret_value);
				}
				else {
					dofunction(c, echofunc, ret_value);
				}
			}
			else {
				if (d->l.size() == 1)
					cpps_base_printfln(c, ret_value);
				else
					cpps_base_printfln_new(c,ret_value);
			}
		}
	}
	void cpps_step_throw(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		if (d->l.size() == 0) {
			throw;
		}
		else if (d->l.size() == 1) {
			cpps_domain* leftdomain = NULL;
			cpps_value	ret_value;
			cpps_calculate_expression(c, domain, root, d->l[0], leftdomain,ret_value);
			CPPS_TO_REAL_VALUE(ret_value);

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
				throw(cpps_error(d->filename, d->line, cpps_error_trycatherror, cpps_to_string(ret_value).c_str()));
			}
		}
		else {
			/* 暂时不支持多个参数 */
		}
	}
	void cpps_step_return(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		
		cpps_domain* leftdomain = NULL;
		cpps_value	ret_value;
		if (d->l.size() == 1)
			cpps_calculate_expression(c, domain, root, d->l[0], leftdomain, ret_value);
		else if(d->l.size() > 1){
			cpps_vector* vec;
			newclass(c, &vec,&ret_value);
			ret_value.tt = CPPS_TTUPLE;
			for (auto nn : d->l) {
				cpps_value vv;
				cpps_calculate_expression(c, domain, root, nn, leftdomain,vv);
				vec->realvector().emplace_back(vv.real());
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
			if (cpps_func_domain->funcRet.tt == CPPS_TREF)
				cpps_func_domain->funcRet = ret_value;
			else if(ret_value.tt == CPPS_TREF)
				cpps_func_domain->funcRet = *ret_value.value.value;
			else
				cpps_func_domain->funcRet = ret_value;

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
		cpps_value	if_value;
		cpps_calculate_expression(c, domain, root, d->l[0]->l[0], leftdomain,if_value);
		CPPS_TO_REAL_VALUE(if_value);

		int32		cpp_bool_value = 1;
		cpp_bool_value = if_value.value.b;
		if (if_value.tt == CPPS_TSTRING)
			cpp_bool_value = 1;
		else if (if_value.tt == CPPS_TNIL)
			cpp_bool_value = 0;
		else if (if_value.tt == CPPS_TCLASSVAR)
			cpp_bool_value = 1;
		else if (if_value.tt == CPPS_TUSERDATA)
			cpp_bool_value = 1;
		
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_exec);
		execdomain->setexecdomain(domain);
		CPPS_SUBTRY
			if (cpp_bool_value)
			{
				if(!d->l[1]->l.empty()) cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d->l[1], false);
			}
			else {
				if (!d->l[2]->l.empty()) cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d->l[2], false);
			}
		CPPS_SUBCATCH2
		
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
		if(!for1->l.empty()) cpps_step_all(c, CPPS_MUNITRET, fordomain, root, for1, false);
		cpps_domain* execdomain = c->domain_alloc();
		while (true && !c->isterminate) {
			cpps_domain* leftdomain = NULL;
			cpps_value	canwhile;
			if (for2->l.size() == 0)
				canwhile = true;
			else
				cpps_calculate_expression(c, fordomain, root, for2->l[0], leftdomain, canwhile);

			CPPS_TO_REAL_VALUE(canwhile);

			bool		b = cpps_converter<bool>::apply(canwhile);
			if (b == false)
				break;
			execdomain->init(fordomain, cpps_domain_type_exec);
			execdomain->setexecdomain(fordomain);
			if (!for4->l.empty())
				cpps_step_all(c, CPPS_MUNITRET, execdomain, root, for4, false);

			bool isbreak = execdomain->isbreak;
			execdomain->destory(c);
			cpps_gc_check_step(c);
			if (isbreak)
				break;
			/* 需要跳出循环 */
			if (!for3->l.empty())
				cpps_step_all(c, CPPS_MUNITRET, fordomain, root, for3, false);
		}
		c->domain_free(execdomain);
		fordomain->destory(c);
		c->domain_free(fordomain);
		cpps_gc_check_step(c);
	}
	void cpps_calculate_expression_quoteoffset(node* d, C* c, cpps_value& ret, cpps_domain* root, cpps_domain*& leftdomain);
	inline void cpps_addandassignment(cpps_value& a, cpps_value b, cpps::cpps_value& _result);
	void cpps_step_foreach(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* foreachdomain = c->domain_alloc();
		foreachdomain->init(domain, cpps_domain_type_foreach);
		foreachdomain->setexecdomain(domain);
		node* for1 = d->l[0];
		node* for2 = d->l[1];
		node* for4 = d->l[2];
		foreachdomain->isbreak = false;
		cpps_domain* leftdomain = NULL;
		cpps_step_all(c, CPPS_MUNITRET,foreachdomain, root, for1, false);
		cpps_regvar* for1_v = foreachdomain->varList.empty() ? NULL : foreachdomain->varList.begin()->second;
		cpps_value v;
		cpps_calculate_expression(c, foreachdomain, root, for2->l[0], leftdomain, v);
		CPPS_TO_REAL_VALUE(v);

		/* 它可能被释放,所以存在某个地方. */
		cpps_regvar* var = CPPSNEW (cpps_regvar);
		var->setval(v);
		var->varName = buildlambda(c) + "foreach-list";
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
					if (!for4->empty()) {

						execdomain->init(foreachdomain, cpps_domain_type_exec);
						execdomain->setexecdomain(foreachdomain);
						cpps_step_all(c, CPPS_MUNITRET, execdomain, root, for4, false);
						bool isbreak = execdomain->isbreak;
						execdomain->destory(c);
						cpps_gc_check_step(c);
						if (isbreak || c->isterminate)
							break;
					}
					/* 需要跳出循环 */
				}
				c->domain_free(execdomain);
			}
		}
		else if (v.isdomain() && v.value.domain->domainname == "set") {
			cpps_set* vec = cpps_converter<cpps_set*>::apply(v);
			if (vec) {
				cpps_domain* execdomain = c->domain_alloc();
				cpps_hash_set& realset = vec->realset();

				cpps_hash_set::iterator	it = realset.begin();
				cpps_hash_set::iterator	itend = realset.end();
				for (; it != itend; ++it) {
					if (for1_v)
						for1_v->setval(*it);
					if (!for4->empty()) {
						execdomain->init(foreachdomain, cpps_domain_type_exec);
						execdomain->setexecdomain(foreachdomain);
						cpps_step_all(c, CPPS_MUNITRET, execdomain, root, for4, false);
						bool isbreak = execdomain->isbreak;
						execdomain->destory(c);
						cpps_gc_check_step(c);

						if (isbreak || c->isterminate)
							break;
					}
					
				}
				c->domain_free(execdomain);
			}
		}
		else if (v.isdomain() && v.value.domain->domainname == "map") {
			cpps_pair* mapnode;
			cpps_value	ret;
			newclass<cpps_pair>(c, &mapnode,&ret);
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
					if (!for4->empty()) {

						execdomain->init(foreachdomain, cpps_domain_type_exec);
						execdomain->setexecdomain(foreachdomain);
						cpps_step_all(c, CPPS_MUNITRET, execdomain, root, for4, false);
						bool isbreak = execdomain->isbreak;
						execdomain->destory(c);
						cpps_gc_check_step(c);
						if (isbreak || c->isterminate)
							break;
					}
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
				//node* for4_node = for4->l.empty() ? NULL: for4->l[0];
				//if (for4_node && for4_node->type == CPPS_OEXPRESSION &&
				//	for4_node->l[0]->symbol &&
				//	for4_node->l[0]->symbol->symboltype == CPPS_SYMBOL_TYPE_ADDANDASSIGNMENT&&
				//	for4_node->l[0]->getleft()&&
				//	(for4_node->l[0]->getleft()->type == CPPS_QUOTEOFFSET || for4_node->l[0]->getleft()->type == CPPS_OOFFSET) &&
				//	for4_node->l[0]->getright()&&
				//	(for4_node->l[0]->getright()->type == CPPS_QUOTEOFFSET || for4_node->l[0]->getright()->type == CPPS_OOFFSET || for4_node->l[0]->getright()->type == CPPS_OINTEGER)
				//	)
				//{
				//	node* left = for4_node->l[0]->getleft();
				//	node* right = for4_node->l[0]->getright();
				//	cpps_value left_value;
				//	cpps_value right_value;
				//	cpps_domain* leftdomain = NULL;
				//	cpps_calculate_expression_quoteoffset(left, c, left_value, root, leftdomain);
				//	cpps_calculate_expression_quoteoffset(right, c, right_value, root, leftdomain);
				//	for (cpps_integer begin = range->begin; begin < range->end; begin += range->inc) {
				//		if (for1_v)
				//			for1_v->getval().value.integer = begin;
				//		left_value.real().value.integer += right_value.real().value.integer;
				//		

				//		/*cpps_value ret;
				//		cpps_addandassignment(left_value, right_value.real(), ret);*/
				//	}
				//}
				//else {
					for (cpps_integer begin = range->begin; begin < range->end; begin += range->inc) {
						if (for1_v)
							for1_v->getval().value.integer = begin;
						execdomain->init(foreachdomain, cpps_domain_type_exec);
						execdomain->setexecdomain(foreachdomain);
						//if(for4_node) cpps_step(c, execdomain, root, for4_node);
						if (!for4->empty()) {
							cpps_step_all(c, CPPS_MUNITRET, execdomain, root, for4, false);
						}
						bool isbreak = execdomain->isbreak;
						execdomain->destory(c);
						cpps_gc_check_step(c);
						if (isbreak)
							break;
							/* 需要跳出循环 */
					}
				//}
				
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
		while (true && !c->isterminate) {
			cpps_domain* leftdomain = NULL;
			cpps_value	canwhile;
			cpps_calculate_expression(c, whiledomain, root, while1->l[0], leftdomain,canwhile);
			CPPS_TO_REAL_VALUE(canwhile);


			bool		b = cpps_converter<bool>::apply(canwhile);
			if (b == false )
				break;
			if (!while2->empty()) {
				execdomain->init(whiledomain, cpps_domain_type_exec);
				execdomain->setexecdomain(whiledomain);
				CPPS_SUBTRY
					cpps_step_all(c, CPPS_MUNITRET, execdomain, root, while2, false);
				CPPS_SUBCATCH2
					bool isbreak = execdomain->isbreak;
				execdomain->destory(c);
				cpps_gc_check_step(c);
				if (isbreak)
					break;
				/* 需要跳出循环 */
			}
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
			cpps_step_all(c, CPPS_MUNITRET, ns, root, d, false);
			return;
		}

		if(v)
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "%s is defined but this is not namespace.", d->s.c_str()));

		ns = CPPSNEW (cpps_domain)(domain, cpps_domain_type_namespace, d->s);
		v = CPPSNEW (cpps_regvar)();
		v->setvarname(d->s);
		auto tmp = cpps_value(ns);
		v->setval(tmp);
		v->setsource(true);

		domain->regvar(c, v);
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			c->_G->regidxvar(d->offset, v);
		}
		cpps_step_all(c, CPPS_MUNITRET, ns, root, d, false);
	}
	void cpps_step_enum(C* c, cpps_domain* domain, cpps_domain* root, node* d) {

		cpps_domain* leftdomain;
		cpps_regvar* v = domain->getvar(d->s, leftdomain, false);
		cpps_domain* ns = NULL;
		if (!v) {
			if (d->s == "_G") {
				ns = domain;
			}
			else {
				ns = CPPSNEW (cpps_domain)(domain, cpps_domain_type_enum, d->s);
				v = CPPSNEW (cpps_regvar)();
				v->setvarname(d->s);
				auto tmp = cpps_value(ns);
				v->setval(tmp);
				v->setsource(true);
				domain->regvar(c, v);
			}
		}
		else {
			if(!v->getval().isdomain() || (v->getval().value.domain->domainType != cpps_domain_type_enum && v->getval().value.domain != c->_G))
				throw(cpps_error(d->filename, d->line, cpps_error_moduleerror, "%s is defined but this is not enum.", d->s.c_str()));

			ns = v->getval().value.domain;
		}

		for (auto n : d->l) {
			leftdomain = NULL;
			auto v2 = ns->getvar(n->s, leftdomain, false);
			if (!v2) {
				v2 = CPPSNEW (cpps_regvar)();
				v2->setvarname(n->s);
				v2->setsource(true);
				ns->regvar(c, v2);
			}
			auto tmp = cpps_value(n->value.integer);
			v2->setval(tmp);
		}
	}
	void cpps_step_module(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain;
		cpps_domain* _Gdomain = c->_G;
		cpps_regvar* v = c->_G->getvar(d->s, leftdomain, false);
		cpps_domain* ns = NULL;

		if (v && (v->getval().isdomain() && v->getval().value.domain->domainType == cpps_domain_type_module))
		{
			ns = v->getval().value.domain;

		/*	if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
				c->_G->regidxvar(d->offset, v);
			}*/

			cpps_step_all(c, CPPS_MUNITRET, ns, root, d, false);
			return;
		}
		if (v) 
			throw(cpps_error(d->filename, d->line, cpps_error_moduleerror, "%s is defined but this is not module.", d->s.c_str()));

		ns = CPPSNEW (cpps_domain)(_Gdomain, cpps_domain_type_module, d->s);
		v = CPPSNEW (cpps_regvar)();
		v->setvarname(d->s);
		auto tmp = cpps_value(ns);
		v->setval(tmp);
		v->setsource(true);
		_Gdomain->regvar(c, v);
	/*	if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			c->_G->regidxvar(d->offset, v);
		}*/
		cpps_step_all(c, CPPS_MUNITRET, ns, root, d, false);
	}
	void cpps_pop_stack_to_here(C* c, cpps_stack* here,bool cleanup) {
		while (!c->getcallstack()->empty()) {
			cpps_stack* takestack2 = c->getcallstack()->back();
			if (here != takestack2) {
				c->stack_free(takestack2);
				c->getcallstack()->pop_back();
			}
			else break;
		}
	}
	void cpps_do_catch(C* c, cpps_domain* parent_domain, cpps_domain* root_domain, cpps_domain* root, node* catchfun, cpps_trycatch_error& e) {
		if (catchfun->l[1]->empty()) return;

		cpps_domain* execdomain2 = c->domain_alloc();
		execdomain2->init(parent_domain, cpps_domain_type_exec);
		execdomain2->setexecdomain(root_domain);
		node* varName = catchfun->l[0];
		cpps_regvar* v = CPPSNEW (cpps_regvar)();
		v->setvarname(varName->s);
		v->setval(cpps_cpp_to_cpps_converter<cpps_trycatch_error*>::apply(c, &e));
		execdomain2->regvar(c, v);
		CPPS_SUBTRY
		cpps_step_all(c, CPPS_MUNITRET, execdomain2, root, catchfun->l[1], false);
		CPPS_SUBCATCH2
		execdomain2->destory(c);
		c->domain_free(execdomain2);
		cpps_gc_check_step(c);
	}

	void printcallstack(std::string& errmsg, C* c)
	{
		char errbuffer[8192];
		sprintf(errbuffer, "Error stack information:\n");
		errmsg.append(errbuffer);
		std::vector<cpps_stack*>* stacklist = c->getcallstack();
		for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)
		{
			cpps::cpps_stack* stack = *it;
			sprintf(errbuffer, "file:%s line:%d func:%s\n", stack->f, stack->l, stack->func);
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
			cpps_step_all(c, CPPS_MUNITRET, execdomain, root, func, false);
			if (execdomain->funcRet.tt != CPPS_TNIL) {
				throwerr = cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "The exception thrown by throw.");
				throwerr._value = execdomain->funcRet;
				hasCatch = true;
			}
		}
		catch (cpps_trycatch_error& e) {
			throwerr = e;

			std::string errmsg;
			printcallstack(errmsg, c);
			throwerr._callstackstr += errmsg;

			cpps_pop_stack_to_here(c, takestack);
			hasCatch = true;

		}
		catch (cpps_error& e) {

			std::string errmsg;
			printcallstack(errmsg, c);


			cpps_pop_stack_to_here(c, takestack);
			/* 清栈 */
			throwerr.attach(e);
			throwerr._callstackstr = errmsg;
			hasCatch = true;
		}
		catch (const char* s) {

			std::string errmsg;
			printcallstack(errmsg, c);

			cpps_pop_stack_to_here(c, takestack);
			/* 清栈 */
			throwerr = cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "Unknown exception thrown by throw.");
			throwerr._value = cpps_value(c,s);
			throwerr._callstackstr = errmsg;
			hasCatch = true;
		}
		catch (...) {
			std::string errmsg;
			printcallstack(errmsg, c);

			throwerr = cpps_trycatch_error(d->filename, d->line, cpps_error_trycatherror, "Unknown exception thrown by throw.");

			cpps_pop_stack_to_here(c, takestack);
			/* 清栈 */
			throwerr._callstackstr = errmsg;
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


				cpps_regvar* v = NULL;
				auto it = cppsclass->varList.find(it2->first);
				if (it != cppsclass->varList.end()) {
					v = it->second;
				}
				else {
					v = CPPSNEW (cpps_regvar)();
				}

				v->clone(it2->second);

				cppsclass->hasVar = true;
				cppsclass->varList.insert(phmap::flat_hash_map<std::string, cpps_regvar*>::value_type(it2->first, v));
			}
		}
		/*注册父类的operator.*/
		for (auto v : parentclass->operatorlist) {
			cppsclass->operatorlist[v.first] = v.second;
		}
		cppsclass->setidxoffset(parentclass, takeoff);
		for (auto grandfather : parentclass->parentclasslist()) {
			cpps_step_class_reg_baseclass_idx_offset(cppsclass, grandfather, takeoff);
			takeoff += grandfather->o->varsize;
		}
	}
	void cpps_step_class(C* c, cpps_domain* domain, node* d) {
		cpps_cppsclass* cppsclass = CPPSNEW (cpps_cppsclass)(d->s, d, domain, cpps_domain_type_class);
		cppsclass->setDefaultCState(c);
		cpps_regvar* v = CPPSNEW (cpps_regvar)();
		v->setvarname(d->s);
		v->setconst(true);
		auto tmp = cpps_value(cppsclass);
		v->setval(tmp);
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
			cppsclass->parentclasslist().emplace_back(parentclass);
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
						cpps_regvar* v2 = NULL;
						auto it2 = cppsclass->varList.find(varName->s);
						if (it2 != cppsclass->varList.end()) {
							v2 = it2->second;
						}
						else {
							v2 = CPPSNEW (cpps_regvar)();
							v2->setvarname(varName->s);
							cppsclass->regvar(c, v2);
						}
						if (v2->issource()) {
							printf("%s",(d->s + "::" + varName->s + "is repeatedly defined , The second has been ignored\r\n").c_str());
						}
						else {
							/* 重写父类的函数 */
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
							cpps_domain* funcdomain = CPPSNEW(cpps_domain)(cppsclass, cpps_domain_type_func, d->s + "::" + varName->s);
							cpps_cppsfunction* func = CPPSNEW(cpps_cppsfunction)(funcdomain, var->l[0], var->l[1], var->size);
							func->setclass(cppsclass);
							func->setIsNeedC(false);
							if (o->type == CPPS_ODEFASYNCVAR) {
								func->setasync(true);
							}
							func->setquatoreturn(varName->quote);
							func->setmulitparams(var->l[0]->quote);
							/*注册operator.*/
							if (varName->symbol) {
								cppsclass->operatorreg(varName->symbol->symbolfuncname, func);
							}

							auto tmp = cpps_value(func);
							v2->setval(tmp);
						}
						cpps_destory_node(o);
						o->release();
						o = NULL;
						it = vars->l.erase(it);
					}
					else {
						++it;
					}
				}
				else {
					++it;
				}
			}
			else {
				cpps_destory_node(o);
				o->release();
				o = NULL;
				it = vars->l.erase(it);
			}
		}
	}
	void cpps_step_import(C* c, cpps_domain* domain, cpps_domain* root, node* o) {
		//cpps_domain* leftdomain = NULL;
		//if (o->l.empty())
		//	return;
		//cpps_value		list;
		//cpps_calculate_expression(c, domain, root, o->l[0], leftdomain,list);
		//CPPS_TO_REAL_VALUE(list);

		//cpps_cppsclass* cppsclass = (cpps_cppsclass*)list.value.domain->parent[0];
		//cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)list.value.domain;
		//if (list.tt == CPPS_TSTRING) {
		//	cpps_loadlibrary(c, cpps_to_string(list));
		//}
		//else if (cppsclass->getclassname() == "vector") {
		//	cpps_vector* vec = (cpps_vector*)cppsclassvar->getclsptr();
		//	for (auto& v : vec->realvector()) {
		//		if (v.tt == CPPS_TSTRING) {
		//			cpps_loadlibrary(c, cpps_to_string(v));
		//		}
		//	}
		//}
	}

	void cpps_step_dofile(C* c, cpps_domain* domain, cpps_domain* root, node* o) {
		for (std::vector<node*>::iterator it = o->l.begin(); it != o->l.end(); ++it) {
			cpps_domain* leftdomain = NULL;
			cpps_value	path;
			cpps_calculate_expression(c, domain, root, *it, leftdomain,path);
			CPPS_TO_REAL_VALUE(path);

			std::string	fpath = cpps_to_string(path);
			std::string fileSrc;
			cpps_load_filebuffer(fpath.c_str(), fileSrc);
			node* o2 = loadbuffer(c, c->_G, fileSrc, fpath);
			cpps_stack* stack = c->stack_alloc();
			stack->init((*it)->filename.c_str(), (*it)->line, "dofile");
			c->push_stack(stack);
			CPPS_SUBTRY
			cpps_step_all(c, CPPS_MUNITRET, c->_G, c->_G, o2, false);
			CPPS_SUBCATCH
			c->pop_stack();
			cpps_gc_check_step(c);
			c->stack_free(stack);
			if (o2) { cpps_destory_node(o2); CPPSDELETE(o2); o2 = NULL; }
		}
	}
	void cpps_step_dostring(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain = NULL;
		cpps_value	path;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,path);
		CPPS_TO_REAL_VALUE(path);

		std::string* str = cpps_get_string(path);
		if (!str)
			return;
		c->buildoffset = false;
		node* o = loadbuffer(c, root, *str, d->filename);
		c->buildoffset = true;
		cpps_stack* stack = c->stack_alloc();
		stack->init(d->filename.c_str(), d->line, "dostring");
		c->push_stack(stack);
		CPPS_SUBTRY
		cpps_step_all(c, CPPS_MUNITRET, domain, root, o, false);
		CPPS_SUBCATCH
		c->pop_stack();
		c->stack_free(stack);
		cpps_gc_check_step(c);

		if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }
	}
	void cpps_step_assert(C* c, cpps_domain* domain, cpps_domain* root, node* d) {

		cpps_domain* leftdomain = NULL;
		object	path;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain, path.getval());
		bool n = object_cast<bool>(path);
		if (!n) {
			printf("CPPS ASSERT: %s , file:%s line:%d\r\n", d->s.c_str(), d->filename.c_str(), d->line);
			if(c->debug)
				abort();
		}
	}
	void cpps_step_donode(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_domain* leftdomain = NULL;
		object	path;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,path.getval());
		node* n = object_cast<node*>(path);
		if (!n) return;
	
		cpps_stack* stack = c->stack_alloc();
		stack->init(d->filename.c_str(), d->line, "donode");
		c->push_stack(stack);
		CPPS_SUBTRY
		cpps_step_all(c, CPPS_MUNITRET, domain, root, n, false);
		CPPS_SUBCATCH2
		c->pop_stack();
		c->stack_free(stack);
		cpps_gc_check_step(c);

	}
	void cpps_step_newclassvar_reg_baselassvar(cpps_cppsclass* cppsclass, C* c, cpps_cppsclassvar* cppsclassvar, cpps_domain* root) {
		for (auto parentclass : cppsclass->parentclasslist()) {
			cpps_step_newclassvar_reg_baselassvar(parentclass, c, cppsclassvar, root);
			if (parentclass->o)
				cpps_step_all(c, CPPS_MUNITRET, cppsclassvar, root, parentclass->o->getright(), false);
		}
	}
	void cpps_calculate_expression_newvar(cpps_domain* domain, node* d, C* c, cpps_domain* root, cpps_domain*& leftdomain, cpps_value& ret) {
		
		cpps_regvar* v = domain->getvar(d->s, leftdomain);
		if (!v)
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "[%s] not found or not defined", d->s.c_str()));
		/*看看有没有使用名空间 */
		node* lastNamespace = d;
		while (lastNamespace && lastNamespace->getleft() && lastNamespace->getleft()->type == CPPS_ONAMESPANCE_CHILD) {
			v = v->getval().value.domain->getvar(lastNamespace->getleft()->s, leftdomain,false);
			if (!v)
				throw(cpps_error(lastNamespace->getleft()->filename, lastNamespace->getleft()->line, cpps_error_normalerror, "[%s] not found or not defined", lastNamespace->getleft()->s.c_str()));
			lastNamespace = lastNamespace->getleft();
		}
		if (v && v->getval().real().tt == CPPS_TCLASS) {
			cpps_cppsclass* cppsclass = (cpps_cppsclass*)v->getval().real().value.domain;
			cpps_cppsclassvar* cppsclassvar = cppsclass->create(c);
			/* 将类对象里面的变量创建出来 */
			cpps_step_newclassvar_reg_baselassvar(cppsclass, c, cppsclassvar, root);
			if (cppsclass->o)
				cpps_step_all(c, CPPS_MUNITRET, cppsclassvar, root, cppsclass->o->getright(), false);
			/* 将新创建出来的添加到新生区稍后检测要不要干掉 */
			cpps_gc_add_gen0(c, cppsclassvar);
			ret = cpps_value(cppsclassvar);
			cpps_domain* takedomain2 = leftdomain;
			leftdomain = cppsclassvar;

			

			/* 数组特殊处理. */
			if (d->s == "vector" && d->getleft() && d->getleft()->type == CPPS_OVECTORSIZE) {
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

							
						
							cpps_domain* takedomain3 = NULL;
							cpps_value v2;
							cpps_calculate_expression(c, domain, root, rr->l[i]->getright()->getleft(), takedomain3,v2);
							CPPS_TO_REAL_VALUE(v2);

						
							cpps_regvar* var = cppsclassvar->getvar(k->s, leftdomain);
							if (var)
								var->setval(v2);
						

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
					if (var && var->getval().real().tt == CPPS_TFUNCTION) {
						/*cpps_function* f = var->getValue().value.func;*/
						cpps_domain* execdomain = c->domain_alloc();
						execdomain->init(domain, cpps_domain_type_func);
						execdomain->setexecdomain(domain);
						cpps_step_callfunction(c, execdomain, root, var->getval().real(), d, leftdomain);
						execdomain->destory(c);
						c->domain_free(execdomain);
					}
				}
			}
			leftdomain = takedomain2;
		}
		else {
			throw(cpps_error(d->filename, d->line, cpps_error_normalerror, "The object out of new [%s] must be a class object type is : %d", d->s.c_str(), v->getval().tt));
		}
	}

	void cpps_step_assemble(C* c, cpps_domain* domain, cpps_domain* root, node* d)
	{
		if (d->empty()) return;
		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_exec);
		execdomain->setexecdomain(domain);
		CPPS_SUBTRY
		cpps_step_all(c, CPPS_MUNITRET, execdomain, root, d, false);
		CPPS_SUBCATCH2
		execdomain->destory(c);
		c->domain_free(execdomain);
		cpps_gc_check_step(c);
	}

	void cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		/*记录当前执行的node*/
		c->curnode = d;

		if (d->type == CPPS_OEXPRESSION) {
			cpps_value ret; cpps_domain* leftdomain = NULL;	cpps_calculate_expression(c, domain, root, d->l[0], leftdomain, ret);
		}
		else if (d->type == CPPS_ODEFVAR) {
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
		else if (d->type == CPPS_ORETURN) {
			cpps_step_return(c, domain, root, d);
		}
		else if (d->type == CPPS_OTHROW) {
			cpps_step_throw(c, domain, root, d);
		}
		else if (d->type == CPPS_OECHO){
			cpps_step_echo(c, domain, root, d);
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
		else if (d->type == CPPS_OENUM) {
			cpps_step_enum(c, domain, root, d);
		}
		else if (d->type == CPPS_OTRYCATCH) {
			cpps_step_trycath(c, domain, root, d);
		}
		else if (d->type == CPPS_ODOFILE) {
			cpps_step_dofile(c, domain, root, d);
		}
		else if (d->type == CPPS_OINCLUDE) {
			/*do nothing...*/
		}
		else if (d->type == CPPS_OIMPORT) {
			//cpps_step_import(c, domain, root, d);
			/*do nothing...*/
		}
		else if (d->type == CPPS_ODOSTRING) {
			cpps_step_dostring(c, domain, root, d);
		}
		else if (d->type == CPPS_ODONODE) {
			cpps_step_donode(c, domain, root, d);
		}
		else if (d->type == CPPS_OASSERT) {
			cpps_step_assert(c, domain, root, d);
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
	int32 cpps_str2ui64(const char* s, cpps_uinteger* result) {
		cpps_number n = 0;
		cpps_str2d(s, &n);
		*result = cpps_number2uinteger(n);
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
			cpps_value	value;
			cpps_calculate_expression(c, domain, root, d->l[i], leftdomain,value);
			params.emplace_back(value);
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
	void cpps_calculate_expression_quote_real(cpps_value& src, cpps_value& tar, bool isconst)
	{
		if (src.tt == CPPS_TREF) {
			//cpps_value tmp = *src.value.value;
			//tar = cpps_value(isconst ? (tmp) : (&*src.value.value));
			tar = isconst ? *src.value.value : cpps_value(&*src.value.value);
		}
		else {
			//cpps_value tmp = src;
			//tar = cpps_value(isconst ? (tmp) : (&src));
			tar = isconst ? src : cpps_value(&src);
		}
	}

	cpps_domain* cpps_calculate_expression_offset_getclassvar(cpps_domain* root)
	{
		if (root == NULL) return NULL;

		if (root->parent[1] && root->parent[1]->domainType == cpps_domain_type_classvar) return root;

		return cpps_calculate_expression_offset_getclassvar(root->parent[1]);
	}
	
	void cpps_calculate_expression_quoteoffset(node* d, C* c, cpps_value& ret, cpps_domain* root, cpps_domain*& leftdomain) {
		/* 快速读取 */
		if (d->offsettype == CPPS_OFFSET_TYPE_GLOBAL) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {
				cpps_calculate_expression_quote_real(var->getval(), ret, var->isconst());
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_SELF) {
			cpps_regvar* var = root->getregidxvar(d->offset);
			if (var) {
				cpps_calculate_expression_quote_real(var->getval(), ret, var->isconst());
			}
		}
		else if (d->offsettype == CPPS_OFFSET_TYPE_LEFTCLASS) {
			cpps_regvar* var = c->_G->getregidxvar(d->offset);
			if (var) {

				cpps_calculate_expression_quote_real(var->getval(), ret, var->isconst());

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
				cpps_calculate_expression_quote_real(var->getval(), ret, var->isconst());
			}
		}
	}
	void cpps_calculate_expression_dofunction(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value var;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain, var);
		CPPS_TO_REAL_VALUE(var);
		cpps_value left;
		if (leftdomain && leftdomain->domainType == cpps_domain_type_classvar) {
			cpps_cppsclassvar* clsvar = (cpps_cppsclassvar*)leftdomain;
			left = clsvar;
		}

		cpps_domain* execdomain = c->domain_alloc();
		execdomain->init(domain, cpps_domain_type_func);
		execdomain->setexecdomain(domain);
		/* cpps_step_for(c, execdomain, d); */
		ret = cpps_step_callfunction(c, execdomain, root, var, d, leftdomain);
		execdomain->destory(c);
		c->domain_free(execdomain);
	}
	void cpps_calculate_expression_ostr(cpps_value& ret, node* d, cpps_domain*& leftdomain, C* c, cpps_domain* domain, cpps_domain* root) {
		if (d->l.size() > 1) {
			std::string str;
			for (size_t i = 0; i < d->l.size(); i++) {
				node* s = d->l[i];
				if (s->type == CPPS_OSTR) {
					str.append(s->s);
				}
				else {
					cpps_domain* takedomian = leftdomain;
					leftdomain = NULL;
					cpps_value v;
					cpps_calculate_expression(c, domain, root, s->l[0], leftdomain,v);
					CPPS_TO_REAL_VALUE(v);
					leftdomain = takedomian;
					str.append(v.tt == CPPS_TSTRING ? *cpps_get_string(v) : cpps_to_string(v));
				}
			}
			std::string* retstr = NULL;
			newclass<std::string>(c, &retstr,&ret);
			retstr->append(str);
		}
		else {
			ret = cpps_value(d->value.val); //临时变量
		}
	}
	void cpps_calculate_expression_array(cpps_value& ret, C* c, node* d, cpps_domain*& leftdomain, cpps_domain* domain, cpps_domain* root) {
		cpps_vector* vec = NULL;
		newclass<cpps_vector>(c, &vec,&ret);
		vec->realvector().reserve(d->l.size());
		for (size_t i = 0; i < d->l.size(); i++) {
			cpps_domain* takedomain = leftdomain;
			leftdomain = NULL;
			cpps_value v;
			cpps_calculate_expression(c, domain, root, d->l[i]->getleft(), leftdomain,v);
			CPPS_TO_REAL_VALUE(v);
			vec->realvector().emplace_back(v);
			leftdomain = takedomain;
		}
	}
	void cpps_calculate_expression_object(cpps_value& ret, C* c, node* d, cpps_domain*& leftdomain, cpps_domain* domain, cpps_domain* root) {
		cpps_map* m = NULL;
		newclass<cpps_map>(c, &m, &ret);
		
		for (size_t i = 0; i < d->l.size(); i++) {
			node* k = d->l[i]->getleft();
			cpps_domain* takedomain = leftdomain;
			leftdomain = NULL;
			cpps_value v;
			cpps_calculate_expression(c, domain, root, d->l[i]->getright()->getleft(), leftdomain,v);
			CPPS_TO_REAL_VALUE(v);
			m->insert(cpps_value(c, k->s), v);
			leftdomain = takedomain;
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
			newclass< cpps_lambda_function >(c, &func,&ret);
			func->c = c;
			func->setrealfunc((cpps_cppsfunction*)v->getval().value.func);
			if (root->parent[0] && root->parent[0]->domainType == cpps_domain_type_func && root->stacklist) {
				size_t c2 = d->size;
				if (c2 > 0) {
					func->stacklist = CPPSNEW (std::vector<cpps_regvar*>)();

					/*check*/
					assert(c2 <= root->stacklist->size());
					for (size_t i = 0; i < c2; i++) {
						auto no = (*(root->stacklist))[i];
						if (no && no->closeure) {
							func->stacklist->emplace_back(no);
							no->closeureusecount++;/*计数*/
						}
						else {
							func->stacklist->push_back(NULL);/*占位*/
						}
					}
				}
			}
			ret.tt = CPPS_TLAMBDAFUNCTION;
		}
	}
	void cpps_calculate_expression_slice(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret)
	{
		cpps_value left;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,left);
		CPPS_TO_REAL_VALUE(left);
		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR || left.tt == CPPS_TSTRING  ) {
				object leftobject = object(left);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(left);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				cpps_function*func = cppsclass->getoperator("[]");
				object symbolfunc = func ?cpps_value(func) : nil;
				if (symbolfunc.isfunction()) {
					
					/*tuple*/
					cpps_vector* vct = NULL;
					cpps_value tuple_value;
					newclass(c, &vct,&tuple_value);
					tuple_value.tt = CPPS_TTUPLE; /*设置成元组.*/

					for (auto nn : d->getright()->l) {
						node* first_node = nn->l[0];
						node* second_node = nn->l[1];
						node* third_node = nn->l[2];

						object first;
						object second;
						object third;

						if (first_node->getleft() && first_node->getleft()->type != CPPS_TNIL)
							cpps_calculate_expression(c, domain, root, first_node->getleft(), leftdomain, first.getval());
						if (second_node->getleft() && second_node->getleft()->type != CPPS_TNIL)
							cpps_calculate_expression(c, domain, root, second_node->getleft(), leftdomain, second.getval());
						if (third_node->getleft() && third_node->getleft()->type != CPPS_TNIL)
							cpps_calculate_expression(c, domain, root, third_node->getleft(), leftdomain, third.getval());


						cpps_integer start_index = 0;
						cpps_integer end_index = MAXINT64;
						cpps_integer step_index = 1;

						if (first.isint()) {
							start_index = first.toint();
							if (start_index < 0) {
								step_index = -1;
							}
						}
						if (second.isint())
							end_index = second.toint();
						if (third.isint())
							step_index = third.toint();


						/*step 为0 非法.*/
						if (step_index == 0) {
							return;
						}

						/*如果它不是空也不是整数说明它需要独立存在*/
						if (!first.isint() && !first.isnull())
						{
							vct->realvector().emplace_back(first.value);
						}
						else {
							cpps_range* range = NULL;
							object range_object = object::create_with_classvar(c, &range);
							range->begin = start_index;
							range->end = end_index;
							range->inc = step_index;

							vct->realvector().emplace_back(range_object.value);
						}
					}
					

					cpps_value src = doclassfunction(c, leftobject, symbolfunc, tuple_value).value;
					cpps_calculate_expression_quote_real(src, ret, false);
					
				}
				else
				{
					/*cpps 暂时不支持多为数组.*/
					node* first_node = d->getright()->getleft()->l[0];
					node* second_node = d->getright()->getleft()->l[1];
					node* third_node = d->getright()->getleft()->l[2];

					object first;
					object second;
					object third;

					if (first_node->getleft() && first_node->getleft()->type != CPPS_TNIL)
						cpps_calculate_expression(c, domain, root, first_node->getleft(), leftdomain, first.getval());
					if (second_node->getleft() && second_node->getleft()->type != CPPS_TNIL)
						cpps_calculate_expression(c, domain, root, second_node->getleft(), leftdomain, second.getval());
					if (third_node->getleft() && third_node->getleft()->type != CPPS_TNIL)
						cpps_calculate_expression(c, domain, root, third_node->getleft(), leftdomain, third.getval());

					cpps_integer start_index = 0;
					cpps_integer end_index = MAXINT64;
					cpps_integer step_index = 1;

					if (first.isint()) {
						start_index = first.toint();
						if (start_index < 0) {
							step_index = -1;
						}
					}
					if (second.isint())
						end_index = second.toint();
					if (third.isint())
						step_index = third.toint();



					/*step 为0 非法.*/
					if (step_index == 0) {
						return;
					}



					if (leftobject.isvector() || leftobject.istuple()) {
						cpps_vector* vct = cpps_to_cpps_vector(left);

						object::vector ret_vec = object::vector::create(c);
						ret = ret_vec.toobject().value;

						if (end_index == MAXINT64) {
							if (start_index < 0)
								end_index = 0;
							else
								end_index = vct->size();
						}

						/*计算真实值.*/
						if (start_index < 0) {
							start_index = vct->size() + start_index;
						}

						if (end_index < 0) {
							end_index = vct->size() + end_index + 1;
						}

						if (start_index >= vct->size()) return;


						/*???? a[1:1]??*/
						if (start_index == end_index) {
							ret_vec.realvector().emplace_back(vct->at(start_index));
							return;
						}

						/*越走越远说明有问题.*/
						auto count = abs(end_index - start_index);
						if (abs(end_index - (start_index + step_index)) > count) {
							return;
						}

						if (start_index < end_index) {

							for (cpps_integer i = (cpps_integer)start_index; i < (cpps_integer)end_index; i += step_index) {
								ret_vec.realvector().emplace_back(vct->at(i));
							}
						}
						else {

							for (cpps_integer i = (cpps_integer)start_index; i > (cpps_integer)end_index; i += step_index) {
								ret_vec.realvector().emplace_back(vct->at(i));
							}
						}

					
					}
					else if (leftobject.isstring())
					{
						std::string* str = cpps_get_string(left);
						std::string ret_str;

						if (end_index == MAXINT64) {
							if (start_index < 0)
								end_index = 0;
							else
								end_index = str->size();
						}

						/*计算真实值.*/
						if (start_index < 0) {
							start_index = str->size() + start_index;
						}

						if (end_index < 0) {
							end_index = str->size() + end_index + 1;
						}

						if (start_index >= (cpps_integer)str->size()) {
							ret = cpps_value(c, ret_str); 
							return;
						}

						/*???? a[1:1]??*/
						if (start_index == end_index) {
							ret_str.push_back(str->at(size_t(start_index)));
							ret = cpps_value(c, ret_str);
							return;
						}

						/*越走越远说明有问题.*/
						auto count = abs(end_index - start_index);
						if (abs(end_index - (start_index + step_index)) > count) {
							return;
						}

						if (start_index < end_index) {

							for (cpps_integer i = (cpps_integer)start_index; i < (cpps_integer)end_index; i += step_index) {
								ret_str.push_back(str->at(size_t(i)));
							}
						}
						else {

							for (cpps_integer i = (cpps_integer)start_index; i > (cpps_integer)end_index; i += step_index) {
								ret_str.push_back(str->at(size_t(i)));
							}
						}

						ret = cpps_value(c, ret_str);
					}
				 }
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a vector before the 'slice[::]'", d->getleft()->s.c_str()));
		}
	}
	void cpps_calculate_expression_getchild(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		cpps_value left;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,left);
		CPPS_TO_REAL_VALUE(left);

		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR || left.tt == CPPS_TTUPLE) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0];
				if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "vector") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_vector* pVec = (cpps_vector*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right;
					cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain,right);
					CPPS_TO_REAL_VALUE(right);

					if (!cpps_isint(right )) {
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array must contain a number as an index."));
					}
					leftdomain = takedomain;
					cpps_integer idx = cpps_to_integer(right);
					if (pVec->size() <= idx)
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array has crossed the current length: [%d]. You need to get the length: [%d]..", pVec->size(), idx));
					if (idx < 0) {
						idx = pVec->size() + idx;
					}
					cpps_calculate_expression_quote_real(pVec->cpps_at(idx), ret, false);
				}
				else if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "map") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_map* pMap = (cpps_map*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right;
					cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain,right);
					CPPS_TO_REAL_VALUE(right);

					leftdomain = takedomain;
					cpps_calculate_expression_quote_real(pMap->cpps_find(right), ret, false);
				}
				else {
					object leftobject = object(left);
					cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(left);
					cpps_cppsclass* cppsclass2 = cppsclassvar->getcppsclass();
					cpps_function* func = cppsclass2->getoperator("[]");
					object symbolfunc = func ? cpps_value(func) : nil;
					if (symbolfunc.isfunction())
					{
						object param;
						cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain, param.getval());
						cpps_value src = doclassfunction(c, leftobject, symbolfunc, param).value;
						if (src.isref())
							cpps_calculate_expression_quote_real(src, ret, false);
						else
							ret = src;
					}
				}
			}
			else if (left.tt == CPPS_TSTRING)
			{
				std::string* str = cpps_get_string(left);
				cpps_domain* takedomain = leftdomain;
				leftdomain = NULL;
				cpps_value right;
				cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain,right);
				CPPS_TO_REAL_VALUE(right);

				if (!cpps_isint(right)) {
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String must contain a number as an index."));
				}
				cpps_integer idx = cpps_to_integer(right);

				leftdomain = takedomain;
				if ((cpps_integer)str->size() <= idx)
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String has crossed the current length: [%d]. You need to get the length: [%d]..", str->size(), idx));
				if (idx < 0) {
					idx = str->size() + idx;
				}

				ret = (cpps_integer)str->at(size_t(idx));
			}
			
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}
	

	void cpps_calculate_expression_quotevarname(C* c, cpps_domain*& leftdomain, cpps_domain* domain, node* d, cpps_value& ret) {


		cpps_regvar* v = NULL;
		if (leftdomain)
			v = leftdomain->getvar(d->s, leftdomain, true, true);
		else
			v = domain->getvar(d->s, leftdomain);

		if (v) {
			cpps_calculate_expression_quote_real(v->getval(),ret,false);
		}
		else {
			if (!c->disabled_non_def_var) {
				printf("Warning:  got a not existent variable of [%s].  line: %d file: %s\n", d->s.c_str(), d->line, d->filename.c_str());
			}
			ret = nil;
		}
	}
	void cpps_calculate_expression_quotegetchild(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {
		
		cpps_value left;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,left);
		CPPS_TO_REAL_VALUE(left);

		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR || left.tt == CPPS_TTUPLE) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain->parent[0];
				if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "vector") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_vector* pVec = (cpps_vector*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right;
					cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain, right);
					CPPS_TO_REAL_VALUE(right);

					if (!cpps_isint(right)) {
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array must contain a number as an index."));
					}
					cpps_integer idx = cpps_to_integer(right);
					leftdomain = takedomain;
					if (pVec->size() <= idx)
						throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "Array has crossed the current length: [%d]. You need to get the length: [%d]..", pVec->size(), idx));
					if (idx < 0) {
						idx = pVec->size() + idx;
					}
					cpps_calculate_expression_quote_real(pVec->cpps_at(idx), ret, false);
				}
				else if (d->getright()->type != CPPS_FUNCNAME && cppsclass->getclassname() == "map") {
					cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)left.value.domain;
					cpps_map* pMap = (cpps_map*)cppsclassvar->getclsptr();
					cpps_domain* takedomain = leftdomain;
					leftdomain = NULL;
					cpps_value right;
					cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain,right);
					CPPS_TO_REAL_VALUE(right);

					leftdomain = takedomain;
					cpps_calculate_expression_quote_real(pMap->cpps_find(right), ret, false);
				}
				else {
					object leftobject = object(left);
					cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(left);
					cpps_cppsclass* cppsclass2 = cppsclassvar->getcppsclass();
					cpps_function* func = cppsclass2->getoperator("[]");
					object symbolfunc = func ? cpps_value(func) : nil;
					if (symbolfunc.isfunction())
					{
						object param;
						cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain, param.getval());
						cpps_value src = doclassfunction(c, leftobject, symbolfunc, param).value;
						if (src.isref())
							cpps_calculate_expression_quote_real(src, ret, false);
						else
							ret = src;
					}
				}
			}
			else if (left.tt == CPPS_TSTRING)
			{
				std::string* str = cpps_get_string(left);
				cpps_domain* takedomain = leftdomain;
				leftdomain = NULL;
				cpps_value right;
				cpps_calculate_expression(c, domain, root, d->getright()->getleft()->getleft()->getleft(), leftdomain,right);
				CPPS_TO_REAL_VALUE(right);

				if (!cpps_isint(right)) {
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String must contain a number as an index."));
				}
				cpps_integer idx = cpps_to_integer(right);
				leftdomain = takedomain;
				if ((cpps_integer)str->size() <= idx)
					throw(cpps_error(d->getright()->filename, d->getright()->line, cpps_error_classerror, "String has crossed the current length: [%d]. You need to get the length: [%d]..", str->size(), idx));

				if (idx < 0) {
					idx = str->size() + idx;
				}

				ret = cpps_value(str->at(size_t(idx)));
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}
	void cpps_calculate_expression_quotegetobject(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret) {


		cpps_value left;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,left);
		CPPS_TO_REAL_VALUE(left);

		if (left.tt != CPPS_TNIL) {
			if (left.tt == CPPS_TCLASSVAR || left.tt == CPPS_TSTRING || left.tt == CPPS_TTUPLE) {
				if (d->quote)
				{
					object leftobject = object(left);
					cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(left);
					cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
					if (cppsclass->classname == "map") {
						cpps_map* vmap = cpps_converter<cpps_map*>::apply(left);

						cpps_calculate_expression_quote_real(vmap->cpps_find(cpps_value(c, d->getright()->s)), ret, false);
						//ret = vmap->cpps_find(cpps_value(c,d->getright()->s));
						return;
					}
					else {
						cpps_function* func = cppsclass->getoperator("->");
						object symbolfunc = func ? cpps_value(func) : nil;
						if (symbolfunc.isfunction())
						{
							cpps_value src = doclassfunction(c, leftobject, symbolfunc).value;
							if (src.isref())
								left = src.real();
							else
								left = src;

							if (!cpps_isclassvar(left))
								throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "-> returnd not a classvar."));
						}
						else {
							/*没找到这个函数?*/
							throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "-> not found the operator function."));

						}
					}
				}
				cpps_domain* execdomain = c->domain_alloc();
				execdomain->init(c->_G, cpps_domain_type_exec);
				leftdomain = left.value.domain;
				execdomain->setexecdomain(left.value.domain);
				cpps_value src;
				cpps_calculate_expression(c, execdomain, root, d->getright(), leftdomain,src);
				if(src.tt != CPPS_TNIL)
					cpps_calculate_expression_quote_real(src, ret, false);
				execdomain->destory(c);
				c->domain_free(execdomain);
			}
			else if (left.tt == CPPS_TCLASS && root->parent[1] && root->parent[1]->domainType == cpps_domain_type_classvar) {
				cpps_cppsclass* cppsclass = (cpps_cppsclass*)left.value.domain;
				cpps_domain* takedomain = NULL;
				/* check this class is parent class.. */
				cpps_cppsclass* selfclass = (cpps_cppsclass*)root->parent[1]->parent[0];
				bool is_parent_class = cpps_check_parent_class(selfclass, cppsclass);
				if (!is_parent_class)
					throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "Non parent function executed: [%s::%s].", d->getleft()->s.c_str(), d->getright()->s.c_str()));

				std::string funcname = d->getright()->s;
				if (d->getright()->s == cppsclass->getclassname()) {
					funcname = "constructor";
				}

				cpps_regvar* var = cppsclass->getvar(funcname, takedomain, false);
				if (var && var->getval().tt == CPPS_TFUNCTION) {
					ret = var->getval();
					leftdomain = root->parent[1];
				}
			}
			else if (left.tt == CPPS_TDOMAIN) {
				leftdomain = left.value.domain;
				cpps_value src;
				cpps_calculate_expression(c, left.value.domain, root, d->getright(), leftdomain,src);
				cpps_calculate_expression_quote_real(src, ret, false);
			}
		}
		else {
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "[%s] must be a class object or a domain before the '.'", d->getleft()->s.c_str()));
		}
	}

	void cpps_calculate_expression_await(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain, cpps_value& ret)
	{
		cpps_value var;
		cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain,var);
		CPPS_TO_REAL_VALUE(var);

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

	void cpps_calculate_expression_this(node *d,cpps_domain* root, cpps_value& ret)
	{
		cpps_domain* _classvarroot = cpps_calculate_expression_offset_getclassvar(root);
		if (!_classvarroot)
			throw(cpps_error(d->filename, d->getleft()->line, cpps_error_classerror, "this need use in class function."));
		if (_classvarroot->parent[1] && _classvarroot->parent[1]->domainType == cpps_domain_type_classvar)
			ret = cpps_value((cpps_cppsclassvar*)_classvarroot->parent[1]);
	}
	void cpps_calculate_expression_global(C*c,node *d,cpps_domain* root, cpps_value& ret)
	{
		ret = cpps::object::globals(c).getval();
	}

	void cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_domain*& leftdomain,cpps_value &ret) {
		if (d->type == CPPS_OOFFSET) {
			cpps_calculate_expression_quoteoffset(d, c, ret, root, leftdomain);
		}
		else if (d->type == CPPS_FUNCNAME) {
			cpps_symbol_handle(c, domain, root, d, ret);
		}
		else if (d->type == CPPS_QUOTEOFFSET) {
			cpps_calculate_expression_quoteoffset(d, c, ret, root, leftdomain);
		}
		else if (d->type == CPPS_VARNAME) {
			cpps_calculate_expression_quotevarname(c, leftdomain, domain, d, ret);
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
		else if (d->type == CPPS_OUINTEGER) {
			ret.tt = CPPS_TUINTEGER;
			ret.value.uinteger = d->value.uinteger;
		}
		else if (d->type == CPPS_OINTEGER16) {
			ret.tt = CPPS_TINTEGER;
			ret.value.integer = d->value.integer;
		}
		else if (d->type == CPPS_ONEWVAR) {
			cpps_calculate_expression_newvar(domain, d, c, root, leftdomain, ret);
		}
		else if (d->type == CPPS_OBRACKET) {
			cpps_domain* leftdomain2 = NULL;
			/* 获取第一个节点 */
			if (d->getleft()) cpps_calculate_expression(c, domain, root, d->getleft(), leftdomain2, ret);

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
			cpps_calculate_expression_this(d,root, ret);
		}
		else if (d->type == CPPS_OGLOBAL)
		{
			cpps_calculate_expression_global(c,d,root, ret);
		}
		else if (d->type == CPPS_OAWAIT)
		{
			cpps_calculate_expression_await(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_VARNAME_LAMBDA) {
			cpps_calculate_expression_lambda(c,domain,root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_OELLIPSIS){
			ret.tt = CPPS_TELLIPSIS;
		}
		else if (d->type == CPPS_OGETCHIILD) {
			cpps_calculate_expression_getchild(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_OSLICE) {
			cpps_calculate_expression_slice(c, domain, root, d, leftdomain, ret);
		}
		else if (d->type == CPPS_OGETOBJECT) {
			cpps_calculate_expression_quotegetobject(c, domain, root, d, leftdomain, ret);
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
			node* o = loadbuffer(c,domain, str, "");
			cpps_stack* stack = c->stack_alloc();
			stack->init(d->filename.c_str(), d->line, "dostring");
			c->push_stack(stack);
			CPPS_SUBTRY
			cpps_step_all(c, CPPS_MUNITRET, domain, root, o, false);
			CPPS_SUBCATCH
			c->pop_stack();
			c->stack_free(stack);
			if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }

		}
	}
	void cpps_debug_trace_domain(C*c,int kg,std::string parent,cpps_domain* root) {
		for (auto var : root->varList)
		{
			cpps_value& value = var.second->getval();
			if (!cpps_isfunction(value)) {
				for (int i = 0; i < kg; i++)
					printf(" ");

				cpps::cpps_console_color(2);
				printf(parent.empty() ? "%s%s": "%s.%s", parent.c_str(), var.second->varName.c_str());
				cpps::cpps_console_clearcolor();
				printf(" = ");
				if (value.isdomain() && !value.value.domain->varList.empty())
				{

					cpps::cpps_console_color(4);
					printf("<%s>\r\n", value.value.domain->domainname.c_str());
					cpps::cpps_console_clearcolor();

					for (int i = 0; i < kg; i++)
						printf(" ");
					printf("{\r\n");
					cpps_domain* value_domain = value.value.domain;
					cpps_debug_trace_domain(c,kg + 4, (parent.empty() ? std::string("") : (parent + ".")) + var.second->varName , value_domain);
					for (int i = 0; i < kg; i++)
						printf(" ");
					printf("}");

				}
				else {
					cpps::cpps_console_color(3);
					cpps_base_printf(c, value);
				}

				cpps::cpps_console_clearcolor();
				printf(",\r\n");
			}
		}
	}
	void cpps_debug_trace(C* c, cpps_domain* domain, cpps_domain* root, node* d) {
		cpps_debug_trace_domain(c,0,"",root);
	}
	cpps_value cpps_step_callfunction(C* c, cpps_domain* domain, cpps_domain* root, cpps_value func, node* d, cpps_domain* leftdomain) {
		cpps_value ret;
		cpps_domain* _tmp = leftdomain;
		if (func.tt == CPPS_TLAMBDAFUNCTION) {
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)func.value.domain;
			cpps_lambda_function* pfunc = (cpps_lambda_function*)cppsclassvar->getclsptr();
			func = pfunc;
			func.tt = CPPS_TFUNCTION;
		}
		if (func.tt == CPPS_TCLASSVAR) {
			cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(func);
			cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
			cpps_function* _func = cppsclass->getoperator("()");
			if (_func) {
				leftdomain = cppsclassvar;
				func = _func;
				func.tt = CPPS_TFUNCTION;
			}
		}

		if (func.tt == CPPS_TFUNCTION) {
			cpps_function* f = func.value.func;
			bool take_disabled_non_def_var = c->disabled_non_def_var;
			if (f->funcname == "isvalid") 
				c->disabled_non_def_var = true;
			cpps_std_vector params;
			cpps_value		isNeedC;
			if (f->getIsNeedC()) {
				isNeedC = cpps_cpp_to_cpps_converter<C*>::apply(c, c);
				params.emplace_back(isNeedC);
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
					newclass< cpps_async_object>(c, &obj,&ret);
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

				}
			}
			if (f->funcname == "isvalid") c->disabled_non_def_var = take_disabled_non_def_var;

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
				printf("cpps warring: [%s] is not function  line:%d , file:%s \n", d->getleft()->getright()->s.c_str(), d->line, d->filename.c_str()); 
			else
				printf("cpps warring: [%s] is not function  line:%d , file:%s \n", d->getleft()->s.c_str(), d->line, d->filename.c_str());
		}
		leftdomain = _tmp;
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


