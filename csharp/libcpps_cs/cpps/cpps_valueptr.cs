using System.Runtime.InteropServices;

namespace cpps
{
    internal class ValuePtr
    {
        private readonly IntPtr _p = IntPtr.Zero;
        public ValuePtr()
        {
            Value value = new Value();
            _p = Marshal.AllocHGlobal(Marshal.SizeOf(value));
            Marshal.StructureToPtr(value, _p, true);
        }
        public IntPtr getPtr() { return _p; }
        public Value ToValue()
        {
            Value? anotherP = (Value?)Marshal.PtrToStructure(_p, typeof(Value));
            if (anotherP == null) return Value.nil;
            Marshal.DestroyStructure<Value>(_p);
            return anotherP;
        }
    }
}
