using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cpps
{
    public class _enum
    {
        public Dictionary<string, long> values = new Dictionary<string, long>();

        public _enum def(string name,long v)
        {
            values.Add(name, v);
            return this;
        }

    }
}
