#import "json"
#import "compress"
#include <lib/setuptools/logging.cpp>
#include <lib/setuptools/dist.cpp>
#include <lib/setuptools/colorprint.cpp>
#include <lib/setuptools/ccompiler.cpp>
#include <lib/setuptools/compiler.cpp>
#include <lib/setuptools/msccompiler.cpp>
#include <lib/setuptools/unixcompiler.cpp>


namespace setuptools{
	
	var setup(var option){
		var args = getargs();
		//dist(option);
		var type = "";
		if(args.size() == 3){
			type = args[2];
			if(args[1] == "-install"){
				type = "install";
			}
		}
		if(type == ""){
			log.error("you need input param [dist,install]");
			sleepexit(10);
		}else if(type == "dist"){
			dist(option);
		}
		else if (type == "install")
		{
			println_color("compiler",1);
			var work_path = "{io.getrealpath()}lib/{option["name"]}";

			if(option["ext_modules"] != null){
				foreach(var module: option["ext_modules"]){
					var output_name = option["name"];
					if(module["output_name"]){
						output_name = module["output_name"];
					}
					compiler(output_name,work_path,module);
				}
			}
		}
		
	}
	var find_packages(){
		var ret = [];
		var list = io.walk(".");
		foreach(var filename:list){
			ret.push_back(filename);
		}
		return ret;
	}
}