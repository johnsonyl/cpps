*更新日志*：
-
2021-01-01 更新
-

新年快乐.

1.http://docs.cppscript.org 文档部分完成95%啦.

2.修复了一些已知的BUG.

3.年前终于可以正式发布第一版CPPS了.

2020-12-29 更新
-

1.修改一些BUG

2.降低CMAKE要求版本

3.添加WIN所需头文件

4.最近在整理最新文档工作.

2020-12-25 更新
-

1.httpserver增加模板标签@page,@csrf_token

2.完善string.trim

3.cpps::object::create_with_classvar与 cpps::object::create_with_cppsclassvar调换一下名字

4.修改httpserver-session存的值可以是任意cpps_value

5.depens for windows 增加一些deps,mysql的lib传不到github不知道为什么.

6.增加string类型有自己的函数支持了.并不需要stirng.XXX() 来操作字符串了.


2020-12-23 更新2
-

1.增加httpserver的cachefile功能

2.增加dump功能,可以自己定义类实现dump输出和返回

3.恢复类this的功能,修改为关键字非成员变量.

2020-12-23 更新
-

1.调整项目结构

2.增加类似django的template

3.httpserver增加基础session功能

4.增加完整c++17支持.

5.增加msvc编译时使用预编译头,增加编译速度.

2020-12-21 更新2
-

1 .增加类似linq的 where select函数

2. 增加一个centos下的编译批处理

3. 暂时屏蔽掉curl库,因为用yum安装的libcurl版本有点老.

4. 解决三元表达式某些时候错误的BUG.

5. 增加hiredis&xredis支持库(未完成)


2020-12-21 更新
-

1.增加http模块uploader类.

2.增加socket模块httpserver对formdata格式的解析支持

3.增加uuid(GUID)模块

4.增加random模块

5.增加一些常用函数io.writefile,io.filesize,io.copy,io.copydir,io.move,io.movedir,string.startswith,sort等

6.增加println 和 type可以打印出类的名称

7.foreach 增加range支持

2020-12-20 更新
-

1.移除stl的map unordered_map使用https://github.com/greg7mdp/parallel-hashmap   (极力推荐)

cpps中就统一map了,不在区分map与unordered_map.

2.调整macos安装目录/usr/local/opt/cpps

3.移动头文件到include/cpps 中,修改包含头文件为 #include <cpps/cpps.h>

1.移除stl的map unordered_map4.修复已知BUG.

2020-12-19 更新
-

1.完善object类.

2020-12-18 更新
-

1.调整代码结构.

2.增加c++部分 cpps::object 支持.(C++ 调用 脚本)

