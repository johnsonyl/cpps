#import "json"

/*1.load package.json*/
var package_buff = io.readfile("package.json");
if(package_buff != null && !package_buff.empty()) 
{
	var package_object = json.decode(package_buff);
	if(!ismap(package_object)) throw "Loading package.json error.";

	/*2.check depens*/
	var self_dependencies = package_object->packages->dependencies;
	var other_dependencies = package_object->dependencies;
	for(var deps : self_dependencies){
		if(!check_module(deps.first())) throw "Module {deps.first()} do not exsit. please use \"cpps -install {deps.first()} -version {deps.second()}\"";
	}
	for(var deps : other_dependencies){
		if(!check_module(deps.first())) throw "Module {deps.first()} do not exsit. please use \"cpps -install {deps.first()} -version {deps.second()->version}\"";
	}
}