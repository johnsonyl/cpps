module hashlib
{
	var    md5encode(var s){
        var encoder = new md5();
        encoder.update(s);
        return encoder.hexdigest();
    }
    var     sha1encode(var s){
        var encoder = new sha1();
        encoder.update(s);
        return encoder.hexdigest();
    }
    var     sha224encode(var s){
        var encoder = new sha224();
        encoder.update(s);
        return encoder.hexdigest();
    }
    var     sha256encode(var s){
        var encoder = new sha256();
        encoder.update(s);
        return encoder.hexdigest();
    }
    var     sha384encode(var s){
        var encoder = new sha384();
        encoder.update(s);
        return encoder.hexdigest();
    }
    var     sha512encode(var s){
        var encoder = new sha512();
        encoder.update(s);
        return encoder.hexdigest();
    }
}