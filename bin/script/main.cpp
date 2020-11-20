import ["json"];
//or
//import "json";
//import "demo";
system("chcp 65001"); //use utf8 string

import ["socket"];




/*

//服务端部分
var socket_accept(var socketIndex)
{
	println("检测到有一个连接:"..socketIndex);
}
var socket_data(var socketIndex,var buffer) 
{
	var s = buffer.tostring();
	println(s);

	if(s == "e")
	{
		srv.closesocket(socketIndex);
	}
	
	var writer = new Buffer();
	writer.writeString(buffer.tostring());
	srv.send(socketIndex, writer);
}
var socket_close(var socketIndex,var err,var errstr) 
{
	println("检测到有一个关闭:{socketIndex},err:{err},errstr:{errstr}");
}
var socket_parser(var headerbuffer)
{

}
println("start server");
var srv = new socket::server().setoption(new ServerOption(){
							ip = "0.0.0.0",
							accept = socket_accept,
							data = socket_data,
							close = socket_close
							}).listen(4060);

// var srv = new socket::server().setOption(new ServerOption(){
// 							ip = "0.0.0.0",
// 							headersize = 8,//字节
// 							accept = socket_accept,
// 							data = socket_data,
// 							close = socket_close,
// 							parser = socket_parser
// 							}).listen(4060);

println("start over");
while (true)
	srv.run();
*/
//客户端部分
var socket_connected()
{
	println("连接成功啦");

	var writer = new Buffer();
	writer.writeString("这是一个乒乓消息");
	client.send(writer);
}
var socket_data(var buffer)
{
	var s = buffer.tostring();
	println(s);

	Sleep(5000);

	var writer = new Buffer();
	writer.writeString(buffer.tostring());
	client.send(writer);
}
var socket_close(var err,var errstr)
{
	println("检测到有一个关闭:err:{err},errstr:{errstr}");
}

var client = new socket::client();
client.setoption(new ClientOption(){
							connected = socket_connected,
							data = socket_data,
							close = socket_close
							});

var b = client.connect("192.168.1.166",4060);



while (true)
{
		client.run();
}