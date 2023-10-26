#import "bigint"

var U256(var v){
	return new bigint::uint256(v);
}

var a = new bigint::uint256();
a = 1;
a = U256(100) + a;


println(a);