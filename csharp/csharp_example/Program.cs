using cpps;
using System.Runtime.InteropServices;
using System.Text;


public class C
{
	[StructLayout(LayoutKind.Sequential)]
	public class TESTCLASS 
    {
		public static Value test(Value _this_value, IntPtr c, Value o)
		{
			Console.WriteLine(_this_value.getType());

			TESTCLASS? _this = API.value_cast<TESTCLASS>(_this_value);

			_this.i = 100;
			Console.WriteLine(_this.i);
			Console.WriteLine("operator:" + o.ToInt());
			return o;
		}
		public int i;
    }
    private IntPtr c;
	public static Value test(IntPtr c, Value o)
	{
		List list = new List(o);
		Value p1 = list.At(0);
		
		Console.WriteLine(p1.ToInt());
		return new Value(500);
	}
	// Use this for initialization
	void Start()
	{
		this.c = API.create();

		API._module(this.c, "test")
			.def("test", test)
			.def_class("TESTCLASS",
				new _class<TESTCLASS>()
					.def("test", TESTCLASS.test)
					.def_operator(">>", TESTCLASS.test))
			.def_enum("TESTENUM",
				new _enum()
					.def("ENUM_ONE",1)
					.def("ENUM_TWO", 1));

		API.dofile(this.c, "main.cpp");

		Value _G = Value.globals(c);
		Value test2 = _G.get("test2");
		Console.WriteLine(test2.ToString());
		test2.assign((Value)20);
		Console.WriteLine(test2.ToString());

		Value.call(c, "printtest");

		API.close(this.c);
	}
	static void Main(string[] args)
	{
		C cpps = new C();
		cpps.Start();
	}
}