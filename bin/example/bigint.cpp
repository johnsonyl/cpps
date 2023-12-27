#import "bigint"

var U256(var v){
	return new bigint::Integer(v);
}

var a = new bigint::Integer();
a = 1;
a = U256(100) + a;


println(a);