#include "cpps/cpps.h"
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace cpps {
	namespace coroutine {

#ifdef _MSC_VER
	Routine::Routine(std::function<void()> f)
	{
		func = f;
		finished = false;
		fiber = nullptr;
	}
	Routine::~Routine()
	{
		DeleteFiber(fiber);
}

	Ordinator::Ordinator(size_t ss)
	{
		isterminate = false;
		current = 0;
		stack_size = ss;
		fiber = ConvertThreadToFiber(nullptr);
	}

	Ordinator::~Ordinator()
	{
		for (auto& routine : routines)
			delete routine;
	}

	routine_t create(Ordinator& ordinator, std::function<void()> f)
	{
		Routine* routine = new Routine(f);

		if (ordinator.indexes.empty())
		{
			ordinator.routines.push_back(routine);
			return ordinator.routines.size();
		}
		else
		{
			routine_t id = ordinator.indexes.front();
			ordinator.indexes.pop_front();
			assert(ordinator.routines[size_t(id - 1)] == nullptr);
			ordinator.routines[size_t(id - 1)] = routine;
			return id;
		}
	}
	void destroy(Ordinator& ordinator, routine_t id)
	{
		Routine* routine = ordinator.routines[size_t(id - 1)];
		assert(routine != nullptr);

		delete routine;
		ordinator.routines[size_t(id - 1)] = nullptr;
		ordinator.indexes.push_back(id);
	}

	void __stdcall entry(LPVOID lpParameter)
	{
		Ordinator& ordinator = *((Ordinator*)lpParameter);
		routine_t id = ordinator.current;
		Routine* routine = ordinator.routines[size_t(id - 1)];
		assert(routine != nullptr);

		routine->func();

		routine->finished = true;
		ordinator.current = 0;

		SwitchToFiber(ordinator.fiber);
	}
	int resume(Ordinator& ordinator, routine_t id)
	{
		assert(ordinator.current == 0);

		Routine* routine = ordinator.routines[size_t(id - 1)];
		if (routine == nullptr)
			return -1;

		if (routine->finished)
			return -2;

		if (routine->fiber == nullptr)
		{
			routine->fiber = CreateFiber(ordinator.stack_size, entry, &ordinator);
			ordinator.current = id;
			SwitchToFiber(routine->fiber);
		}
		else
		{
			ordinator.current = id;
			SwitchToFiber(routine->fiber);
		}

		return 0;
	}
	void yield(Ordinator& ordinator)
	{
		routine_t id = ordinator.current;
		Routine* routine = ordinator.routines[size_t(id - 1)];
		assert(routine != nullptr);

		ordinator.current = 0;
		SwitchToFiber(ordinator.fiber);
	}
	routine_t current(Ordinator& ordinator)
	{
		return ordinator.current;
	}
#else
	Routine::Routine(std::function<void()> f)
	{
		func = f;
		stack = nullptr;
		finished = false;
	}


	Routine::~Routine()
	{
		delete[] stack;
	}

	Ordinator::Ordinator(size_t ss)
	{
		isterminate = false;
		current = 0;
		stack_size = ss;
	}

	Ordinator::~Ordinator()
	{
		for (auto& routine : routines)
			delete routine;
	}
	routine_t create(Ordinator& ordinator, std::function<void()> f)
	{
		Routine* routine = new Routine(f);

		if (ordinator.indexes.empty())
		{
			ordinator.routines.push_back(routine);
			return ordinator.routines.size();
		}
		else
		{
			routine_t id = ordinator.indexes.front();
			ordinator.indexes.pop_front();
			assert(ordinator.routines[id - 1] == nullptr);
			ordinator.routines[id - 1] = routine;
			return id;
		}
	}
	void destroy(Ordinator& ordinator, routine_t id)
	{
		Routine* routine = ordinator.routines[id - 1];
		assert(routine != nullptr);

		delete routine;
		ordinator.routines[id - 1] = nullptr;
	}
	void entry(Ordinator* ordinator)
	{
		routine_t id = ordinator->current;
		Routine* routine = ordinator->routines[id - 1];
		routine->func();

		routine->finished = true;
		ordinator->current = 0;
		ordinator->indexes.push_back(id);
	}
	int resume(Ordinator& ordinator, routine_t id)
	{
		assert(ordinator.current == 0);

		Routine* routine = ordinator.routines[id - 1];
		if (routine == nullptr)
			return -1;

		if (routine->finished)
			return -2;

		if (routine->stack == nullptr)
		{
			//initializes the structure to the currently active context.
			//When successful, getcontext() returns 0
			//On error, return -1 and set errno appropriately.
			getcontext(&routine->ctx);

			//Before invoking makecontext(), the caller must allocate a new stack
			//for this context and assign its address to ucp->uc_stack,
			//and define a successor context and assign its address to ucp->uc_link.
			routine->stack = new char[ordinator.stack_size];
			routine->ctx.uc_stack.ss_sp = routine->stack;
			routine->ctx.uc_stack.ss_size = ordinator.stack_size;
			routine->ctx.uc_link = &ordinator.ctx;
			ordinator.current = id;

			//When this context is later activated by swapcontext(), the function entry is called.
			//When this function returns, the  successor context is activated.
			//If the successor context pointer is NULL, the thread exits.
			makecontext(&routine->ctx, reinterpret_cast<void (*)()>(entry), 1, &ordinator);

			//The swapcontext() function saves the current context,
			//and then activates the context of another.
			swapcontext(&ordinator.ctx, &routine->ctx);
		}
		else
		{
			ordinator.current = id;
			swapcontext(&ordinator.ctx, &routine->ctx);
		}

		return 0;
	}
	void yield(Ordinator& ordinator)
	{
		routine_t id = ordinator.current;
		Routine* routine = ordinator.routines[id - 1];
		assert(routine != nullptr);
#ifdef DEBUG
		char* stack_top = routine->stack + ordinator.stack_size;
		char stack_bottom = 0;
		assert(size_t(stack_top - &stack_bottom) <= ordinator.stack_size);
#endif
		ordinator.current = 0;
		swapcontext(&routine->ctx, &ordinator.ctx);
	}
	routine_t current(Ordinator& ordinator)
	{
		return ordinator.current;
	}
#endif

	
	}
}
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic pop
#endif