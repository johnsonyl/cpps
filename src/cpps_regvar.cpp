#include "cpps.h"


namespace cpps
{

	void cpps_regvar::setValue(cpps_value v)
	{
		if (v.tt == CPPS_TCLASSVAR)
		{
			//cpps_cppsclassvar *var = (cpps_cppsclassvar *)v.value.domain;
		}
		if (value.tt == CPPS_TCLASSVAR)
		{
			//cpps_cppsclassvar *var = (cpps_cppsclassvar *)value.value.domain;
		}
		value = v;
	}

	cpps_regvar::~cpps_regvar()
	{
		if (value.tt == CPPS_TCLASSVAR)
		{
			//cpps_cppsclassvar *var = (cpps_cppsclassvar *)value.value.domain;
		}
	}

}