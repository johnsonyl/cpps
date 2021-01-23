system("chcp 65001"); //use utf8 string

#import "socket"




//server
var socket_accept(var socketIndex)
{
	println("acceptd:{socketIndex}");
}
var socket_data(var socketIndex,var buffer) 
{
	var packageSize = buffer.readInt32();

	var s = buffer.readString(packageSize-4);
	println(s);

	if(s == "e")
	{
		srv.closesocket(socketIndex);
	}
	
	var writer = new Buffer();
	writer.writeString(s);
	socket_send(socketIndex, writer);
}
var socket_close(var socketIndex,var err,var errstr) 
{
	println("closed :{socketIndex},err:{err},errstr:{errstr}");
}
var socket_parser(var headerbuffer)
{
	var size = headerbuffer.readInt32();
	return size;
}
var socket_send(var socketIndex,var buffer)
{
	var writer = new Buffer();
	writer.writeInt32(buffer.length() + 4);
	writer.write(buffer,buffer.length());
	srv.send(socketIndex, writer);
}

println("start server");


var srv = new socket::server().setoption(new ServerOption(){
							ip = "0.0.0.0",
							headersize = 4,//header 4 bytes
							accept = socket_accept,
							data = socket_data,
							close = socket_close,
							parser = socket_parser
							}).listen(4060);

println("start over");
while (true){
	srv.run();
	Sleep(1);
}
