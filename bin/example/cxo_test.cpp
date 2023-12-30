#import "json"

class A
{
	var a = 10;
	var b = 20;
	A()
	{
		println("A init");
	}
	var add(var _a,var _b){
		return _a+_b;
	}
}

var g = [1,2,3,4,5];

var g_json = json.encode(g);



println("cxo done.");