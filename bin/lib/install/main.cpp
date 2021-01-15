#import "compress"
#import "http"
#import "json"

var args = getargs();
var compiler_result = false;
var install_path = "";
var build_type = sys.debug;
var modulename = args[2];
if(modulename == "" || modulename == null) exit(0); 

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
var getmoduleinfo()
{
	var url = "http://192.168.31.124:8080/download/downfile?name={modulename}";
	var ret = http.get(url);
	return json.decode(ret);
}
var install()
{
	var modulepath = "{io.getrealpath()}lib/{modulename}";
	var exists = io.file_exists(modulepath);
	if(exists)
	{
		println_color("The {modulename} module is already installed.",1)
		exit(0);
	}

	println_color("-- Start install {modulename} module..",3);

	io.mkdir("tmp");

	//1.获取模块文件
	//post 请求 获取压缩包地址 与 config.json地址
	print("-- Loading module info...");
	var info = getmoduleinfo();
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
	var modulepath = "{io.getrealpath()}lib/{modulename}";
	io.mkdirs(modulepath);

	var jsonfile_realpath = "{modulepath}/{io.getfilename(jsonfilepath)}";
	var targzfile_realpath = "{modulepath}/{io.getfilename(targzfilepath)}";
	io.copy(jsonfilename,jsonfile_realpath);
	io.copy(targzfilename,targzfile_realpath);
	

	var file = tarfile.open(targzfile_realpath,"r:gz",102400000);
	if(!file) {
		println("faild");
		uninstall();
		exit(0);
	}
	file.extractall("{modulepath}/");
	println_color("ok",2);

	install_path = "{io.getrealpath()}lib/{modulename}/";
	println_color("-- Start compiler module files...",3);

	dofile("{modulepath}/setup.cpp"); //compiler
	if(compiler_result)
		println_color("-- Install {modulename} is success.",2);
	else
	{
		println_color("-- Ops.. install {modulename} is faild. uninstall {modulename} module.",1);
		uninstall();
	}

}

var uninstall()
{
	
	var modulepath = "{io.getrealpath()}lib/{modulename}";
	var exists = io.file_exists(modulepath);
	if(!exists)
	{
		println_color("The {modulename} module is not already installed.",1);
		return;
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
var update()
{
	var modulepath = "{io.getrealpath()}lib/{modulename}";
	var exists = io.file_exists(modulepath);
	if(!exists)
	{
		println_color("The {modulename} module is not already installed.",1);
		return;
	}
	print("-- Loading module info...");
	var info = getmoduleinfo();
	println_color("ok",2);
	print("-- Checking module version info...");
	println_color("ok",2);
	uninstall();
	install();
}
if(args[1] == "-install"){
	install();
}
else if (args[1] == "-uninstall")
{
	uninstall();
}
else if (args[1] == "-update")
{
	update();
}