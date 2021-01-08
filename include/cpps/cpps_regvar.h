#ifndef CPPS_REGVAR_CPPS_HEAD_
#define CPPS_REGVAR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/23 (yy/mm/dd)
//@Module		:	CPPS_REGVAR
//@Description	:	×¢²á±äÁ¿
//@website		:	http://cppscript.org
//==================================



namespace cpps
{
	struct cpps_domain;
	struct cpps_regvar
	{
		cpps_regvar();
		~cpps_regvar();
		void					setvarname(std::string n);
		void					setval(cpps_value v);
		cpps_value&				getval();
		int8					isconst();
		void					setconst(int8 c);
		bool					issource();
		void					setsource(bool src);
	public:
		cpps_value				value;
		std::string				varName;
		int8					nconst;
		int32					offset;
		int32					offsettype;
		bool					closeure;
		int32					closeureusecount;
		cpps_domain*			stackdomain;
		bool					sourcestate;
	};

	

}

#endif // CPPS_REGVAR_CPPS_HEAD_