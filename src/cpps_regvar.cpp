#include "cpps/cpps.h"


namespace cpps
{

	void cpps_regvar::setval(const cpps_value& v)
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

	bool cpps_regvar::issource()
	{
		return sourcestate;
	}

	void cpps_regvar::setsource(bool src)
	{
		sourcestate = src;
	}


	void cpps_regvar::clone(cpps_regvar *right)
	{
		if (!cpps_isnull(right->value)) value = right->value;
		varName = right->varName;
		nconst = right->nconst;
		offset = right->offset;
		offsettype = right->offsettype;
		closeure = right->closeure;
		closeureusecount = right->closeureusecount;
		stackdomain = right->stackdomain;
		sourcestate= false;
	}

	void cpps_regvar::ref(cpps_regvar* right)
	{
		if (!cpps_isnull(right->value)) value = right->value.ref();
		varName = right->varName;
		nconst = right->nconst;
		offset = right->offset;
		offsettype = right->offsettype;
		closeure = right->closeure;
		closeureusecount = right->closeureusecount;
		stackdomain = right->stackdomain;
		sourcestate = false;
	}

	cpps_regvar::cpps_regvar()
	{
		nconst = 0;
		offset = -1;
		offsettype = -1;
		closeure = false;
		closeureusecount = 0;
		stackdomain = NULL;
		sourcestate = false;
	}

	cpps_regvar::~cpps_regvar()
	{
		
	}

	void cpps_regvar::setvarname(std::string n)
	{
		varName = n;
	}

	std::string cpps_regvar::getvarname()
	{
		return varName;
	}

	void cpps_regvar::release()
	{
		CPPSDELETE(this);
	}

}