```
c++ page

#include <iostream>
#include "cpps.h"

using namespace cpps;
using namespace std;

namespace cpps { std::string cpps_io_getfilepath(std::string str);std::string cpps_rebuild_filepath(std::string path);std::string cpps_real_path(); }

class CppClassTest
{
public:
	void	testFunc(cpps::object val)
	{
		if (val.isint())
		{
			printf("val:%d\r\n", object_cast<int32>(val));
		}
		printf("%s:%s\r\n", "CppClassTest::testFunc",type_s(val).c_str());
	}
};
int32 main(int argc,char **argv)
{
	std::string path = "main.cpp";

	if (argc >= 2) {
		path = argv[1];
	}



	path = cpps_rebuild_filepath(path);
#ifdef WIN32
	SetCurrentDirectoryA(cpps_io_getfilepath(path).c_str());
#else
	chdir(cpps_io_getfilepath(path).c_str());
#endif


	C* c = cpps::create(argc,argv);

	cpps::_module(c)[
		_class<CppClassTest>("CppClassTest")
			.def("testFunc",&CppClassTest::testFunc)
	];

	_CPPS_TRY
	cpps::dofile(c, path.c_str());
	_CPPS_CATCH;

	/*
	// script page

	class Agent
	{
		var val;
		var test()
		{
			println(val);
		}
	}

	var a = [1,2,3,4,1000];

	var b = { a:1 , b:2 ,c:3};
	
	*/

	//loop vector
	cpps::object a = cpps::object::globals(c)["a"];
	for (cpps::object v : cpps::object::vector(a))
	{
		cpps_integer val = v.toint();
		cout << val << endl;
	}
	//print vector a value
	cpps::object vv = a[4];
	cout << "a[4] = " << object_cast<cpps_integer>(vv) << endl; 
	a.set(4, cpps::object::create(c, 2000));
	vv = a[4];
	cout << "a[4] = " << object_cast<cpps_integer>(vv) << endl;

	cpps::object::vector vec(a);
	vec[4] = cpps::object::create(c, 3000);
	cout << "a[4] = " << object_cast<cpps_integer>(vec[4]) << endl;


	//////////////////////////////////////////////////////////////////////////

	//loop map
	cpps::object b = cpps::object::globals(c)["b"];
	for (auto v : cpps::object::map(c,b))
	{
		std::string key = object_cast<std::string>(v.first); // object_cast or std::string key = object(v.first).tostring();
		cpps_integer val = cpps::object(v.second).toint();

		cout << key << ":" << val << endl;
	}
	//print map a value.
	cpps::object bb = b["b"];
	cout << "b['b'] = " << object_cast<cpps_integer>(bb) << endl;
	b.set("b", cpps::object::create(c, 2000)); // It's Work.
	bb = b["b"];
	cout << "It's Work b['b'] = " << object_cast<cpps_integer>(bb) << endl;


	b.set("z", cpps::object::create(c, 2000)); //  It doesn't work. Because z doesn't exist
	cpps::object bz = b["z"];
	cout << "doesn't work b['z'] type = " << cpps::type_s(bz) << endl;


	cpps::object key = cpps::object::create(c, "z");
	b.set(key, cpps::object::create(c, 2000));  // It's Work. z must be created.
	bz = b["z"];
	cout << " It's Work b['z'] = " << object_cast<cpps_integer>(bz) << endl;


	cpps::object::map bmap(c, b);
	bmap["z00"] = cpps::object::create(c, "hello world");
	cout << "bmap-> It's Work bmap['z00'] = " << object_cast<std::string>(bmap["z00"]).c_str() << endl;

	bmap[1] = cpps::object::create(c, "interge key it's work too");
	cout << "bmap-> It's Work bmap[1] = " << object_cast<std::string>(bmap[1]).c_str() << endl;

	//////////////////////////////////////////////////////////////////////////

	//defined a global var;
	cpps::object::define(c, "x", cpps::object::create(c, "hello world"));

	cout << "x = " << object_cast<std::string>(cpps::object::globals(c)["x"]).c_str() << endl;
	cpps::object::globals(c)["x"] = cpps::object::create(c, "wonderful world");
	cout << "x = " << object_cast<std::string>(cpps::object::globals(c)["x"]).c_str() << endl;

	_G(c)["x"] = cpps::object::create(c, "_G get var it's work.");// work too
	cout << "x = " << object_cast<std::string>(_G(c)["x"]).c_str() << endl;

	//////////////////////////////////////////////////////////////////////////

	//new script class var ,and set dofunction.
	cpps::object Agent = cpps::object::globals(c)["Agent"];
	if (Agent.isclass())
	{
		cpps::object Agentvar = cpps::object::create_with_classvar(c, Agent);
		Agentvar.set("val", cpps::object::create(c, "this is string.") );
		cpps::object testfunc = Agentvar["test"];
		if (testfunc.isfunction())
		{
			cpps::doclassfunction(c, Agentvar,testfunc);
		}
		Agentvar["val"] = cpps::object::create(c, "change string value.");
		if (testfunc.isfunction())
		{
			cpps::doclassfunction(c, Agentvar, testfunc);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//new cpp class var
	CppClassTest* ptr = NULL; //You don't need to delete it. it has GC to delete it.
	cpps::object cppclassvar = cpps::object::create_with_cppclassvar< CppClassTest >(c, &ptr);
	//ptr->testFunc(xxx); //you can do something in cpp .

	cpps::object testFunc = cppclassvar["testFunc"];
	if (testFunc.isfunction()) {
		cpps::doclassfunction(c, cppclassvar, testFunc); //or get function and call it.
	}

	if (testFunc.isfunction()) {
		cpps::doclassfunction(c, cppclassvar, testFunc,cpps::object::create(c,123456)); //or get function and call it.
	}

	if (testFunc.isfunction()) {
		cpps::doclassfunction(c, cppclassvar, testFunc,cpps::object::create(c,"asdfsadf")); //or get function and call it.
	}



	cpps::close(c);

	return 0;
}
```

