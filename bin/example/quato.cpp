class Vec2D
{
	Vec2D(){

	}

	var& operator [](var key){
		return xx[key];
	}
	
	var xx = [1];
}


var p1 = new Vec2D();
p1[0] = 1000;
println(p1[0]);

var &a = p1[0];
a = 100;
println(p1[0]);

var testset(var &v)
{
	v = 200;
}
testset(a);
testset(p1[0]);

println(a);
println(p1[0]);

var &b = p1;
println(b[0]);