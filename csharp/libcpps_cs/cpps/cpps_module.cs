using System.Runtime.InteropServices;
using System.Text;


namespace cpps
{
	public delegate Value cpps_func(IntPtr c, Value o);
	public delegate IntPtr cpps_alloc();
	public delegate void cpps_free(IntPtr p);
	public class cpps_module
	{
		private IntPtr c;
		private IntPtr _module;
		public IntPtr Module { get => _module; set => _module = value; }


		public cpps_module(IntPtr _c)
		{
			this.c = _c;
		}
		public cpps_module def(string name, cpps_func func)
		{
			API.module_def(this.c, this._module, name, func);
			return this;
		}

        public cpps_module def_class<T>(string name, _class<T> p) where T : new()
        {
			IntPtr _class = API.module_def_class(this.c, this._module, name, p.alloc, p.free);
			foreach(var item in p.funcs)
            {
				API.module_class_def(this.c, _class, item.Key,item.Value);
            }
			foreach(var item in p.operator_funcs)
            {
				API.module_class_def_operator(this.c, _class, item.Key,item.Value);
            }
			API.module_def_class_free(_class);
			return this;
		}
		public cpps_module def_enum(string name, _enum p)
		{
			IntPtr __enum = API.module_def_enum(this.c, this._module, name);
			foreach (var item in p.values)
			{
				API.module_enum_def(this.c, __enum, item.Key, item.Value);
			}
			API.module_def_enum_free(__enum);
			return this;
		}
	}
}
