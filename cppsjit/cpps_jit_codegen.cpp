#include "cpps_jit_codegen.h"



cpps_jit_codegen::cpps_jit_codegen() {
    __context = NULL;
}

cpps_jit_codegen::~cpps_jit_codegen()
{
    if (__context) {
        delete __context;
        __context = NULL;
    }
}

void cpps::cpps_jit_codegen::init() {

    passBuilder.registerModuleAnalyses(moduleAnalysisManager);
    passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
    passBuilder.registerFunctionAnalyses(functionAnalysisManager);
    passBuilder.registerLoopAnalyses(loopAnalysisManager);

    // This is the important line:
    passBuilder.crossRegisterProxies(
        loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager, moduleAnalysisManager);

}

ThreadSafeModule cpps::cpps_jit_codegen::build(cpps_jit_data* __moduledata, std::string& __funcname, cpps::C* c, cpps::cpps_domain* domain, cpps::node* _params, cpps::node* _context) {
    auto context = std::make_unique<LLVMContext>();
    IRBuilder<> builder(*context);
    // Create a module
    auto module = std::make_unique<Module>(__funcname, *context);

    __moduledata->object_structType = StructType::create(*context, "object");
    std::vector<Type*> elements;
    elements.push_back(Type::getInt8Ty(*context));
    elements.push_back(Type::getInt64Ty(*context));
    __moduledata->object_structType->setBody(elements);

    std::vector<Constant*> ele_vec;
    ele_vec.push_back(ConstantInt::get(Type::getInt64Ty(*context), 0));
    ele_vec.push_back(ConstantInt::get(Type::getInt64Ty(*context), 0));
    __moduledata->object_nil = ConstantStruct::get(__moduledata->object_structType, ele_vec);


    // Create a function that looks like:
    //   int Add(int a, int b)
    std::vector<Type*> parameters(3, builder.getInt8PtrTy());
    FunctionType* functionType = FunctionType::get(builder.getInt64Ty(), parameters, false);
    Function* function = Function::Create(functionType, GlobalValue::ExternalLinkage, __funcname.c_str(), module.get());
    // Set arguments for the function
    function->getArg(0)->setName("c");
    function->getArg(1)->setName("domain");
    function->getArg(2)->setName("stack");

    // Create a block in the function
    BasicBlock* block = BasicBlock::Create(*context, "entry", function);
    builder.SetInsertPoint(block);

    // Create a new pass manager attached to it.

    Value* _c = function->getArg(0);
    Value* _domain = function->getArg(1);
    Value* _stack = function->getArg(2);

    //1.变量压进堆
    std::vector<Value*> __func_var_list;
    varname_map         __func_var_map;

    cpps_jit_builder_runtime runtime = {
        __moduledata,context,builder,c,domain,_c,_domain,_stack,NULL,&__func_var_map,&__func_var_list,function,create_object(SET_DEFAULT_VARS),create_object(SET_DEFAULT_VARS)
    };


    Value* _leftdomain = create_object(SET_DEFAULT_VARS);
    runtime._leftdomain = _leftdomain;



    if (domain->getidxvarcount() != 0) __func_var_list.resize((size_t)domain->getidxvarcount());
    size_t _stack_index = 0;
    for (size_t i = 0; i < _params->l.size(); i++)
    {

        node* d = _params->l[i];
        node* varname = d->l[0];
        if (varname->type == CPPS_VARNAME)
        {
            Value* bPtr = create_object(SET_DEFAULT_VARS);// builder.CreateAlloca(builder.getPtrTy(sizeof(cpps::object)), nullptr, varname->s);
            bPtr->setName(varname->s);
            //__func_var_list[_stack_index++] = bPtr;
            //__func_var_map[varname->s] = bPtr;

            if (varname->offsettype == CPPS_OFFSET_TYPE_SELF)
            {
                func_regvar(SET_DEFAULT_VARS, varname, bPtr);
                //funcdomain.regidxvar(varname->offset, v);
            }

        }
    }

    dump(_context);
    gen(SET_DEFAULT_VARS, _context);


    Value* result = builder.getInt64(0);
    builder.CreateRet(result);

    // Print the IR
    verifyFunction(*function);
    module->print(outs(), nullptr);

    //optimize_func(function);

    ModulePassManager MPM = passBuilder.buildModuleOptimizationPipeline(OptimizationLevel::O3, llvm::ThinOrFullLTOPhase::None);

    // Optimize the IR!

    MPM.run(*module, moduleAnalysisManager);

    module->print(outs(), nullptr);



    return ThreadSafeModule(std::move(module), std::move(context));
}


