#include "cpps.h"

namespace cpps
{
	void C::push(Node *_o)
	{
		o = _o;
	}

	void C::push_stack(cpps_stack* stack)
	{
		if (_callstack[GetCurrentThreadId()] == NULL)
		{
			_callstack[GetCurrentThreadId()] = new std::vector<cpps_stack*>;
		}
		_callstack[GetCurrentThreadId()]->push_back(stack);
	}

	void C::pop_stack()
	{
		_callstack[GetCurrentThreadId()]->pop_back();
	}

	std::vector<cpps_stack*>* C::getcallstack()
	{
		return _callstack[GetCurrentThreadId()];
	}
	std::unordered_set<cpps_regvar*>* C::getBarrierList(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (barrierList[tid] == NULL)
		{
			barrierList[tid] = new std::unordered_set<cpps_regvar*>;
		}
		return barrierList[tid];
	}
	std::unordered_set<cpps_cppsclassvar *>* C::getGen0(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen0[tid] == NULL)
		{
			gen0[tid] = new std::unordered_set<cpps_cppsclassvar *>;
		}
		return gen0[tid];
	}

	std::unordered_set<cpps_cppsclassvar *>* C::getGen1(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen1[tid] == NULL)
		{
			gen1[tid] = new std::unordered_set<cpps_cppsclassvar *>;
		}
		return gen1[tid];
	}
	void		C::setGen0size(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen0size.find(tid) == gen0size.end())
		{
			gen0size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		gen0size[tid] = s;
	}
	size_t		C::getGen0size(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen0size.find(tid) == gen0size.end())
		{
			gen0size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		return gen0size[tid];
	}

	void		C::setGen1size(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen1size.find(tid) == gen1size.end())
		{
			gen1size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		gen1size[tid] = s;
	}
	size_t		C::getGen1size(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (gen1size.find(tid) == gen1size.end())
		{
			gen1size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		return gen1size[tid];
	}
	void		C::setLastgensize(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (lastgensize.find(tid) == lastgensize.end())
		{
			lastgensize.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		lastgensize[tid] = s;
	}
	size_t		C::getLastgensize(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		if (lastgensize.find(tid) == lastgensize.end())
		{
			lastgensize.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		return lastgensize[tid];
	}
	C::C()
	{
		gen0size.clear();
		gen1size.clear();
		lastgensize.clear();
		func = NULL;
		debug = false;
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
		if (_callstack.find(GetCurrentThreadId()) == _callstack.end()) return;

		for (int i = 0; i < _callstack[GetCurrentThreadId()]->size(); i++)
		{
			cpps_stack* stack = _callstack[GetCurrentThreadId()]->at(i);
			delete stack;
		}
		_callstack[GetCurrentThreadId()]->clear();
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