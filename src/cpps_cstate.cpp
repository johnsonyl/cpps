#include "cpps.h"

namespace cpps
{
	void C::push(node *_o)
	{
		o = _o;
	}

	void C::push_stack(cpps_stack* stack)
	{
		_callstack.push_back(stack);
	}

	void C::pop_stack()
	{
		_callstack.pop_back();
	}

	std::vector<cpps_stack*>* C::getcallstack()
	{
		std::vector<cpps_stack*>* ret = &_callstack;
		return ret;
	}
	std::unordered_set<cpps_regvar*>* C::getbarrierlist()
	{
		std::unordered_set<cpps_regvar*>*ret = &barrierList;
		return ret;
	}

	cpps::cpps_domain* C::domain_alloc()
	{
		return domain_pool.alloc();
	}

	void C::domain_free(cpps::cpps_domain* domain)
	{
		domain_pool.free(domain);
	}

	cpps::cpps_stack* C::stack_alloc()
	{
		return stack_pool.alloc();
	}

	void C::stack_free(cpps::cpps_stack* stack)
	{
		stack_pool.free(stack);
	}

	std::unordered_set<cpps_cppsclassvar*>* C::getgen0()
	{
		std::unordered_set<cpps_cppsclassvar*>*ret = &gen0;
		return ret;
	}

	std::unordered_set<cpps_cppsclassvar *>* C::getgen1()
	{
		std::unordered_set<cpps_cppsclassvar*>* ret = &gen1;
		return ret;
	}
	void		C::setgen0size(size_t s)
	{
		gen0size = s;
	}
	size_t		C::getgen0size()
	{
		return gen0size;
	}

	void		C::setgen1size(size_t s)
	{
		gen1size = s;
	}
	size_t		C::getgen1size()
	{
		return gen1size;
	}
	void		C::setlastgensize(size_t s)
	{
		lastgensize = s;
	}
	size_t		C::getlastgensize()
	{
		return lastgensize;
	}
	C::C(int argc, char** argv)
	{
		o = NULL;
		_G = NULL;
		curnode = NULL;
		gen0size = 0;
		gen1size = 0;
		lastgensize = 0;
		func = NULL;
		debug = false;
		buildoffset = false;
		application_argc = argc;
		application_argv = argv;
	}

	void C::debugopen()
	{
		debug = true;
	}

	void C::debugclose()
	{
		debug = false;
	}

	void C::resume()
	{
		for (size_t i = 0; i < _callstack.size(); i++)
		{
			cpps_stack* stack = _callstack.at(i);
			delete stack;
		}
		_callstack.clear();
	}


	cpps_value * cpps_stack::create_cpps_value()
	{
		return new cpps_value();
	}

	std::vector<cpps_value> * cpps_stack::createparams()
	{
		return new std::vector<cpps_value>();
	}

	

}