void cpps::cpps_jit_codegen::optimize_func(Function* F)
{

    auto FPM2 = passBuilder.buildFunctionSimplificationPipeline(llvm::OptimizationLevel::O2,llvm::ThinOrFullLTOPhase::None);
   /* FPM2.addPass(createPromoteMemoryToRegisterPass());
    FPM2.addPass(createInstructionCombiningPass());
    FPM2.addPass(createReassociatePass());
    FPM2.addPass(createGVNPass());
    FPM2.addPass(createCFGSimplificationPass());*/

    // Run the optimizations over all functions in the module being added to
    // the JIT.
    FPM2.run(*F, functionAnalysisManager);

}
ThreadSafeModule cpps::cpps_jit_codegen::optimize_module(ThreadSafeModule TSM)
{
    TSM.withModuleDo([](Module& M) {


        // Create a function pass manager.
        auto FPM = std::make_unique<legacy::FunctionPassManager>(&M);

        // Add some optimizations.
        FPM->add(createPromoteMemoryToRegisterPass());
        FPM->add(createInstructionCombiningPass());
        FPM->add(createReassociatePass());
        FPM->add(createGVNPass());
        FPM->add(createCFGSimplificationPass());
        FPM->doInitialization();
        // Run the optimizations over all functions in the module being added to
        // the JIT.
        for (auto& F : M)
            FPM->run(F);

        



        });

    return std::move(TSM);
}
int32 cpps::cpps_jit_codegen::compile(cpps_jit_data* __moduledata, cpps::C* c, cpps::cpps_domain* domain, cpps::node* _params, cpps::node* _context) {
    ExitOnError ExitOnErr;
    std::string __func_name = cpps_jit_buildlambda(c);
    auto __module = build(__moduledata, __func_name, c, domain, _params, _context);
   /* auto __module2 = optimize_module(std::move(__module));

    __module2.getModuleUnlocked()->print(outs(), nullptr);*/

    ExitOnErr(__moduledata->jit->addIRModule(std::move(__module)));
    auto functionSymbol = ExitOnErr(__moduledata->jit->lookup(__func_name.c_str()));
    __context = new (_cpps_jit_context)();
    __context->__func = (AddFunctionType)functionSymbol.getValue();
    return 0;
}

void cpps::cpps_jit_codegen::gen(DEFAULT_VARS, cpps::node* _context) {
    for (size_t i = 0; i < _context->l.size(); i++) {
        node* _step = _context->l[i];
        if (_step == NULL) continue;
        step(SET_DEFAULT_VARS, _step);
    }
}

