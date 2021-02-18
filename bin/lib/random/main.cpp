module random
{
    var randint(var start,var end){
        return math.random(start,end);
    }
    var randrange(var start,var end){
        return math.random(start,end-1);
    }
    var choice(var v){
        if(isstring(v)){
            var p = randrange(0,len(v));
            return string.at(v,p);
        }
        else if(isvector(v)){
            var p = randrange(0,len(v));
            return v[p];
        }
    }
    var simple(var v,var count = 1){
        if(!isstring(v) && !isvector(v)) return null;


        var hased = [];
        var ret = [];
        while(hased.size() < len(v) && hased.size() < count){
            if(isstring(v)){
                var p = randrange(0,len(v));
                if(hased.has(p)) continue;
                hased.push_back(p);
                ret.push_back(string.at(v,p));
            }
            else if(isvector(v)){
                var p = randrange(0,len(v));
                if(hased.has(p)) continue;
                hased.push_back(p);
                ret.push_back(v[p]);
            }
        }
        return ret;
    }
}