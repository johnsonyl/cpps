

var t1 = time.gettickcount()

var _isprime(var n){
    for (var i : xrange(2,n - 1))
        if (n % i == 0)
            return false
    return true
}


var primes(var n){
    var count = 0

    for (var i : xrange(2,n)){
        if (_isprime(i))
            count++;
	}
    return count
}

var N = 10 * 10000;

println(primes(N),time.gettickcount() - t1)