void cpps::cpps_jit_codegen::step(DEFAULT_VARS, cpps::node* _step)
{
    switch (_step->type) {
    case CPPS_OEXPRESSION:
    {
        expression(SET_DEFAULT_VARS, _step->getleft());
        break;
    }
    case CPPS_ODEFVAR:
    {
        createvar(SET_DEFAULT_VARS, _step);
        break;
    }
    case CPPS_OIF:
    {
        Value* if_value = create_object(SET_DEFAULT_VARS);
        expression(SET_DEFAULT_VARS, _step->l[0]->l[0], if_value);
        BasicBlock* thenBlock = BasicBlock::Create(*runtime.context, "if.then", runtime.function);
        BasicBlock* elseBlock = BasicBlock::Create(*runtime.context, "if.else", runtime.function);
        BasicBlock* returnBlock = BasicBlock::Create(*runtime.context, "if.end", runtime.function);

        Value* condition = cpps_jit_to_bool(SET_DEFAULT_VARS, if_value);
        //if(tobool(if_value))
        runtime.builder.CreateCondBr(condition, thenBlock, elseBlock);
        runtime.builder.SetInsertPoint(thenBlock);
        //{
        if (!_step->l[1]->l.empty()) gen(SET_DEFAULT_VARS, _step->l[1]);

        //}
        runtime.builder.CreateBr(returnBlock);

        //else
        runtime.builder.SetInsertPoint(elseBlock);
        //{
        if (!_step->l[2]->l.empty()) gen(SET_DEFAULT_VARS, _step->l[2]);
        //}
        runtime.builder.CreateBr(returnBlock);

        runtime.builder.SetInsertPoint(returnBlock); //end if


        break;
    }
    case CPPS_OFOREACH:
    {
        node* for1 = _step->l[0];
        node* for2 = _step->l[1];
        node* for4 = _step->l[2];
        //1.map
        // auto it = object::map(v)::begin();
        // auto end = object::map(v)::end();
        // int8 type = 1;
        // while( condition(it , end, i)){
        //   ..dosomthing.
        //   inc(it,type);
        // }
        //2.vector
        //3.set
        //4.range


        BasicBlock* forConditionBlock = BasicBlock::Create(*runtime.context, "for.condition", runtime.function);
        BasicBlock* forBodyBlock = BasicBlock::Create(*runtime.context, "for.body", runtime.function);
        BasicBlock* forIncrementBlock = BasicBlock::Create(*runtime.context, "for.increment", runtime.function);
        BasicBlock* forEndBlock = BasicBlock::Create(*runtime.context, "for.end", runtime.function);
        Value* _iPtr = foreach_createvar(SET_DEFAULT_VARS, for1);
        Value* itPtr = create_object(SET_DEFAULT_VARS);
        Value* endPtr = create_object(SET_DEFAULT_VARS);
        Value* vPtr = create_object(SET_DEFAULT_VARS);
        expression(SET_DEFAULT_VARS, for2->l[0], vPtr);
        Value* typePtr = cpps_jit_foreach_type(SET_DEFAULT_VARS, vPtr);
        cpps_jit_foreach_begin(SET_DEFAULT_VARS, vPtr, itPtr, _iPtr, typePtr);
        cpps_jit_foreach_end(SET_DEFAULT_VARS, vPtr, endPtr, typePtr);


        runtime.builder.CreateBr(forConditionBlock);

        runtime.builder.SetInsertPoint(forConditionBlock);

        Value* forConditionCompare = cpps_jit_foreach_condition(SET_DEFAULT_VARS, vPtr, itPtr, endPtr, typePtr);

        runtime.builder.CreateCondBr(forConditionCompare, forBodyBlock, forEndBlock);


        runtime.builder.SetInsertPoint(forBodyBlock);
        //body
        cpps_jit_foreach_set_value(SET_DEFAULT_VARS, itPtr, _iPtr, typePtr);


        if (!for4->l.empty()) gen(SET_DEFAULT_VARS, for4);

        runtime.builder.CreateBr(forIncrementBlock);

        runtime.builder.SetInsertPoint(forIncrementBlock);
        //inc
        cpps_jit_foreach_inc(SET_DEFAULT_VARS, itPtr, typePtr);

        runtime.builder.CreateBr(forConditionBlock);


        runtime.builder.SetInsertPoint(forEndBlock);

        break;
    }
    case CPPS_OFOR:
    {

        node* for1 = _step->l[0];
        node* for2 = _step->l[1];
        node* for3 = _step->l[2];
        node* for4 = _step->l[3];

        //BasicBlock* forConditionBlock = BasicBlock::Create(*runtime.context, "for.condition", runtime.function);
        //BasicBlock* forBodyBlock = BasicBlock::Create(*runtime.context, "for.body", runtime.function);
        //BasicBlock* forIncrementBlock = BasicBlock::Create(*runtime.context, "for.increment", runtime.function);
        //BasicBlock* forEndBlock = BasicBlock::Create(*runtime.context, "for.end", runtime.function);

        //Value* iPtr = cpps_jit_createvar(SET_DEFAULT_VARS, for1);
        //runtime.builder.CreateBr(forConditionBlock);

        //runtime.builder.SetInsertPoint(forConditionBlock);

        //Value* forConditionCompare = cpps_jit_to_bool(SET_DEFAULT_VARS, if_value);

        //runtime.builder.CreateCondBr(forConditionCompare, forBodyBlock, forEndBlock);


        //runtime.builder.SetInsertPoint(forBodyBlock);
        ////body
        //runtime.builder.CreateBr(forIncrementBlock);

        //runtime.builder.SetInsertPoint(forIncrementBlock);
        ////inc
        //runtime.builder.CreateBr(forConditionBlock);


        //runtime.builder.SetInsertPoint(forEndBlock);


        // Add some basic blocks to the function
        BasicBlock* forConditionBlock = BasicBlock::Create(*runtime.context, "for.condition", runtime.function);
        BasicBlock* forBodyBlock = BasicBlock::Create(*runtime.context, "for.body", runtime.function);
        BasicBlock* forIncrementBlock = BasicBlock::Create(*runtime.context, "for.increment", runtime.function);
        BasicBlock* forEndBlock = BasicBlock::Create(*runtime.context, "for.end", runtime.function);


        // Fill the "entry" block (1):
        //   int b = 0;

        // Fill the "entry" block (2):
        //   for (int i = 0; ...)
        Value* iPtr = runtime.builder.CreateAlloca(runtime.builder.getInt64Ty(), nullptr, "i.address");
        runtime.builder.CreateStore(runtime.builder.getInt64(0), iPtr);

        Value* a = runtime.builder.CreateAlloca(runtime.builder.getInt64Ty(), nullptr, "a.address");
        runtime.builder.CreateStore(runtime.builder.getInt64(100000000), a);

        runtime.builder.CreateBr(forConditionBlock);

        // Fill the "for.condition" block:
        //   for (... i < a; ...)
        runtime.builder.SetInsertPoint(forConditionBlock);
        Value* i0 = runtime.builder.CreateLoad(runtime.builder.getInt64Ty(), iPtr);
        Value* i1 = runtime.builder.CreateLoad(runtime.builder.getInt64Ty(), a);
        Value* forConditionCompare = runtime.builder.CreateICmpSLT(i0, i1, "for.condition.compare.result");
        runtime.builder.CreateCondBr(forConditionCompare, forBodyBlock, forEndBlock);

        // Fill the "for.body" block:
        //   b = b + i;
        runtime.builder.SetInsertPoint(forBodyBlock);

        if (!for4->l.empty()) gen(SET_DEFAULT_VARS, for4);

        runtime.builder.CreateBr(forIncrementBlock);

        // Fill the "for.increment" block:
        //   for (... i++)
        runtime.builder.SetInsertPoint(forIncrementBlock);
        Value* i2 = runtime.builder.CreateLoad(runtime.builder.getInt64Ty(), iPtr);
        Value* incrementedI = runtime.builder.CreateAdd(i2, runtime.builder.getInt64(1), "i.incremented");
        runtime.builder.CreateStore(incrementedI, iPtr);
        runtime.builder.CreateBr(forConditionBlock);

        // Fill the "for.end" block:
        //   return b;
        runtime.builder.SetInsertPoint(forEndBlock);

        break;
    }
    }
}

