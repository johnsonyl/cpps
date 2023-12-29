var t = time.gettickcount();
var loop(var n){
	var count = 0;

	for( var i : range(n)){
		count++;
	}
	return count;
}
var a = loop(10 * 10000000)
println(a,time.gettickcount()-t);