#include "cpps/cpps.h"


namespace cpps
{
	
	cpps_cppsclass::cpps_cppsclass(std::string _classname, node* _o, cpps_domain* p, char type) :cpps_domain(p, type, _classname)
	{
		if (_o) { o = CPPSNEW(node)(); o->clone(_o); }
		else o = NULL;
		classname = _classname;
		

	}

	cpps_cppsclass::~cpps_cppsclass()
	{
		if (o) {
			cpps_destory_node(o); //«Â¿Ìnode.
			CPPSDELETE(o);
			o = NULL;
		}
	}

	cpps::cpps_cppsclassvar* cpps_cppsclass::create(C* c, bool alloc /*= true*/)
	{
		return (CPPSNEW(cpps_cppsclassvar)(getclassname(), this, cpps_domain_type_classvar, alloc));
	}

	bool cpps_cppsclass::iscppsclass()
	{
		return true;
	}

	std::string cpps_cppsclass::getclassname()
	{
		return classname;
	}

	std::vector<cpps_cppsclass*>& cpps_cppsclass::parentclasslist()
	{
		return _parentclasslist;
	}

	void cpps_cppsclass::release()
	{
		CPPSDELETE(this);
	}

	void cpps_cppsclass::operatorreg(std::string symbolname, cpps_function* func)
	{
		operatorlist[symbolname] = func;
	}

	cpps::cpps_function* cpps_cppsclass::getoperator(std::string symbolname)
	{
		return operatorlist[symbolname];
	}

	cpps::node* cpps_cppsclass::getvars_node()
	{
		return o;
	}

}