void cpps::cpps_jit_codegen::run(cpps::C* c, cpps::cpps_domain* domain, cpps::cpps_stack* stack) {
    __context->__func(c, domain, stack);
}

void cpps::cpps_jit_codegen::expression(DEFAULT_VARS, node* _exp, Value* ret)
{
    switch (_exp->type) {
    case CPPS_ODOFUNCTION:
    {
        node* _funcname = _exp->getleft();
        node* _params = _exp->getright();

        Value* _func = runtime.temp_func;
        expression(SET_DEFAULT_VARS, _funcname, _func);
        Value* _args = NULL;
        _args = DEF_OBJECT_ARRAY(_params->l.size() == 0 ? 1 : _params->l.size());

        for (uint64_t i = 0; i < _params->l.size(); i++) {
            node* _p = _params->l[i];
            Value* _v = OBJECT_ARRAY(_args, i);
            Value* _vv = create_object(SET_DEFAULT_VARS);
            expression(SET_DEFAULT_VARS, _p, _vv);
            cpps_jit_set_value(SET_DEFAULT_VARS, _v, _vv, BOOLVALUE(SET_DEFAULT_VARS, true));
        }

        if (ret == NULL)
            cpps_jit_function_caller(SET_DEFAULT_VARS, _func, _args, INTVALUE(SET_DEFAULT_VARS, _params->l.size()));
        else
            cpps_jit_function_caller_ret(SET_DEFAULT_VARS, _func, _args, INTVALUE(SET_DEFAULT_VARS, _params->l.size()), ret);

        break;
    }
    case CPPS_VARNAME:
    {
        Value* _varname = runtime.builder.CreateGlobalStringPtr(_exp->s);
        cpps_jit_try_get_Var(SET_DEFAULT_VARS, _varname, ret);
        break;
    }
    case CPPS_OINTEGER16:
    case CPPS_OINTEGER:
    {
        Value* v = INTVALUE(SET_DEFAULT_VARS, _exp->value.integer);
        cpps_jit_set_value_integer(SET_DEFAULT_VARS, ret, v);
        break;
    }
    case CPPS_OUINTEGER:
    {
        Value* v = INTVALUE(SET_DEFAULT_VARS, _exp->value.uinteger);
        cpps_jit_set_value_uinteger(SET_DEFAULT_VARS, ret, v);
        break;
    }
    case CPPS_ONUMBER:
    {
        Value* v = NUMBERVALUE(SET_DEFAULT_VARS, _exp->value.number);
        cpps_jit_set_value_number(SET_DEFAULT_VARS, ret, v);
        break;
    }
    case CPPS_OBOOL:
    {
        Value* v = BOOLVALUE(SET_DEFAULT_VARS, _exp->s[0] == 't');
        cpps_jit_set_value_bool(SET_DEFAULT_VARS, ret, v);
        break;
    }
    case CPPS_QUOTEOFFSET:
    case CPPS_OOFFSET:
    {
        if (_exp->offsettype == CPPS_OFFSET_TYPE_SELF) {
            Value* v = runtime.__func_var_list->at(_exp->offset);
            //暂时没处理const
            cpps_jit_set_value(SET_DEFAULT_VARS, ret, v, BOOLVALUE(SET_DEFAULT_VARS, true));
        }
        break;
    }
    case CPPS_FUNCNAME:
    {
        node* _tmp = _exp->l[0];
        node* _tmp2 = _exp->l.size() > 1 ? _exp->l[1] : NULL;
        Value* a = runtime.temp_func;
        expression(SET_DEFAULT_VARS, _tmp, a);
        if (ret == NULL) {
            ret = runtime.non_ret;
        }
        switch (_exp->symbol->symboltype)
        {
        case CPPS_SYMBOL_TYPE_RIGHTAUTOINCREASE:
        {
            cpps_jit_symbol_rightautoincrease(SET_DEFAULT_VARS, a, ret);
            break;
        }
        case CPPS_SYMBOL_TYPE_EQUEL:
        {
            Value* b = create_object(SET_DEFAULT_VARS);
            expression(SET_DEFAULT_VARS, _tmp2, b);
            cpps_jit_symbol_equel(SET_DEFAULT_VARS, a, b, ret);

            break;
        }
        case CPPS_SYMBOL_TYPE_SUBTRACT:
        {
            Value* b = create_object(SET_DEFAULT_VARS);
            expression(SET_DEFAULT_VARS, _tmp2, b);
            cpps_jit_symbol_subtract(SET_DEFAULT_VARS, a, b, ret);
            break;
        }
        default:
            break;
        }

        break;
    }
    }
}

