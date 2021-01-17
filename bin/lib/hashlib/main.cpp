module hashlib
{
	var    md5encode(var s,var key){
        var encoder = new md5();
        encoder.update(s);
        var ret = encoder.hexdigest();
        if(isstring(key)){
            var encoder_key = new md5();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
    var     sha1encode(var s,var key){
        var encoder = new sha1();
        encoder.update(s);
        if(isstring(key)){
            var encoder_key = new sha1();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
    var     sha224encode(var s,var key){
        var encoder = new sha224();
        encoder.update(s);
        if(isstring(key)){
            var encoder_key = new sha224();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
    var     sha256encode(var s,var key){
        var encoder = new sha256();
        encoder.update(s);
       if(isstring(key)){
            var encoder_key = new sha256();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
    var     sha384encode(var s,var key){
        var encoder = new sha384();
        encoder.update(s);
       if(isstring(key)){
            var encoder_key = new sha384();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
    var     sha512encode(var s,var key){
        var encoder = new sha512();
        encoder.update(s);
        if(isstring(key)){
            var encoder_key = new sha512();
            encoder_key.update(ret);
            encoder_key.update(key);
            ret = encoder_key.hexdigest();
        }
        return ret;
    }
}