2020-12-17 更新
-

完善GC,根据新生代,老生代与使用数量判断是否可以释放.

曾经只根据全局变量加局部变量来判断是否需要GC,导致一些返回值与传参都要注册很多不必要的临时变量证明它不需要释放.

现在增加了计数功能. 当变量不被全局与局部使用的时候.并且计数为0时就可以被释放了.

这样做下一步就可以实现脚本多线程了.

2020-12-16 更新
-

1. 增加支持多返回值.

```
var func2()
{
    return "sadf";
}
var func1()
{
    return 1,2,func2();
}

var [a,b,e] = func1();

println(a);
println(b);
println(e);
```

2020-12-15 更新2

1. socket模块httpserver的 request增加 get post接口可以分别取get post对应变量

2.增加serializer序列化模块,可以将类对象序列化成vector或者map. 也可以反序列化回对象.

```
class ABC{
    var a = 10;
    var b = 20;
    var c = "123123";
}

var a = new ABC();

var l = serializer.encode(a,serializer.map);

var a2 = serializer.decode(ABC,l);
println(a2.b);

```

2020-12-15 更新

1.增加smtp模块,可以发邮件了.

2.增加htmlparser模块.

3.增加quotedprintable模块,用于邮件encode

4.增加io.readfile函数, vector map增加empty函数, 增加string.cut函数切割文本.

5.增加脚本定义module模块,可以在c++模块基础上扩展脚本模块.

6.修复tar打包顺序错误问题导致其他解压缩软件报错

7.移除一些windows的警告.


```
#import "smtp"

//基础版本 
var code = smtp.sendmail({
			host:"smtp.aliyun.com",                     //必填
            port:25,                                    //必填
			username:"********@aliyun.com",            //必填
			pwd:"**********",                            //必填
			content:"<a>hello</a>",                    //必填
            subject:"hello",                           //必填
			messageid:"{time.gettime()}@aliyun.com",      //按这个格式换下
			to:[
                ["88481106@qq.com","名字"],             //2种格式
            ],
            onread:[](var s){
                println(s);
            }
		});
println(code);


//旗舰版本
// smtp.sendmail({
// 			host:"smtp.aliyun.com",                     //必填
//             port:25,                                    //必填
// 			username:"********@aliyun.com",            //必填
// 			pwd:"*********",                            //必填
//             ssl:false,
//             timeout:30,
// 			mailfrom:"*********@aliyun.com",
// 			from:"******@aliyun.com",
// 			sender:"johnson",
// 			content:"<a>hello</a>",                    //必填
//             subject:"hello",                           //必填
//             verbose:true,
//             encoding:2,
// 			boundary:smtp.makeboundary(),
// 			messageid:"{time.gettime()}@aliyun.com",      //按这个格式换下
// 			charset:"UTF-8",
// 			tls:false,
// 			to:[
//                 ["88481106@qq.com","名字"],             //2种格式
//                 "14827@qq.com"
//             ],
// 			cc:[],
// 			bcc:[],
// 			headers:{
// 				GUID:"MakeGUID", //自定义邮件头
// 			},
// 			attachments:{ //附件
// 				aaa.txt:{
// 					type:"text/plain", 
// 					id:"content-id-test",
// 					content:"aaaa",
// 				},
//                 bbb.txt:{
// 					type:"application/x-msdownload", 
// 					id:"content-id-test",
// 					content:"bbbb",
// 				}
// 			}
// 		});
```

```
脚本扩展模块例子

module sys
{
   var test()
   {
		println("sys.test()");
   }
}

sys.test();

```

```
var html = io.readfile("test.html");

var parser = new htmlparser::parser();
var element = parser.parse(html);
if(element)
{
    println(element.text());
}
```

```
```

2020-12-14 更新
-

1.增加使用setuptools编译例子 bin/helloworld-setuptools-build-example(独立编译可执行文件), mysqltest( 如何编译扩展mysql到cpps示例. )

