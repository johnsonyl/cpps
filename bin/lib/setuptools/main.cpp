#import "json"
#import "compress"

namespace setuptools{
	//生成安装文件
	var setup(var option){
		io.mkdir("dist");
		var filename = "{option["name"]}-{option["username"]}-{option["version"]}-{option["platfrom"]}";
		var configname = "dist/{filename}.json";
		print("build config json file...");
		var jsonval = json.encode(option);
		io.remove(configname);
		var jsonfile = io.fopen(configname,"wb+");
		if(jsonfile){
			io.fwrites(jsonfile,jsonval);
			io.fclose(jsonfile);
			console.color(2);
			println("success!");
			console.clearcolor();
		}
		else{
			console.color(1);
			println("faild!");
			console.clearcolor();
			return;
		}

		var targzname = "dist/{filename}.tar.gz";
		io.remove(targzname);
		print("create tar.gz file...");
		var file = tarfile.open(targzname,"x:gz");
		if(!file){

			console.color(1);
			println("faild!");
			console.clearcolor();
			return;	
		} 
		console.color(2);
		println("success!");
		console.clearcolor();

		var projectpath = "{option["name"]}/";
	
		var count = option["packages"].size();
		var idx = 1;
		foreach(var filename : option["packages"]){
			var pos = string.find(filename,projectpath);
			var filename2 = filename;
			if(pos == 0){
				filename2 = string.substr(filename,string.length(projectpath),string.npos);
			}
			console.color(4);
			print("[{idx}/{count}] ");
			idx++;
			console.clearcolor();

			print("{filename} ...");
			var fileinfo = file.gettarinfo(filename,filename2);	
			if(fileinfo){
				file.addfile(fileinfo);
				console.color(2);
				println("ok");
				console.clearcolor();
			}
		}
		print("saving compressed file...");
		file.close();
		console.color(2);
		println("success!");
		console.clearcolor();
		exit(0);
	}
	var find_packages(var projectname){
		var ret = [];
		var b = io.file_exists("README.md");
		if(b) ret.push_back("README.md");
		var b = io.file_exists("LICENSE");
		if(b) ret.push_back("LICENSE");

		var list = io.walk(projectname);
		foreach(var filename:list){
			ret.push_back(filename);
		}
		return ret;
	}
}