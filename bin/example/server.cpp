system("chcp 65001"); //use utf8 string

#import "socket"




//服务端部分
var socket_accept(var socketIndex)
{
	println("检测到有一个连接:{socketIndex}");
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
	println("检测到有一个关闭:{socketIndex},err:{err},errstr:{errstr}");
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
// var srv = new socket::server().setoption(new ServerOption(){
// 							ip = "0.0.0.0",
// 							accept = socket_accept,
// 							data = socket_data,
// 							close = socket_close
// 							}).listen(4060);

var srv = new socket::server().setoption(new ServerOption(){
							ip = "0.0.0.0",
							headersize = 4,//字节
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
