var t = Date.now();

function loop( n){
    var ret = "";
    var last = "";

    for (var i = 2;i < n ; i++){
        ret = last + "ret" + i;
        last = "last" + i;	
	}
    return ret;
}

var N = 1000000;

console.log(loop(N),Date.now()-t);