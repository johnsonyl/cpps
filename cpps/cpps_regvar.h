#ifndef CPPS_REGVAR_CPPS_HEAD_
#define CPPS_REGVAR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
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
			isJoinBarrier = false;
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
		void setIsJoinBarrier(bool b)
		{
			isJoinBarrier = b;
		}
		bool getIsJoinBarrier()
		{
			return isJoinBarrier;
		}
	private:
		cpps_value			value;
	public:
		std::string			varName;
		bool				isJoinBarrier;
	};

	

}

#endif // CPPS_REGVAR_CPPS_HEAD_