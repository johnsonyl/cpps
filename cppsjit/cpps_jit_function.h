#pragma once
using namespace cpps;
using namespace llvm;
using namespace llvm::orc;
inline
void testprint(cpps::cpps_value *v)
{
    v->tt = 0;
    v->value.b = 0;
    printf("testprint - hello world\n");
}
template<typename T>
Value* CXX2Value(IRBuilder<>& builder, std::unique_ptr<LLVMContext>& context, T _f) {
    
    llvm::ConstantInt* addressConstant = llvm::ConstantInt::get(*context, llvm::APInt(sizeof(std::uintptr_t) * 8, reinterpret_cast<uint64_t>(_f), false));
    llvm::Value* _v = builder.CreateIntToPtr(addressConstant, llvm::Type::getInt8PtrTy(*context));
    return _v;
}
#define CVALUE runtime._c
#define DOMAINVALUE runtime._domain
#define STACKVALUE runtime._stack
#define LEFTDOMAINVALUE runtime._leftdomain
#define _CFV(v) CXX2Value(runtime.builder,runtime.context,v)
#define _CFFC(ft,v) FunctionCallee(ft,_CFV(v)) 
#define FT_VOID FunctionType::get(runtime.builder.getVoidTy(), false)
#define OBJECT_ARRAY(_args,i) runtime.builder.CreateConstGEP1_64(runtime.__moduledata->object_structType, _args,  i)
#define DEF_OBJECT_ARRAY(size) runtime.builder.CreateAlloca(runtime.__moduledata->object_structType,INTVALUE(SET_DEFAULT_VARS,size))

inline FunctionType* make_fty(Type* ret, std::initializer_list<Type*>& li)
{
    std::vector<Type*> parameters = li;
    FunctionType* functionType = FunctionType::get(ret, parameters, false);
    return functionType;
}

#define DEFAULT_VARS cpps_jit_builder_runtime &runtime
#define SET_DEFAULT_VARS runtime

template<typename T>
inline FunctionCallee CREATE_FUNCTION(DEFAULT_VARS, T func, Type* ret, std::initializer_list<Type*> vars) { return _CFFC(make_fty(ret, vars), func); }
#define _VOIDTY runtime.builder.getVoidTy()
#define _PTRTY runtime.builder.getInt8PtrTy()
#define _INTTY runtime.builder.getInt64Ty()
#define _UINTTY runtime.builder.getInt64Ty()
#define _BOOLTY runtime.builder.getInt8Ty()
#define _NUMBERTY runtime.builder.getDoubleTy()
//#define CREATE_OBJECT() cpps_jit_init_object(SET_DEFAULT_VARS, CREATE_VAR(cpps::object))
inline std::string cpps_jit_buildlambda(C* c) {
    std::string	ret = "__jitfunc@" + std::to_string(c->_lambdanum++) + "_";
    int		i;
    int		len = 20;
    for (i = 0; i < len; ++i)
        ret += 'A' + rand() % 26;
    return(ret);
}
#define SPC(spaces) for (auto i = 0; i < spaces; i++) printf("    ");
inline void dump(cpps::node* n, int spaces = 0) {
    SPC(spaces);
    printf("node:type = %d , s = %s\n", n->type, n->s.c_str());
    if (!n->empty()) {
        SPC(spaces);
        printf("[[[[\n");
    }
    for (size_t i = 0; i < n->l.size(); i++) {
        node* _n2 = n->l[i];
        if (_n2) dump(_n2, spaces + 1);
    }
    if (!n->empty()) {
        SPC(spaces);
        printf("]]]]\n");
    }

}
inline Value* create_object(DEFAULT_VARS) {
    Value* alloca_struct = runtime.builder.CreateAlloca(runtime.__moduledata->object_structType);
    runtime.builder.CreateStore(runtime.__moduledata->object_nil, alloca_struct);
    return alloca_struct;
}
template<typename T>
inline Value* INTVALUE(DEFAULT_VARS,T v) {
    return llvm::ConstantInt::get(*runtime.context, llvm::APInt(sizeof(T) * 8, static_cast<T>(v), false));
}
inline Value* NUMBERVALUE(DEFAULT_VARS, cpps_number v) {
    return llvm::ConstantFP::get(*runtime.context, APFloat(v));
}
inline Value* BOOLVALUE(DEFAULT_VARS,bool v) {
    return runtime.builder.getInt8(v);
}
#define CREATE_VAR(_type) runtime.builder.CreateAlloca(runtime.builder.getInt8PtrTy(),INTVALUE(sizeof(_type)))
///

