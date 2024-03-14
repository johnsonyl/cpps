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

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IRTransformLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/OptimizationLevel.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include <memory>

#include <iostream>
#include <utility>
#include <vector>
#include <initializer_list>
#include "cpps_jit_data.h"
#include "cpps_jit_builder_runtime.h"
#include "cpps_jit_context.h"
#include "cpps_jit_function.h"


using namespace llvm;
using namespace llvm::orc;
namespace cpps {
	class cpps_jit_codegen
	{
	public:
		cpps_jit_codegen();
		~cpps_jit_codegen();

		void				init();
		ThreadSafeModule	build(cpps_jit_data* __moduledata, std::string& __funcname, cpps::C* c, cpps::cpps_domain* domain, cpps::node* _params, cpps::node* _context);
		void				optimize_func(Function* F);
		ThreadSafeModule	optimize_module(ThreadSafeModule module);
		int32				compile(cpps_jit_data* __moduledata, cpps::C* c, cpps::cpps_domain* domain, cpps::node* _params, cpps::node* _context);
		void				run(cpps::C* c, cpps::cpps_domain* domain, cpps::cpps_stack* stack);
	private:
		void				gen(DEFAULT_VARS, cpps::node* _context);
		void				step(DEFAULT_VARS, cpps::node* _step);
        void				expression(DEFAULT_VARS, node* _exp, Value* ret = NULL);
        void				func_regvar(DEFAULT_VARS, node* var, Value* v);
		Value*				createvar(DEFAULT_VARS, cpps::node* _step);
		Value*				foreach_createvar(DEFAULT_VARS, cpps::node* _step);
	private:
		_cpps_jit_context* __context;

		llvm::PassBuilder passBuilder;
		llvm::LoopAnalysisManager loopAnalysisManager; // true is just to output debug info
		llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
		llvm::ModuleAnalysisManager moduleAnalysisManager;
		llvm::FunctionAnalysisManager functionAnalysisManager;

	};

}

