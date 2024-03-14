#pragma once

#include <cpps/cpps.h> //Single Header
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <utility>
#include <vector>
#include <initializer_list>



using namespace llvm;
using namespace llvm::orc;

typedef phmap::flat_hash_map<std::string, Value*> varname_map;

namespace cpps
{
	class cpps_jit_data;

	struct cpps_jit_builder_runtime
	{
		cpps_jit_data* __moduledata;
		std::unique_ptr<LLVMContext>& context;
		IRBuilder<>& builder;
		cpps::C* c;
		cpps::cpps_domain* domain;
		Value* _c;
		Value* _domain;
		Value* _stack;
		Value* _leftdomain;
		varname_map* __func_var_map;
		std::vector<Value*>* __func_var_list;
		Function* function;
		Value* non_ret;
		Value* temp_func;
	};

}