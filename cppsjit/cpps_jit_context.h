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


typedef void (*AddFunctionType)(cpps::C* c, cpps::cpps_domain* domain, cpps::cpps_stack* stake);

namespace cpps
{
    struct _cpps_jit_context
    {
        AddFunctionType __func;
    };
}