2.setuptools 已经基本完成功能:打包,编译动态,静态以及可执行文件

3.修复 io.getrealpath() 在 macos下会获取软连接地址的错误.

4.增加isset 判断是否存在变量,可以再dofile 跨文件中来判断全局变量是否存在并且不出warning警告.


2020-12-13 更新
-

1.调整module注册改为统一cpps::_module()

2.所有库已可以在macos-clang下编译.终于三大平台都通了.

3.setuptools 半成品.已经接近尾声了.

2020-12-11 更新
-

1.增加一个基于cpps的MVC框架.目录 bin/webbase

2020-12-10 更新
-

1. 修复[]偶发找不到变量的BUG.

2. 修复#include 对应文件与行号不一致的BUG

3. 更新socket::httpserver path都以小写来检测

4. 增加http模块 downloader类

5. cppsc 支持部分命令 可以查看-h 或 -help  , 例子 cppsc -h

6. linux walk支持.

7. mysql模块  增加affected_rows 获取最后一次执行影响行数

8. 取消cppsc 可继续执行语句功能.

9. 修改cppsc默认当前目录main.cpp

10. 修改cppsc 为建立软连接模式

2020-12-9 更新
-

1.增加compress库 

1.1 支持 zlib gzip 解压与压缩 

1.2 增加 zip ,tar, tar,gz 的文件压缩与解压缩.

注: bz2 lzma 暂时未支持.后续更新.

```
system("chcp 65001"); //use utf8 string

#import "compress"

 var srcstr = "123456789";
//zlib
var tarstr = zlib.compress(srcstr);
println(string.length(tarstr));
var srcstr2 = zlib.decompress(tarstr);

println(srcstr2);

println(zlib.adler32(srcstr2));
println(zlib.crc32(srcstr2));

println(zlib.ZLIB_VERSION);
println(zlib.ZLIB_RUNTIME_VERSION());


//gzip
//
var tarstr = gzip.compress(srcstr);
println(string.length(tarstr));
var srcstr2 = gzip.decompress(tarstr);

println(srcstr2);




// //zipfile
var file = zipfile.open("test.zip");
file.extractall();

//输出个别需要文件
foreach(var info: file.infolist()){
	if(!info.is_dir()){
		var buffer = file.read(info.filename());
		if(buffer){
			var filename = io.getfilename(info.filename());
			println(filename);
			var file = io.fopen(filename,"wb+");
			if(file){
				println("save ok");
				io.fwrite(file,buffer);
				io.fclose(file);
			}
		}
	}
}

//zip 追加文件
var file = zipfile.open("test2.zip","","a");
file.write("client.cpp");
file.close();

//tarfile
//tar解压缩
var file = tarfile.open("test.tar","r",10240000);
file.extractall();

//tar.gz 解压缩
var file = tarfile.open("test.tar.gz","r:gz",10240000);
file.extractall();


//tar.gz 写入
var file = tarfile.open("test2.tar.gz","x:gz",10240000);

var fileinfo = file.gettarinfo("server.cpp");

file.addfile(fileinfo);
file.close();


```

2020-12-6 更新
-

1.增加协程内置模块 asyncio 

```
//无需import asyncio...

async var test()
{
	println("do test function");
	await asyncio.sleep(2000);
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
var test_call_back(var task,var context)
{
	println(task.get_name());
	println(context);
}

async var main(){

	var task1 = asyncio.create_task(test());
	task1.set_name("自定义名字用于区分");
	task1.add_done_callback(test_call_back,"自定义context内容");
	var task2 = asyncio.create_task(test1());

	var ret = await asyncio.wait(task1);
	if(ret.timeout())
	{
		println("task1 was timeouted.");
	}
	println("state:{ret.state()}");
	println(ret.result());
	println(await task2);
	task1.cleanup();// asyncio.wait or asyncio.wait_for need cleanup.
	//task2.cleanup(); // crash here..  await will cleanup it.

}

asyncio.run(main());


```

2020-12-5 更新
-

1.增加lambda闭包.类似js.解决了内存泄露问题.

2. 优化cpps运行速度.cpps_value减少一个8字节指针,理论又提速了.

