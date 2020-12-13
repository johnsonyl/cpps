class ccompiler
{
	var sources;
    var output_dir;
    var include_dirs;
    var library_dirs;
    var libraries;
    var debug;
    var extra_preargs;
    var extra_postargs;
    var depends ;
    var work_path;
    var vcvarsall_path;
    var output_name;
    var define_macros;
    var set_work_path(var path){
    	work_path = io.normpath(path);
    }

    var set_output_name(var name){
    	output_name = name;
    }
    var set_include_dirs(var tmp_include_dirs){
        include_dirs = tmp_include_dirs;
    }
	var set_libraries(var tmp_libraries){
        libraries = tmp_libraries;
    }
	var set_library_dirs(var tmp_library_dirs){
        library_dirs = tmp_library_dirs;
    }
	var set_define_macros(var tmp_define_macros){
        define_macros = tmp_define_macros;
    }
}