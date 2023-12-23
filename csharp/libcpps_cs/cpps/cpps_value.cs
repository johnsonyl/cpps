using System.Runtime.InteropServices;
using System.Text;

namespace cpps
{
	public class ValueType
    {
		public static byte CPPS_TNIL = 0;
		public static byte CPPS_TINTEGER = 1;
		public static byte CPPS_TNUMBER = 2;
		public static byte CPPS_TBOOLEAN = 3;
		public static byte CPPS_TSTRING = 4;
		public static byte CPPS_TUSERDATA = 5;
		public static byte CPPS_TFUNCTION = 6;
		public static byte CPPS_TDOMAIN = 7;
		public static byte CPPS_TREF = 8;
		public static byte CPPS_TCLASS = 9;
		public static byte CPPS_TCLASSVAR = 10;
		public static byte CPPS_TLAMBDAFUNCTION = 11;
		public static byte CPPS_TTUPLE = 12;
		public static byte CPPS_TSTRINGV = 13;
		public static byte CPPS_TELLIPSIS = 14;
		public static byte CPPS_TUINTEGER = 15;
		public static byte CPPS_TQUOTECLASSVAR = 16;
	}
	[StructLayout(LayoutKind.Explicit,Size = 9)]
	public class Value
	{

		public static Value nil = new Value();

		~Value()
        {
			decruse(); //gc -count
		}

		public Value()
        {
			type = 0;
			value = IntPtr.Zero;
        }
		public Value(long v)
        {
			type = ValueType.CPPS_TINTEGER;
			integer = v;
        }
		public Value(ulong v)
        {
			type = ValueType.CPPS_TUINTEGER;
			uinteger = v;
        }
		public Value(double v)
        {
			type = ValueType.CPPS_TNUMBER;
			number = v;
        }
		public Value(bool v)
        {
			type = ValueType.CPPS_TBOOLEAN;
			b = v?1:0;
        }
		public Value clone()
        {
			Value ret = new Value();
			ret.value = value;
			ret.type = type;
			ret.incruse();
			return ret;
        }
		public void incruse() { API.cpps_value_incruse(this); }
		public void decruse() { API.cpps_value_decruse(this); }
		public static Value globals(IntPtr c) { ValuePtr _v = new ValuePtr(); API.cpps_value_global(c, _v.getPtr()); return _v.ToValue(); }
		public Value get(int idx) { ValuePtr _v = new ValuePtr(); API.cpps_value_get_by_idx(this, idx, _v.getPtr()); return _v.ToValue(); }
		public Value get(string name) { ValuePtr _v = new ValuePtr(); API.cpps_value_get_by_name(this, name, _v.getPtr()); return _v.ToValue(); }
		public static Value call(IntPtr c,Value func, params Value[] ps)
        {
			List _ps = List.Create(c);
			for (int i = 0; i < ps.Length; i++)
			{
				_ps.Add(ps[i]);
			}
			ValuePtr _v = new ValuePtr();
			API.cpps_function_call(c, func, _ps.ToValue(), _v.getPtr());
			return _v.ToValue();
        }
		public static Value call(IntPtr c, string func_name, params Value[] ps)
		{
			Value func = Value.globals(c).get(func_name);
			if (!func.isfunction()) return Value.nil;
			return call(c, func, ps);
		}
		public static Value class_call(IntPtr c,Value _classvar,Value func,params Value[] ps)
		{
			List _ps = List.Create(c);
			for (int i = 0; i < ps.Length; i++)
            {
				_ps.Add(ps[i]);
            }
			ValuePtr _v = new ValuePtr();
			API.cpps_function_class_call(c, _classvar,func, _ps.ToValue(), _v.getPtr());
			return _v.ToValue();
		}
		public static Value class_call(IntPtr c,Value _classvar,string func_name,params Value[] ps)
		{
			Value func = _classvar.get(func_name);
			if (!func.isfunction()) return Value.nil;
			return class_call(c, _classvar, func,ps);
		}
		public long ToInt(){ return integer; }
		public ulong ToUInt(){ return uinteger; }
		public double ToDouble() { return number; }
		public float ToFloat() { return (float)number; }
		public int ToInt32(){ return (int)integer; }
		public bool ToBool() { return b != 0; }
		public string ToStr() { int count = API.cpps_string_size(this); StringBuilder sb = new StringBuilder(count); API.cpps_string_strcopy(this, sb); return sb.ToString(); }
		public int getType() { return type; }
		public bool islist() { return API.cpps_value_isvector(this); }
		public bool ispair() { return API.cpps_value_ispair(this); }
		public bool isset() { return API.cpps_value_isset(this); }
		public bool isdictionary() { return API.cpps_value_ismap(this); }
        public bool isbasevar() { return API.cpps_value_isbasevar(this); }
		public bool isstring() { return API.cpps_value_isstring(this); }
		public bool isrange() { return API.cpps_value_isrange(this); }
		public bool isint() { return API.cpps_value_isint(this); }
		public bool ischar() { return API.cpps_value_ischar(this); }
		public bool isuint() { return API.cpps_value_isuint(this); }
		public bool isbool() { return API.cpps_value_isbool(this); }
		public bool isnumber() { return API.cpps_value_isnumber(this); }
		public bool isnull() { return API.cpps_value_isnull(this); }
		public bool isfunction() { return API.cpps_value_isfunction(this); }
		public bool isclassvar() { return API.cpps_value_isclassvar(this); }
		public bool isclass() { return API.cpps_value_isclass(this); }
		public bool isellipsis() { return API.cpps_value_isellipsis(this); }
		public bool istuple() { return API.cpps_value_istuple(this); }
		public string? getclassname() { 
			StringBuilder sb = new StringBuilder(256);
			API.cpps_value_getclassname(this, sb);
			return sb.ToString();
		}


		[FieldOffset(0)] public IntPtr value;
        [FieldOffset(0)] public long integer;
        [FieldOffset(0)] public ulong uinteger;
        [FieldOffset(0)] public double number;
		[FieldOffset(0)] public int b;
		[FieldOffset(8)] public byte type;
	}
}
