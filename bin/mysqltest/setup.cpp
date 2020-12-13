#import "setuptools"


var long_description = "";
var file = io.fopen("README.md","r");
if(file){
	var buffer = new Buffer();
	io.fread(file,buffer,io.fsize(file));
	long_description = buffer.tostring();
}


var module_windows = { 
                platfrom:"windows",
                sources :['src/cpps_mysql.cpp','src/cpps_mysql_record.cpp',"src/cpps_mysql_result.cpp","src/mysql.cpp"],
                include_dirs:["src"],
                libraries:["libmysql.lib","mysqlclient.lib","mysqlservices.lib"],
                library_dirs:[],
                define_macros:[]
                };

var module_linux = { 
                platfrom:"linux",
                sources :['src/cpps_mysql.cpp','src/cpps_mysql_record.cpp',"src/cpps_mysql_result.cpp","src/mysql.cpp"],
                include_dirs:[],
                libraries:["mysqlclient","ssl","crypto"],
                library_dirs:["/usr/lib64/mysql"],
                define_macros:[]
                };
                


setuptools::setup({
	username:"johnsonyl",
    name:"mysqltest", 
    version:"0.0.1",
    author:"johnsonyl",
    author_email:"mysqltest@mysqltest.com",
    description:"A mysql example ext librarie.",
    long_description:long_description,
    long_description_content_type:"text/markdown",
    url:"https://github.com/johnsonyl/mysqlexaple",
    packages:setuptools::find_packages(),
    platfrom:"all", // windows or linux or macos or all
    ext_modules:[module_windows,module_linux],
    cpps_requires:'>=1.0',
});
