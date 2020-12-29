#import "setuptools"


var long_description = io.readfile("README.md");


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

var module_macos = { 
                platfrom:"macos",
                sources :['src/cpps_mysql.cpp','src/cpps_mysql_record.cpp',"src/cpps_mysql_result.cpp","src/mysql.cpp"],
                include_dirs:["/usr/local/opt/mysql-client/include"],
                libraries:["mysqlclient","ssl","crypto"],
                library_dirs:["/usr/local/opt/mysql-client/lib"],
                define_macros:[]
                };
                


setuptools::setup({
	username:"zheng1",
    apikey:"39ae9ae32348147b4c7a53e3a25c2bac",
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
    ext_modules:[module_windows,module_linux,module_macos],
    cpps_requires:'>=1.0.0',
});

