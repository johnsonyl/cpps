#include "cpps/cpps.h"

namespace cpps
{
	void C::push(node *_o)
	{
		o = _o;
	}

	void C::push_stack(cpps_stack* stack)
	{
		_callstack->emplace_back(stack);
	}

	void C::pop_stack()
	{
		_callstack->pop_back();
	}

	std::vector<cpps_stack*>* C::getcallstack()
	{
		std::vector<cpps_stack*>* ret = _callstack;
		return ret;
	}
	void C::setcallstack(std::vector<cpps_stack*>* ret)
	{
		if (ret == NULL) return;

		 _callstack = ret;
	}
	phmap::flat_hash_set<cpps_regvar*>* C::getbarrierlist()
	{
		phmap::flat_hash_set<cpps_regvar*>*ret = &barrierList;
		return ret;
	}



	void C::setmoduledata(std::string modulename, cpps_module_data* data)
	{
		savemoduledatas[modulename] = data;
	}

	cpps_module_data* C::getmoduledata(std::string modulename)
	{
		return savemoduledatas[modulename];
	}

	CLASSVARSET* C::getgen0()
	{
		CLASSVARSET*ret = &gen0;
		return ret;
	}

	CLASSVARSET* C::getgen1()
	{
		CLASSVARSET* ret = &gen1;
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
		disabled_non_def_var = false;
		o = NULL;
		_G = NULL;
		_emptynode = CPPSNEW(node)();
		_emptynode->filename = "None";
		_emptynode->line = 1;
		curnode = _emptynode;
		gen0size = 0;
		gen1size = 0;
		lastgensize = 0;
		func = NULL;
		debug = false;
		buildoffset = false;
		application_argc = argc;
		application_argv = argv;
		ordinator = NULL;
		isterminate = false;
		isloadbase = false;
		_callstack = CPPSNEW( std::vector<cpps_stack*>)();
		_parentCState = NULL;
		_lambdanum = 0;
		cpps_jit_compile = NULL;
		cpps_jit_run = NULL;
	}

	C::~C()
	{
		if(_callstack) CPPSDELETE(_callstack); 
		_callstack = NULL;
		if (_emptynode) CPPSDELETE(_emptynode);
		_emptynode = NULL;
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
		if (_callstack == NULL) return;
		_callstack->clear();
	}

	void C::clone(C* clone_c2)
	{
		C* clone_c = clone_c2->_parentCState ? clone_c2->_parentCState : clone_c2;
		if (clone_c == this) return;
		_G->clone(clone_c->_G);
		_parentCState = clone_c;
		_gen0lock = new cpps_lock();
		_classvarlock = clone_c->_classvarlock;
		isloadbase = true;
		leftsymbolmap = clone_c->leftsymbolmap;
		symbolmap = clone_c->symbolmap;
		for (int i = 0; i < MAXSYMBOLPRIO; i++) {
			symbolleftasso[i] = clone_c->symbolleftasso[i];
		}
		savemoduledatas = clone_c->savemoduledatas;
		cpps_jit_compile = clone_c->cpps_jit_compile;
		cpps_jit_run = clone_c->cpps_jit_run;
	}

	void C::set_cxo_handle_func(cpps_cxo_func __func)
	{
		_cxo_func = __func;
	}

	cpps_value * cpps_stack::create_cpps_value()
	{
		return CPPSNEW( cpps_value)();
	}

	cpps_stack::cpps_stack()
	{
		memset(f, 0, 255);
		memset(func, 0, 64);
		l = 0;
	}

	void cpps_stack::init(const char* pf, int32 pl, const char* pfunc)
	{
		strcpy(f, pf);
		strcpy(func, pfunc);
		l = pl;
	}

	cpps_std_vector* cpps_stack::createparams()
	{
		return CPPSNEW( cpps_std_vector)();
	}

	

}