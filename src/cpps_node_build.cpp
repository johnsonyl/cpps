#include <cpps/cpps.h>

namespace cpps {
	node* cpps_parse_getsymbol2node(C* c, node* o, std::string  symbolStr, cppsbuffer& buffer, bool leftsymbol);
	int32 cpps_str2i64(const char* s, cpps_integer* result);
	int32 cpps_str2ui64(const char* s, cpps_uinteger* result);
	node* cpps_get_root_node(node* n);
	void cpps_class_parent_buildoffset(C* c, node* child, node* root, node* parent, std::set<node*>& parentset, std::set<cpps_cppsclass*>& parentset_class);
	void cpps_parse_var_buildoffset(C* c, node* root, node* str, cpps_node_domain* domain);
	void cpps_varname_build_offset(C* c, node* root, node* param, node* o);
	bool	cpps_loadlibrary(C* c, std::string libname, cppsbuffer& buffer);
	node* cpps_get_realroot(node* _n) {
		switch (_n->type)
		{
		case CPPS_ROOT:
		case CPPS_ODEFVAR_FUNC:
		case CPPS_ODEFVAR_LAMBDA_FUNC:
		case CPPS_OCLASS:
		case CPPS_ONAMESPACE:
			return _n;
		default:
			if (_n->parent) return cpps_get_realroot(_n->parent);
			break;
		}
		return NULL;
	}

