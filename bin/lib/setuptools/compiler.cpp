/*
* start compiler cxx to share lib.
 */
var compiler(var output_name,var work_path,var option,var real_install_path,var real_build_type,var is_nocpps_build,var link_type)
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
	if(is_nocpps_build == false && link_type != 1)
	{
		log.error("cpps Module Just Link dynamic Share library. You Can Setup nocpps : false {is_nocpps_build} {link_type}");
		sleep(10);return false;
	}
	if(!compiler_maker){
		log.error("Unknow platfrom. Stop Compiler.");
		sleep(10);return false;
	}
	compiler_maker.set_work_path(work_path);
	compiler_maker.set_output_name(output_name);
	compiler_maker.set_include_dirs(option["include_dirs"]);
	compiler_maker.set_libraries(option["libraries"]);
	compiler_maker.set_library_dirs(option["library_dirs"]);
	compiler_maker.set_define_macros(option["define_macros"]);
	compiler_maker.set_install_path(real_install_path);
	compiler_maker.set_build_type(real_build_type);
	compiler_maker.set_nocpps_build(is_nocpps_build);
	compiler_maker.set_link_type(link_type);
	

	var code = compiler_maker.compiler(option);
	return code;
}