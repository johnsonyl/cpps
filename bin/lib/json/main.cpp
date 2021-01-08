module json
{
    var encode_file(var path,var object)
    {
        if(ismap(object)){
             var _json_object = {};
            foreach(var item: object){
                var m = serializer.encode(item.second(),serializer.map);
                _json_object[item.first()] = m;
            }
            var _json = json.encode(_json_object);
            io.writefile(path,_json);
        }
        else if(isvector(object)){
            var _json_object = [];
            foreach(var item: object){
                var m = serializer.encode(item,serializer.map);
                _json_object.push_back(m);
            }
            var _json = json.encode(_json_object);
            io.writefile(path,_json);
        }
    }
    var decode_file(var path,var __class)
    {
        var ret;
        var _json = io.readfile(path);
        var _json_object = json.decode(_json);
        if(ismap(_json_object)){
            ret = {};
            foreach(var item : _json_object){
                var model = serializer.decode(__class,item.second());
                ret[item.first()] = model;
            }
        }
        else if(isvector(_json_object)){
            ret = [];
            foreach(var item : _json_object){
                var model = serializer.decode(__class,item);
                ret.push_back(model);
            }
        }
        return ret;
    }
}