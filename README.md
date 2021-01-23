# cpps script
#### **欢迎**

CPPS 是一个功能强大，运行速度快，容易上手的解释型脚本语言，非常容易学习和使用，并且可以很容易的嵌入到C++项目中。

CPPS 被设计的初衷是让那些和作者一样不喜欢LUA,PY,等等脚本语法的开发者使用，它可以对接各种项目，无论游戏，WEB，或者是人工智能方面。

它的语法极为像C++,所以C++开发者可以很快速的上手，无需太深入的学习它。

*招募协同软件作者,三方模块作者.*

#### **下载与编译**

CPPS 的下载与安装非常简单，CPPS不依赖于任何第三方库，仅仅下载就可以编译。

*LINUX & MACOS*：
-
```
git clone https://github.com/johnsonyl/cpps.git

cmake .

make

make install

;根据需要是否编译扩展模块.
cpps buildlibs

```
*Windows*
-
```
git clone https://github.com/johnsonyl/cpps.git

cmake .

使用Visual Studio打开libcpps.sln进行编译即可。

cd libs

;根据需要是否编译扩展模块.

cmake .

使用Visual Studio打开cppslibs.sln进行编译即可。

```

是不是非常简单？

libs 文件夹是内部扩展模块.具体编译规则请查看文档.

[更新日志](changelog.md) 最后更新日期:2021-01-23 02:58:00 Beijing/cn

脚本文档在

http://docs.cppscript.org

QQ群：

CPPS脚本交流群 282081601

作者Q：88481106


Example:
-

Hello World:
-

```
println("hello world");
```

base64:
-

```
#import "base64"


var s = "abc";
println("source:{s}");
var encode_s = base64.encode(s);
println("encode_s:{encode_s}");
var deocde_s = base64.decode(encode_s);
println("deocde_s:{deocde_s}");
```


asyncio:
-

```

async var test()
{
	println("do test function");
	await asyncio.wait_for(asyncio.sleep(2000),100);
	println("do test function done");
	return "test";

}
async var test1()
{
	println("do test1 function");
	await asyncio.sleep(1000);
	println("do test1 function done");
	
	return "test1";
}
var test_call_back(var task, var context)
{
	println(task.get_name());
	println(context);
}

async var main() {

	var task1 = asyncio.create_task(test());
	task1.set_name("Custom names are used to distinguish");
	task1.add_done_callback(test_call_back, "Custom context content");
	var task2 = asyncio.create_task(test1());

	var ret = await asyncio.wait(task1);
	if (ret.timeout())
	{
		println("task1 was timeouted.");
	}
	println("state:{ret.state()}");
	println(ret.result());
	try {
		println(await task2);
	}
	catch (var e)
	{
		println("oh,we catch some error");
		println("{e.what()} line:{e.line()} file:{e.file()}");
		println(e.callstack());
	}

	println("finish");
}

asyncio.run(main());

```

socket - server:
-

```
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

```

client:
-

```
system("chcp 65001"); //use utf8 string

#import "socket"


//client
var socket_connected()
{
	println("connected");

	var writer = new Buffer();
	writer.writeString("this is a pingpong msg");
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
	println("closed:err:{err},errstr:{errstr}");
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
							headersize = 4,//header 4 bytes
							parser = socket_parser
							});

var b = client.connect("127.0.0.1",4060);



while (true)
{
		client.run();
		Sleep(1);
}
```

httprequest:
-

```
system("chcp 65001"); //use utf8 string

#import "http"

var request = new http::httprequest();
request.setcookiefile("cookies.txt");
//request.setproxy(httpproxy.SOCK5,"192.168.1.166:25455");
//request.setproxyaccount("johnsonyl","mima");
request.addheaders({User-Agent:"cppsrequest/1.1.0"});
var ret = request.get("http://127.0.0.1:8080/Home/test?kkk=100");


println("-------------------GET-------------------------");
println("get:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");


println("-------------------POST-------------------------");
ret = request.post("http://127.0.0.1:8080/Home/test","kkk=100");
println("post:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");
println("-------------------END-------------------------");
```

See More Example: (bin\example)[bin\example]
