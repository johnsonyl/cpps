using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace cpps
{
    public class List
    {
        public Value v;
        public List(Value _v) { v = _v.clone(); }

        public static List Create(IntPtr c) { ValuePtr _v = new ValuePtr(); API.cpps_vector_create(c,_v.getPtr()); return new List(_v.ToValue()); }
        public void Add(Value _v)
        {
            API.cpps_vector_push_back(v, _v);
        }
        public int Count()
        {
            return API.cpps_vector_size(v);
        }
        public void Clear()
        {
            API.cpps_vector_clear(v);
        }
        public void RemoveAt(int idx)
        {
            API.cpps_vector_erase(v, idx);
        }
        public Value At(int idx)
        {
            ValuePtr _v = new ValuePtr();
            API.cpps_vector_at(v, idx, _v.getPtr());
            return _v.ToValue();
        }
        public Value ToValue() { return v.clone(); }
    }
}
