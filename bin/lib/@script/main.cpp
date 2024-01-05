println("Type CPPS syntax to run.");

var quit(){
	exit(0);
}
var help(var _module_)
{
	println("help")
	if(_module_ == null)
	{

	}
}

while(true){
	print(">>>");
	var _line = io.getline();
	if(_line == "quit") exit(0);
	if(_line == "help") {
		help();
		continue;
	}
	try{
		g_dostring(_line);
	}
	catch(var e){
		println(e);
		
		if(e.what() == "Unexpected end"){
			g_dostring("if(isvalid({_line})) echo {_line}; else echo nil;");
		}else{
			println("{e.what()}");	
		}
	}
}