class mscompiler : ccompiler
{
	mscompiler()
	{

	}
	var 	get_builder_version()
	{
		var prefix = "MSC v.";
		var v = string.strcut(sys.builder_version,prefix," ");
		var majorVersion = toint(toint(v) / 100) - 6;
		if(majorVersion >= 13)
			majorVersion++;
		var minorVersion = (toint(v) % 100) / 10;
		if(majorVersion == 6)
			minorVersion = 0;
		if(majorVersion >= 6)
			return majorVersion + minorVersion;
		return null;
	}
	var find_vcvarsall(var ver)
	{
		var vsver = "2019";
		if(ver >= 16.0){
			vsver = "2019";
		}
		else if(ver >= 15.0){
			vsver = "2017";
		}
		else if(ver >= 14.0){
			vsver = "2015";
		}
		else
			return null;
		
		//default installed Visual Studio path. need use cmd> cppsc -install xx -vcvarsall="path" change other path.
		var path = "C:/Program Files (x86)/Microsoft Visual Studio/{vsver}/Community/VC/Auxiliary/Build/vcvarsall.bat";
		var exists = io.file_exists(path);
		if(!exists) return null;
		return path;
	}
	var removeDuplicates(var variable){
		var oldlist = string.split(variable,";");
		var newlist = [];
		foreach(var i : oldlist)
			if(newlist.has(i) == false)
				newlist.push_back(i);
		var newvariable = string.join(";",newlist);
		return newvariable;
	}
	var query_vcvarsall(var vcvarsall,var ver,var arch="x64"){
		var interesting = ["include", "lib", "libpath", "path"];
		var result = {};
		log.debug("Calling 'vcvarsall.bat {arch}' (version={ver})");
		var cmd = '"{vcvarsall}" {arch} & set';
		var out = execmd(cmd);
		string.replace(out,"\r",""); //replace \r to nullstr
		foreach(var line : string.split(out,"\n")){
			if(string.find(line,"=") == string.npos)
				continue;
			//line = string.trim(line);
			var kv = string.split(line,"=");
			var key = string.lower(kv[0]);
			var value = kv[1];
			if(interesting.has(key))
			{
				if(string.endswith(value,";"))
					string.pop_back(value,1);
				result[key] = removeDuplicates(value);
			}
		}
		return result;
	}
	var 	find_exe(var __paths, var exe)
	{
		foreach(var p : __paths){
			var fn = "{p}/{exe}";
			if(io.isfile(fn))
				return fn;
		}

		log.error("oh,no.. I can't found {exe}, you need installed new version Visual Studio..");
		sleepexit(10);
	}
	var normalize_and_reduce_paths(var paths){
		var reduced_paths = [];
		foreach (var p : paths){
			var np = io.normpath(p);
			if(!reduced_paths.has(np))
				reduced_paths.push_back(np);
		}
		return reduced_paths;
	}
	var		compiler(var option)
	{
		var cl = "cl.exe";
		var link = "link.exe";
		var lib = "lib.exe";
		var rc = "rc.exe";
		var mc = "mc.exe";
		var ver = get_builder_version();
		if(ver < 14.0){
			log.error("msc build need mscver > 14.0. current mscver is {ver}.");
			sleepexit(10);
		}
		var vcvarsall = vcvarsall_path;
		if(vcvarsall == null ) 
			vcvarsall = find_vcvarsall(ver);
		if(vcvarsall == null || vcvarsall == ""){
			log.error("Unable to find vcvarsall.bat");
			sleepexit(10);
		}
		var arch = "x64";
		if(sys.platform == "win64" )
			arch = "x64";
		else if(sys.platform == "win32")
			arch = "x86"

		var vc_env = query_vcvarsall(vcvarsall,ver,arch);
		
		var __paths = string.split(vc_env["path"],";");
		environ.set("lib",vc_env["lib"]);
		environ.set("include",vc_env["include"]);

		if(len(__paths) == 0)
		{
			log.error("env path is empty, install faild.");
			sleepexit(10);
		}

		cl = find_exe(__paths,"cl.exe");
		link = find_exe(__paths,"link.exe");
		lib = find_exe(__paths,"lib.exe");
		rc = find_exe(__paths,"rc.exe");
		mc = find_exe(__paths,"mc.exe");

		foreach(var p : string.split(environ.get("path"),";"))
			if(p != "") __paths.push_back(p);	

		__paths = normalize_and_reduce_paths(__paths);

		environ.set("path",string.join(";",__paths));
		var preprocess_options = null;
		var compile_options;
		var compile_options_debug;
		if(arch == "x86"){
			compile_options = [ '/nologo', '/EHsc' , '/Ox', '/MD', '/W3',
                                     '/DNDEBUG', '/D_CRT_SECURE_NO_WARNINGS',"/DWIN32","/D_WINDOWS"];
			compile_options_debug = ['/nologo', '/EHsc', '/Od', '/MDd', '/W3',
                                          '/Z7', '/D_DEBUG', '/D_CRT_SECURE_NO_WARNINGS',"/DWIN32","/D_WINDOWS"];
		}
		else{
			compile_options = [ '/nologo', '/EHsc', '/Ox', '/MD', '/W3', '/GS-' ,
                                     '/DNDEBUG', '/EHsc','/D_CRT_SECURE_NO_WARNINGS',"/DWIN32","/D_WINDOWS"];
            compile_options_debug = ['/nologo', '/Od', '/MDd', '/W3', '/GS-',
                                          '/Z7', '/D_DEBUG' ,'/D_CRT_SECURE_NO_WARNINGS',"/DWIN32","/D_WINDOWS"];
		}
		var ldflags_shared;
		var ldflags_shared_debug;
		var ldflags_static;
		if(arch == "x86"){
			ldflags_shared = ['/DLL', '/nologo', '/INCREMENTAL:NO'];
			ldflags_shared_debug = [
					'/DLL', '/nologo', '/INCREMENTAL:no', '/DEBUG'
					];
			ldflags_static = [ '/nologo'];
		}
		else{
			ldflags_shared = ['/DLL', '/MACHINE:X64' , '/nologo', '/INCREMENTAL:NO'];
			ldflags_shared_debug = [
					'/DLL', '/nologo', '/MACHINE:X64' ,'/INCREMENTAL:no', '/DEBUG'
					];
			ldflags_static = [ '/nologo'];
		}
		if(define_macros != null){
			foreach(var macros:define_macros){
				compile_options.push_back('/D{macros}');
				compile_options_debug.push_back('/D{macros}');
			}
		}
		//baselib
		var complier_base_libs = ['"cpps.lib"','"kernel32.lib"','"user32.lib"','"gdi32.lib"','"winspool.lib"','"shell32.lib"','"ole32.lib"','"oleaut32.lib"','"uuid.lib"','"comdlg32.lib"','"advapi32.lib"'];
		if(libraries != null){
			foreach(var lib:libraries){
				complier_base_libs.push_back('"{lib}"');
			}
		}

		//baseinclude
		var base_include_cpps = '{work_path}../../../include';
		base_include_cpps = io.normpath(base_include_cpps);
		if(string.endswith(base_include_cpps,"\\"))
			string.pop_back(base_include_cpps,1);

		//base deps include
		var base_deps_include_cpps = '{work_path}../../../deps/deps/include';
		base_deps_include_cpps = io.normpath(base_deps_include_cpps);
		if(string.endswith(base_deps_include_cpps,"\\"))
			string.pop_back(base_deps_include_cpps,1);

		var complier_base_includes = ['/I"{base_include_cpps}"','/I"{base_deps_include_cpps}"'];
		if(include_dirs != null){
			foreach(var inc:include_dirs){
				complier_base_includes.push_back('/I"{work_path}{inc}"');
			}
		}
	
		//baselibpath
		
		var base_lib_cpps = '{work_path}../../../lib';
		base_lib_cpps = io.normpath(base_lib_cpps);
		if(string.endswith(base_lib_cpps,"\\"))
			string.pop_back(base_lib_cpps,1);

		var base_lib_deps_path_cpps = '{work_path}../../../deps/deps/lib';
		base_lib_deps_path_cpps = io.normpath(base_lib_deps_path_cpps);
		if(string.endswith(base_lib_deps_path_cpps,"\\"))
			string.pop_back(base_lib_deps_path_cpps,1);
		
		var complier_base_lib_path = ['/LIBPATH:"{base_lib_deps_path_cpps}"','/LIBPATH:"{base_lib_cpps}"'];
		if(library_dirs != null){
			foreach(var libpath:library_dirs){
				var tmppath = "{work_path}{libpath}";
				tmppath = io.normpath(tmppath);
				complier_base_lib_path.push_back('/LIBPATH:"{tmppath}"');
			}
		}

        //生成完整cpp 与 obj地址
        sources = option["sources"];
        var newsources = [];
        var objs = [];
        foreach(var p : sources){
        	var fn = io.getfilename(p);
        	var path = io.normpath("{work_path}{io.getfilepath(p)}");
        	var full_path = "{path}{fn}";
        	objs.push_back("{full_path}.obj");
        	newsources.push_back(full_path);
        	
        }
        println_color("Building Path:{work_path}",6);
        
		debug = true;
		
        var c = newsources.size();
        foreach(var i : xrange(0,c-1)){
        	var src = newsources[i];
        	var obj = objs[i];
        	var opt;
			if(!debug) 
				opt = string.join(" ",compile_options);
			else
				opt = string.join(" ",compile_options_debug);
        	var inc = string.join(" ",complier_base_includes);
        	print("[{i+1}/{c}]");
        	println_color(" Building CXX object {sources[i]} -> {sources[i]}.obj",2);
			var cmd = '{cl} /c {opt} {inc} /Fo"{obj}" "{src}"';
        	var s = execmd(cmd);
        	if(string.find(src,s) != string.npos ) { println_color(s,1);}
        	

        }

        println_color("Linking CXX shared library {output_name}.dll",3);
        var opt = "";
		if(!debug)
			opt = string.join(" ",ldflags_shared);
		else
			opt = string.join(" ",ldflags_shared_debug);

        var libs = string.join(" ",complier_base_libs);
        var lib_paths = string.join(" ",complier_base_lib_path);
        var objslist = string.join(" ",objs);
		var cmd = '{link} {opt} {lib_paths} "/out:{work_path}{output_name}.dll" {libs} {objslist}';
        var s = execmd(cmd);
		if(string.find(s,".exp") == string.npos) println_color(s,1);
		else println_color("compiler is done.",2); 
		return 0;
	}
}
