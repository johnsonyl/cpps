#import "setuptools"


var long_description = io.readfile("README.md");


var module1 = { 
                platfrom:"all",
                sources :["src/main.cpp"],
                include_dirs:["inc"],
                libraries:[],
                library_dirs:[],
                define_macros:[]
                };



setuptools::setup({
	username:"johnsonyl",
    name:"helloworld", 
    version:"0.0.1",
    author:"johnsonyl",
    author_email:"mysqltest@mysqltest.com",
    description:"A setuptools build executable..",
    long_description:long_description,
    debug:false, //release build
    link_type:3,// 1. dynamic Share library 2.Stand library 3.executable  2 or 3 only be used when nocpps is compiled!
    nocpps:true, //no cpps module dynamic Share library
    long_description_content_type:"text/markdown",
    url:"https://github.com/johnsonyl/mysqlexaple",
    packages:setuptools::find_packages(),
    platfrom:"all", // windows or linux or macos or all
    ext_modules:[module1],
    cpps_requires:'>=1.0',
});

