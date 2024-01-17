#include "cpps/cpps.h"


namespace cpps
{
	
	cpps_cppsclass::cpps_cppsclass(std::string _classname, node* _o, cpps_domain* p, char type) :cpps_domain(p, type, _classname.c_str())
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

	inline cpps_cppsclassvar* cpps_cppsclass::create(C* c, bool alloc)
	{
		return (CPPSNEW(cpps_cppsclassvar)(getclassname_c_str(), this, cpps_domain_type_classvar, alloc));
	}

	bool cpps_cppsclass::iscppsclass()
	{
		return true;
	}

	const char* cpps_cppsclass::getclassname_c_str()
	{
		return classname.c_str();
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

	C* cpps_cppsclass::getDefaultCState() { return _c; }

	void cpps_cppsclass::setDefaultCState(C* c) { _c = c; }

	cpps_class2::cpps_class2(std::string _classname, cpps_domain* p, char type, cpps_class_alloc __alloc, cpps_class_free __free) :cpps_cppsclass(_classname, NULL, p, type) {
		_alloc = __alloc;
		_free = __free;
	}

}