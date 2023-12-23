namespace CSharpDllExport;
using System.Runtime.InteropServices;
using cpps;

public class Class1
{
    public class DoSomethings
    {
        public static Value Add(IntPtr c,Value o)
        {

            List list = new List(o);
            Value p1 = list.At(0);
            Value p2 = list.At(1);

            Console.WriteLine(o.getType());
            Console.WriteLine(p1.ToInt());
            Console.WriteLine(p2.ToInt());

            return new Value(p1.ToInt() + p2.ToInt() + 50 );
        }
        [UnmanagedCallersOnly(EntryPoint = "cpps_attach")]
        public static void cpps_attach(IntPtr c)
        {
           
            API._module(c, "csharp")
                .def("add", Add);
        }
        [UnmanagedCallersOnly(EntryPoint = "cpps_detach")]
        public static void cpps_detach(IntPtr c)
        {
			API._unmodule(c,"csharp");
        }
    }
}