```
var outer() {
      var result = [];
      result.resize(10);

      for (var i = 0; i<10;i++){
        result[i] = [](var num){
             return [](){
                   println(num);   // 此时访问的num，是上层函数执行环境的num，数组有10个函数对象，每个对象的执行环境下的number都不一样
             };
        }(i);
     }
     return result;
}
var l = outer();
foreach(var n : l)
{
	n();
}
```

2020-12-2 更新
-

1.增加logging模块

1.1 支持 logging.StreamHandler logging.FileHandler logging.handlers.RotatingFileHandler logging.handlers.TimedRotatingFileHandler

2.修复已知BUG

*** 基础版
```
#import "logging"

logging.create({level:logging.DEBUG,
                format:'[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s',
                filename:'log/myapp.log',
                filemode:'ab+'});
var logger = logging.getlogger('');
var streamhandler = new logging::StreamHandler();
streamhandler.setlevel(logging.DEBUG);
streamhandler.setformatter('[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s');
streamhandler.setcolor([1,2,3,4,5]);

logger.addhandler(streamhandler);



logger.debug("debug");
logger.info("info");
logger.warning("warning");
logger.error("error");
logger.critical("critical");
```

*** 旗舰版(配置版)
```
#import "logging"

var simple_format = '[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s';
var standard_format = '[%(asctime)s][task_id:%(name)s][%(filename)s:%(lineno)d][%(levelname)s][%(message)s]';

var LOGGING_CONFIG = {
    version: 1,  // 版本号
    disable_existing_loggers: false,  //　固定写法
    formatters: {
        standard: {
            format: standard_format
        },
        simple: {
            format: simple_format
        },
    },
    filters: {},
    handlers: {
        //打印到终端的日志
        sh: {
            level: 'DEBUG',
            class: 'logging.StreamHandler',  // 打印到屏幕
            color: [5,4,3,2,1] , // DEBUG,INFO,WARNING,ERROR,CRITICAL 颜色
            formatter: 'simple'
        },
        fh: {
            level: 'DEBUG',
            class: 'logging.handlers.TimedRotatingFileHandler',  // 打印到屏幕 
            formatter: 'standard',
            filename: "log/log.txt",
            mode: "ab+",
            backupCount:5,   //保留5个
            when:"S",		//秒
            interval:5     //5秒
        },
    },
    loggers: {        
        root: { //root 为默认
            handlers: ['sh','fh'],  //这里把上面定义的两个handler都加上，即log数据既写入文件又打印到屏幕
            level: 'DEBUG',
            propagate: true,  // 向上（更高level的logger）传递
        },
    },
}

logging.create_with_config(LOGGING_CONFIG);

while(true){
	logging.debug("debug");
	logging.info("info");
	logging.warning("warning");
	logging.error("error");
	logging.critical("critical");
	sleep(2000);
}

```

2020-12-1 更新
-

1.修复更新的BUG.

2.增加console基础库

3.增加读取传入参数

4.增加文本地址读取顺序.


2020-11-30 更新
-

1.增加foreach xrange.

2.优化cpps速度


```
foreach(var i : xrange(0,999))
{
	println(i);
}

foreach(var i : nrange(0,999,3)) //每次递增3
{
	println(i);
}

```


2020-11-29 更新
-

1.花了几天的时间优化cpps. 提升运行速度几十倍. 不过优化的还是不够. 大概速度和python 差不多. 目标是lua的速度.以前瞧不起lua,真正运行起来才知道lua的速度不是盖的.没有使用JIT就可以达到这样的速度.优化是真的NB.但是这次更新估计会有很多BUG.特此记录一下.

2020-11-24 更新
-

1.增加hashlib库 md5 sha1 sha224 sha256 sha384 sha512

2.http模块增加urlencode,urldecode

3.增加字符串encoding gbk<->utf8

