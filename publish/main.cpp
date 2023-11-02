
var args = getargs();

if(args.size() == 3)
{
	var arch = args[2];
	var cppspath = io.normpath("{io.getrealpath()}../");
	var publishpath = io.normpath("{io.getcwd()}/windows/{arch}/");

	//bin

	io.mkdirs("{publishpath}deps/deps/bin");
	io.mkdirs("{publishpath}deps/deps/include");
	io.mkdirs("{publishpath}deps/deps/lib");
	
	io.rmdir("{publishpath}bin");
	io.mkdir("{publishpath}bin");

	var binfiles = ["cpps.exe","libmariadb.dll","uv.dll","libcrypto-1_1-x64.dll","libcurl.dll","libmysql.dll","libssl-1_1-x64.dll","msvcp140.dll","vcruntime140.dll","vcruntime140_1.dll","zlib1.dll"];
	foreach(var filename: binfiles)
	{
		var sourcepath = io.normpath("{cppspath}bin/{filename}");
		string.pop_back(sourcepath,1);
		var targetpath = io.normpath("{publishpath}bin/{filename}");
		string.pop_back(targetpath,1);
		println("copy {sourcepath} -> {targetpath}");
		io.copy(sourcepath,targetpath);
	}

	//bin/lib

	var sourcebindirpath = io.normpath("{cppspath}bin/lib");
	var tarbinlibpath = io.normpath("{publishpath}bin/lib");
	string.pop_back(sourcebindirpath,1);
	string.pop_back(tarbinlibpath,1);
	
	io.rmdir(tarbinlibpath);
	io.mkdir(tarbinlibpath);

	var sourcebindirpathfiles = io.walk(sourcebindirpath,true);
	foreach(var sourcefile : sourcebindirpathfiles)
	{
		var tartfile = io.normpath(sourcefile);
		string.pop_back(tartfile,1);
		string.replace(tartfile,sourcebindirpath,tarbinlibpath);

		if(io.isfile(sourcefile)){
			if(string.endswith(sourcefile,".cpp") || string.endswith(sourcefile,".dll")){
				println("copy {sourcefile} -> {tartfile}");
				io.copy(sourcefile,tartfile);
			}
		}
		else if(io.isdir(sourcefile)){
			println("mkdir {tartfile}");
			
			io.mkdir(tartfile);

		}
	}

	//include
	io.rmdir("{publishpath}include");
	io.mkdirs("{publishpath}include/cpps");
	var sourceincludepath = io.normpath("{cppspath}include/cpps");
	var tarincludepath = io.normpath("{publishpath}include/cpps");
	println("copydir {sourceincludepath} -> {tarincludepath}");
	io.copydir(sourceincludepath,tarincludepath);

	//lib
	io.rmdir("{publishpath}lib");
	io.mkdirs("{publishpath}lib");
	var sourcelibfile = "{cppspath}lib/libcpps.lib";
	var tarlibfile = "{publishpath}lib/libcpps.lib";
	println("remove {tarlibfile}");
	io.remove(tarlibfile);
	println("copy {sourcelibfile} -> {tarlibfile}");
	io.copy(sourcelibfile,tarlibfile);
	
	
	
	//deps
	io.rmdir("{publishpath}deps");
	io.mkdirs("{publishpath}deps/deps/bin");
	io.mkdirs("{publishpath}deps/deps/include");
	io.mkdirs("{publishpath}deps/deps/lib");
	var sourceincludepath = io.normpath("{cppspath}deps/deps");
	var tarincludepath = io.normpath("{publishpath}deps/deps");
	println("copydir {sourceincludepath} -> {tarincludepath}");
	io.copydir(sourceincludepath,tarincludepath);
	

	println("publish done.");

	
}