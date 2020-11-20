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
	struct cpps_regvar
	{
		cpps_regvar()
		{
			n8isConst = 0;
		}
		~cpps_regvar();
		void	setVarName(std::string n)
		{
			varName = n;
		}

		void setValue(cpps_value v);
		cpps_value& getValue()
		{
			return value;
		}
	
		int8 isConst()
		{
			return n8isConst;
		}
		void setIsConst(int8 c)
		{
			n8isConst = c;
		}
	private:
		cpps_value			value;
	public:
		std::string			varName;
		int8				n8isConst;
	};

	

}

#endif // CPPS_REGVAR_CPPS_HEAD_