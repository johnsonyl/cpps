#ifndef CPPS_STRING_CPPS_HEAD_
#define CPPS_STRING_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_STRING
//@Description	:	创建String接口
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	std::string cpps_base_type(cpps_value v);
	struct string
	{
	public:
		cpps_integer	cpps_size();
		cpps_integer	cpps_string_find(std::string v2,object off);
		cpps_integer	cpps_string_rfind(std::string v2,object off);
		cpps::string*	cpps_string_replace(std::string v2, std::string v3);
		void			cpps_string_clear();
		void			cpps_string_copyto(cpps::string *tar,object off,object len);
		cpps_value		cpps_string_split(C* c, std::string v2, object len);
		cpps_value		cpps_string_cut(C* c, cpps_integer len);
		std::string		cpps_string_strcut( std::string v2,std::string v3);
		cpps_value		cpps_string_strcuts(C*c, std::string v2,std::string v3);
		bool			cpps_string_empty();
		bool			cpps_string_startswith(std::string v);
		bool			cpps_string_endswith(std::string v);
		std::string		cpps_string_sub(cpps_integer off,object len);
		cpps_integer	cpps_string_at(cpps_integer off);
		cpps::string*	cpps_string_erase(cpps_integer off, object _count);
		std::string		cpps_string_tolower();
		std::string		cpps_string_toupper();
		std::string		cpps_string_join(cpps_vector *vec);
		void			cpps_string_trim();
		void			cpps_string_ltrim();
		void			cpps_string_rtrim();
		void			cpps_string_pop_back(object len);
		void			cpps_string_push_back(cpps_integer charcode);
		void			cpps_string_append(std::string v);
		std::string		cpps_string_title();
		std::string		cpps_string_center(cpps_integer width);
		bool			cpps_string_isalnum();
		bool			cpps_string_isalpha();
		bool			cpps_string_isspace();
		bool			cpps_string_isdecimal();
		void			cpps_string_resize(cpps_integer s);
		void			cpps_string_insert_real(size_t& off, object& v);
		void			cpps_string_insert(cpps_integer off, object v);
	public:
		std::string&	real();
		std::string		__str;
	};

	template<>
	class cpps_is_string<cpps::string>
	{
	public:
		cpps_is_string()
		{
			b = true;
		}

	public:
		bool b;
	};

	template<>
	struct cpps_converter<cpps::string*>
	{
		static bool	match(cpps_value obj)
		{
			return obj.tt == CPPS_TSTRING;
		}
		static cpps::string* apply(cpps_value obj)
		{
			if (!match(obj))
				throw(cpps_error(__FILE__, __LINE__, 0, "cppsvalue can't convert to cpps_string, cppsvalue type is %s , conversion failed.", cpps_base_type(obj).c_str()));


			cpps_cppsclassvar* clsvar = (cpps_cppsclassvar*)obj.value.domain;
			return static_cast<cpps::string*>(clsvar->getclsptr());
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
			C* pc = c->_parentCState ? c->_parentCState : c;
			pc->_classvarlock->lock();

#ifdef _DEBUG
			std::map<void*, cpps_cppsclassvar*>::iterator it = pc->_class_map_classvar.find(v);
#else
			phmap::flat_hash_map<void*, cpps_cppsclassvar*>::iterator it = pc->_class_map_classvar.find(v);
#endif
			cpps_cppsclassvar* var;
			if (it == pc->_class_map_classvar.end())
			{
				var = cpps_class_singleton<cpps::string*>::instance()->getcls()->create(pc, false);
				var->setclsptr((void*)v);

				//将新创建出来的添加到新生区稍后检测要不要干掉
				cpps_gc_add_gen0(c, var);
			}
			else
				var = it->second;
			pc->_classvarlock->unlock();

			ret.value.domain = var;
			ret.value.domain->incruse();

			return ret;

		}
	};
	void	cpps_regstring(C *c);

}

#endif