#import "json"
#import "compress"
#import "http"
#include <lib/setuptools/logging.cpp>
#include <lib/setuptools/dist.cpp>
#include <lib/setuptools/upload.cpp>
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
				type = "build";
			}
		}
		if(type == ""){
			log.error("you need input param [dist,install]");
			return false;
		}else if(type == "dist"){
			dist(option);
		}
		else if(type == "upload"){
			var ret = dist(option);
			if(ret) upload(option);
		}
		else if (type == "build")
		{
			var work_path = io.getcwd(); //工作目录
			var real_install_path = "{work_path}/dist/"; //编译目录
			var real_build_type = sys.debug;
			var link_type = 1; // 1. dynamic Share library 2.Stand library 3.executable  2 or 3 only be used when nocpps is compiled!


			var is_nocpps_build = false;
			if(isset(install_path)) {
				real_install_path = install_path;
				work_path = install_path;
			}
			if(isset(build_type))
				real_build_type = build_type;
			if( option["link_type"] != null)
				link_type = option["link_type"];

			if( option["nocpps"] != null)
				is_nocpps_build =  option["nocpps"];
			
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
					return false;
				}
				
				foreach(var mod: option["ext_modules"]){
					var output_name = option["name"];
					if(mod["output_name"]){
						output_name = mod["output_name"];
					}
					var taros = mod["platfrom"];
					
					if(taros == null || curos == taros || taros == "all"){
						var code = compiler(output_name,work_path,mod,real_install_path,real_build_type,is_nocpps_build,link_type);
						if(code == false){
							return false;
						}
					}
				}
			}
			if(isset(compiler_result)) compiler_result = true; //返回通知编译结果.
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