inline void cpp_function_caller(C* c, cpps::object* object, cpps::object** _args,int64 argc )
{

    cpps_std_vector vct;

    cpps_function* func = object->tofunction();
    if (func->getIsNeedC()) {
        cpps::object _c = cpps::object::create(c, c);
        vct.emplace_back(_c.getval());
    }

    for (int i = 0; i < argc; i++) {
        vct.emplace_back(_args[i]->getval());
    }

    function_caller(c, *object, vct);
}

inline
void cpps_jit_function_caller(DEFAULT_VARS, Value* _func, Value* _args, Value* _argc)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_function_caller, _VOIDTY, { _PTRTY,_PTRTY,_PTRTY,_INTTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE, _func,_args ,_argc });
}
inline
void cpp_function_caller_ret(C* c, cpps::object* object, cpps::object** _args, int64 argc, cpps::object* ret)
{

    cpps_std_vector vct;

    cpps_function* func = object->tofunction();
    if (func->getIsNeedC()) {
        cpps::object _c = cpps::object::create(c, c);
        vct.emplace_back(_c.getval());
    }

    for (int i = 0; i < argc; i++) {
        vct.emplace_back(_args[i]->getval());
    }

    *ret = function_caller(c, *object, vct);
}

inline
void cpps_jit_function_caller_ret(DEFAULT_VARS, Value* _func, Value* _args, Value* _argc,Value *ret)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_function_caller_ret, _VOIDTY, { _PTRTY,_PTRTY,_PTRTY,_INTTY,_PTRTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE, _func,_args ,_argc ,ret });
}

inline
void cpp_quote_real(cpps_domain* left, cpps_value& src, cpps_value& tar, bool isconst)
{
    if (src.tt == CPPS_TQUOTECLASSVAR) {
        cpps_classvar_quato* f = src.value.quato;
        tar = f->getvalue(left);
    }
    else if (src.tt == CPPS_TREF) {
        if (isconst) {
            tar = *src.value.value;
        }
        else {
            tar.tt = CPPS_TREF;
            tar.value.value = src.value.value;
        }
    }
    else {
        if (isconst) {
            tar = src;
        }
        else {
            tar.tt = CPPS_TREF;
            tar.value.value = &src;
        }
    }
}
inline
void cpp_try_get_var(C* c,cpps_domain *domain, cpps::object* leftdomain_obj, char *varname, cpps::object* ret)
{
    cpps_domain* &leftdomain = leftdomain_obj->getval().value.domain;

    cpps_regvar* v = NULL;
    if (leftdomain && !leftdomain_obj->isnull())
        v = leftdomain->getvar(varname, leftdomain, true, true);
    else
        v = domain->getvar(varname, leftdomain);

    if (v) {
        cpp_quote_real(leftdomain ? leftdomain : domain, v->getval(), ret->getval(), false);
    }
    else {
        *ret = cpps::nil;
    }
}
inline
void cpps_jit_try_get_Var(DEFAULT_VARS, Value* _varname ,Value *ret)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_try_get_var, _VOIDTY, { _PTRTY,_PTRTY,_PTRTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE, DOMAINVALUE,LEFTDOMAINVALUE,_varname,ret });
}
inline
void cpp_set_value_integer(C*c, cpps::object* ret,cpps_integer v)
{
    *ret = v;
}

inline
void cpps_jit_set_value_integer(DEFAULT_VARS,  Value* ret,Value *v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_set_value_integer, _VOIDTY, { _PTRTY,_PTRTY,_INTTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE,ret,v });
}
inline
void cpp_set_value_uinteger(C*c, cpps::object* ret,cpps_uinteger v)
{
    *ret = v;
}

inline
void cpps_jit_set_value_uinteger(DEFAULT_VARS,  Value* ret,Value *v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_set_value_uinteger, _VOIDTY, { _PTRTY,_PTRTY,_UINTTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE,ret,v });
}
inline
void cpp_set_value_number(C*c, cpps::object* ret,cpps_number v)
{
    *ret = v;
}

