loadlibrary("demo");

demotest();

freelibrary("demo");

namespace std
{
	var add(var a,var b)
	{
		return a + b ;
	}
	class A
	{
		var add(var a,var b)
		{
			return a + b;
		}
	}
}

println(std::add(100,200));

var aaa = new std::A;
println(aaa.add(300,400));