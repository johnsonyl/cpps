class ccompiler
{
	var sources;
    var output_dir;
    var include_dirs;
    var library_dirs;
    var libraries;
    var debug ;
    var extra_preargs;
    var extra_postargs;
    var depends ;
    var work_path;
    var vcvarsall_path;
    var output_name;
    var define_macros;
    var real_install_path;
    var is_nocpps_build;
    var link_type;
    var real_path = io.getrealpath(); //cppsc 目录
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
	var set_install_path(var tmp_real_install_path){
        real_install_path = tmp_real_install_path;
    }
	var set_nocpps_build(var tmp_is_nocpps_build){
        is_nocpps_build = tmp_is_nocpps_build;
    }
	var set_link_type(var tmp_link_type){
        link_type = tmp_link_type;
    }
    var set_build_type(var tmp_debug)
    {
        if(tmp_debug == null){
            debug = false;
        }
        else 
            debug = tmp_debug;
    }
    
}