#include "cpps/cpps.h"


namespace cpps
{

	void cpps_regvar::setval(cpps_value v)
	{
		value = v;
	}

	cpps::cpps_value& cpps_regvar::getval()
	{
		return value;
	}

	cpps::int8 cpps_regvar::isconst()
	{
		return nconst;
	}

	void cpps_regvar::setconst(int8 c)
	{
		nconst = c;
	}

	cpps_regvar::cpps_regvar()
	{
		nconst = 0;
		offset = -1;
		offsettype = -1;
		closeure = false;
		closeureusecount = 0;
		stackdomain = NULL;
	}

	cpps_regvar::~cpps_regvar()
	{
		
	}

	void cpps_regvar::setvarname(std::string n)
	{
		varName = n;
	}

}