#include "cpps.h"

namespace cpps
{
	void C::push(Node *_o)
	{
		o = _o;
	}

	void C::push_stack(cpps_stack* stack)
	{
		stacklock.lock();
		if (_callstack[GetCurrentThreadId()] == NULL)
		{
			_callstack[GetCurrentThreadId()] = new std::vector<cpps_stack*>;
		}
		_callstack[GetCurrentThreadId()]->push_back(stack);
		stacklock.unlock();
	}

	void C::pop_stack()
	{
		stacklock.lock();
		_callstack[GetCurrentThreadId()]->pop_back();
		stacklock.unlock();
	}

	std::vector<cpps_stack*>* C::getcallstack()
	{
		stacklock.lock();
		std::vector<cpps_stack*>* ret = _callstack[GetCurrentThreadId()];
		stacklock.unlock();
		return ret;
	}
	std::unordered_set<cpps_regvar*>* C::getBarrierList(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}

		stacklock.lock();
		if (barrierList[tid] == NULL)
		{
			barrierList[tid] = new std::unordered_set<cpps_regvar*>;
		}

		std::unordered_set<cpps_regvar*>*ret = barrierList[tid];
		stacklock.unlock();
		return ret;
	}
	std::unordered_set<cpps_cppsclassvar *>* C::getGen0(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen0[tid] == NULL)
		{
			gen0[tid] = new std::unordered_set<cpps_cppsclassvar *>;
		}
		std::unordered_set<cpps_cppsclassvar*>*ret = gen0[tid];
		stacklock.unlock();
		return ret;
	}

	std::unordered_set<cpps_cppsclassvar *>* C::getGen1(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen1[tid] == NULL)
		{
			gen1[tid] = new std::unordered_set<cpps_cppsclassvar *>;
		}
		std::unordered_set<cpps_cppsclassvar*>* ret = gen1[tid];
		stacklock.unlock();
		return ret;
	}
	void		C::setGen0size(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen0size.find(tid) == gen0size.end())
		{
			gen0size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		gen0size[tid] = s;
		stacklock.unlock();
	}
	size_t		C::getGen0size(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen0size.find(tid) == gen0size.end())
		{
			gen0size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		size_t ret = gen0size[tid];
		stacklock.unlock();
		return ret;
	}

	void		C::setGen1size(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen1size.find(tid) == gen1size.end())
		{
			gen1size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		gen1size[tid] = s;
		stacklock.unlock();
	}
	size_t		C::getGen1size(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (gen1size.find(tid) == gen1size.end())
		{
			gen1size.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		size_t ret = gen1size[tid];
		stacklock.unlock();
		return ret;
	}
	void		C::setLastgensize(size_t s, int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (lastgensize.find(tid) == lastgensize.end())
		{
			lastgensize.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		lastgensize[tid] = s;
		stacklock.unlock();
	}
	size_t		C::getLastgensize(int tid)
	{
		if (tid == 0)
		{
			tid = GetCurrentThreadId();
		}
		stacklock.lock();
		if (lastgensize.find(tid) == lastgensize.end())
		{
			lastgensize.insert(std::unordered_map<int, size_t>::value_type(tid, 0));
		}
		size_t ret = lastgensize[tid];
		stacklock.unlock();
		return ret;
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
		stacklock.lock();
		if (_callstack.find(GetCurrentThreadId()) == _callstack.end()) return;

		for (int i = 0; i < _callstack[GetCurrentThreadId()]->size(); i++)
		{
			cpps_stack* stack = _callstack[GetCurrentThreadId()]->at(i);
			delete stack;
		}
		_callstack[GetCurrentThreadId()]->clear();
		stacklock.unlock();
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