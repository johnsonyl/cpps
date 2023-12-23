

namespace cpps
{
    internal class Dictionary
    {
        public Value v;
        public Dictionary(Value _v) { v = _v.clone(); }
        public static Dictionary Create(IntPtr c) { ValuePtr _v = new ValuePtr(); API.cpps_map_create(c,_v.getPtr()); return new Dictionary(_v.ToValue()); }
        public void Add(Value key,Value value) { API.cpps_map_insert(v,key, value); }
        public int Count() { return API.cpps_map_size(v); }
        public void Clear() { API.cpps_map_clear(v); }
        public void Remove(Value key) { API.cpps_map_erase(v,key); }
        public Value GetValue(Value key) { ValuePtr _v = new ValuePtr(); API.cpps_map_find(v, key, _v.getPtr()); return _v.ToValue(); }
        public Value ToValue() { return v; }
    }
}
