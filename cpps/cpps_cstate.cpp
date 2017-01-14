#include "cpps.h"

namespace cpps
{
	void C::push(Node *_o)
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

	std::vector<cpps_stack*>& C::getcallstack()
	{
		return _callstack;
	}

	C::C()
	{
		gen0size = 0;
		gen1size = 0;
		lastgensize = 0;
		func = NULL;
		debug = false;
		isCheckGen1 = false;
	}

	void C::debugopen()
	{
		debug = true;
	}

	void C::debugclose()
	{
		debug = false;
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