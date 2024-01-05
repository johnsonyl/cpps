var t = tick();
var loop(var n){
	var count = 0;

	for( var i : range(n)){
		count++;
	}
	return count;
}
println(loop(10 * 10000000),tick()-t);

