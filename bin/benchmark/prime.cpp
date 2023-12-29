

var t1 = time.gettickcount()


var primes(var n){
    var count = 0

    for (var i : xrange(2,n)){
        if (isprime(i))
            count++;
	}
    return count
}

var N = 10 * 10000;

println(primes(N),time.gettickcount() - t1)