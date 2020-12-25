#ifndef CPPS_CONVERTER_CPPS_HEAD_
#define CPPS_CONVERTER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_CONVERTER
//@Description	:	Cpps转C++ 或 C++转Cpps
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	void					cpps_gc_add_gen0(C*c, cpps_cppsclassvar *p);
	std::string				type_s(object o);
	struct string;
	template<class R>
	struct cpps_converter
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TCLASSVAR || obj.tt == CPPS_TNIL;
		}
		static R		apply(cpps_value obj)
		{
			if(!match(obj))
				throw(cpps_error("0", 0, 0, "cppsvalue can't convert to %s, cppsvalue type is %s , conversion failed.", typeid(R).name() ,type_s(obj).c_str()));

			if (obj.tt == CPPS_TNIL) return NULL;

			cpps_cppsclassvar *clsvar = (cpps_cppsclassvar *)obj.value.domain;
			return static_cast<R>(clsvar->getclsptr());
		}
	};
	template<>
	struct cpps_converter<cpps::string*>
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING;
		}
		static cpps::string*		apply(cpps_value obj)
		{
			if (!match(obj))
				throw(cpps_error("0", 0, 0, "cppsvalue can't convert to cpps_string, cppsvalue type is %s , conversion failed.", type_s(obj).c_str()));


			cpps_cppsclassvar* clsvar = (cpps_cppsclassvar*)obj.value.domain;
			return static_cast<cpps::string *>(clsvar->getclsptr());
		}
	};
#define def_cpps_number_cast(t,v) template<>\
	struct cpps_converter<t>\
	{\
	static bool	match(cpps_value obj)\
	{\
	return obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TNUMBER; \
	}\
	static t		apply(cpps_value obj)\
	{\
	return static_cast<t>(CPPS_PP_CAT(cpps_to_, v)(obj)); \
	}\
	}

	def_cpps_number_cast(char, integer);
	def_cpps_number_cast(signed char, integer);
	def_cpps_number_cast(unsigned char, integer);

	def_cpps_number_cast(signed short, integer);
	def_cpps_number_cast(unsigned short, integer);

	def_cpps_number_cast(signed int, integer);
	def_cpps_number_cast(long, integer);

	def_cpps_number_cast(unsigned int, integer);
	def_cpps_number_cast(unsigned long, integer);

	def_cpps_number_cast(cpps_integer, integer);
#ifdef _WIN32
#ifdef _M_X64
	def_cpps_number_cast(size_t, integer);
#endif
#endif

	def_cpps_number_cast(float, number);
	def_cpps_number_cast(double, number);
	def_cpps_number_cast(long double, number);
#undef def_cpps_number_cast
	//string  转换
	template<>
	struct cpps_converter<std::string>
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING || obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TBOOLEAN || obj.tt == CPPS_TNUMBER || obj.tt == CPPS_TNIL;
		}
		static std::string		apply(cpps_value obj)
		{
			return cpps_to_string(obj);
		}
	};
	
	template<>
	struct cpps_converter < const char * >
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING || obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TBOOLEAN || obj.tt == CPPS_TNUMBER || obj.tt == CPPS_TNIL;
		}
		static const char *	apply(cpps_value obj)
		{
			return (const char *)cpps_get_string(obj)->c_str();
		}
	};
	template<>
	struct cpps_converter < const unsigned char * >
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING || obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TBOOLEAN || obj.tt == CPPS_TNUMBER || obj.tt == CPPS_TNIL;
		}
		static const unsigned char *		apply(cpps_value obj)
		{
			return (const unsigned char *)cpps_get_string(obj)->c_str();
		}
	};
	template<>
	struct cpps_converter < char * >
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING || obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TBOOLEAN || obj.tt == CPPS_TNUMBER || obj.tt == CPPS_TNIL;
		}
		static char *		apply(cpps_value obj)
		{
			return (char *)cpps_get_string(obj)->c_str();
		}
	};
	template<>
	struct cpps_converter < unsigned char * >
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING || obj.tt == CPPS_TINTEGER || obj.tt == CPPS_TBOOLEAN || obj.tt == CPPS_TNUMBER || obj.tt == CPPS_TNIL;
		}
		static unsigned char *		apply(cpps_value obj)
		{
			return (unsigned char *)cpps_get_string(obj)->c_str();
		}
	};
	template<>
	struct cpps_converter<cpps_value>
	{
		static bool	match(cpps_value obj)
		{
			return true;
		}
		static cpps_value		apply(cpps_value obj)
		{
			return obj;
		}
	};

	template<>
	struct cpps_converter<bool>
	{
		static bool	match(cpps_value obj)
		{
			return true;
		}
		static bool		apply(cpps_value obj)
		{

			switch (obj.tt)
			{
			case CPPS_TNUMBER:
				if (obj.value.number != 0)
				{
					return true;
				}
				else
				{
					return false;
				}
				break;
			case CPPS_TINTEGER:
				if (obj.value.integer != 0)
				{
					return true;
				}
				else
				{
					return false;
				}
				break;
			case CPPS_TSTRING:
				if (!((std::string*)obj.value.domain)->empty())
				{
					return true;
				}
				else
				{
					return false;
				}
				break;
			case CPPS_TBOOLEAN:
				return obj.value.b != 0;
			case CPPS_TCLASSVAR:
				return obj.value.domain != NULL;
			case CPPS_TNIL:
				return false;
			case CPPS_TFUNCTION:
				return true;
			case CPPS_TDOMAIN:
				return true;
			case CPPS_TREGVAR:
				return true;
			case CPPS_TCLASS:
				return true;
			default:
				return false;
				break;
			}

			return static_cast<bool>(obj.value.b != 0);
		}
	};

	template<>
	struct cpps_converter<object>
	{
		static bool	match(cpps_value obj)
		{
			return true;
		}
		static object		apply(cpps_value obj)
		{
			return object(obj);
		}
	};

	//c++ to cpps

	template<class Type>
	struct cpps_cpp_to_cpps_converter
	{
		static bool match(C *c, Type v)
		{
			//检测是否支持
			return cpps_class_singleton<Type>::instance() != NULL && c!=NULL;
		}
		static cpps_value apply(C *c, Type v)
		{
			cpps_value ret;
			if (v == NULL) //NULL  == nil
				return ret;

			ret.tt = CPPS_TCLASSVAR;
			
			phmap::flat_hash_map<void*, cpps_cppsclassvar*>::iterator it = c->_class_map_classvar.find(v);
			cpps_cppsclassvar* var;
			if (it == c->_class_map_classvar.end())
			{
				var = cpps_class_singleton<Type>::instance()->getcls()->create(c,false);
				var->setclsptr((void*)v);

				//将新创建出来的添加到新生区稍后检测要不要干掉
				cpps_gc_add_gen0(c, var);
			}
			else
				var = it->second;

			ret.value.domain = var;
			ret.value.domain->incruse();

			return ret;
		}
	};
	template<>
	struct cpps_cpp_to_cpps_converter < cpps_value >
	{
		static bool match(C *c, cpps_value value)
		{
			//检测是否支持
			return true;
		}
		static cpps_value apply(C *c, cpps_value value)
		{
			if (value.tt == CPPS_TREGVAR)
			{
				return *(value.value.value);
			}
			return value;
		}
	};

	template<>
	struct cpps_cpp_to_cpps_converter<void>
	{
		static bool match(C *c )
		{
			//检测是否支持
			return true;
		}
		static cpps_value apply(C *c)
		{
			cpps_value ret;
			
			return ret;
		}
	};

	template<>
	struct cpps_cpp_to_cpps_converter<object>
	{
		static bool match(C *c, object v)
		{
			//检测是否支持
			return true;
		}
		static cpps_value apply(C *c, object v)
		{
			return v.value;
		}
	};


