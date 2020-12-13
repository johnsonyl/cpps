#import "setuptools"


var long_description = "";
var file = io.fopen("README.md","r");
if(file){
	var buffer = new Buffer();
	io.fread(file,buffer,io.fsize(file));
	long_description = buffer.tostring();
}

/*
如果你需要编译多平台,请指定platfrom 告知编译器使用哪套方案. 可以根据不同平台传递不同的目录地址
var module_all = { platfrom:"linux",sources :['src/demo.cpp'],
                include_dirs:['/usr/local/include'],
                libraries:["curl"],
                library_dirs = ['/usr/local/lib']
                define_macros=[{TEST_DEFINE:"test"}] };

*/

var module1 = { platfrom:"all",sources :['src/demo.cpp']};


setuptools::setup({
	username:"YOUR-USERNAME-HERE",
    name:"testlib", 
    version:"0.0.1",
    author:"Example Author",
    author_email:"author@example.com",
    description:"A small example package",
    long_description:long_description,
    long_description_content_type:"text/markdown",
    url:"https://github.com/pypa/sampleproject",
    packages:setuptools::find_packages(),
    platfrom:"windows", // windows or linux or macos or all
    ext_modules:[module1],
    cpps_requires:'>=1.0',
});