```
system("chcp 65001"); //use utf8 string

#import "hashlib"


var md5 = new hashlib::md5();
md5.update("123456");
println(md5.hexdigest()); //e10adc3949ba59abbe56e057f20f883e


var sha1 = new hashlib::sha1();
sha1.update("123456");
println(sha1.hexdigest()); //7c4a8d09ca3762af61e59520943dc26494f8941b


var sha224 = new hashlib::sha224();
sha224.update("123456");
println(sha224.hexdigest());//f8cdb04495ded47615258f9dc6a3f4707fd2405434fefc3cbf4ef4e6


var sha256 = new hashlib::sha256();
sha256.update("123456");
println(sha256.hexdigest()); //8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92


var sha384 = new hashlib::sha384();
sha384.update("123456");
println(sha384.hexdigest());//0a989ebc4a77b56a6e2bb7b19d995d185ce44090c13e2984b7ecc6d446d4b61ea9991b76a4c2f04b1b4d244841449454


var sha512 = new hashlib::sha512();
sha512.update("123456");
println(sha512.hexdigest());//ba3253876aed6bc22d4a6ff53d8406c6ad864195ed144ab5c87621b6c233b548baeae6956df346ec8c17f5ea10f35ee3cbc514797ed7ddd3145464e2a0bab413
```

```
//windows default GBK 
//system("chcp 65001"); //use utf8 string

#import "encoding"


var s = "我是中文";
println(s); //鎴戞槸涓枃

var utf8 = encoding.encode(s,encoding.UTF8);
println(utf8); //鎴戞槸涓枃
var gbk = encoding.encode(s,encoding.GBK);
println(gbk);//我是中文
utf8 = encoding.encode(gbk,encoding.UTF8);
println(utf8);//鎴戞槸涓枃
s = encoding.encode(utf8,encoding.GBK);
println(s);//我是中文

```

2020-11-23 更新
-

1.增加base64模块

2.增加mysql模块

3.优化cpps

4.修复字符串format 调用函数找不到变量的BUG.

```
//base64例子
system("chcp 65001"); //use utf8 string

#import "base64"


var s = "abc";
println("source:{s}");
var encode_s = base64.encode(s);
println("encode_s:{encode_s}");
var deocde_s = base64.decode(encode_s);
println("deocde_s:{deocde_s}");

```

```
//mysql例子
#import "mysql"


var option = new MysqlOption()
{
	db = "dbname",
	host = "127.0.0.1",
	user = "root",
	passwd = "*******",
	prot = 3306,
	isreconnect = true
}
var conn = mysql.connect(option);
if(conn == null)
{
	println("connect error");
	exit(0);
}

var results = conn.call("selectuser",["john"])
println(conn.error());
foreach(var result : results)
{
	foreach(var record : result.records())
	{
		println("----------------------------------------");
		foreach(var field: record.fields())
		{
			println("{field.first()}:{field.second()}");
		}
	}
}


var results = conn.execute("select * from users where username = 'john';")
println(conn.error());
foreach(var result : results)
{
	foreach(var record : result.records())
	{
		println("id:"..record.get("id"));
		println("username:"..record.get("username"));
		println("userpwd:"..record.get("userpwd"));
	}
}


```

2020-11-22 更新2
-

1.增加http模块 httprequest类

```
system("chcp 65001"); //use utf8 string

#import "http"

var request = new http::httprequest();
request.setcookiesfile("cookies.txt");
//request.setproxy(httpproxy.SOCK5,"192.168.1.166:25455");
//request.setproxyaccount("johnsonyl","mima");
request.addheaders({User-Agent:"cppsrequest/1.1.0"});
var ret = request.get("http://192.168.1.166:8080/Home/test?kkk=100");


println("-------------------GET-------------------------");
println("get:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");


println("-------------------POST-------------------------");
ret = request.post("http://192.168.1.166:8080/Home/test","kkk=100");
println("post:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");
println("-------------------END-------------------------");

```

2020-11-22 更新
-

1.增加httpserver

2.增加cpps 一些对外函数

3.修复一些已知BUG

