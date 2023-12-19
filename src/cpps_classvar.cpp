#include "cpps/cpps.h"

cpps::cpps_cppsclassvar::cpps_cppsclassvar(std::string clsname, cpps_domain* p, char type, bool alloc)
	:cpps_domain(p, type, clsname), isalloc(alloc) {
	usecount = 0;
}

void* cpps::cpps_cppsclassvar::getclsptr() { return NULL; }

void cpps::cpps_cppsclassvar::setclsptr(void* p) { }

bool cpps::cpps_cppsclassvar::isallocclass() { return isalloc; }

void cpps::cpps_cppsclassvar::release() { CPPSDELETE(this); }

size_t cpps::cpps_cppsclassvar::size() { return sizeof(*this); }

cpps::cpps_cppsclass* cpps::cpps_cppsclassvar::getcppsclass() { return (cpps_cppsclass*)parent[0]; }
