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

using namespace llvm;
using namespace llvm::orc;


namespace cpps
{
	class cpps_jit_data : public cpps_module_data
	{
	public:

		std::unique_ptr<LLJIT>									jit;
		StructType*												object_structType;
		Constant*												object_nil;
	};
}