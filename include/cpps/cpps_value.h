#ifndef CPPS_VALUE_CPPS_HEAD_
#define CPPS_VALUE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_VALUE
//@Description	:	cpps值
//@website		:	http://cppscript.org
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
	struct cpps_classvar_quato;
	struct cpps_set;
	struct cpps_pair;
#pragma pack(1)
	struct cpps_value
	{
		cpps_value();
		cpps_value(const cpps_value& v);
		cpps_value(const cpps_number n);
		cpps_value(const cpps_integer i);
#ifdef _WIN32
		cpps_value(const unsigned __int64 i);
#else
		cpps_value(const long unsigned int i);
#endif
		cpps_value(char& strv);
		cpps_value(const bool b);
		cpps_value(cpps_function* f);
		cpps_value(cpps_classvar_quato* f);
		cpps_value(cpps_domain* d);
		cpps_value(cpps_cppsclass* d);
		cpps_value(cpps_cppsclassvar* d);
		cpps_value(cpps_value* v);
		cpps_value(C*c, const char* s);
		cpps_value(C*c,const std::string& s);
		cpps_value(C* c, const std::string&& s);
		~cpps_value();
		//为了可以当map的key 需要实现 < > == 3个函数
		bool				operator < (const cpps_value &right) const;
		bool				operator <= (const cpps_value right) const;
		bool				operator >(const cpps_value &right) const;
		bool				operator >=(const cpps_value right) const;
		bool				operator == (const cpps_value &right) const;
		cpps_value&			operator=(const cpps_value& v);
		void				decruse();
		void				incruse();
		void				decruse() const;
		void				incruse() const;
		bool				isdomain() const;
		bool				isref() const;
		const cpps_value&	real() const;
		cpps_value&			real();
		cpps_value			ref();

		struct hash
		{	
			size_t operator()(const cpps_value& _Keyval) const;
		};

		union Value
		{
			cpps_number			number;		// double float 
			cpps_integer		integer;	// __int64 
			cpps_uinteger		uinteger;	// unsigned __int64
			int32				b;			// bool
			void*				p;			// 用户自定义数据
			cpps_function*		func;		// func
			cpps_domain *		domain;		// domain
			cpps_regvar *		var;		// 变量指针
			cpps_value *		value;		// 值引用
			char*				strv;		// String[]扩展.
			cpps_classvar_quato* quato;		// c++classvar quato
		};
		Value					value;		//值。
		int8					tt;			//类型


	private:
		void _initstring(C* c, const char* s);
		void _initstring(C* c, const std::string& s);
	};
#pragma pack()
#define cpps_integer2number(i) (cpps_number)(i)
#define cpps_number2integer(n) (cpps_integer)(n)
#define cpps_number2uinteger(n) (cpps_uinteger)(n)



	bool	cpps_to_bool(const cpps_value& src);
	cpps_number	cpps_to_number(const cpps_value& src);
	cpps_integer cpps_to_integer(const cpps_value& src);
	cpps_uinteger cpps_to_uinteger(const cpps_value& src);
	std::string cpps_to_string(const cpps_value& src);
	std::string* cpps_get_string(const cpps_value& src);
	cpps_vector* cpps_to_cpps_vector(const cpps_value& src);
	cpps_map* cpps_to_cpps_map(const cpps_value& src);
	cpps_set* cpps_to_cpps_set(const cpps_value& src);
	cpps_pair* cpps_to_cpps_pair(const cpps_value& src);
	cpps_domain* cpps_to_domain(const cpps_value& src);
	cpps_cppsclass* cpps_to_cpps_cppsclass(const cpps_value& src);
	cpps_cppsclassvar* cpps_to_cpps_cppsclassvar(const cpps_value& src);

	static cpps_value		nil;
}


#endif // CPPS_VALUE_CPPS_HEAD_