var globalconfig ;
var initconfig(){
	globalconfig = serializer.decode(models::config,json.decode(io.readfile("config/app.json")));
}
var saveconfig(){
	var jsonstr = json.encode(serializer.encode(globalconfig,serializer.map));
	io.writefile("config/app.json",jsonstr);
}