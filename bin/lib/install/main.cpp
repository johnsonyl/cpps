#import "compress"
#import "http"
#import "json"

var args = getargs();
var compiler_result = false;
var install_path = "";
var build_type = sys.debug;
var _modulename = args[2];
if(_modulename == "" || _modulename == null) exit(0); 
var get_package_json(var modulename){

	var modulepath = module_path(modulename);
	var package_buff = io.readfile("{modulepath}package.json");
	if(package_buff != null && !package_buff.empty()) 
	{
		return json.decode(package_buff);
	}
	return {
		"name" : modulename,
		"package" : {
			"dependencies" : {}
		},
		"dependencies" : {}
	};
}
var create_app_package_json(){
	var app_name = io.normpath(io.getrealpath());
	return {
		"name" : app_name.substr(0,app_name.rfind("/")),
		"package" : {
			"dependencies" : {}
		},
		"dependencies" : {}
	};
}
var get_app_package_json(){
	var package_buff = io.readfile("package.json");
	if(package_buff != null && !package_buff.empty()) 
	{
		return json.decode(package_buff);
	}
	return create_app_package_json();
}
var write_app_package_json(var v){
	io.writefile("package.json",json.encode(v));
}
var make_pageage_info(var module_package,var info){
	return module_package{
		"version" : module_package->version,
		"jsonfilepath" : info->downurl,
		"targzfilepath" : info->downurl1,
		"hash": info->hash,
		"dependencies": module_package->packages->dependencies,
		"cpps": ">1.0.2"
	};
}
var package_json_install(var modulename,var info){
	/**/
	var module_package = get_package_json(modulename);
	var app_package = get_app_package_json();
	if(ismap(module_package) && ismap(app_package)){
		app_package->dependencies += module_package->dependencies;
		app_package->packages->dependencies.insert(modulename,module_package->version);
		app_package->dependencies.insert(modulename,make_pageage_info(module_package,info));
		write_app_package_json(app_package);
	}
}
var package_json_uninstall(var modulename){
	/**/
	var module_package = get_package_json(modulename);
	var app_package = get_app_package_json();
	if(ismap(module_package) && ismap(app_package)){
		for(var deps : app_package->packages->dependencies){
			if(deps.first() != modulename){
				var other_package = get_package_json(deps->first());
				if(ismap(other_package)){
					module_package->dependencies -= other_package->dependencies;
				}
			}
		}
		app_package->dependencies -= module_package->dependencies;
		app_package->packages->dependencies.erase(modulename);
		app_package->dependencies->erase(modulename);
		write_app_package_json(app_package);
	}
}
var download(var file,var showname)
{
	
	println_color("-- File: {io.getfilename(showname)}",2);
	var cl = ["/","-","\\","|"];
	var progress_max = 70;
	var kg = "                                                                       ";
	var dy = "=======================================================================";

	var lastcur = 0;
	var down = new http::downloader();
	down.seturl(file);
	down.setfilepath(showname);
	var ret = down.download([](var filesize,var size,var currsize){
		var x = filesize / progress_max;
		var n = toint(currsize / filesize * 100 );
		var cur = toint(currsize / x);
		if(n < 99) print("{cl[cur%4]}");

		print("[");
		if(cur > 0)
			print(dy[0:cur]);
		print(">");
		if(cur < progress_max)
			print(kg[cur:-1]);
		print("]");
		print_color(" {n}%/100% {int(currsize/1024)} Kb/{int(filesize/1024)} Kb    ",2);
		print("\r");
	});
	println("");
	return ret;
}
var getmoduleinfo(var modulename)
{
	var url = "http://c.cppscript.org:88/download/downfile?name={modulename}";
	var ret = http.get(url);
	return json.decode(ret);
}
var install(var modulename,var version,var is_update_package)
{
	var modulepath = module_path(modulename);
	var exists = io.file_exists(modulepath);
	if(exists)
	{
		println_color("The {modulename} module is already installed.",1)
		exit(0);
	}
	//默认安装在运行目录
	modulepath = "lib/{modulename}";

	println_color("-- Start install {modulename} module..",3);

	io.mkdir("tmp");

	//1.获取模块文件
	//post 请求 获取压缩包地址 与 config.json地址
	print("-- Seraching for module info...");
	var info = getmoduleinfo(modulename);
	if(!ismap(info)){
		println_color("faild",1);
		println("Ops, cpps server is crash...")
		return;
	}
	if(info["code"] != 0){
		println_color("faild",1);
		if(info["code"] == -1){
			println("Ops, cpps server is crash...")
		}
		else if(info["code"] == -2){
			println("No match for module : {modulename}")
		}
		else if(info["code"] == -3){
			println("Match for module : {modulename}, But no has download url")
		}
		else{
			println("Unknow error:{info["code"]}")
		}
		return;
	}
	println_color("ok",2);
	var jsonfilepath = info["downurl"];
	var targzfilepath = info["downurl1"];
	//2.downloader 下载 json 与 tar.gz
	println_color("-- Start downloading.",3);

	var jsonfilename = "tmp/{io.getfilename(jsonfilepath)}";
	var targzfilename = "tmp/{io.getfilename(targzfilepath)}";

	download(jsonfilepath,jsonfilename);
	download(targzfilepath,targzfilename);



	//3.校验压缩包
	print("-- Check packages...");
	println_color("ok",2);

	//4.解压压缩包
	print("-- Start uncompressed module files...");
	io.mkdirs(modulepath);

	var jsonfile_realpath = "{modulepath}/{io.getfilename(jsonfilepath)}";
	var targzfile_realpath = "{modulepath}/{io.getfilename(targzfilepath)}";
	io.copy(jsonfilename,jsonfile_realpath);
	io.copy(targzfilename,targzfile_realpath);
	

	var file = tarfile.open(targzfile_realpath,"r:gz",102400000);
	if(!file) {
		println_color("faild",1);
		uninstall();
		exit(0);
	}
	file.extractall("{modulepath}/");
	println_color("ok",2);


	if(is_update_package){
		print("-- Downloading module dependencies files...");
		var module_package = get_package_json(modulename);
		var app_package = get_app_package_json();
		if(ismap(module_package) && ismap(app_package)){
			module_package->dependencies -= app_package->dependencies;
			for(var deps : module_package->dependencies){
				install(deps.first(),dep.scecond()->version,false);
			}
		}
	}
	

	install_path = modulepath;
	println_color("-- Start compiler module files...",3);

	dofile("{modulepath}/setup.cpp"); //compiler
	if(compiler_result){
		println_color("-- Install {modulename} is success.",2);
		if(is_update_package) package_json_install(modulename,info);
	}
	else
	{
		println_color("-- Ops.. install {modulename} is faild. uninstall {modulename} module.",1);
		uninstall(modulename, false);
	}

}

