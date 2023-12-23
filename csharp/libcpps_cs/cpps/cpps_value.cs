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
		public Value(Value v)
        {
			ValuePtr _v = new ValuePtr();
			API.cpps_value_ref(v,_v.getPtr());
			type = ValueType.CPPS_TREF;
			value = _v.ToValue().value;
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
		public Value get(int idx) { ValuePtr _v = new ValuePtr(); API.cpps_value_get_by_idx(this.real(), idx, _v.getPtr()); return _v.ToValue(); }
		public void set(int idx, Value v) { Value _v = get(idx);_v.assign(v); }
		public Value get(string name) { ValuePtr _v = new ValuePtr(); API.cpps_value_get_by_name(this.real(), name, _v.getPtr()); return _v.ToValue(); }
		public void set(string name, Value v) { Value _v = get(name); _v.assign(v); }
		public static Value call(IntPtr c,Value func, params Value[] ps)
        {
			List _ps = List.Create(c);
			for (int i = 0; i < ps.Length; i++)
			{
				_ps.Add(ps[i]);
			}
			ValuePtr _v = new ValuePtr();
			API.cpps_function_call(c, func.real(), _ps.ToValue(), _v.getPtr());
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
			API.cpps_function_class_call(c, _classvar,func.real(), _ps.ToValue(), _v.getPtr());
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
		public string ToStr() { int count = API.cpps_string_size(this.real()); StringBuilder sb = new StringBuilder(count+3); API.cpps_string_strcopy(this.real(), sb); return sb.ToString(); }
		public int getType() { return type; }
		public bool islist() { return API.cpps_value_isvector(this.real()); }
		public bool ispair() { return API.cpps_value_ispair(this.real()); }
		public bool isset() { return API.cpps_value_isset(this.real()); }
		public bool isdictionary() { return API.cpps_value_ismap(this.real()); }
        public bool isbasevar() { return API.cpps_value_isbasevar(this.real()); }
		public bool isstring() { return API.cpps_value_isstring(this.real()); }
		public bool isrange() { return API.cpps_value_isrange(this.real()); }
		public bool isint() { return API.cpps_value_isint(this.real()); }
		public bool ischar() { return API.cpps_value_ischar(this.real()); }
		public bool isuint() { return API.cpps_value_isuint(this.real()); }
		public bool isbool() { return API.cpps_value_isbool(this.real()); }
		public bool isnumber() { return API.cpps_value_isnumber(this.real()); }
		public bool isnull() { return API.cpps_value_isnull(this.real()); }
		public bool isfunction() { return API.cpps_value_isfunction(this.real()); }
		public bool isclassvar() { return API.cpps_value_isclassvar(this.real()); }
		public bool isclass() { return API.cpps_value_isclass(this.real()); }
		public bool isellipsis() { return API.cpps_value_isellipsis(this.real()); }
		public bool istuple() { return API.cpps_value_istuple(this.real()); }
		public bool isdomain()
		{
			if (type == ValueType.CPPS_TINTEGER) return false;
			if (type == ValueType.CPPS_TUINTEGER) return false;
			if (type == ValueType.CPPS_TNUMBER) return false;
			if (type == ValueType.CPPS_TBOOLEAN) return false;
			if (type == ValueType.CPPS_TNIL) return false;
			return type == ValueType.CPPS_TDOMAIN || type == ValueType.CPPS_TCLASS || type == ValueType.CPPS_TCLASSVAR || type == ValueType.CPPS_TSTRING || type == ValueType.CPPS_TTUPLE;
		}
		public string? getclassname() { 
			StringBuilder sb = new StringBuilder(256);
			API.cpps_value_getclassname(this.real(), sb);
			return sb.ToString();
		}
		public static implicit operator sbyte(Value d) => (sbyte)d.integer;
		public static explicit operator Value(sbyte b) => new Value(b);

		public static implicit operator short(Value d) => (short)d.integer;
		public static explicit operator Value(short b) => new Value(b);

		public static implicit operator int(Value d) => (int)d.integer;
		public static explicit operator Value(int b) => new Value(b);

		public static implicit operator long(Value d) => (long)d.integer;
		public static explicit operator Value(long b) => new Value(b);

		public static implicit operator ulong(Value d) => (ulong)d.uinteger;
		public static explicit operator Value(ulong b) => new Value(b);

		public static implicit operator ushort(Value d) => (ushort)d.uinteger;
		public static explicit operator Value(ushort b) => new Value(b);

		public static implicit operator uint(Value d) => (uint)d.uinteger;
		public static explicit operator Value(uint b) => new Value(b);

		public static implicit operator byte(Value d) => (byte)d.uinteger;
		public static explicit operator Value(byte b) => new Value(b);

		public static implicit operator double(Value d) => (double)d.number;
		public static explicit operator Value(double b) => new Value(b);

		public static implicit operator float(Value d) => (float)d.number;
		public static explicit operator Value(float b) => new Value(b);

		public static implicit operator bool(Value d) => d.b != 0;
		public static explicit operator Value(bool b) => new Value(b);

		public Value assign(Value v)
		{
			if (isRef())
            {
				API.cpps_value_ref_assign(this, v);
				return this;
            }

			Value? _v_real = v.real();
			Value _self_real = this;
			if (_v_real == null) return this;

			_v_real.incruse();
			_self_real.decruse();
			_self_real.type = _v_real.type;
			_self_real.value = _v_real.value;
			return this;
        }
		public bool isRef() 
		{
			return type == ValueType.CPPS_TREF;
		}
		public Value real()
        {
			if(type != ValueType.CPPS_TREF) return this;
			ValuePtr _v = new ValuePtr();
			API.cpps_value_real(this,_v.getPtr());
			return _v.ToValue();
		}
		public Value Ref()
        {
			if (isRef()) return this;
			return new Value(this);
		}
        public override string ToString()
        {
			Value obj = real();

			if (obj.isnumber())
			{
				return obj.number.ToString();
			}
			else if (obj.isuint())
			{
				return obj.uinteger.ToString();
			}
			else if (obj.isint())
			{
				return obj.integer.ToString();
			}
			else if (obj.isstring())
			{
				return obj.ToStr();
			}
			else if (obj.isbool())
			{
				return (obj.b == 0 ? "false" : "true");
			}
			else if (obj.isdomain())
            {
				return "Domain<"+obj.getclassname()+">";
            }
			else if (obj.isnull())
            {
				return "null";
            }

			return "Address<"+ obj.value +">";
		}

        [FieldOffset(0)] public IntPtr value;
        [FieldOffset(0)] public long integer;
        [FieldOffset(0)] public ulong uinteger;
        [FieldOffset(0)] public double number;
		[FieldOffset(0)] public int b;
		[FieldOffset(8)] public byte type;
	}
}
