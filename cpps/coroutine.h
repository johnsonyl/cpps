/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
准备实现的功能，但是还在考虑要不要加入到cpps中。 协程的优缺点都很多。

var func1(var p)
{
	println("我在做饭，用时2秒");
	await Sleep(2); //非阻塞 await 会调用 yield 让其他人先运行
	return "做好饭了";
}
var func2(var p)
{
	println("我在烧水，用时3秒");
	await Sleep(3);
	return "烧好水了";
}

var t = time.gettime();
var task1 = coroutine.create(func1,nil);	//创建协程1
var task2 = coroutine.create(func2,nil);	//创建协程2
while(task1.runing()||task2.runing());

println(task1.get());
println(task2.get());
println("总共用时{time.gettime()-t}秒"); // 应该是3秒
*/
#ifndef STDEX_COROUTINE_H_
#define STDEX_COROUTINE_H_

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

using ::std::string;
using ::std::wstring;

#ifdef _MSC_VER
#include <Windows.h>

#include <thread>
#include <experimental/coroutine>
#include <chrono>
#include <functional>
#else
#if defined(__APPLE__) && defined(__MACH__)
#define _XOPEN_SOURCE
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

			Ordinator(size_t ss = STACK_LIMIT)
			{
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

		thread_local static Ordinator ordinator;

		inline routine_t create(std::function<void()> f)
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

		inline void destroy(routine_t id)
		{
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			delete routine;
			ordinator.routines[id - 1] = nullptr;
			ordinator.indexes.push_back(id);
		}

		inline void __stdcall entry(LPVOID lpParameter)
		{
			routine_t id = ordinator.current;
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			routine->func();

			routine->finished = true;
			ordinator.current = 0;

			SwitchToFiber(ordinator.fiber);
		}

		inline int resume(routine_t id)
		{
			assert(ordinator.current == 0);

			Routine* routine = ordinator.routines[id - 1];
			if (routine == nullptr)
				return -1;

			if (routine->finished)
				return -2;

			if (routine->fiber == nullptr)
			{
				routine->fiber = CreateFiber(ordinator.stack_size, entry, 0);
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

		inline void yield()
		{
			routine_t id = ordinator.current;
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			ordinator.current = 0;
			SwitchToFiber(ordinator.fiber);
		}

		inline routine_t current()
		{
			return ordinator.current;
		}

		
		template<typename Function, class... _ArgTypes>
		inline std::_Invoke_result_t<std::decay_t<Function>, std::decay_t<_ArgTypes>...>
			wait_for(Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func,_Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(0));

			while (status == std::future_status::timeout)
			{
				if (ordinator.current != 0)
					yield();

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
			ucontext_t ctx;

			inline Ordinator(size_t ss = STACK_LIMIT)
			{
				current = 0;
				stack_size = ss;
			}

			inline ~Ordinator()
			{
				for (auto& routine : routines)
					delete routine;
			}
		};

		thread_local static Ordinator ordinator;

		inline routine_t create(std::function<void()> f)
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

		inline void destroy(routine_t id)
		{
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			delete routine;
			ordinator.routines[id - 1] = nullptr;
		}

		inline void entry()
		{
			routine_t id = ordinator.current;
			Routine* routine = ordinator.routines[id - 1];
			routine->func();

			routine->finished = true;
			ordinator.current = 0;
			ordinator.indexes.push_back(id);
		}

		inline int resume(routine_t id)
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
				makecontext(&routine->ctx, reinterpret_cast<void (*)(void)>(entry), 0);

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

		inline void yield()
		{
			routine_t id = ordinator.current;
			Routine* routine = ordinator.routines[id - 1];
			assert(routine != nullptr);

			char* stack_top = routine->stack + ordinator.stack_size;
			char stack_bottom = 0;
			assert(size_t(stack_top - &stack_bottom) <= ordinator.stack_size);

			ordinator.current = 0;
			swapcontext(&routine->ctx, &ordinator.ctx);
		}

		inline routine_t current()
		{
			return ordinator.current;
		}

		template<typename Function, class... _ArgTypes>
		inline std::_Invoke_result_t<std::decay_t<Function>, std::decay_t<_ArgTypes>...>
			wait_for(Function&& func, _ArgTypes&&... _Args)
		{
			auto future = std::async(std::launch::async, func, _Args...);
			std::future_status status = future.wait_for(std::chrono::milliseconds(0));

			while (status == std::future_status::timeout)
			{
				if (ordinator.current != 0)
					yield();

				status = future.wait_for(std::chrono::milliseconds(0));
			}
			return future.get();
		}


#endif



	}
}
#endif //STDEX_COROUTINE_H_