module ffi{
	class FFI_FUNCTION
	{
		var _module;
		var _func;
		var _return_type;
		var _args_type;
		FFI_FUNCTION(var __module,var __func,var __return_type,var __args_type)
		{
			_module = __module;
			_func = __func;
			_return_type = __return_type;
			_args_type = __args_type;
		}
		var operator () (...Args)
		{
			ffi_call(_func,_return_type,_args_type,Args);
		}
	}
	var dlopen(var path,var funcs)
	{
		var __module = ffi.ffi_dlopen(path);
		if(__module == 0) return null;
		var _interface = {};
		_interface["__module"] = __module;
		for(var _f: funcs){
			var funcname = _f.first();
			var __func = ffi.ffi_dlsym(__module,funcname);
			if(__func != 0){
				var _v = _f.second();
				var return_type = _v[0];
				var args_type = _v[1];
				_interface[funcname] = new FFI_FUNCTION(__module,__func,return_type,args_type);
			}
		}
		return _interface;
	}
	var dlclose(var _module){
		ffi.ffi_dlclose(_module->__module);
	}
}