inline
void cpps_jit_set_value_number(DEFAULT_VARS,  Value* ret,Value *v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_set_value_integer, _VOIDTY, { _PTRTY,_PTRTY,_NUMBERTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE,ret,v });
}
inline
void cpp_set_value_bool(C*c, cpps::object* ret,bool v)
{
    *ret = cpps::object(v);
}

inline
void cpps_jit_set_value_bool(DEFAULT_VARS,  Value* ret,Value *v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_set_value_bool, _VOIDTY, { _PTRTY,_PTRTY,_BOOLTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE,ret,v });
}

inline
void cpp_set_value(C* c, cpps::object* ret, cpps::object* v,bool quote)
{
    if (quote)
        *ret = v->ref();
    else
        *ret = v->toreal();
}

inline
void cpps_jit_set_value(DEFAULT_VARS,  Value* ret,Value *v ,Value * quote)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_set_value, _VOIDTY, { _PTRTY,_PTRTY,_PTRTY,_BOOLTY });
    runtime.builder.CreateCall(cpp_function_callee, { CVALUE,ret,v ,quote });
}

inline
bool cpp_to_bool(C* c, cpps::object* ret)
{
    return cpps_to_bool(ret->realval());
}

inline
Value* cpps_jit_to_bool(DEFAULT_VARS,  Value* v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_to_bool, _BOOLTY, { _PTRTY,_PTRTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,v});
}


#define SYMBOL_FUNC2(name) inline void CPPS_PP_CAT(cpp_symbol_,name)(C* c, cpps::object* a, cpps::object* ret)\
{\
    CPPS_PP_CAT(cpps_,name)(a->getval(), ret->getval());\
}\
\
inline Value* CPPS_PP_CAT(cpps_jit_symbol_,name)(DEFAULT_VARS, Value* v, Value* ret)\
{\
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &CPPS_PP_CAT(cpp_symbol_,name), _VOIDTY, { _PTRTY,_PTRTY,_PTRTY });\
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE, v , ret});\
}



#define SYMBOL_FUNC(name) inline void CPPS_PP_CAT(cpp_symbol_,name) (C* c, cpps::object* a, cpps::object* b, cpps::object* ret)\
{\
    CPPS_PP_CAT(cpps_,name)(a->realval(),b->realval(),ret->getval());\
}\
\
inline Value* CPPS_PP_CAT(cpps_jit_symbol_,name)(DEFAULT_VARS,  Value* a, Value* b,Value *ret)\
{\
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &CPPS_PP_CAT(cpp_symbol_,name), _VOIDTY, { _PTRTY,_PTRTY,_PTRTY,_PTRTY });\
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE, a , b , ret});\
}


SYMBOL_FUNC2(rightautoincrease)


SYMBOL_FUNC(equel)
SYMBOL_FUNC(subtract)

////


inline
int64 cpp_foreach_type(C* c, cpps::object* ret)
{
    if (ret->ismap()) {
        return 1;
    }
    if (ret->isvector()) {
        return 2;
    }
    if (ret->isset()) {
        return 3;
    }
    if (ret->isrange()) {
        return 4;
    }
    return 0;
}

inline
Value* cpps_jit_foreach_type(DEFAULT_VARS, Value* v)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_type, _INTTY, { _PTRTY,_PTRTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,v });
}
inline
void cpp_foreach_end(C* c, cpps::object* v, cpps::object* retend,int64 type)
{
    if (type == 1) {
        cpps::object::map _v(c,*v);
        *retend = _v.end_obj(c);
    }
    if (type == 2) {
        cpps::object::vector _v( *v);
        *retend = _v.end_obj(c);
    }
    if (type == 3) {
        cpps::object::set _v(c,*v);
        *retend = _v.end_obj(c);
    }
    if (type == 4) {
        cpps_range* range = object_cast<cpps_range*>(*v);
        *retend = cpps::object(range->end);
    }
}

inline
Value* cpps_jit_foreach_end(DEFAULT_VARS, Value* v,Value *retend,Value* type)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_end, _VOIDTY, { _PTRTY,_PTRTY ,_PTRTY ,_INTTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,v,retend,type });
}
inline
void cpp_foreach_begin(C* c, cpps::object* v, cpps::object* retend,cpps::object* _iPtr, int64 type)
{
    if (type == 1) {
        cpps::object::map _v(c, *v);
        *retend = _v.begin_obj(c);
        cpps_create_class_var(cpps_pair, c, cpps_value_iterator, cpps_iterator_ptr);
        *_iPtr = cpps_value_iterator;
    }
    if (type == 2) {
        cpps::object::vector _v(*v);
        *retend = _v.begin_obj(c);
    }
    if (type == 3) {
        cpps::object::set _v(c, *v);
        *retend = _v.begin_obj(c);
    }
    if (type == 4) {
        cpps_range* range = object_cast<cpps_range*>(*v);
        *retend = cpps::object(range->begin);
    }
}

