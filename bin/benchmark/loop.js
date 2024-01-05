var t = Date.now();
function loop(n){
	var count = 0;

	for( var i = 0; i < n; i++){
		count++;
	}
	return count;
}
console.log(loop(10 * 10000000),Date.now()-t);

