class A
{
    var name = "test";
	var test()
	{
		println("hello world");
	}
}

class B : A
{
	var test(){
		println("B World");
	}
}

class C : A
{

}

var b = new B();
var c = new C();

b.test();   // B world
c.test();  //hello world

var name = "john";
//这样修改没问题
var A::test()
{
	println("wonderful world");
}
b.test();  // B world
c.test();  //wonderful world


//这样就失效了.
var B::test()
{
	println("change the world");

}

b.test();  //change the world
c.test();  //wonderful world
