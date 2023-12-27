

#include <cpps/cpps.h>
#include <ffi.h>
#include <ffitarget.h>

namespace cpps{
	std::string cpps_rebuild_filepath(std::string path);
}
using namespace cpps;
using namespace std;

typedef void(*PPROC)(void);

ffi_type* _ffi_s_to_type(std::string& s) {
	if (s == "int" || s == "int32" || s == "sint32" ) {
		return &ffi_type_sint32;
	}
	else if (s == "unsigned int" || s == "uint" || s == "uint32" || s == "DWORD") {
		return &ffi_type_uint32; 
	}
	else if (s == "__int64" || s == "long long" || s == "int64" || s == "sint64") {
		return &ffi_type_sint64;
	}
	else if (s == "long long unsigned int" || s == "unsigned long long" || s == "uint64") {
		return &ffi_type_uint64;
	}
	else if (s == "short" || s == "int16" || s == "sint16") {
		return &ffi_type_sint16;
	}
	else if (s == "unsigned short" || s == "ushort" || s == "uint16" || s == "WORD") {
		return &ffi_type_uint16;
	}
	else if (s == "char" || s == "int8" || s == "sint8" || s == "sbyte") {
		return &ffi_type_sint8;
	}
	else if (s == "unsigned char" || s == "uchar" || s == "uint8" || s == "BYTE" ) {
		return &ffi_type_uint8;
	}
	else if (s == "float") {
		return &ffi_type_float;
	}
	else if (s == "double") {
		return &ffi_type_double;
	}
	else if (s == "void") {
		return &ffi_type_void;
	}
	else {
		return &ffi_type_pointer;
	}
}
void _ffi_fill_value(ffi_type* _type,void *__value_ptr,object __v) {
	if (_type == &ffi_type_sint32) {
		int32* p = (int32*)__value_ptr;
		*p = (int32)__v.toint();
	}
	else if (_type == &ffi_type_uint32) {
		usint32* p = (usint32*)__value_ptr;
		*p = (usint32)__v.touint();
	}
	else if (_type == &ffi_type_uint64) {
		int64* p = (int64*)__value_ptr;
		*p = (int64)__v.toint();
	}
	else if (_type == &ffi_type_uint64) {
		usint64* p = (usint64*)__value_ptr;
		*p = (usint64)__v.touint();
	}
	else if (_type == &ffi_type_sint16) {
		int16* p = (int16*)__value_ptr;
		*p = (int16)__v.toint();
	}
	else if (_type == &ffi_type_uint16) {
		usint16* p = (usint16*)__value_ptr;
		*p = (usint16)__v.touint();
	}
	else if (_type == &ffi_type_sint8) {
		int8* p = (int8*)__value_ptr;
		*p = (int8)__v.toint();
	}
	else if (_type == &ffi_type_uint8) {
		usint8* p = (usint8*)__value_ptr;
		*p = (usint8)__v.touint();
	}
	else if (_type == &ffi_type_float) {
		float* p = (float*)__value_ptr;
		*p = (float)__v.tonumber();
	}
	else if (_type == &ffi_type_double) {
		double* p = (double*)__value_ptr;
		*p = (double)__v.tonumber();
	}
	else if (_type == &ffi_type_void) {
		
	}
	else if (_type == &ffi_type_pointer) {
		void* p = (void*)__value_ptr;
		p = (void*)__v.touint();
	}

}
object ffi_return_cast(C*c, ffi_type* _type,void * __value_ptr)
{
	if (_type == &ffi_type_sint32) {
		int32* p = (int32*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_uint32) {
		usint32* p = (usint32*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_uint64) {
		int64* p = (int64*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_uint64) {
		usint64* p = (usint64*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_sint16) {
		int16* p = (int16*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_uint16) {
		usint16* p = (usint16*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_sint8) {
		int8* p = (int8*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_uint8) {
		usint8* p = (usint8*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_float) {
		float* p = (float*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_double) {
		double* p = (double*)__value_ptr;
		return *p;
	}
	else if (_type == &ffi_type_void) {
		return nil;
	}
	else if (_type == &ffi_type_pointer) {
		void* p = (void*)__value_ptr;
		return p;
	}
	return nil;
}
void free_all(ffi_type** ffi_arg_types, void** ffi_args, void* return_ptr, unsigned int _arg_count)
{
#ifndef _DEBUG
	if(return_ptr) free(return_ptr);
#endif
	for (unsigned int i = 0; i < _arg_count; i++) {
		if(ffi_args[i]) free(ffi_args[i]);
	}
	free(ffi_args);
	free(ffi_arg_types);
}

object	ffi_call_function(C*c,
						  object func,
						  object return_arg_types,
						  object arg_types,
						  object arg_values)
{
	PPROC _func = (PPROC)func.touint();

	object::vector _arg_types = object::vector(arg_types);
	object::vector _arg_values = object::vector(arg_values);
	unsigned int _arg_count = (unsigned int)_arg_types.size();

	ffi_type** ffi_arg_types = (ffi_type **)malloc(sizeof(ffi_type*) * _arg_count);
	void** ffi_args = (void**)malloc(sizeof(void*) * _arg_types.size());
	ffi_type* ffi_return_type = _ffi_s_to_type(return_arg_types.tostring());
	void* return_ptr = NULL;
	if (ffi_return_type->size) {
		return_ptr = malloc(ffi_return_type->size);
	}


	for (unsigned int i = 0; i < _arg_count; i++) {
		std::string __type = _arg_types[i].tostring();
		ffi_arg_types[i] = _ffi_s_to_type(__type);
		void* ffi_arg_ptr = malloc(ffi_arg_types[i]->size);
		_ffi_fill_value(ffi_arg_types[i], ffi_arg_ptr, _arg_values[i]);
		ffi_args[i] = ffi_arg_ptr;
	}
	ffi_cif cif;
	ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, _arg_count, ffi_return_type, ffi_arg_types);
	if (ffiPrepStatus == FFI_OK) {
		
		ffi_call(&cif, _func, return_ptr, ffi_args);
		object ret = ffi_return_cast(c,ffi_return_type, return_ptr);

		free_all(ffi_arg_types, ffi_args, return_ptr, _arg_count);

		return ret;
	}

	free_all(ffi_arg_types, ffi_args, return_ptr, _arg_count);

	return nil;
}

cpps_uinteger ffi_dlopen(C* c, std::string path)
{
	path = cpps_rebuild_filepath(path);
	if (!path.empty())
	{
#ifdef _WIN32
		HMODULE __module = ::LoadLibraryA(path.c_str());
#else
		HMODULE __module = dlopen(fpath.c_str(), RTLD_LAZY);
#endif
		if (__module == NULL) {
			return 0;
		}

		return (cpps_uinteger)__module;
	}
	return 0;
}
cpps_uinteger ffi_dlsym(C* c, cpps_uinteger __module,std::string func)
{

#ifdef _WIN32
	FARPROC __func = GetProcAddress((HMODULE)__module, func.c_str());
#else
	void* __func = dlsym((HMODULE)__module, func);
#endif
	if (__func == NULL) {
		return 0;
	}

	
	return (cpps_uinteger)__func;
}
void ffi_dlclose(C* c, cpps_uinteger __module)
{

#ifdef _WIN32
	FreeLibrary((HMODULE)__module);
#else
	dlclose((HMODULE)__module);
#endif
	

	
}

cpps_export_void cpps_attach(cpps::C* c)
{
	cpps::cpps_init_cpps_class(c);
	
	cpps::_module(c, "ffi")[
		def_inside("ffi_call",&ffi_call_function),
		def_inside("ffi_dlopen",&ffi_dlopen),
		def_inside("ffi_dlsym",&ffi_dlsym),
		def_inside("ffi_dlclose",&ffi_dlclose)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "ffi");
}

cpps_export_finish