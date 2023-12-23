using cpps;

namespace cpps
{
    public class String
    {
        public Value v;
        public static String Create(IntPtr c) { ValuePtr _v = new ValuePtr(); API.cpps_string_create(c, _v.getPtr()); String ret = new String(_v.ToValue()); return ret; }
        public static String Create(IntPtr c,string str) { ValuePtr _v = new ValuePtr(); API.cpps_string_create(c, _v.getPtr()); String ret = new String(_v.ToValue()); ret.Assign(str); return ret; }
        public String(Value _v) { v = _v; }
        public string ToStr() { return v.ToStr(); }
        public String Append(string s) { API.cpps_string_append(v,s,s.Length); return this; }
        public String Assign(string s) { API.cpps_string_assignment(v,s,s.Length);return this; }
        public String Append(String s) { return Append(s.ToStr()); }
        public String Assign(String s) { return Assign(s.ToStr()); }
        public String Clear() { return Assign(""); }
        public int Length { get { return API.cpps_string_size(v); } }
        public Value ToValue() { return v; }
    }
}
