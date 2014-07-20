using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WcfService1;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            WcfService1.Service1 test = new WcfService1.Service1();
            int z = test.Soma(3, 4);
        }
    }
}