```
system("chcp 65001"); //use utf8 string

#import "socket"

class Home
{
	var name;
	Home()
	{
		name = "I'm John ,32 years old."
	}
	//http://127.0.0.1:8080 or http://127.0.0.1:8080/Home or http://127.0.0.1:8080/Home/Index
	var Index(var request)
	{
		println("Home/Index..");
		println(name);
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(name.."\n");

		request.send(200,"OK");
	}
}

class Student
{
	var name;
	var age;
	Student()
	{
		name = "xiaoming";
		age = 18;
	}
	//http://127.0.0.1:8080/Student/ or http://127.0.0.1:8080/Student/Index
	var Index(var request)
	{
		
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append("i'm {name}, {age} years old.");

		request.send(200,"OK");
	}
	//http://127.0.0.1:8080/Student/Name
	var Name(var request)
	{
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(name);

		request.send(200,"OK");
	}
	//http://127.0.0.1:8080/Student/Age
	var Age(var request)
	{
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(age);

		request.send(200,"OK");
	}
}
//http://127.0.0.1:8080/Home/test?kkk=100
var test(var request)
{
	request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

	request.append("test page.\n");
	request.append("path:"..request.getpath().."\n");
	request.append("uri:"..request.geturi().."\n");
	request.append("kkk:"..request.getparam("kkk").."\n");
	request.append("User-Agent header:"..request.getheader("User-Agent").."\n");
	request.append("getbuffer():"..request.getbuffer().."\n");

	request.send(200,"OK");
}

var option = new HttpServerOption()
{
	exceptionfunc = [](var request,var errmsg)
	{
		println(errmsg);

		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(errmsg);

		request.send(500,"SERVER ERROR");
	},
	notfoundfunc = [](var request)
	{

		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append("there's nothing here.");

		request.send(404,"Not Found");
	}
}

var http = new socket::httpserver().setoption(option).listen(8080);


http.register_controller_default(Home);
http.register_controller(Student);
http.register_handlefunc("/Home/test",test);

while (true){
	http.run();
}

```

2020-11-20 更新
-

1.修改项目结构

2.修改编译为cmake方式

2020-11-19 更新
-

1.增加第一版socket库基于libevent.
2.修复一些BUG

```
system("chcp 65001"); //use utf8 string

#import "socket"

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
```

2020-11-16 更新
-

添加curl支持库

```
#import ["json","demo","curl"]
//or
//#import "json"
//#import "demo"


var url = curl.curl_easy_init();

curl.curl_easy_setopt(url, curl.CURLOPT_URL, "http://blog.chinaunix.net");
curl.curl_easy_setopt(url, curl.CURLOPT_VERBOSE, 0);
var res = curl.curl_easy_perform(url);
println(res);
println(url.getdata());


curl.curl_easy_cleanup(url);
```

2020-11-15 更新
-

添加json支持库

简化json.encode默认支持为utf8格式

增加 import 导入支持库功能

```
#import ["json", "demo"]
//or
//#import "json"
//#import "demo"

var arr = [1, 2, 3, 4, 5, 6, 7];
var obj = { a : "我是开发者", b : "hehe" };


println(arr);
println(obj);


var arr2 = json.encode(arr);
var obj2 = json.encode(obj);



println(arr2);
println(obj2);


println(json.decode(arr2));
println(json.decode(obj2));


```

2020-11-14 更新
-

1.增加linux 动态库功能

详情见demo/demo目录

脚本中使用方法

```
loadlibrary("demo");
demotest();
freelibrary("demo");
```

2.增加demo/hello world 对linux的编译支持

3.修改cpps make时编译文件目录到bin文件夹



2020-11-12 更新
-

1.支持类似py,php的字符串format处理功能

话不多说，上代码

```

class A
{
	var name = "john";
	var age = 32;
}
var a = new A();
var s = "my name is {a.name} , i'm {a.age} years old.";
println(s);

输出：
[Cpps 1.2.0 Copyright (C) 2015-2020 By Johnson, Pub-test]
my name is john , i'm 32 years old.
>

值得注意： { 与 } 需要使用 \{ 与 \} 来特殊处理了。
```

2020-11-10 更新2
-

1.修复父类构造函数执行顺序

2.增加调用父类虚函数的功能

3.增加父类对名空间的支持

4.foreach 还是修改成C++ 方式。 并且 : 后为表达式

```

foreach(var item : [1,2])
{
	println(item);
	foreach(var item2 : [3,4])
	{
		println(item2);
	}
}


foreach(var item : {a:1,b:"2"})
{
	println(item.first());
	println(item.second());
}

var b = {a:1,b:"2"};

foreach(var item : b)
{
	println(item.first());
	println(item.second());
}

```

