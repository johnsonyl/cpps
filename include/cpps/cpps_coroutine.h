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

			Routine(std::function<void()> f);

			~Routine();
		};

		struct Ordinator
		{
			std::vector<Routine*> routines;
			std::list<routine_t> indexes;
			routine_t current;
			size_t stack_size;
			LPVOID fiber;
			bool	isterminate;

			Ordinator(size_t ss = STACK_LIMIT);

			~Ordinator();
		};

		

		routine_t create(Ordinator& ordinator, std::function<void()> f);

		void destroy(Ordinator& ordinator, routine_t id);

		void __stdcall entry(LPVOID lpParameter);

		int resume(Ordinator& ordinator, routine_t id);

		void yield(Ordinator& ordinator);

		routine_t current(Ordinator& ordinator);

		
		template<typename Function, class... _ArgTypes>
		std::_Invoke_result_t<std::decay_t<Function>, std::decay_t<_ArgTypes>...>
			wait_for(Ordinator* ordinator,Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func,_Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(1));

			while (status == std::future_status::timeout && !ordinator->isterminate)
			{
				if (ordinator->current != 0)
					yield(*ordinator);

				status = future.wait_for(std::chrono::milliseconds(1));
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

			Routine(std::function<void()> f);

			~Routine();
		};

		struct Ordinator
		{
			std::vector<Routine*> routines;
			std::list<routine_t> indexes;
			routine_t current;
			size_t stack_size;
			bool	isterminate;
			ucontext_t ctx;

			Ordinator(size_t ss = STACK_LIMIT);

			~Ordinator();
		};


		routine_t create(Ordinator& ordinator, std::function<void()> f);

		void destroy(Ordinator& ordinator, routine_t id);

		void entry(Ordinator* ordinator);

		int resume(Ordinator& ordinator, routine_t id);

		void yield(Ordinator& ordinator);

		routine_t current(Ordinator& ordinator);

		template<typename Function, class... _ArgTypes>
		typename std::result_of<Function(_ArgTypes...)>::type
			wait_for(Ordinator* ordinator, Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func,_Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(1));

			while (status == std::future_status::timeout && !ordinator->isterminate)
			{
				if (ordinator->current != 0)
					yield(*ordinator);

				status = future.wait_for(std::chrono::milliseconds(1));
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