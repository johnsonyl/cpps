import ["json", "demo"];
//or
//import "json";
//import "demo";
system("chcp 65001"); //use utf8 string

demotest();



var arr = [1, 2, 3, 4, 5, 6, 7];
var obj = { a : "我是开发者", b : "hehe" };


println(arr);
println(obj);


var arr2 = json.encode(arr, json.encode_utf8);
var obj2 = json.encode(obj, json.encode_utf8);



println(arr2);
println(obj2);


println(json.decode(arr2));
println(json.decode(obj2));