var uninstall(var modulename,var is_update_package)
{
	var modulepath = module_path(modulename);
	var exists = io.file_exists(modulepath);
	if(!exists)
	{
		println_color("The {modulename} module is not already installed.",1);
		return;
	}
	if(is_update_package){
		println("-- Uninstall module dependencies files...");
		var module_package = get_package_json(modulename);
		var app_package = get_app_package_json();
		if(ismap(module_package) && ismap(app_package)){
			for(var deps : app_package->packages->dependencies){
				if(deps.first() != modulename){
					var other_package = get_package_json(deps->first());
					if(ismap(other_package)){
						module_package->dependencies -= other_package->dependencies;
					}
				}
			}
			for(var deps : module_package->dependencies){
				uninstall(deps.first(),false);
			}
		}
		package_json_uninstall(modulename);
	}

	var list = io.walk(modulepath);
	println_color("-- Start uninstall {modulename}...",3);

	var i = list.size()-1;
	for(; i >= 0; i--){
		var path = list[i];
		var stat = io.stat(path);
		print_color("-- Remove",2);
		print(" {path} ...");
		if(stat.isdir())
			io.rmdir(path);
		else
			io.remove(path);
		println_color("ok",2);
	}
	io.rmdir(modulepath);
	println_color("-- Uninstall {modulename} is done.",2);
}
var update(var modulename)
{
	var modulepath = module_path(modulename);
	var exists = io.file_exists(modulepath);
	if(!exists)
	{
		println_color("The {modulename} module is not already installed.",1);
		return;
	}
	print("-- Loading module info...");
	var info = getmoduleinfo(modulename);
	println_color("ok",2);
	print("-- Checking module version info...");
	println_color("ok",2);
	uninstall(modulename,true);
	install(modulename,"last",true);
}
if(args[1] == "-install"){
	install(_modulename,_version,true);
}
else if (args[1] == "-uninstall")
{
	uninstall(_modulename,true);
}
else if (args[1] == "-update")
{
	update(_modulename);
}