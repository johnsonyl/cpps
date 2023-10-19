var func2()
{
    return "sadf";
}
var func1()
{
    return 1,2,func2();
}

var [a,b,e] = func1();

println(a);
println(b);
println(e);

var func3(var a,var b,var e)
{
	println(a);
	println(b);
	println(e);
}
function_caller(func3,func1());
