#import "json"
#import "compress"
#include <lib/setuptools/logging.cpp>
#include <lib/setuptools/dist.cpp>
#include <lib/setuptools/colorprint.cpp>
#include <lib/setuptools/ccompiler.cpp>
#include <lib/setuptools/compiler.cpp>
#include <lib/setuptools/msccompiler.cpp>
#include <lib/setuptools/unixcompiler.cpp>
#include <lib/setuptools/macoscompiler.cpp>


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
			var work_path = "{io.getrealpath()}lib/{option["name"]}";

			if(option["ext_modules"] != null){
				var curos = "";
				if(sys.platform == "win64" || sys.platform == "win32")
					curos = "windows";
				else if(sys.platform == "linux32" || sys.platform == "linux64")
					curos = "linux";
				else if(sys.platform == "macos32" || sys.platform == "macos64")
					curos = "macos";
				var taros = option["platfrom"];
				if(taros != null && curos != taros && taros != "all")
				{
					log.error("The Module Don't Support {curos} Platfrom. Stop Compiler.");
					sleepexit(10);
				}
				
				foreach(var module: option["ext_modules"]){
					var output_name = option["name"];
					if(module["output_name"]){
						output_name = module["output_name"];
					}
					var taros = module["platfrom"];
					
					if(taros == null || curos == taros || taros == "all")
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