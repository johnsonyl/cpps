#ifndef CPPS_VALUE_CPPS_HEAD_
#define CPPS_VALUE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_VALUE
//@Description	:	cpps值
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{

	struct cpps_regvar;
	struct cpps_cppsclassvar;
	struct cpps_function;
	struct cpps_domain;
	struct cpps_cppsclass;
	struct cpps_cppsclassvar;
	struct cpps_vector;
	struct cpps_map;
	struct cpps_unordered_map;
	struct cpps_value
	{
		cpps_value()
		{
			tt = CPPS_TNIL;
			value.number = 0;
		}
		
		cpps_value(const cpps_number n)
		{
			tt = CPPS_TNUMBER;
			value.number = n;
		}
		cpps_value(const cpps_integer i)
		{
			tt = CPPS_TINTEGER;
			value.integer = i;
		}

#ifdef _WIN32
		cpps_value(const unsigned __int64 i)
		{
			tt = CPPS_TINTEGER;
			value.integer = i;
		}

#else
		cpps_value(const long unsigned int i)
		{
			tt = CPPS_TINTEGER;
			value.integer = i;
		}
#endif
		cpps_value(const bool b)
		{
			tt = CPPS_TBOOLEAN;
			value.b = b;
		}
		cpps_value(cpps_function* f)
		{
			tt = CPPS_TFUNCTION;
			value.func = f;
		}

		cpps_value(cpps_domain* d)
		{
			tt = CPPS_TDOMAIN;
			value.domain = d;
		}
		cpps_value(cpps_cppsclass* d)
		{
			tt = CPPS_TCLASS;
			value.domain = (cpps_domain*)(d);
		}
		cpps_value(cpps_cppsclassvar* d)
		{
			tt = CPPS_TCLASSVAR;
			value.domain = (cpps_domain*)(d);
		}
// 		cpps_value(cpps_regvar* v)
// 		{
// 			tt = CPPS_TREGVAR;
// 			value.var = v;
// 		}
		cpps_value(cpps_value* v)
		{
			tt = CPPS_TREGVAR;
			value.value = v;
		}
		cpps_value(C*c, const char* s);
		cpps_value(C*c,const std::string& s);
		~cpps_value()
		{

		}
		//为了可以当map的key 需要实现 < > == 3个函数
		bool				operator < (const cpps_value &right) const;

		bool				operator >(const cpps_value &right) const;

		bool				operator == (const cpps_value &right) const;
		
		bool				isdomain()
		{
			return tt == CPPS_TDOMAIN || tt == CPPS_TCLASS || tt == CPPS_TCLASSVAR;
		}

		struct hash
		{	
			size_t operator()(const cpps_value& _Keyval) const;
		};

		union Value
		{
			cpps_number			number;		// double float 
			cpps_integer		integer;	// double float 
			int32				b;			// bool
			void*				p;			// 用户自定义数据
			cpps_function*		func;		// func
			cpps_domain *		domain;		// domain
			cpps_regvar *		var;		// 变量指针
			cpps_value *		value;		// 值引用
		};
		Value					value;		//值。
		int8					tt;			//类型

	};

#define cpps_integer2number(i) (cpps_number)(i)
#define cpps_number2integer(n) (cpps_integer)(n)

	cpps_number	cpps_to_number(cpps_value obj);
	cpps_integer cpps_to_integer(cpps_value obj);
	std::string cpps_to_string(cpps_value obj);
	std::string* cpps_get_string(cpps_value obj);
	cpps_vector* cpps_to_cpps_vector(cpps_value obj);
	cpps_map* cpps_to_cpps_map(cpps_value obj);
	cpps_unordered_map* cpps_to_cpps_unordered_map(cpps_value obj);
	cpps_cppsclass* cpps_to_cpps_cppsclass(cpps_value obj);

	static cpps_value		nil;
}


#endif // CPPS_VALUE_CPPS_HEAD_