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