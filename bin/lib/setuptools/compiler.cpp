/*
* start compiler cxx to share lib.
 */
var compiler(var output_name,var work_path,var option)
{
	var compiler_maker = null;
	var plantfrom = option["platfrom"] ;
	
	if(sys.platform == "win64" || sys.platform == "win32"){
		if(plantfrom == null || plantfrom == "windows" || plantfrom == "all")
			compiler_maker = new mscompiler();
	}
	else if(sys.platform == "linux32" || sys.platform == "linux64"){
		if(plantfrom == null || plantfrom == "linux" || plantfrom == "all")
			compiler_maker = new unixcompiler();
	}
	else if(sys.platform == "macos32" || sys.platform == "macos64"){
		if(plantfrom == null || plantfrom == "macos" || plantfrom == "all")
			compiler_maker = new macoscompiler();
	}		
	if(!compiler_maker){
		log.error("Unknow platfrom. Stop Compiler.");
		sleepexit(10);
	}
	compiler_maker.set_work_path(work_path);
	compiler_maker.set_output_name(output_name);
	compiler_maker.set_include_dirs(option["include_dirs"]);
	compiler_maker.set_libraries(option["libraries"]);
	compiler_maker.set_library_dirs(option["library_dirs"]);
	compiler_maker.set_define_macros(option["define_macros"]);
	var code = compiler_maker.compiler(option);
	return code;
}