```
namespace cpps
{
	class C
	{
		C()
		{
			println("C class");
		}
		var test()
		{
			println("C->test()");
			
		}
	}
}
class A : cpps::C
{
	var name;
	A(int a){
		println("A class:"..name);
	}
	
	var test()
	{
		println("A->test()");
				
		println(name);
	}
}

class B:A
{
	var age;
	B(){
		A::constructor(10); //如果构造函数非0参数时，需要手动调用，否则不会调用
		println("B class");
	}
	var test()
	{
		println("B->test()");
		println(age);
		cpps::C::test();
	}
}


var b = new B(){
	name = "john",
	age = 32
};

b.test();

输出：
[Cpps 1.2.0 Copyright (C) 2015-2020 By Johnson, Pub-test]
C class
A class:john
B class
B->test()
32
C->test()
>
```

2020-11-10 更新
-

重写foreach 

老版本使用lambda 方式，很不好。

修改成 

见上方新方法

增加 new 时设置变量功能类似c#

```
class A
{
    var name;
    var age;
}
var a = new A(){
    name = "john",
    age = 32
}
```

2020-1-28 更新
-

新年快乐~~ happy new year!

1.今天恢复了对linux gcc++ 的编译支持。

对linux实在不太熟悉。一堆警告。很糟心。

2.增加make lib 与 make install指令

3.cpps 默认工作目录为指定文件目录。

2020-1-21 更新
-

解决了&&与|| 未按照顺序执行导致逻辑错误的BUG。

2020-1-20 更新
-

增加const的支持。

const var abc = "10"; //不可修改。

2020-1-19 更新
-

1.增加调用函数参数支持到11个

2.增加支持构造函数。 c++ 部分需要注册一个名为constructor的函数当构造函数

3.并且修复多层名空间有错误的BUG。

4.增加类似js 当创建出匿名函数后并且调用的方法 例子：
```
[](var a){

	println(a);

}(123);
```


2019-12-7 更新
-

之前老版本是在函数结束后才调用GC，那么如果想在主函数写一个死循环执行逻辑就会造成内存永不会被GC释放，现在会在符合内存大小等限制情况下调用GC释放内存了。具体大小见CPPS_GEN0_CHECKSIZE ，等宏定义。

2019-11-01
-

修复lambda函数在传递时当前与偶发崩溃的BUG。

增加foreach 函数支持break操作

前面更新就不介绍了。乱七八糟的。

2017-8-31
-
1.新增CPPS内部函数resume() 在内部抛出thorw异常的时候使用resume可解决GC异常问题

2.新增assert函数

3.新增部分数学库

4.新增CPPS定义库变量 类似math.pi 

5.修复vector变量[] 括号内不能使用公式的BUG

6.新增string.format字符串格式化功能

7.修复三元表达式与namespace在某些情况下冲突的问题

8.增加16进制变量值

```
var i = toint(io.getc());

if(i == 10)
{
	println("i 等于 10");
}
else if(i > 10)
{
	println("i 大于 10");
}
else
{
	println("i 小于 10");
}

for(var i = 0;i < 10;i++)
{
	println(i);
}

class abc
{
	var a;
    var b = 10; //可以定义默认值
    var testfunc(var c)
    {
    	println(c);
        println(a);
        println(this.b);
    }
}
var obj = new abc;
obj.testfunc(10);

//继承关系
class abcd : abc
{
	var d = 10;
}

var obj2 = new abcd;
obj2.testfunc(20);
println(obj2.d);

var str = "hello world"; //字符串类型
var i = 10; //整数类型
var n = 3.1415926; //浮点类型
var b = true; //布尔类型
var g = new map; //对象类型
class abc
{
	var a = 10;
}
var a = new abc; //类对象类型
// 2-9日更新 可定义多个变量
var a1 = 0, a2 = 10,a3 = 20;
//8月31日更新
var b = 0xffff;

var str = "hello";
println(str.."world");

var o = str.."haha".."ooo"..str;
println(o);

var testfunc(var i)
{
	println(i);
}

testfunc(10);

更多例子，请查看脚本文档。。。


```