#define def_cpps_cpp_to_cpps_number_cast(t,v) template<>\
	struct cpps_cpp_to_cpps_converter<t>\
	{\
	static bool	match(C *c,t obj)\
	{\
	return true; \
	}\
	static cpps_value		apply(C *c,t obj)\
	{\
	return cpps_value(CPPS_PP_CAT(cpps_, v)(obj)); \
	}\
	}

	def_cpps_cpp_to_cpps_number_cast(char, integer);
	def_cpps_cpp_to_cpps_number_cast(signed char, integer);
	def_cpps_cpp_to_cpps_number_cast(unsigned char, integer);

	def_cpps_cpp_to_cpps_number_cast(signed short, integer);
	def_cpps_cpp_to_cpps_number_cast(unsigned short, integer);

	def_cpps_cpp_to_cpps_number_cast(signed int, integer);
	def_cpps_cpp_to_cpps_number_cast(long, integer);

	def_cpps_cpp_to_cpps_number_cast(unsigned int, integer);
	def_cpps_cpp_to_cpps_number_cast(unsigned long, integer);

	def_cpps_cpp_to_cpps_number_cast(cpps_integer, integer);
#ifdef _WIN32
#ifdef _M_X64
	def_cpps_cpp_to_cpps_number_cast(size_t, integer);
#endif
#endif
	def_cpps_cpp_to_cpps_number_cast(float, number);
	def_cpps_cpp_to_cpps_number_cast(double, number);
	def_cpps_cpp_to_cpps_number_cast(long double, number);

#undef def_cpps_cpp_to_cpps_number_cast

	template<>
	struct cpps_cpp_to_cpps_converter<std::string>
	{
		static bool				match(C *c, std::string obj)
		{
			return true;
		}
		static cpps_value		apply(C *c, std::string obj)
		{
			return cpps_value(c,obj);
		}
	};
	template<>
	struct cpps_cpp_to_cpps_converter<cpps::string*>
	{
		static bool				match(C* c, cpps::string* v)
		{
			return true;
		}
		static cpps_value		apply(C* c, cpps::string* v)
		{
			cpps_value ret;
			if (v == NULL) //NULL  == nil
				return ret;

			ret.tt = CPPS_TSTRING;

			phmap::flat_hash_map<void*, cpps_cppsclassvar*>::iterator it = c->_class_map_classvar.find(v);
			cpps_cppsclassvar* var;
			if (it == c->_class_map_classvar.end())
			{
				var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(c, false);
				var->setclsptr((void*)v);

				//将新创建出来的添加到新生区稍后检测要不要干掉
				cpps_gc_add_gen0(c, var);
			}
			else
				var = it->second;

			ret.value.domain = var;
			ret.value.domain->incruse();

			return ret;

		}
	};
	template<>
	struct cpps_cpp_to_cpps_converter<char *>
	{
		static bool				match(C *c, char * obj)
		{
			return true;
		}
		static cpps_value		apply(C *c, char * obj)
		{
			return cpps_value(c,obj);
		}
	};
	template<>
	struct cpps_cpp_to_cpps_converter<const char *>
	{
		static bool				match(C *c, const char * obj)
		{
			return true;
		}
		static cpps_value		apply(C *c, const char * obj)
		{
			return cpps_value(c,obj);
		}
	};

	template<>
	struct cpps_cpp_to_cpps_converter<bool>
	{
		static bool				match(C *c, bool obj)
		{
			return true;
		}
		static cpps_value		apply(C *c,bool obj)
		{
			return cpps_value(obj);
		}
	};
}



#endif // CPPS_CONVERTER_CPPS_HEAD_