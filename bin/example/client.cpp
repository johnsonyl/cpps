system("chcp 65001"); //use utf8 string

#import "socket"


//客户端部分
var socket_connected()
{
	println("连接成功啦");

	var writer = new Buffer();
	writer.writeString("这是一个乒乓消息");
	socket_send(writer);
}
var socket_data(var buffer)
{
	var packageSize = buffer.readInt32();
	var s = buffer.readString(packageSize-4);
	println(s);

	Sleep(100);

	
	var writer = new Buffer();
	writer.writeString(s);
	socket_send(writer);
}
var socket_close(var err,var errstr)
{
	println("检测到有一个关闭:err:{err},errstr:{errstr}");
}
var socket_parser(var headerbuffer)
{
	var size = headerbuffer.readInt32();
	return size;
}
var socket_send(var buffer)
{
	var writer = new Buffer();
	var size = buffer.length();
	writer.writeInt32(size + 4);
	writer.write(buffer,buffer.length());
	client.send(writer);
}
var client = new socket::client();
client.setoption(new ClientOption(){
							connected = socket_connected,
							data = socket_data,
							close = socket_close
							headersize = 4,//字节
							parser = socket_parser
							});

var b = client.connect("127.0.0.1",4060);



while (true)
{
		client.run();
		Sleep(1);
}