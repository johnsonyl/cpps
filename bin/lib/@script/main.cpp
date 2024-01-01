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
	g_dostring(_line);
}