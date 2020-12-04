#ifndef CPPS_REGVAR_CPPS_HEAD_
#define CPPS_REGVAR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/23 (yy/mm/dd)
//@Module		:	CPPS_REGVAR
//@Description	:	×¢²á±äÁ¿
//@website		:	http://cpps.wiki
//==================================



namespace cpps
{
	struct cpps_domain;
	struct cpps_regvar
	{
		cpps_regvar()
		{
			nconst = 0;
			offset = -1;
			offsettype = -1;
			closeure = false;
			closeureusecount = 0;
			stackdomain = NULL;
		}
		~cpps_regvar();
		void	setvarname(std::string n)
		{
			varName = n;
		}

		void setval(cpps_value v);
		cpps_value& getval()
		{
			return value;
		}
	
		int8 isconst()
		{
			return nconst;
		}
		void setconst(int8 c)
		{
			nconst = c;
		}
	public:
		cpps_value			value;
		std::string			varName;
		int8				nconst;

		int32				offset;
		int32				offsettype;
		bool				closeure;
		int32				closeureusecount;
		cpps_domain*		stackdomain;
	};

	

}

#endif // CPPS_REGVAR_CPPS_HEAD_