void cpps::cpps_jit_codegen::func_regvar(DEFAULT_VARS, node* var, Value* v)
{
    if (var->offsettype == CPPS_OFFSET_TYPE_SELF)
        (*runtime.__func_var_list)[var->offset] = v;
    (*runtime.__func_var_map)[var->s] = v;
}

Value* cpps::cpps_jit_codegen::createvar(DEFAULT_VARS, cpps::node* _step)
{
    Value* _first = NULL;
    for (size_t i = 0; i < _step->l.size(); ++i) {
        node* varName = _step->l[i];
        if (varName->type == CPPS_MULTIVAR) {

        }
        else if (varName->type == CPPS_VARNAME) {
            node* var = varName->getleft();
            if (var && (var->type == CPPS_ODEFVAR_VAR || var->type == CPPS_ODEFVAR_NIL)) {
                Value* v = create_object(SET_DEFAULT_VARS);
                v->setName(varName->s);
                func_regvar(SET_DEFAULT_VARS, varName, v);
                if (_first == NULL) _first = v;
                //const  -- ??
                //closeure -- ??
                if (var->type == CPPS_ODEFVAR_VAR) {
                    Value* _value = create_object(SET_DEFAULT_VARS);
                    expression(SET_DEFAULT_VARS, var->l[0], _value);
                    if (varName->quote) {
                        cpps_jit_set_value(SET_DEFAULT_VARS, v, _value, BOOLVALUE(SET_DEFAULT_VARS, varName->quote));
                    }
                    else {
                        cpps_jit_set_value(SET_DEFAULT_VARS, v, _value, BOOLVALUE(SET_DEFAULT_VARS, varName->quote));
                    }
                }
            }
        }
    }
    return _first;
}

Value* cpps::cpps_jit_codegen::foreach_createvar(DEFAULT_VARS, cpps::node* _step) {
    Value* _first = NULL;
    for (auto _n : _step->l) {
        Value* _take = createvar(SET_DEFAULT_VARS, _n);
        if (_first == NULL) _first = _take;
    }
    return _first;
}
