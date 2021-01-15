var a = "123456";

println(a[...]);
println(a[::2]);

var b = [1,2,3,4,5,6,7,8,9];
println(b[2]);
println(b[::][0:5:2][-1]);
var c = 0;
println(b[c:-3:]);

class A
{
	var operator [](var item)
	{
		if(isvector(item[0])){
			println("is vector");
		}
		else if(istuple(item) && item.size() >= 2 && isrange(item[1])){
			println("is range");
		}
		else if(isellipsis(item[0])){
			println("is ellipsis");
		}
		else if(istuple(item)){
			println("is tuple");
		}
		else{
			println("normal value");
		}
		
	}
}

var a = new A();

a[...];
a[[1,2,3],[3,4,5]];
a[...,1::];
a[1,2,3];
a[1];