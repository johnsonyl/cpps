#import "ffi"


var test = ffi.dlopen("test.dll",{
	"test":["int",["int","int"]]
});

var i = test->test(10,30);
println(i);
println(test);
