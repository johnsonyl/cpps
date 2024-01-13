

var t1 = time.gettickcount()

var loop(var n){
    var ret = "";
    var last = "";

    for (var i : xrange(2, n)){
        ret = last.."ret"..i;
        last = "last"..i;
	}
    return ret
}

var N = 1000000;

println(loop(N),time.gettickcount() - t1)