	void cpps_node_build(C* c, node* _root, node* _node, cppsbuffer& buffer) {
		if (_node->type == CPPS_ROOT) return;
		if (_node->type == CPPS_ONIL) return;
		node* __root = cpps_get_realroot(_node->parent);

		switch (_node->type)
		{
		case CPPS_OIMPORT:
		{
			cpps_loadlibrary(c, _node->s, buffer);
			break;
		}
		case CPPS_VARNAME: {
			if (c->buildoffset) {
				//1.func args
				if (_node->parent && _node->parent->type == CPPS_VARNAME && _node->parent->parent) {
					node* right = _node->parent->parent;
					if (right->parent && (right->parent->type == CPPS_ODEFVAR_FUNC || right->parent->type == CPPS_ODEFVAR_LAMBDA_FUNC))
						/* 根节点.. */ {
						_node->offset = right->parent->size++;
						_node->offsettype = CPPS_OFFSET_TYPE_SELF;
						right->parent->regnode(_node->s, _node);
					}
				}
				//2.def var - multivar
				else if (_node->parent && _node->parent->type == CPPS_VARNAMES && _node->parent->parent) {
					cpps_parse_var_buildoffset(c, __root, _node, __root->domain);
				}
				//3.def var - const,async,var
				else if (_node->parent && (
					_node->parent->type == CPPS_ODEFVAR ||
					_node->parent->type == CPPS_ODEFCONSTVAR ||
					_node->parent->type == CPPS_ODEFASYNCVAR) &&
					_node->parent->parent) {
					//4.build symbol 无法确认是否是符号，只能逐个检测，后续优化
					cpps_symbol* symbol = cpps_parse_getsymbol(c, _node->s, false);
					if (symbol) {
						_node->symbol = symbol;
					}
					cpps_parse_var_buildoffset(c, __root, _node, __root->domain);
				}
				else
				{

					cpps_varname_build_offset(c, __root, _node, __root);
				}


			}
			break;
		}
		case CPPS_ONEWVAR: {
			_node->offset = 0;
			_node->offsettype = 0;
			_node->varsize = 0;
			_node->size = 0;
			break;
		}
		case CPPS_OTRYCATCH: {
			cpps_node_domain* trycatchdomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_trycatch, "trycatch");
			_node->setdomain(trycatchdomain);
			break;
		}
		case CPPS_ODEFVAR_FUNC:
		case CPPS_OCLASS_CONSTRUCTOR:
		case CPPS_ODOFUNCTION:
		{
			_node->size = 0;
			_node->varsize = 0;

			cpps_node_domain* funcdomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_func, _node->parent->s);
			_node->setdomain(funcdomain);
			break;
		}
		case CPPS_OIF: {
			cpps_node_domain* ifdomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_if, "if");
			_node->setdomain(ifdomain);
			break;
		}
		case CPPS_OFOR: {
			cpps_node_domain* fordomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_for, "for");
			_node->setdomain(fordomain);
			break;
		}
		case CPPS_OFOREACH: {
			cpps_node_domain* foreachdomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_for, "foreach");
			_node->setdomain(foreachdomain);
			break;
		}
		case CPPS_OWHILE: {
			cpps_node_domain* whiledomain = CPPSNEW(cpps_node_domain)(_node, __root->domain, cpps_domain_type_while, "while");
			_node->setdomain(whiledomain);
			break;
		}
		case CPPS_FUNCNAME: {
			cpps_symbol* symbol = cpps_parse_getsymbol(c, _node->s, _node->closure);
			if (!symbol) cpps::error(c, "symbol error");
			_node->symbol = symbol;
			break;
		}
		case CPPS_OSTR: {
			//const string
			if (_node->l.size() == 1 || _node->l.size() == 0) {
				std::string* tmpstr;
				_node->value.val = CPPSNEW(cpps_value)();
				newclass(c, &tmpstr, _node->value.val);
				_node->needdelete = true;
				if (_node->l.size() == 1) tmpstr->append(_node->l[0]->s);
			}
			break;
		}
		case CPPS_OENUM: {
			_node->offset = 0;
			_node->offsettype = 0;
			_node->varsize = 0;
			_node->size = 0;

			for (auto& _cn : _node->l) {
				cpps_str2i64(_cn->s.c_str(), &_cn->value.integer);
			}
			break;
		}
		case CPPS_ONUMBER: {
			cpps_str2d(_node->s.c_str(), &_node->value.number);
			break;
		}
		case CPPS_OINTEGER:
		case CPPS_OINTEGER16: {
			cpps_str2i64(_node->s.c_str(), &_node->value.integer);
			break;
		}
		case CPPS_OUINTEGER: {
			cpps_str2ui64(_node->s.c_str(), &_node->value.uinteger);
			break;
		}
		case CPPS_OCLASS: {
			_node->varsize = 0;
			_node->size = 0;
			if (c->buildoffset) {
				if (__root->type == CPPS_ROOT)
					/* 根节点.. */ {
					_node->offset = __root->size++;
					_node->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
					__root->regnode(_node->s, _node);
				}
				else if (__root->type == CPPS_ONAMESPACE) {
					node* r = cpps_get_root_node(__root);
					/* namespace不注册到自己里面 */
					_node->offset = r->size++;
					_node->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
					r->regnode(_node->s, _node);
				}

				//parents
				node* parents = _node->l[0];
				node* vars = _node->l[1];
				std::set<node*> parentset;
				std::set<cpps_cppsclass*> parentset_class;

				for (node* _parent : parents->l) {
					cpps_class_parent_buildoffset(c, _node, __root, _parent, parentset, parentset_class);
				}
			}
			break;
		}
		case CPPS_ONAMESPACE: {

			_node->offset = 0;
			_node->offsettype = 0;
			_node->varsize = 0;
			_node->size = 0;
			if (c->buildoffset) {
				if (__root->type == CPPS_ROOT)
					/* 根节点.. */ {
					_node->offset = __root->size++;
					_node->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
					__root->regnode(_node->s, _node);
				}
				else if (__root->type == CPPS_ONAMESPACE) {
					node* r = cpps_get_root_node(__root);
					/* namespace不注册到自己里面 */
					_node->offset = r->size++;
					_node->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
					r->regnode(_node->s, _node);
				}
			}

			break;
		}
		case CPPS_ODEFVAR: {
			node* r = cpps_get_root_node(_node->parent);
			node* str = _node->l[0];
			node* lambda = str->l[0];
			if (lambda->type == CPPS_ODEFVAR_LAMBDA_FUNC) {

				if (c->buildoffset) {
					str->offset = r->size++;
					str->offsettype = CPPS_OFFSET_TYPE_GLOBAL;
				}

				node* lastopnode = _node->parent;
				node* lambdaparam = lastopnode->l[1];

				if (__root->type == CPPS_ODEFVAR_LAMBDA_FUNC || __root->type == CPPS_ODEFVAR_FUNC) {
					lambda->size = __root->size;/*为了兼容闭包*/
				}
				usint16 takesize = lambda->size;

				if (c->buildoffset) {
					lambdaparam->offset = str->offset;
					lambdaparam->offsettype = str->offsettype;
				}
				lambdaparam->size = takesize;/*记录当时使用时它父类长度.*/
			}
			break;
		}
		}
	}
}