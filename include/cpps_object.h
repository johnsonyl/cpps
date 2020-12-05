#ifndef CPPS_OBJECT_CPPS_HEAD_
#define CPPS_OBJECT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_OBJECT
//@Description	:	CppsתC++
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct cpps_domain;
	struct object
	{
		object()
		{
			
		}
		
		object(cpps_value v)
		{
			value = v;
		}


		template<class Type>
		object(C*c,Type v)
		{
			if (!cpps_cpp_to_cpps_converter<Type>::match(c,v))
			{
				throw(cpps_error("0", 0, 0, "Convert failed"));
			}

			value = cpps_cpp_to_cpps_converter<Type>::apply(c,v);
		}
	
		object		operator [] (std::string k)
		{
			cpps_value ret;
			if(value.isdomain() )
			{
				cpps_domain* leftdomain = NULL;

				cpps_regvar* var = value.value.domain->getvar(k,leftdomain);
				if (var)
				{
					ret = var->getval();
				}
			}
			return static_cast<object>(ret);
		}

		cpps_value	value;
	};

	static int32		type(object o)
	{
		return o.value.tt;
	}
	static std::string type_s(object o)
	{
		switch (type(o))
		{
		case CPPS_TNIL:
			return "nil";
		case CPPS_TINTEGER:
			return "integer";
		case CPPS_TNUMBER:
			return "number";
		case CPPS_TBOOLEAN:
			return "boolean";
		case CPPS_TSTRING:
			return "string";
		case CPPS_TCLASSVAR:
			return "userdata";
		case CPPS_TFUNCTION:
			return "function";
		}
		return "unknow";
	}
}

#endif // CPPS_OBJECT_CPPS_HEAD_