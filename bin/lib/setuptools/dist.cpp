//生成安装文件
var dist(var option){
	io.mkdir("dist");
	var filename = "{option["name"]}-{option["username"]}-{option["version"]}-{option["platfrom"]}";
	var configname = "dist/{filename}.json";
	

	var targzname = "dist/{filename}.tar.gz";
	io.remove(targzname);
	print("create tar.gz file...");
	var file = tarfile.open(targzname,"x:gz");
	if(!file){

		println_color("faild!",1);
		return false;	
	} 
	println_color("success!",2);

	var projectpath = "{option["name"]}/";

	var count = 0;
	foreach(var filename : option["packages"]){
		var pos = string.find(filename,"./");
		var filename2 = filename;
		if(pos == 0){
			filename2 = string.substr(filename,2,string.npos);
		}
		pos = string.find(filename2,"dist/");
		if(pos == 0){
			continue;
		}
		pos = string.find(filename2,"dist");
		if(pos == 0){
			continue;
		}
		count++;
	}
	var idx = 1;
	foreach(var filename : option["packages"]){
		var pos = string.find(filename,"./");
		var filename2 = filename;
		if(pos == 0){
			filename2 = string.substr(filename,2,string.npos);
		}
		pos = string.find(filename2,"dist");
		if(pos == 0){
			continue;
		}

		print_color("[{idx}/{count}] ",4);
		idx++;

		print("{filename} -> {filename2} ...");
		var fileinfo = file.gettarinfo(filename,filename2);	
		if(fileinfo){
			file.addfile(fileinfo);
			println_color("ok",2);
		}
	}
	print("saving compressed file...");
	var sourcetargzsize = file.close();
	if(sourcetargzsize == -1){
		println_color("faild!",2);
		return false;
	}
	else
		println_color("success!",2);
	
	print("build config json file...");
	option["sourcetargzsize"] = sourcetargzsize;
	var jsonval = json.encode(option);
	io.remove(configname);
	var jsonfile = io.fopen(configname,"wb+");
	if(jsonfile){
		io.fwrites(jsonfile,jsonval);
		io.fclose(jsonfile);
		println_color("success!",2);
	}
	else{
		println_color("faild!",1);
		return false;
	}

	return true;
}