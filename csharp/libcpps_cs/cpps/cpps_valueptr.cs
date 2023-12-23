using System.Runtime.InteropServices;

namespace cpps
{
    internal class ValuePtr
    {
        private IntPtr _p;
        public ValuePtr()
        {
            _p = Marshal.AllocHGlobal(Marshal.SizeOf(new Value()));

        }
        public IntPtr getPtr() { return _p; }
        public Value ToValue()
        {
            Value? anotherP = (Value?)Marshal.PtrToStructure(_p, typeof(Value));
            if (anotherP == null) return Value.nil;
            Value ret = anotherP.clone();
            Marshal.FreeHGlobal(_p);
            return ret;
        }
    }
}
