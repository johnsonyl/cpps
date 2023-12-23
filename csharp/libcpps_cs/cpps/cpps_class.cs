using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace cpps
{
    public delegate Value cpps_class_func(Value _this,IntPtr c, Value o);
    public class _class<T> where T : new()
    {
        public Dictionary<string, cpps_class_func> funcs = new Dictionary<string, cpps_class_func>();
        public Dictionary<string, cpps_class_func> operator_funcs = new Dictionary<string, cpps_class_func>();

        public  IntPtr alloc() 
        {
            IntPtr ret = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(T)));
            Marshal.StructureToPtr(new T(), ret, true);
            return ret;
        }
        public  void free(IntPtr p)
        {
            Marshal.FreeHGlobal(p);

        }
        public _class()
        {
        }

        public _class<T> def(string name, cpps_class_func func)
        {
            funcs.Add(name, func);
            return this;
        }
        public _class<T> def_operator(string name, cpps_class_func func)
        {
            operator_funcs.Add(name, func);
            return this;
        }

    }
    public class Class
    {
        public static Value New(IntPtr c,Value _class) { ValuePtr _v = new ValuePtr();API.cpps_classvar_create(c, _class, _v.getPtr()); return _v.ToValue(); }
        public static Value Create(IntPtr c,Value _class) { return New(c, _class); }
    }
}
