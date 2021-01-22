#ifndef CPPS_COROUTINE_H_
#define CPPS_COROUTINE_H_

#ifndef STACK_LIMIT
#define STACK_LIMIT (1024*1024)
#endif
#include "cpps_def.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cassert>

#include <string>
#include <vector>
#include <list>
#include <thread>
#include <future>

#include <chrono>
#include <functional>

using ::std::string;
using ::std::wstring;

#ifdef _MSC_VER
#include <Windows.h>

#include <thread>
//#if _MSVC_LANG > 201799L
//#include <coroutine>
//#else
//#include <experimental/coroutine>
//#endif
#else
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <ucontext.h>
#else
#include <ucontext.h>
#endif
#endif
namespace cpps {
	namespace coroutine {

	typedef cpps::usint64 routine_t;
#ifdef _MSC_VER

		struct Routine
		{
			std::function<void()> func;
			bool finished;
			LPVOID fiber;

			Routine(std::function<void()> f)
			{
				func = f;
				finished = false;
				fiber = nullptr;
			}

			~Routine()
			{
				DeleteFiber(fiber);
			}
		};

		struct Ordinator
		{
			std::vector<Routine*> routines;
			std::list<routine_t> indexes;
			routine_t current;
			size_t stack_size;
			LPVOID fiber;
			bool	isterminate;

			Ordinator(size_t ss = STACK_LIMIT)
			{
				isterminate = false;
				current = 0;
				stack_size = ss;
				fiber = ConvertThreadToFiber(nullptr);
			}

			~Ordinator()
			{
				for (auto& routine : routines)
					delete routine;
			}
		};

		

		inline routine_t create(Ordinator &ordinator,std::function<void()> f)
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

		inline void destroy(Ordinator& ordinator, routine_t id)
		{
			Routine* routine = ordinator.routines[size_t(id - 1)];
			assert(routine != nullptr);

			delete routine;
			ordinator.routines[size_t(id - 1)] = nullptr;
			ordinator.indexes.push_back(id);
		}

		inline void __stdcall entry( LPVOID lpParameter)
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

		inline int resume(Ordinator& ordinator, routine_t id)
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

		inline void yield(Ordinator& ordinator)
		{
			routine_t id = ordinator.current;
			Routine* routine = ordinator.routines[size_t(id - 1)];
			assert(routine != nullptr);

			ordinator.current = 0;
			SwitchToFiber(ordinator.fiber);
		}

		inline routine_t current(Ordinator& ordinator)
		{
			return ordinator.current;
		}

		
		template<typename Function, class... _ArgTypes>
		inline std::_Invoke_result_t<std::decay_t<Function>, std::decay_t<_ArgTypes>...>
			wait_for(Ordinator* ordinator,Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func,_Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(0));

			while (status == std::future_status::timeout && !ordinator->isterminate)
			{
				if (ordinator->current != 0)
					yield(*ordinator);

				status = future.wait_for(std::chrono::milliseconds(0));
			}
			return future.get();
		}

#else

		struct Routine
		{
			std::function<void()> func;
			char* stack;
			bool finished;
			ucontext_t ctx;

			Routine(std::function<void()> f)
			{
				func = f;
				stack = nullptr;
				finished = false;
			}

			~Routine()
			{
				delete[] stack;
			}
		};

		struct Ordinator
		{
			std::vector<Routine*> routines;
			std::list<routine_t> indexes;
			routine_t current;
			size_t stack_size;
			bool	isterminate;
			ucontext_t ctx;

			inline Ordinator(size_t ss = STACK_LIMIT)
			{
				isterminate = false;
				current = 0;
				stack_size = ss;
			}

			inline ~Ordinator()
			{
				for (auto& routine : routines)
					delete routine;
			}
		};


		inline routine_t create(Ordinator& ordinator, std::function<void()> f)
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

		inline void destroy(Ordinator& ordinator, routine_t id)
		{
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			delete routine;
			ordinator.routines[id - 1] = nullptr;
		}

		inline void entry(Ordinator* ordinator)
		{
			routine_t id = ordinator->current;
			Routine* routine = ordinator->routines[id - 1];
			routine->func();

			routine->finished = true;
			ordinator->current = 0;
			ordinator->indexes.push_back(id);
		}

		inline int resume(Ordinator& ordinator, routine_t id)
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
				makecontext(&routine->ctx, reinterpret_cast<void (*)()>(entry),1, &ordinator);

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

		inline void yield(Ordinator& ordinator)
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

		inline routine_t current(Ordinator& ordinator)
		{
			return ordinator.current;
		}

		template<typename Function, class... _ArgTypes>
		inline typename std::result_of<Function(_ArgTypes...)>::type
			wait_for(Ordinator* ordinator, Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func,_Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(0));

			while (status == std::future_status::timeout && !ordinator->isterminate)
			{
				if (ordinator->current != 0)
					yield(*ordinator);

				status = future.wait_for(std::chrono::milliseconds(0));
			}
			return future.get();
		}

#endif



	}
}
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic pop
#endif
#endif //CPPS_COROUTINE_H_