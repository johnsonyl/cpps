

var t1 = Date.now();

function _isprime( n){
    var nn = n - 1
    for (var i  = 2; i < nn; i++)
        if (n % i == 0)
            return false;
    return true;
}


function primes( n){
    var count = 0

    for (var i = 2; i < n ; i++){
        if (_isprime(i))
            count++;
	}
    return count
}

var N = 10 * 10000;

console.log(primes(N),Date.now() - t1)