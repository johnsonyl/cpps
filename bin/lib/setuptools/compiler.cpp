/*
* start compiler cxx to share lib.
 */
var compiler(var output_name,var work_path,var option)
{
	var compiler_maker = null;
	if(sys.platform == "win64" || sys.platform == "win32"){
		if(option["platfrom"] != "windows") return -1;
		compiler_maker = new mscompiler();
	}else if(sys.platform == "linux32" || sys.platform == "linux64"){
		if(option["platfrom"] != "linux") return -1;
		compiler_maker = new unixcompiler();
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