inline
Value* cpps_jit_foreach_begin(DEFAULT_VARS, Value* v, Value* retend, Value* _iPtr, Value* type)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_begin, _VOIDTY, { _PTRTY,_PTRTY ,_PTRTY,_PTRTY ,_INTTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,v,retend,_iPtr,type });
}
inline
bool cpp_foreach_condition(C* c, cpps::object* it, cpps::object* end, int64 type)
{
    if (type == 4) {
        /* cpps_integer _it = it->toint();
         cpps_integer _end = end->toint();*/
         //printf("%d < %d\n", _it, _end);
        return it->value.value.integer < end->value.value.integer;
    }
    else if (type == 1) {
        cpps_hash_map::iterator& _it = *(object_cast<cpps_hash_map::iterator*>(*it));
        cpps_hash_map::iterator& _end = *(object_cast<cpps_hash_map::iterator*>(*end));
        return _it != _end;
    }
    else if (type == 2) {
        cpps_std_vector::iterator& _it = *(object_cast<cpps_std_vector::iterator*>(*it));
        cpps_std_vector::iterator& _end = *(object_cast<cpps_std_vector::iterator*>(*end));
        return _it != _end;
    }
    else if (type == 3) {

        cpps_hash_set::iterator& _it = *(object_cast<cpps_hash_set::iterator*>(*it));
        cpps_hash_set::iterator& _end = *(object_cast<cpps_hash_set::iterator*>(*end));
        return _it != _end;
    }
    return false;
}

inline
Value* cpps_jit_foreach_condition(DEFAULT_VARS, Value* v,Value* it, Value* retend, Value* type)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_condition, _BOOLTY, { _PTRTY,_PTRTY ,_PTRTY ,_INTTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,it, retend,type });
}
inline
void cpp_foreach_inc(C* c,  cpps::object* it,  int64 type)
{
    if (type == 1) {
        cpps_hash_map::iterator& _it = *(object_cast<cpps_hash_map::iterator*>(*it));
        _it++;
    }
    if (type == 2) {
        cpps_std_vector::iterator& _it = *(object_cast<cpps_std_vector::iterator*>(*it));
        _it++;
    }
    if (type == 3) {

        cpps_hash_set::iterator& _it = *(object_cast<cpps_hash_set::iterator*>(*it));
        _it++;
    }
    if (type == 4) {
        it->getval().value.integer++;
    }
}

inline
Value* cpps_jit_foreach_inc(DEFAULT_VARS, Value* it, Value* type)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_inc, _VOIDTY, { _PTRTY,_PTRTY ,_INTTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,it,type });
}
inline
void cpp_foreach_set_value(C* c,  cpps::object* it,cpps::object* _i,  int64 type)
{
    if (type == 1) {
        cpps_hash_map::iterator& _it = *(object_cast<cpps_hash_map::iterator*>(*it));
        cpps_pair* mapnode = object_cast<cpps_pair*>(*_i);
        mapnode->_first = _it->first;
        mapnode->_second = _it->second;

    }
    if (type == 2) {
        cpps_std_vector::iterator& _it = *(object_cast<cpps_std_vector::iterator*>(*it));
        *_i = *it;
    }
    if (type == 3) {
        cpps_hash_set::iterator& _it = *(object_cast<cpps_hash_set::iterator*>(*it));
        *_i = *it;
    }
    if (type == 4) {
        *_i = cpps::object(it->value.value.integer);
    }
}

inline
Value* cpps_jit_foreach_set_value(DEFAULT_VARS, Value* it,Value* _i, Value* type)
{
    FunctionCallee cpp_function_callee = CREATE_FUNCTION(SET_DEFAULT_VARS, &cpp_foreach_set_value, _VOIDTY, { _PTRTY,_PTRTY,_PTRTY ,_INTTY });
    return runtime.builder.CreateCall(cpp_function_callee, { CVALUE,it,_i,type });
}