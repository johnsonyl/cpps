#import "compress"
#import "http"

var args = getargs();
var compiler_result = false;
var install_path = "";
var build_type = sys.debug;
var modulename = args[2];
if(modulename == "" || modulename == null) exit(0); 

var download(var file,var showname)
{
	var filesize = 5000;
	var progress_max = 70;
	var x = filesize / progress_max;
	var lastcur = 0;
	println("file: {showname}");
	var cl = ["/","-","\\","|"];
	foreach(var i : xrange(1,filesize))
	{
		var n = toint(i / filesize * 100 );
		var cur = toint(i / x);
		if(cur != lastcur){
			if(n < 99) print("{cl[(cur/20)%4]} ");
			print("[");
			foreach(var j : xrange(1,cur)){
				print("=");	
			}
			print(">");
			foreach(var j: xrange(cur,progress_max)){
				print(" ");
			}
			print("] {n}%/100%");
			print("\r");
			lastcur = cur;

			sleep(10);
		}
	}
	println("");
}

var install()
{
	var modulepath = "lib/{modulename}";
	var exists = io.file_exists(modulepath);
	if(exists)
	{
		println("The {modulename} module is already installed.")
		exit(0);
	}

	println("start install {modulename} module..");

	io.mkdir("tmp");

	//1.获取模块文件
	//post 请求 获取压缩包地址 与 config.json地址
	print("loading module info...");
	println("ok");
	var jsonfilepath = "tmp/mysqltest-johnsonyl-0.0.1-all.json";
	var targzfilepath = "tmp/mysqltest-johnsonyl-0.0.1-all.tar.gz";
	//2.downloader 下载 json 与 tar.gz
	println("start downloading.");
	println("----------------------------------------\r\n");

	

	download(jsonfilepath,"io.getfilename(jsonfilepath)");
	download(targzfilepath,"{io.getfilename(targzfilepath)}");

	println("");
	println("----------------------------------------\r\n");

	//3.校验压缩包
	print("check packages...");
	println("ok");

	//4.解压压缩包
	print("start uncompressed module files...");
	var modulepath = "lib/{modulename}";
	io.mkdirs(modulepath);

	var file = tarfile.open(targzfilepath,"r:gz",102400000);
	if(!file) {
		println("faild");
		uninstall();
		exit(0);
	}
	file.extractall("{modulepath}/");
	println("ok");
	println("\r\n----------------------------------------\r\n");

	install_path = "{io.getrealpath()}lib/{modulename}/";
	println("start compiler module files...\r\n");

	dofile("{modulepath}/setup.cpp"); //compiler
	if(compiler_result)
		println("\r\ninstall {modulename} is success.");
	else
	{
		println("\r\ninstall {modulename} is faild. uninstall {modulename} module.");
		uninstall();
	}

}

var uninstall()
{
	var modulepath = "lib/{modulename}";
	var list = io.walk(modulepath);
	println("start uninstall {modulename}...");
	println("----------------------------------------\r\n");
	var i = list.size()-1;
	for(; i >= 0; i--){
		var path = list[i];
		var stat = io.stat(path);
		print("remove {path} ...");
		if(stat.isdir())
			io.rmdir(path);
		else
			io.remove(path);
		println("ok");
	}
	io.rmdir(modulepath);
	println("\r\n----------------------------------------\r\n");
	println("uninstall {modulename} is done.");
	exit(0);
}

if(args[1] == "-install"){
	install();
}
else if (args[1] == "-uninstall")
{
	uninstall();
}