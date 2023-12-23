using System.Runtime.InteropServices;
using System.Text;

namespace cpps
{
	public class API
	{

		public static cpps_module _module(IntPtr c, string name)
		{
			cpps_module m = new cpps_module(c);
			m.Module = module(c, name);
			return m;
		}
		public static void _unmodule(IntPtr c, string name)
		{
			unmodule(c, name);
		}
		public static T? ptr_cast<T>(IntPtr this_ptr)
		{
			T? _this = (T?)Marshal.PtrToStructure(this_ptr, typeof(T));
			if (_this == null) return default;
			return _this;
		}
		public static T? value_cast<T>(Value v)
		{
			IntPtr p = API.cpps_value_cast(v);
			return ptr_cast<T>(p);
		}
		[DllImport("cpps_interface", EntryPoint = "cpps_create")]
		public static extern IntPtr create();
		[DllImport("cpps_interface", EntryPoint = "cpps_close")]
		public static extern void close(IntPtr c);

		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_dofile")]
		public static extern void dofile(IntPtr c, string path);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_defmodule")]
		public static extern IntPtr module(IntPtr c, string name);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_unmodule")]
		public static extern IntPtr unmodule(IntPtr c, string name);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_module_def")]
		public static extern void module_def(IntPtr c, IntPtr domain, string name, cpps_func __func);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_def_class")]
		public static extern IntPtr module_def_class(IntPtr c, IntPtr domain, string name, cpps_alloc __alloc,cpps_free __free);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_def_class_free")]
		public static extern IntPtr module_def_class_free(IntPtr _class);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_class_def")]
		public static extern void module_class_def(IntPtr c, IntPtr _class, string name, cpps_class_func __func);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_class_def_operator")]
		public static extern void module_class_def_operator(IntPtr c, IntPtr _class, string name, cpps_class_func __func);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_def_enum")]
		public static extern IntPtr module_def_enum(IntPtr c, IntPtr domain, string name);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_def_enum_free")]
		public static extern IntPtr module_def_enum_free(IntPtr _enum);
		[DllImport("cpps_interface", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "cpps_enum_def")]
		public static extern void module_enum_def(IntPtr c, IntPtr _enum, string name, long v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_ispair")]
		public static extern bool cpps_value_ispair(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isset")]
		public static extern bool cpps_value_isset(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_ismap")]
		public static extern bool cpps_value_ismap(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isbasevar")]
		public static extern bool cpps_value_isbasevar(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isstring")]
		public static extern bool cpps_value_isstring(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isvector")]
		public static extern bool cpps_value_isvector(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isrange")]
		public static extern bool cpps_value_isrange(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isint")]
		public static extern bool cpps_value_isint(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_ischar")]
		public static extern bool cpps_value_ischar(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isuint")]
		public static extern bool cpps_value_isuint(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isbool")]
		public static extern bool cpps_value_isbool(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isnumber")]
		public static extern bool cpps_value_isnumber(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isnull")]
		public static extern bool cpps_value_isnull(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isfunction")]
		public static extern bool cpps_value_isfunction(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isclassvar")]
		public static extern bool cpps_value_isclassvar(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isclass")]
		public static extern bool cpps_value_isclass(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_isellipsis")]
		public static extern bool cpps_value_isellipsis(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_istuple")]
		public static extern bool cpps_value_istuple(Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_value_getclassname")]
		public static extern void cpps_value_getclassname(Value v,StringBuilder _out);
		[DllImport("cpps_interface", EntryPoint = "cpps_value_release")]
		public static extern void cpps_value_release(Value vct);

		[DllImport("cpps_interface", EntryPoint = "cpps_value_get_by_name")]
		public static extern void cpps_value_get_by_name(Value obj,string name,IntPtr _out);

		[DllImport("cpps_interface", EntryPoint = "cpps_value_get_by_idx")]
		public static extern void cpps_value_get_by_idx(Value obj,int idx,IntPtr _out);

		[DllImport("cpps_interface", EntryPoint = "cpps_value_global")]
		public static extern void cpps_value_global(IntPtr c,IntPtr _out);
		[DllImport("cpps_interface", EntryPoint = "cpps_value_incruse")]
		public static extern void cpps_value_incruse(Value _v);
		[DllImport("cpps_interface", EntryPoint = "cpps_value_decruse")]
		public static extern void cpps_value_decruse(Value _v);
		[DllImport("cpps_interface", EntryPoint = "cpps_value_cast")]
		public static extern IntPtr cpps_value_cast(Value _v);
		//vector
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_push_back")]
		public static extern void cpps_vector_push_back(Value vct,Value v);
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_size")]
		public static extern int cpps_vector_size(Value vct);
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_erase")]
		public static extern void cpps_vector_erase(Value vct,int idx);
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_at")]
		public static extern void cpps_vector_at(Value vct,int idx,IntPtr _out);
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_clear")]
		public static extern void cpps_vector_clear(Value vct);
		[DllImport("cpps_interface",  EntryPoint = "cpps_vector_create")]
		public static extern void cpps_vector_create(IntPtr c, IntPtr _out);
		//map
		[DllImport("cpps_interface", EntryPoint = "cpps_map_insert")]
		public static extern void cpps_map_insert(Value map, Value key,Value v);
		[DllImport("cpps_interface", EntryPoint = "cpps_map_size")]
		public static extern int cpps_map_size(Value map);
		[DllImport("cpps_interface", EntryPoint = "cpps_map_clear")]
		public static extern void cpps_map_clear(Value map);
		[DllImport("cpps_interface", EntryPoint = "cpps_map_erase")]
		public static extern void cpps_map_erase(Value map,Value key);
		[DllImport("cpps_interface", EntryPoint = "cpps_map_find")]
		public static extern void cpps_map_find(Value map,Value key, IntPtr _out);
		[DllImport("cpps_interface", EntryPoint = "cpps_map_create")]
		public static extern void cpps_map_create(IntPtr c, IntPtr _out);
		//function
		[DllImport("cpps_interface", EntryPoint = "cpps_function_call")]
		public static extern void cpps_function_call(IntPtr c,Value func,Value ps, IntPtr _out);
		[DllImport("cpps_interface", EntryPoint = "cpps_function_class_call")]
		public static extern void cpps_function_class_call(IntPtr c,Value _classvar,Value func,Value ps, IntPtr _out);
		//string
		[DllImport("cpps_interface", EntryPoint = "cpps_string_size")]
		public static extern int cpps_string_size(Value _str);
		[DllImport("cpps_interface", EntryPoint = "cpps_string_append")]
		public static extern int cpps_string_append(Value _str,string s,int count);
		[DllImport("cpps_interface", EntryPoint = "cpps_string_strcopy")]
		public static extern void cpps_string_strcopy(Value _str,StringBuilder _out);
		[DllImport("cpps_interface", EntryPoint = "cpps_string_assignment")]
		public static extern void cpps_string_assignment(Value _str,string s,int count);
		[DllImport("cpps_interface", EntryPoint = "cpps_string_create")]
		public static extern void cpps_string_create(IntPtr c,IntPtr _out);
		//class
		[DllImport("cpps_interface", EntryPoint = "cpps_classvar_create")]
		public static extern void cpps_classvar_create(IntPtr c,Value _class,IntPtr _out);


	}
}
