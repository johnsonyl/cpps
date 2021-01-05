# CPPS 文档 v1.0.0 [![star](https://gitee.com/cppscript/cpps/badge/star.svg?theme=dark)](https://gitee.com/cppscript/cpps/stargazers)[![Github stars](https://img.shields.io/github/stars/johnsonyl/cpps.svg?style=plastic&logo=appveyor)](https://github.com/johnsonyl/cpps)

CPPS是一种语法类似C++的轻量级嵌入式脚本语言,它拥有目前主流语言所拥有的很多特性包括协程,面向对象,Lambda,闭包,泛型变量,自定义模块支持,GC垃圾回收以及跨平台.CPPS通过内置语法解析将程序解释成字节码在虚拟机中运行.
CPPS只需要几行代码就可以很容易的嵌入到C++代码中,并且用户可以自己利用c++的类与函数扩展给CPPS以便CPPS处理更广泛的领域,例如人工智能,GUI,网站以及游戏开发.
CPPS遵循MIT许可证协议,免费使用,但CPPS并不能保证程序稳定没有任何BUG.希望您再使用过程中发现任何问题可以提交给作者.如果您有解决方案可以帮助CPPS更茁壮的成长.
  
作者QQ: 88481106
EMAIL: 88481106@qq.com
CPPS-QQ交流群: 282081601

[TOC]

# 1.如何编译CPPS.
在本文中我们会详细的介绍如何在三大主流平台编译运行CPPS.
如果在过程中有任何问题,都可以联系作者EMAIL:88481106@qq.com 或者添加QQ群:282081601
## **1.1 在windows上编译**
绝大多数开发者应该都是在windows上开发.然后在其他平台编译运行,因为在windows中拥有最强的VS编辑器.
但是同时windows开发的缺点就是很多三方库的编译最让人苦恼.
CPPS自身并不需要任何三方库所以编译CPPS几乎并不费力气,作者使用的是Visual Studio 2019版本
如果你也是VS2019那么编译会变得非常简单.
\
我们第一步只需要打开VS2019 Native tools ,本文中为x64模式编译.我们非常推荐使用x64模式.
![](images/screenshot_1609029849590.png)
找不到的小伙伴可以自行百度如何打开.
进入到工作目录输入
```
cmake .
```
就可以生成VS所需要的.sln项目工程. 只要打开 libcpps.sln 就可以编译cpps了.
但麻烦事随之而来,我们需要编译内置扩展库.**它在libs文件夹.**,这非常重要.
我们紧接着在VS2019 Native tools输入
```
cd libs
cmake .
```
就可以生成cppslibs.sln
它使用了很多著名的三方库 zlib,libcurl,openssl,libevent,hiredis,mysqlclient.所以在编译扩展模块之前.我们需要编译这些三方库.
这里教你如何编译 zlib openssl libcurl [跳转连接](编译-windows-x64-openssl-zlib-libcurl.md)
hiredis 我在目录中已经放置好了.在 **CPPS项目目录/deps/** 中 ,只需要编译一下即可
mysqlclient 我也放置在里面了. 在 **CPPS项目目录/deps/deps/include/mysql** 以及  **CPPS项目目录/deps/deps/lib** 是官方已经编译好的lib 与 dll.
只需要把dll拷贝到bin目录就可以使用了!
libevent 在编译完zlib与openssl后 它会非常简单.小伙伴们百度一下就可以找到很多文章.\
\
在windows上放置目录我推荐分别放到**CPPS项目目录/deps/deps/include** 与**CPPS项目目录/deps/deps/lib**目录,这样非常方便统一管理以及在多平台编译. 如果大家都遵循这样的方式,那么就很容易把自己的模块分享给其他人.
至此相信小伙伴们已经编译好了上述所需要的三方库.同时也正确的放到了相应的目录中
我们只要进入libs/cppslibs.sln然后很容易的就可以编译过了.
\
恭喜你,你已经成功的编译好cpps了.

> **注:但实际上我会把我编译好的头文件,lib与dll分别放到deps里.不过它是vs2019 x64编译的,如果你和我的环境一样,你会省去很多时间.如果你和我不一样的环境,请先删除它们.然后按照上续步骤重新编译安装三方库**



## **1.2 在linux上编译**
linux 编译起来就不像windows那么麻烦,因为很多系统都提供了devel开发版本.不过最头疼的就是linux有上百个版本的操作系统.大家安装软件的方式各不相同.作者目前就仅仅在centos ubuntu 编译了,并且写了2个快捷编译的批处理为**build-centos.sh**与**build-ubuntu.sh** 
分别还有 build-debian.sh 与 build-fedora.sh 作者并没有深入尝试,希望有此系统的小伙伴可以尝试使用它编译并且把最终正确的方法提供给作者以更新让更多小伙伴可以顺利简化编译.
\
希望小伙伴们可以为作者提供更多版本操作系统的快捷编译批处理.这样其他小伙伴在使用编译时就会简单多了.
\
如果并非上面所说的操作系统.那么您可以按照下面步骤去编译它. 

我们需要进入到项目目录cmake它,生成makefile文件.
```
cmake .
make
make install 
```
我们就可以顺利的编译完成cpps,因为cpps并不需要任何第三方库.
下面我们需要编译内置扩展库模块.
它使用了很多著名的三方库 zlib,libcurl,openssl,libevent,hiredis,mysqlclient.所以在编译扩展模块之前.我们需要编译这些三方库.
hiredis我放到了deps目录下.除此之外小伙伴们需要到各自的官网下载,并且按照它们提供的方法编译安装它们.
至此编译安装好第三方库后就可以进入libs目录编译内置扩展模块了.
```
cd libs
cmake .
make
```
\
恭喜你,你已经成功的编译好cpps了!

## **1.3 在macos上编译**
在苹果系统上编译比较简单,只要保证xcode是最新的.然后使用终端进入cpps目录
```
cmake .
make
make install
```
下面我们需要编译地址扩展模块.
下面我们需要编译内置扩展库模块.
它使用了很多著名的三方库 zlib,libcurl,openssl,libevent,hiredis,mysqlclient.所以在编译扩展模块之前.我们需要编译这些三方库.
```
brew install zlib
brew install openssl
brew install curl
brew install libevent
brew install mysql-connector-c
brew install hiredis
```
我们推荐小伙伴们安装brew, 百度有如何安装homebrew的方法.建议使用中科大或者阿里云的源.
brew 可以帮你更快捷的安装cpps内置扩展模块所需的三方库.
至此编译安装好第三方库后就可以进入libs目录编译内置扩展模块了.
```
cd libs
cmake .
make
```
\
恭喜你,你已经成功的编译好cpps了!

# 2.数据类型介绍
CPPS支持8大数据类型,下面分别向您介绍他们的作用.
## 2.1 整数
整数的类型是cpps_integer(long long),它比如(-20,0,5,29,100)无小数部分.
```
var i = 100;
println(i);

结果:
100
```
你也可以通过 +,-,*,/ 来操作它, 也可以使用()用来分组.
```
var i = 100 * 5 - (20 * 3) * (100-98);
println(i);
结果:
380
```
同样我们还支持16进制的整数定义.它会在运行时变成10进制的数.
```
var i = 0x10;
println(i);
结果
16
```
## 2.2 字符串
CPPS 提供了std::string 的扩展,你可以跟string一样的方式来使用字符串.并且在它之上增加了很多内置函数更易操作.
在定义字符串的时候我们可以使用双引号("....") 或者单引号('....') 来定义字符串.反斜杠提供转译操作方法与c++是一致的.
```
var s = "John:\"hello world\"";
println(s);

结果
John:"hello world"
```
类似std::string内置函数.
```
var s = "abcdefg";
var pos = s.find("d");
if(pos != string.npos)
     println(s.substr(pos));

结果
defg
```
字符串拼接 我们使用了类似lua的方式,使用了..来拼接字符串.并没有使用 + 
```
var s1 = "hello ";
var s2 = "world";
println(s1..s2);
结果
hello world

s1 ..= "world";
println(s1);
结果
hello world

```
利用fotmat方式拼接字符串,这是非常好用也是最常用的一个字符串操作功能,它非常高效.远远高于..的拼接.非常推荐使用这种方式.
```
var age = 32;
var name = "johnson"
var about = "My Name is {name}, I'm {age} years old.";
println(about);

结果
My Name is johnson, I'm 32 years old.

//甚至在{}中还可以使用各种表达式,例如调用函数
var test()
{
     return "Working";
}
about = "My Name is {name}, I'm {age} years old. I'm {test()} Now";
println(about);

结果:
My Name is johnson, I'm 32 years old. I'm Working Now
```
### ***class String***
在CPPS中封装了很多std::string内部函数,并且在此基础上增加了一些实用的函数.但是还有一些函数作者并没全部封装,如果您有遇到没有封装的可以与作者取得联系.
#### ***String::size()***
返回String中的长度
*****
#### ***String::find(var text,var pos = 0)***
在字符串中查找内容。在字符串中搜索由其参数指定的序列的第一次出现。当指定pos时，搜索仅包括位置pos处或之后的字符，忽略包括pos之前的字符在内的任何可能的事件。
*****
#### ***String::rfind(var text,var pos = string.end)***
倒序查找内容,在字符串中搜索由其参数指定的序列的第一次出现。当指定pos时，搜索仅包括位置pos处或之前的字符，忽略包括pos之后的字符在内的任何可能的事件。
*****
#### ***String::replace(var src,var tar)***
本函数与stl中的replace有所不同,cpps中替换为全文替换.
*****
#### ***String::clear()***
清空String的所有内容.
*****
#### ***String::copyto(String tarstring)***
目标必须是一个字符串变量,将自己的内容拷贝进去.这样可以保证不会在new出一个新的String.
*****
#### ***String::split(var sep,var count = -1)***
根据sep拆分成数组,当指定了count时只拆分count个数的数组. 当count为-1时则全文拆分.
*****
#### ***String::cut(var len)***
拆分成多个len长度的字符串数组.
*****
#### ***String::strcut(var left,var right)***
根据left与right来截取字符串.
```
var s = "您当前的验证码为:[102345],感谢您使用XXX.";
var code = s.strcut("[","]");
println(code);
结果:
102345
```
*****
#### ***String::strcuts(var left,var right)***
根据left与right截取相匹配的多个字符串.
```
var a = "<a src='http://www.baidu.com/'>下载一</a><a src='http://www.google.com/'>下载二</a>";
var srcs = a.strcuts("src='","'");
println(srcs);
结果
[http://www.baidu.com/,http://www.google.com/,]
```
*****
#### ***String::empty()***
判断一个String是否为空.如果为空则返回true.
*****
#### ***String::substr(var pos,var len = string.npos)***
截取一段字符串.当len存在则根据pos位置截取len长度的字符串.
*****
#### ***String::at(var idx)***
返回String中idx的字节(整数)
*****
#### ***String::tolower()***
本函数并不修改自身,而是返回一个转为小写的新字符串.如果想修改自身可以使用string.lower(str);
*****
#### ***String::toupper()***
本函数并不修改自身,而是返回一个转为大写的新字符串.如果想修改自身可以使用string.upper(str);
*****
#### ***String::trim()***
将左右两侧的空格剔除.
*****
#### ***String::ltrim()***
将左侧的空格剔除.
*****
#### ***String::rtrim()***
将右侧的空格剔除.
*****
#### ***String::join(vector vct)***
将vct列表中每一个对象转换成字符串并追加一个String自身.
```
var s = ",";
var vct = [1,2,3,4,5,6];
println(s.join(vct));
结果:
1,2,3,4,5,6,
```
*****
#### ***String::startswith(var str)***
如果String中开头为str则返回为true.否则false
*****
#### ***String::endswith(var str)***
如果String中结尾为str则返回为true.否则false
*****
#### ***String::pop_back(var count = -1)***
当count 被指定数量时则移除尾部count数量的字节.
*****
#### ***String::push_back(var charcode)***
向String末尾插入一个charcode.
*****
#### ***String::append(var str)***
向String末尾追加一个str字符串.
*****

## 2.3 布尔
布尔类型与c++的用法一致.
```
var b = true;
println(b)
b = false;
println(b)
结果
true
false
```
## 2.4 浮点数
在cpps里我们使用的是double类型,并非float. 它与整数一样同样支持 +,-,*,/ 的方式来计算.与浮点数计算后最终结果都会转成浮点数.
```
var i= 5; //整数
var f = i + 1.22; //转换成浮点
println(f);
结果
6.22
```
## 2.5 函数
函数的定义也与c++相类似.我们的类型只有泛型所以返回类型只有var一种.
在定义参数时,我们可以给出默认参数,当调用是不传参默认为设置的值. 如果没有设置默认值.那则为nil (null).
```
var funcname(var p1,var p2 = 100)
{
    println(p1);
    println(p2);
}
funcname("text");
funcname(1.5,"text");

结果:
text
100
1.5
text
```

## 2.6 class 类
CPPS的类为标准面向对象,它可以是继承多个父类,这个操作与c++一致,只有一点需要注意我们的函数不支持多态,因为所有变量均为泛型.多态并没办法支持.不过用户可以根据传进来的类型实现类似多态.
下面演示一些基础的类操作
```
class A
{
    //同名的为构造函数
    A()
    {
        println("A:A();");
    }
    var test()
    {
        println("A::test();");
    }
}

  

class B : A
{
    var name = "johnson"; //可以在这里直接设置值
    var age;
    var state;
    B()
    {
        age = 32; //也可以在构造函数里设置值
        println("B:B()");
    }
    var test()
    {
        println("B::test()");
        A::test();
    }
}
var b = newB(){
    state = "Working" //也可以在这里设置值
};
b.test();
println("My Name is {b.name}, I'm {b.age} years old. I'm {b.state} Now");
结果:
A:A();
B:B()
B::test()
A::test();
My Name is johnson, I'm 32 years old. I'm Working Now
```
## 2.7 namespace 名空间
CPPS里面保留了名空间的关键字与用法. 用法接近于c++,但是并没有c++那么健壮.暂时未实现using namespace功能.
```
namespace cpps
{
    class A
    {
        A()
        {
            println("cpps::A::A()");
        }
    }
    var func()
    {
        println("cpps::func()");
    }
    var abc = 100;
}
var a = new cpps::A();
cpps::func();
println(cpps::abc);

结果
cpps::A::A()
cpps::func()
100
```
## 2.8 module 模块
CPPS允许在脚本中定义模块标签,或者扩展C++提供的模块.它看上去和名空间很像,但是又略有不同.
```
//并非重写sys,而是在系统的sys基础上扩展新变量,函数或者是类.
module sys
{
    var test()
    {
        println("sys.test()");
    }
    class A
    {
        var name = "johnson";
    }
}
sys.test();
var a = new sys::A();
println(a.name);

结果
sys.test()
johnson
```
## 2.9 Lambda函数
CPPS中Lambda函数支持类似JS的闭包 并且定义lambda函数与C++ 几乎一致. 
下面来演示一种闭包的操作.
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
结果:
0
1
2
3
4
5
6
7
8
9
```
## 2.10 vector
CPPS中vector就是对std::vector的一层封装.几乎用法也与vector一致.但是可能有一些函数并没全部实现,如果有需要作者添加的可以联系作者添加.或者在github上 pullrequest. 我会非常感谢.
下面看一下使用的例子.
```
//方法1.
var a = new vector();
a.push_back(100);
println(a);

//方法2
var b = new vector[10];// 创建一个10长度的数组
b[0] = 100;
b[9] = 200;
println(b);

//方法3 (推荐)
var c = [1,2,3,4,5,6,7];
println(c);

结果:
[100,]
[100,nil,nil,nil,nil,nil,nil,nil,nil,200,]
[1,2,3,4,5,6,7,]
```

您也可以查看更多vector内置函数.
## 2.11 map
CPPS中map使用了[https://github.com/greg7mdp/parallel-hashmap](https://github.com/greg7mdp/parallel-hashmap)来替代unordered_map与map,这个hashmap的速度要比stl的快10-60倍.非常强烈推荐.里面的函数也与stl的map 几乎一致.
下面看一下使用例子.
```
//方法1.
var a = new map();
a.insert("key","value");
a["key2"] = "value2";
println(a["key"]);
println(a);

//方法2
var b = { 
    key : "value",
    key2 : "value2"
};
println(b);

结果:
value
{key:value,key2:value2,}
{key:value,key2:value2,}
```
您也可以查看更多map内置函数.
# 3.关键字语句
## 3.1 if
CPPS中if 的使用方法与C++一致.
```
print("Please enter an integer:");
var x = int(io.getc());
if(x < 0)
{
    x = 0;
    println("Negative changed to zero");
}
else if(x == 0)
    println("Zero");
else if(x == 1)
    println("Single");
else
    println("More");

结果
Please enter an integer:42
More
```
## 3.2 for
CPPS中的for 与c++操作基本一致.C++11之后的for功能使用了foreach来操作.
下面看一下例子:
```
for(var i = 0; i < 10; i++ )
{
    println(i);
}

结果:
0
1
2
3
4
5
6
7
8
9
```
## 3.3 foreach
在C++11中 for可以遍历有迭代器的变量了.那么现在cpps也有类似的功能了 只是叫foreach.
下面来看一下如何使用的.
```
//range
println("range")
foreach(var i : range(10))
{
    println(i);
}
//vector
println("vector");
var a = [1,2,3,4,5,6,7];
foreach(var i : a)
{
    println(i);
}

//map
println("map");
var b = {
    a:1,
    b:3,
    c:5
};
foreach(var it : b)
{
    println("key={it.first()},value={it.second()}");
}
结果
range
0
1
2
3
4
5
6
7
8
9
10
vector
1
2
3
4
5
6
7
map
key=a,value=1
key=b,value=3
key=c,value=5
```
## 3.4 while
while与C++操作一致.
下面看下用法示例
```
var a = 0;
while(a < 10)
{
    println(a++);
}

结果:
0
1
2
3
4
5
6
7
8
9
```
## 3.5 break
break 与 C++操作一致
```
var i = 0;
while(true)
{
    if(i > 10) break;
    println(i);
    i++;
}
结果
0
1
2
3
4
5
6
7
8
9
10
```
## 3.6 continue
continue 与C++操作一致
```
foreach(var i : range(10))
{
    if(i % 2 == 0) continue;
    println(i);
}
结果
1
3
5
7
9
```
## 3.7 try catch thorw
捕获异常机制与c++部分一致.暂时还不是很强壮.例如final 还未实现下个版本在实现吧.
下面看下例子
```
try
{
    var a = null;
    a.asd();
}
catch(var e)
{
    println("{e.geterrstr()} line:{e.getline()} file:{e.getfile()}");
    println(e.callstack());
}

结果:
[a] must be a class object or a domain before the '.' line:4 file:main.cpp
Error stack information:
```
当然也可以自定义异常.
```
class diyerror
{
    diyerror(var id,var msg)
    {
        errorid = id;
        err = msg;
    }
    var errorid ;
    var err;
}
try
{
    throw new diyerror(10,"test msg");
}
catch(var e)
{
    var err = e.getvalue();
    println(  type(err) ); //可以通过type来判断是什么错误.
    println(err.errorid);
    println(err.err);
}
结果
diyerror
10
test msg
```

### ***class cpps_trycatch_error***
#### ***cpps_trycatch_error::geterrno()***
*****
#### ***cpps_trycatch_error::geterrstr()***
*****
#### ***cpps_trycatch_error::getfile()***
*****
#### ***cpps_trycatch_error::getline()***
*****
#### ***cpps_trycatch_error::getvalue()***
*****
#### ***cpps_trycatch_error::callstack()***
*****

## 3.8 return
return 增加了类似c++17的多返回值,但是使用更加便捷简单.
下面看下示例.
```
//单返回值
var func2()
{
    return "sadf";
}

//多返回值
var func1()
{
    return 1,2,func2();
} 

//接收多返回值
var [a,b,e] = func1();

println(a);
println(b);
println(e);
结果:
1
2
sadf
```
## 3.9 const
在CPPS里面不像C++一样有编译错误.所以你要强制修改一个const的变量,并没有过多的错误提示.但是它们并不能真正的给const变量赋值.这里需要开发者多多注意. 切记不要尝试给const变量修改值,导致修改不成功影响程序运行.
下面看一下代码示例:
```
const var idx = 100;
println(idx);
idx = 1000;  // here is not work..
println(idx);

结果
100
100
```
## 3.10 async await yield
CPPS中支持了协程,但是值得一提的是CPPS中asio相关的扩展库还不多.不过作者也会尽快开发相关异步模块提供给协程使用.

async 来定义一个函数是否为协程函数.当使用了async时它就不能被直接执行了,而是需要创建协程任务在协程中调用. 例如await 或者 create_task.

await 用来等待一个协程任务执行结束并获取返回值.

yield 用来通知协程可切换其他协程运行. 当前协程无任务可做了.但是并不退出.

下面看一下基础的asyncio如何使用吧.
```

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
	var a ;
	a.asd();
	return "test1";
}
var test_call_back(var task,var context)
{
	println(task.get_name());
	println(context);
}

async var main(){

	var task1 = asyncio.create_task(test());
	task1.set_name("Custom names are used to distinguish");
	task1.add_done_callback(test_call_back,"Custom context content");
	var task2 = asyncio.create_task(test1());

    yield; //暂时让其他协程先运行.

	var ret = await asyncio.wait(task1);
	if(ret.timeout())
	{
		println("task1 was timeouted.");
	}
	println("state:{ret.state()}");
	println(ret.result());
	try{
		println(await task2);	
	}
	catch(var e)
	{
		println("oh,we catch some error");
		println("{e.geterrstr()} line:{e.getline()} file:{e.getfile()}");
		println(e.callstack());
	}
	
	
	println("finish");
}

asyncio.run(main());



结果:
do test function
do test1 function
do test1 function done
do test function done
Custom names are used to distinguish
Custom context content
state:2
test
oh,we catch some error
[a] must be a class object or a domain before the '.' line:16 file:main.cpp
Error stack information:
main.cpp [30] test1
Error stack information:
main.cpp [54] main

finish
```
> 下面还有更详细的asyncio介绍.请查看asyncio模块.

## 3.11 new
new 操作与C++一致. 唯独的区别就是不需要delete, cpps自身有GC (垃圾回收) 可以根据使用状态释放应该释放的对象.
```
class A{
    var a = 100;
}
var a = new A();
println(a.a);
结果
100
```
## 3.12 #include
方法与C++一致.在解释时把文件载入到缓冲区一并解释成字节码.这里需要注意的是,它并不是在运行时才载入.而是在loadfile时一并按顺序载入.
下面看一下代码示例
```
a.cpp
var abc = 100;


b.cpp
#include <a.cpp>
println(abc);

结果
100
```
## 3.13 #import
这里是cpps 模块导入方法.解释格式为 #import 字符串或者数组. 数组可以让用户批量载入很多模块.
下面看下代码示例
```
#import "uuid"
#import "base64"
或者
#import ["uuid","base64"]

var id = uuid.uuid4(); 
println(id);
println(base64.encode(id));

结果
51319FBD-AA3E-4298-87F6-8DE10F3A4891
NTEzMTlGQkQtQUEzRS00Mjk4LTg3RjYtOERFMTBGM0E0ODkx
```
## 3.14 dofile
这里dofile与#include 是有区别的.它是在运行时才会载入进来.并且无论你在任何地方调用,它的作用域都在全局,而不在当前作用域.
下面看下代码示例
```
a.cpp
var abc = 100;

b.cpp

var test()
{
    dofile("a.cpp");
}
test();
println(abc);
结果
100
```
## 3.15 dostring
dostring 也是在运行时才会载入,它的作用域会在当前运行的作用域中,如果需要把一整个文件载入进来请不要使用dofile 而是载入文件到一个字符串,然后dostring.
下面看一下代码示例
```
dostring("println('hello world');");
结果
hello world
```
## 3.16 注释
注释与C++一致,支持// 与 /**/
```
//这里是一行注释
/*
这里是多行注释
可以随意定义注释内容与行数.
*/
```
# 4.内置模块
## 4.1 基础API(base)
### ***4.1.1 print(var v)***
向控制台打印一个对象.可以是数字也可以是字符串.或者是vector,map.
```
print(1);
print("100");
```
*****
### ***4.1.2 println(var v)***
它与print功能一致,只是会在输出的末尾增加一个换行符.
*****
### ***4.1.2 print_color(var v,var color)***
它与print功能一致.并且可以设置颜色.
*****
### ***4.1.2 println_color(var v,var color)***
它与print功能一致,只是会在输出的末尾增加一个换行符.并且可以设置颜色.
*****
### ***4.1.3 dump(var v)***
dump是为了让用户更好的调试或者格式化自己的类对象.用户只要在类中实现dump函数,就可以让任何人dump自己的类对象
下面看下代码示例
```
class A
{
    var s = "10000";
    var dump()
    {
        return "dump s = {s}";
    }
}

var a = new A();
var s = dump(a);
println(s);
结果
dump s = 10000
```
*****
### ***4.1.4 exit(var exitcode)***
它就是调用stdlib的exit函数. 让程序在此时此刻退出进程
```
exit(0);
```
*****
### ***4.1.5 sleep && Sleep(var ms)***
开头字母可以是大写,也可以是小写.同等于调用linux的 usleep MSVC的Sleep,让程序休眠 ms 毫秒
```
sleep(1000) //休眠1秒
```
*****
### ***4.1.6 tonumber && double(var a)***
将一个变量强制类型转换为浮点型
```
var a = "16.2";
var d = tonumber(a);
println(d);
var d2 = double(a);
println(d2);
结果
16.2
16.2
```
*****
### ***4.1.7 toint && int(var v)***
将一个变量强制类型转换为整数型
```
var d = 16.2;
var i = toint(d);
println(i);
var i2 = int(d);
println(i2);

结果
16
16
```
*****
### ***4.1.8 tostring && str(var v)***
强制类型转换为字符串
```
var  a = 100;
var s = tostring(a);
println(s);
var s2 = str(a);
println(s2);

结果
100
100
```
*****
### ***4.1.9 isstring(var v)***
判断一个变量是否为字符串类型
*****
### ***4.1.10 isint(var v)***
判断一个变量否为整数型
*****
### ***4.1.11 isbool(var v)***
判断一个变量是否为布尔类型
*****
### ***4.1.12 isvector(var v)***
判断一个变量是否为数组
*****
### ***4.1.13 ismap(var v)***
判断一个变量是否为map
*****
### ***4.1.14 isnull(var v)***
判断一个变量是否为NULL
*****
### ***4.1.15 isnumber(var v)***
判断一个变量是否为浮点型
*****
### ***4.1.16 isfunction(var v)***
判断一个变量是否为函数
*****
### ***4.1.17 isclassvar(var v)***
判断一个变量是否为对象
*****
### ***4.1.18 isclass(var v)***
判断一个变量是否为类
*****
### ***4.1.19 objtype(var v)***
返回变量的类型 可以参见 ot 
```
var a  = 10;
if(objtype(a) == ot.int)
   println("is int");
```
*****
### ***4.1.20 type(var v)***
返回字符串格式的变量类型
```
var a = 10;
println(type(a));
结果
integer
```
*****
### ***4.1.21 system(var cmd)***
调用stdlib的system,cmd命令详情参考各操作系统说明.
*****
### ***4.1.22 len(var v)***
支持类型vector map 以及 字符串. 返回他们的size函数
```
var vec = [1,2,3,4];
println(len(vec));
var s = "abcdefg";
println(len(s));

结果
4
```
*****
### ***4.1.23 sort(var vec)***
使用快速排序法为vector进行排序.
```
var  vec = [2,1,5,3,32,7,10,4];
sort(vec);
println(vec)
结果
[1,2,3,4,5,7,10,32,]
```
*****
### ***4.1.23 shuffle(var vec)***
使用快速洗牌法为vector进行打乱顺序.
```
var  vec = [1,2,3,4,5,6,7];
shuffle(vec);
println(vec)
结果
[7,1,2,4,3,6,5,]
```
*****
### ***4.1.24 isset(var v)***
判断一个变量是否存在或者不为null
```
if(isset(a))
    println(a); // a不存在所以什么都不会输出.
```
*****
### ***4.1.25 assert(var bool)***
断言.不过多介绍了. C++开发者最熟悉不过的了.
*****
### ***4.1.26 getargs()***
获取main(int argc,char **argv) 中的参数,返回格式为vector.
```
cpps test.cpp 123 456 789
test.cpp
println(getargs());
结果
[cpps.exe,test.cpp,123,456,789,]
```
*****
### ***4.1.27 execmd(var cmd)***
类似system,但是它可以把执行结果返回给程序.
*****
### ***4.1.28 now()***
获取当前unix时间戳,同等于 time.getime()
````
var t = now();
````
*****
## 4.2 sys
### ***4.2.1 ***sys.platform******
sys.platform 包含操作系统的一些信息 譬如windows分win32与win64,linux分linux32与linux64,macos分macos32与macos64等.
*****
### 4.2.2 ***sys.esayplanform***
它就比较简单, 只有windows,linux,macos(目前只支持这3中操作系统)
*****
### 4.2.3 ***sys.builder_version***
它包含了编译器的版本信息.
*****
### 4.2.4 ***sys.version***
它包含了CPPS最详细的编译版本信息.
*****
### 4.2.5 ***sys.versionno***
它为数字版本的CPPS版本号.1.0.0 对应是 10000 ,例如51.21.23 版本 = 512123版本. cpps 没有百位版本号.
*****
### 4.2.6 ***sys.debug***
判断是否为debug编译版本.
*****
## 4.3 ot
可以把ot看做成枚举,但是cpps暂时还没支持枚举.
```
enum ot
{
    int,
    bool,
    string,
    classvar
    function
    domain,
     ptr,
     number,
    nil,
    lambda
}
```
*****
## 4.4 environ
系统支持环境变量
*****
### ***environ.get(var key)***
获取系统环境变量
```
var path = environ.get("PATH");
```
*****
### ***environ.set(var key,var value)***
设置系统环境变量
```
environ.set("SELFKEY","SELFVALUE");
```
*****
## 4.5 serializer
序列化是一个非常有用的功能.它可以把一个对象变成一个map或者vector,然后在从一个vector或者map 反序列化成一个对象.
*****
### ***serializer.encode(var v,var type = serializer.vector)***
序列化会默认序列化成数组格式.
```
class A
{
    var a = 10;
    var b = 20;
    var c = 30;
}

var a = new A();

println(serializer.encode(a,serializer.vector));
println(serializer.encode(a,serializer.map));
结果:
[10,20,30,]
{a:10,b:20,c:30,}
```
*****
### ***serializer.decode(var __class,var v)***
```
class A
{
    var a ;
    var b ;
    var c ;
}

var arr = [20,50,100];

var a = serializer.decode(A,arr);
println(a.a);
println(a.b);
println(a.c);
结果:
20
50
100
```
*****
### ***serializer.vector***
代表序列化成vector格式.
*****
### ***serializer.map***
代表序列化成map格式.
*****
## 4.6 console
### ***console.color(var __color)***
其实每个平台中控制台的字体颜色很多,但是颜色相近的很少,我整理出7种相近的不过已经够用了.
下面可以尝试一下.
```
foreach(var i : range(6))
{
    console.color(i);
    println("color:{i}");
    console.clearcolor();
}
结果(windows)
0 黑色
1 红色
2 绿色
3 黄色
4.蓝色
5 紫色
6 青色
```
![](images/screenshot_1609060599161.png)
*****
### ***console.clearcolor()***
将控制台字体还原成初始颜色(一般为白色).
```
var i = 1; //red
console.color(i);
println("color:{i}");
console.clearcolor();
```
*****
### ***console.clear()***
清屏功能,在windows下system("cls"),linux或者mac上为system("clear");
在这集合成一个函数,更方便使用了.
*****
## 4.7 debug
### ***debug.open()***
启用debug模式
*****
### ***debug.close()***
关闭debug模式
*****
### ***debug.log(var v)***
根据debug模式是否打印日志.
*****
### ***debug.breakpoint()***
断点模式.可以在断点时利用cpps语法调试代码.
```
var a = 10;
debug.breakpoint();
println(a);

结果:
quit debug command : quit
debug>a = 20
debug>quit
20
```
*****
### ***debug.trace()***
这是一个非常好用的调试功能,它可以把当前作用域中所有存在的变量都打印出来,
下面看一下代码示例
```
class A
{
    var a = 20;
    var b = 2000;
    var c = 5000;
}
var func()
{
    var a = 200;
    var b = new A();
    var c = 1000;
    debug.trace();
}
func();

结果:
a = 200
b = class <A>
{
    b.a = 20
    b.b = 2000
    b.c = 5000
}

c = 1000
```
*****
## 4.8 time
### ***time.gettime()***
获取当前UNIX时间戳返回为整数型(单位秒)
*****
### ***time.gettimestr()***
获取当前UNIX时间戳返回为字符串型(单位秒)
```
var ts = time.gettimestr();
println(ts);
结果:
2020-12-29 13:04:19
```
*****
### ***time.maketime(var year,var mon,var day,var hour,var min,var sec)***
生成一个UNIX系统时间.返回为整数型.
```
var t = time.maketime(2020,12,29,13,00,00);
println(t);
结果
1609218000
```
*****
### ***time.gettickcount()***
获取从开机到现在所用的毫秒数.
```
var tc = time.gettickcount();
println(tc);
结果:
18024223
```
*****
### ***time.time2str(var t)***
将整数型UNIX时间转换成字符串类型时间戳.
```
var t = time.gettime();
var ts = time.time2str(t);
println(ts);
结果:
2020-12-29 13:34:43
```
*****
### ***time.str2time(var ts)***
将字符串类型UNIX时间戳转换到整数型时间戳
```
var ts = "2020-12-29 13:34:43";
var t = time.str2time(ts);
println(t);
结果:
1609220083
```
*****
### ***time.issomeday(var t1,var t2)***
判断2个整数型时间戳是否为同一天.
*****
### ***time.getyear(var t)***
根据整数型时间戳获取年份
*****
### ***time.getmon(var t)***
根据整数型时间戳获取月份
*****
### ***time.getday(var t)***
根据整数型时间戳获取本月天数
*****
### ***time.gethour(var t)***
根据整数型时间戳获取本日小时部分
*****
### ***time.getmin(var t)***
根据整数型时间戳获取本小时分钟部分
*****
### ***time.getsec(var t)***
根据整数型时间戳获取本分钟描述部分.
*****
### ***time.getyday(var t)***
根据整数型时间戳获取是本年的某天.
*****
### ***time.getwday(var t)***
根据整数型时间戳获取是本周的某天
*****
### ***time.addmonths(var t,var mons)***
在整数型时间戳上增加或减少mons月.
*****
### ***time.adddays(var t,var days)***
在整数型时间戳上增加或减少days天.
*****
### ***time.addyears(var t,var years)***
在整数型时间戳上增加或减少years年.
*****
### ***time.addweeks(var t,var weeks)***
在整数型时间戳上增加或减少weeks周
*****
### ***time.addhours(var t,var hours)***
在整数型时间戳上增加或减少 hours 小时
*****
### ***time.addminutes(var t,var mins)***
在整数型时间戳上增加或减少 mins 分钟
*****
### ***time.addseconds(var t,var secs)***
在整数型时间戳上增加或减少 secs 秒钟
*****
### ***time.strftime(String fmt,var unixtime)***
将unix时间戳格式化成字符串
```
var t = time.maketime(2017,1,1,12,0,5);
var t2 = time.strftime("%F",t);
println(t2);
结果
2017-01-01
```
格式化参数
```
%a 星期几的简写

%A 星期几的全称

%b 月份的简写 

%B 月份的全称

%c 标准的日期的时间串

%C 年份的前两位数字

%d 十进制表示的每月的第几天

%D 月/天/年

%e 在两字符域中，十进制表示的每月的第几天

%F 年-月-日

%g 年份的后两位数字，使用基于周的年

%G 年份，使用基于周的年

%h 简写的月份名

%H [24小时制]的小时

%I 12小时制的小时

%j 十进制表示的每年的第几天

%m 十进制表示的月份

%M 十时制表示的分钟数

%n 新行符

%p 本地的AM或PM的等价显示

%r 12小时的时间

%R 显示小时和分钟：hh:mm

%S 十进制的秒数

%t 水平[制表符]

%T 显示时分秒：hh:mm:ss

%u 每周的第几天，星期一为第一天 （值从1到7，星期一为1）

%U 第年的第几周，把星期日作为第一天（值从0到53）

%V 每年的第几周，使用基于周的年

%w 十进制表示的星期几（值从0到6，星期天为0）

%W 每年的第几周，把星期一做为第一天（值从0到53）

%x 标准的日期串

%X 标准的时间串

%y 不带世纪的十进制年份（值从0到99）

%Y 带世纪部分的十制年份

%z，%Z 时区名称，如果不能得到时区名称则返回空字符。

%% [百分号]
```
*****
## 4.9 io
### ***io.getc()***
获得从控制台输入的文字 实际调用的是std::cin .所以遇到空格,回车,换行就会触发截停.
```
var c = io.getc();
println(c);
结果
hello world
hello
```
* 因为hello world中间有空格 就只读取到了hello 剩下的world需要在调用一次io.getc()才可以获取到.如果想要获取一整行可以调用io.getline()
*****
### ***io.getline()***
获得从控制台输入的一行文字.C++ 实际调用是getline(std::sin,ret); 
*****
### ***io.fopen(var path,var mode)***
打开一个文件并返回句柄
mode 模式与c++一致.
```
“r” ：以只读方式打开文件，该文件必须存在。
“w” ：打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。  
“a” ：以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）  
“r+” ：以可读写方式打开文件，该文件必须存在。  
“w+” ：打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。  
“a+”：以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）  
“rb” ：只读打开一个二进制文件，只允许读数据。  
“wb” ：只写打开或建立一个二进制文件，只允许写数据。  
“ab” ：追加打开一个二进制文件，并在文件末尾写数据。  
“rb+” ：读写打开一个二进制文件，允许读写数据，文件必须存在。 
“wb+” ：读写打开或建立一个二进制文件，允许读和写。  
“ab+” ：读写打开一个二进制文件，允许读，或在文件末追加数据。  
“rt” ：只读打开一个文本文件，只允许读数据。  
“wt” ：只写打开或建立一个文本文件，只允许写数据。  
“at” ：追加打开一个文本文件，并在文件末尾写数据。  
“rt+” ：读写打开一个文本文件，允许读和写。  
“wt+” ：读写打开或建立一个文本文件，允许读写。  
“at+” ：读写打开一个文本文件，允许读，或在文件末追加数据。
```
*****
### ***io.writefile(var path,var content)***
快速写入一个文件.content为字符串类型.
```
var content = "abcdefg";
io.writefile("a.txt",content);
```
*****
### ***io.readfile(var path)***
快速读取一个文件返回字符串类型里,字符串实际类型为std::string,所以并不会因为\0而少读取资源.可根据size函数获取长度.
```
a.txt
abcdefg

var fcontent = io.readfile("a.txt");
println(fcontent);
结果
abcdefg
```
*****
### ***io.filesize()***
快速返回一个文件的大小
```
var fsize = io.filesize("a.txt");
```
*****
### ***io.fsize(FILE file)***
使用fopen打开的文件句柄来获取文件大小.
```
var file = io.fopen("a.txt","rb");
var filesize = io.fsize(file);
```
*****
### ***io.fread(FILE file,Buffer buf,var len)***
使用fopen打开的文件句柄来读取数据.第二个参数为Buffer.
下面看一下代码示例
```
var buf = new Buffer();
var file = io.fopen("a.txt","rb");
if(file)
{
    var filesize = io.fsize(file);
    io.fread(file,buf,filesize);
    io.fclose(file);
}
println(buf.tostring());
结果
abcdefg
```
*****
### ***io.fwrite(FILE file,Buffer buf)***
使用fopen打开的文件句柄来写入数据.第二个参数为Buffer
下面看一下代码示例
```
var buf = new Buffer();
buf.writeString("abcdefg");
var file = io.fopen("a.txt","wb+");
if(file)
{
    io.fwrite(file,buf);
    io.fclose();
}
```
*****
### ***io.fwrites(FILE file,var str)***
使用fopen打开的文件句柄来写入数据.第二个参数为字符串
```
var file = io.fopen("a.txt","wb+");
if(file)
{
    io.fwrites(file,"abcdefg");
    io.fclose();
}
```
*****
### ***io.fseek(FILE file,var offset,var origin)***
使用fopen打开的文件句柄来操作游标,与C++使用方法一致.
```
io.fseek(file,0,io.SEEK_END);
```
*****
### ***io.fclose(FILE file)***
关闭一个文件句柄.
*****
### ***io.fflush(FILE file)***
使用fopen打开的文件句柄来冲洗流中的信息
*****
### ***io.remove(var filepath)***
删除路径中的文件.
*****
### ***io.copy(var srcfilepath,var tarfilepath)***
拷贝一个文件.
*****
### ***io.move(var srcfilepath,var tarfilepath)***
剪切一个文件
*****
### ***io.copydir(var srcdirpath,var tardirpath)***
拷贝一个文件夹.
*****
### ***io.movedir(var srcdirpath,var tardirpath)***
剪切一个文件夹
*****
### ***io.rename(var srcname,var newsrcname)***
重命名一个文件或文件夹
*****
### ***io.getfileext(var filepath)***
得到一个路径中文件的扩展名
*****
### ***io.getfilepath(var filepath)***
得到一个路径中文件的路径部分
*****
### ***io.getfilename(var filepath)***
得到一个路径中文件名字部分
*****
### ***io.getfilenamenotext(var filepath)***
得到一个路径中文件名字部分但去掉扩展名
*****
### ***io.getcwd()***
获取当前程序的工作目录.
*****
### ***io.mkdir(var dirpath)***
创建一个文件夹.它不能创建多层文件夹.如果需要创建多层需要使用 ***io.mkdirs***
*****
### ***io.rmdir(var dirpath)***
删除一个文件夹,它不能删除还存在内容的文件夹,如果需要强行删除带有内容的文件夹请使用 ***io.rmdirs***
*****
### ***io.mkdirs(var dirpath)***
创建多层文件夹.
*****
### ***io.chdir(var dirpath)***
修改工作目录.
*****
### ***io.isdir(var path)***
判断目录是否为文件夹
*****
### ***io.isfile()***
判断目录是否为文件
*****
### ***io.normpath(path)***
路径格式化.这个功能与python中的normpath一致.将所有'/' '\\' 一律统一为 '\\' , 将../或者./ 修改成最终正式目录.
```
var path = "d:\\work\\code\\bin/script/../cpps.exe";

path = io.normpath(path);
println(path);

path.pop_back();
path.replace("\\","/");

println(path);

结果:
d:\work\code\bin\cpps.exe\
d:/work/code/bin/cpps.exe
```
* 值得注意的是它会在最后增加一个\\ 我们需要根据需要删除它. path.pop_back();, 当linux时需要整体把'\\'替换成'/'. path.replace("\\","/");
*****
### ***io.isabspath(var path)***
判断一个路径是否为绝对路径.
```
var path1 = "d:/work/";
println(io.isabspath(path1));

var path2 = "work/code";
println(io.isabspath(path2));

var path3 = "/work/code";
println(io.isabspath(path3));

结果
true
false
true
```
*****
### ***io.splitdrive(var path)***
把一个路径拆分数组,0位为盘符,1位为绝对路径
```
var path = "d:/work/code";
var l = io.splitdrive(path);
println(l[0]);
println(l[1]);
结果
d:
/work/code
```
*****
### ***io.getrealpath()***
获取当前可执行文件的真实路径,非工作路径.
*****
### ***io.file_exists(var path)***
检测文件是否存在.
*****
### ***io.walk(var path,var findchildren = true)***
遍历文件夹中所有文件与文件夹
```
var filelist = io.walk("d:/work/code");
println(filelist);
```
*****
### ***io.listdir(var path,var findchildren = true)***
只遍历文件夹中的文件夹.
```
var filelist = io.listdir("d:/work/code");
println(filelist);
```
*****
### ***io.stat(var path)***
获取文件或文件夹的stat属性并返回一个class statinfo对象.
*****
### ***class statinfo***
#### ***statinfo::dev()***
返回stat.st_dev
*****
#### ***statinfo::ino()***
返回stat.st_ino
*****
#### ***statinfo::mode()***
返回stat.st_mode
*****
#### ***statinfo::nlink()***
返回stat.st_nlink
*****
#### ***statinfo::uid()***
返回stat.st_uid
*****
#### ***statinfo::gid()***
返回stat.st_gid
*****
#### ***statinfo::rdev()***
返回stat.st_rdev
*****
#### ***statinfo::size()***
返回stat.st_size
*****
#### ***statinfo::atime()***
返回stat.st_atime
*****
#### ***statinfo::mtime()***
返回stat.st_mtime
*****
#### ***statinfo::ctime()***
返回stat.st_ctime
*****
#### ***statinfo::isdir()***
判断是否为文件夹
*****
#### ***statinfo::isreg()***
判断是否为文件
*****
#### ***statinfo::isblk()***
是否是一个块设备
*****
#### ***statinfo::ischr()***
是否是一个字符设备
*****
#### ***statinfo::isfifo()***
是否是一个FIFO文件
*****
#### ***statinfo::islink()***
是否是一个连接
*****
#### ***statinfo::issock()***
是否是一个SOCKET文件
*****
### ***io.last_write_time(var filepath)***
获取文件最后修改时间.
*****
### ***io.sep***
在windows系统中路径分隔符为'\\',在linux与unix系统中路径分割符为'/'
*****
### ***io.linesep***
在windows系统中换行符为"\r\n",在linux与unix系统中换行符为'\n'
*****
## 4.10 math
### ***math.abs(var num)***
取num的绝对值并返回.
*****
### ***math.srand(var seed)***
设置随机数种子. 常用的是方法是io.srand(time.gettime());
*****
### ***math.randf()***
返回0-1之间的随机数.
*****
### ***math.rand()***
返回一个整数的随机.非C++的 rand(). 最大值非RAND_MAX.
*****
### ***math.random(var min,var max)***
返回min-max之间的随机数.
*****
### ***math.acos(var x)***
返回以弧度表示的 x 的反余弦
*****
### ***math.asin(var x)***
返回以弧度表示的 x 的反正弦
*****
### ***math.atan(var x)***
返回以弧度表示的 x 的反正切
*****
### ***math.ceil(var x)***
返回大于或等于 x 的最小的整数值
*****
### ***math.cos(var x)***
返回x的弧度的余弦值
*****
### ***math.exp(var x)***
返回 e 的 x 次幂的值
*****
### ***math.floor(var x)***
返回小于或等于 x 的最大的整数值
*****
### ***math.fmod(var x,var y)***
返回 x 除以 y 的余数
*****
### ***math.log(var x)***
返回 x 的自然对数
*****
### ***math.max(var x,var y)***
返回 x 与 y 中最大的数.
*****
### ***math.min(var x,var y)***
返回 x 与 y 中最小的数.
*****
### ***math.sin(var x)***
返回的x弧度的正弦值
*****
### ***math.sqrt(var x)***
返回数字x的平方根
*****
### ***math.tan(var x)***
返回x弧度的正切值
*****
### ***math.HUGE***
官方解释:
当函数的结果不可以表示为浮点数时。如果是因为结果的幅度太大以致于无法表示，则函数会设置 errno 为 ERANGE 来表示范围错误，并返回一个由宏 HUGE_VAL 或者它的否定（- HUGE_VAL）命名的一个特定的很大的值。

如果结果的幅度太小，则会返回零值。在这种情况下，error 可能会被设置为 ERANGE，也有可能不会被设置为 ERANGE。
*****
### ***math.maxinteger***
返回 ((cpps_integer)(MAXUINT64 >> 1)) 即为9223372036854775807
*****
### ***math.mininteger***
返回 ((cpps_integer)~MAXINT64) 即为 -9223372036854775808
*****
### ***math.pi***
返回 3.14159265358979323846
*****
## 4.11 string
**在老版本cpps中未封装std::string,所以都是使用string模块辅助字符串的一些操作.所以很多函数都可以在字符串类中操作了.**
### ***string.find(String _str,var text,var pos = 0)***
同String::find();
*****
### ***string.rfind(String _str,var text,var pos = 0)***
同String::rfind();
*****
### ***string.length(String _str)***
同String::size();
*****
### ***string.strlen(String _str)***
内部实现为 strlen(_str.c_str()); 真是获取文本的长度.遇到'\0'则停止.
*****
### ***string.replace(String _str,var src,var tar)***
同String.repalce();
*****
### ***string.clear(String _str)***
同String::clear();
*****
### ***string.copyto(String srcsstr,String tarstr)***
同String::copyto();
*****
### ***string.split(String _str,var sep,var count = -1)***
同String::split();
*****
### ***string.cut(String _str,var len)***
同String::cut();
*****
### ***string.strcut(String _str,var left,var right)***
同String::strcut();
*****
### ***string.strcuts(String _str,var left,var right)***
同String::strcuts();
*****
### ***string.empty(String _str)***
同String::empty();
*****
### ***string.substr(String _str,var pos,var len = string.npos)***
同String::substr();
*****
### ***string.npos***
同c++中std::string::npos.
*****
### ***string.at(String _str,var idx)***
同String::at();
*****
### ***string.format(var fmt,...)***
类似sprintf操作. 比较古老的一种方式了.
```
c	字符
d   或 i	有符号十进制整数
e	使用 e 字符的科学科学记数法（尾数和指数）
E	使用 E 字符的科学科学记数法（尾数和指数）
f	十进制浮点数
g	自动选择 %e 或 %f 中合适的表示法
G	自动选择 %E 或 %f 中合适的表示法
o	有符号八进制
s	字符的字符串
u	无符号十进制整数
x	无符号十六进制整数
X	无符号十六进制整数（大写字母）
p	指针地址
n	无输出
%	字符
```
*****
### ***string.lower(String _str)***
将_str中英文全部转为小写.
*****
### ***string.upper(String _str)***
将_str中英文全部转为大写.
*****
### ***string.ltrim(String _str)***
同String::ltrim();
*****
### ***string.rtrim(String _str)***
同String::rtrim();
*****
### ***string.trim(String _str)***
同String::trim();
*****
### ***string.join(String _str,vector vct)***
同String::join();
*****
### ***string.between(var left,var right,vector vct)***
类似String::join, 将vct中的每一个字符串对象左侧增加left,右侧增加right.
```
var vct = [1,2,3,4,5,6,7];
println(string.between("<",">,",vct));
结果
<1>,<2>,<3>,<4>,<5>,<6>,<7>,
```
*****
### ***string.startswith(String _str,var str)***
同String::startswith();
*****
### ***string.endswith(String _str,var str)***
同String::endswith();
*****
### ***string.chr(var charcode)***
将一个charcode转成字符串.(不推荐使用)
*****
### ***string.push_back(String _str,var charcode)***
同String::push_back();
*****
### ***string.unicode_charCodeAt(String _str,var idx)***
获取string中unicode编码值.
*****
### ***string.unicode_fromCodeAt(String _str,vector vct)***
把vct中的unicode编码添加到_str中.
*****
## 4.12 GC
### ***GC.Collect()***
清理新生代
*****
### ***GC.CollectAll()***
清理新生代&老生代
*****
## 4.13 asyncio
协程，又称微线程，纤程。它运行在单线程中,可以实现多任务并行执行.由cpps自身控制上下文切换.可以减少更多函数等待时间,从而大大的增加运行速度.
### ***asyncio.get_event_loop()***
获取asyncio协程主循环对象.
*****
### ***asyncio.create_task(asyncio::ASYNC_OBJECT func)***
创建一个协程任务,这里创建后并加入循环队列执行.等待await获取结果.
```
async var testasio()
{
    return 100;
}

async var main()
{
    var task1 = asyncio.create_task(testasio());
    var ret = await task1;
    println(ret);

}
asyncio.run(main());

结果
100
```
await 并不一定需要create_task.
await testasio(); 方法也可以执行.
在协程中有一些函数并不需要返回值.只要关注执行进度的时候,create_task就比较实用了.
*****
### ***asyncio.sleep(var ms)***
协程中的休眠,休眠ms毫秒,它不同于sleep.此函数为协程函数.
*****
### ***asyncio.wait(var asyncfunc)***
asyncfunc可以是ASYNC_OBJECT 或者 ASYNC_TASK, 它会等待直到函数运行结束返回ASYNC_TASK对象.
```
async var testasio()
{
    return 100;
}

async var main()
{
    var ret = await asyncio.wait(testasio());
	println("state:{ret.state()}");
	println(ret.result());
    ret = NULL;
}
asyncio.run(main());
结果:
state:2
100
```
*****
### ***asyncio.wait_for(asyncio::ASYNC_OBJECT func,var ms)***
同wait函数类似,但是它有一个等待时间,超出等待时间会让强制停止协程运行并返回ASYNC_TASK对象
> 但是await asyncio.sleep(2000000); 并不会因为wait_for停止而停止.在开发协程时需注意wait_for不要丢给业务层.要在可能会引起超时的外部函数时来决定调用wait_for最佳,否可能导致并没有真正意义上结束掉协程.

但main()主协程结束时,会强制停止所有协程.
```
async var testasio()
{
    await asyncio.sleep(2000000); //没有真正被停掉.
    return 100;
}

async var main()
{
    var ret = await asyncio.wait_for(testasio(),1000); //只等待1秒.
	if(ret.timeout())
	{
		println("task1 was timeouted.");
	}
	println("state:{ret.state()}");
	println(ret.result());
    ret = NULL;
}
asyncio.run(main());
结果:
task1 was timeouted.
state:4
nil
```
> 下面推荐的做法是.

```
async var testasio(var timeout)
{
    await asyncio.wait_for(asyncio.sleep(2000000),timeout); //这样才能保证正确停止.
    return 100;
}

async var main()
{
    var ret = await testasio(1000); //只等待1秒.
	println(ret);
    ret = NULL;
}
asyncio.run(main());
结果:
100
```
*****
### ***asyncio.run(var task)***
在协程中我们推荐使用asyncio.run来开始启动协程,原则上推荐全程序只有一个run被执行.例如以下方式
```
async var main()
{
    //do something..
}
asyncio.run(main());
```
*****
### ***asyncio.pending***
协程状态,代表已创建但未加入队列运行.
*****
### ***asyncio.running***
协程状态,代表已经在运行中
*****
### ***asyncio.done***
协程状态,代表已经运行完成
*****
### ***asyncio.cancelled***
协程状态,代表已经被取消运行
*****
### ***asyncio.timeout***
协程状态,代表已经运行超时.
*****
### ***class asyncio::ASYNC_OBJECT***
协程函数当被执行时并非真实运行,而是生成一个ASYNC_OBJECT对象.等待加入到任务队列.
```
async var test()
{
    return 100;
}
var async_object = test();

println(async_object);
结果
class <ASYNC_OBJECT>
```
*****
### ***class asyncio::ASYNC_LOOP***
协程的主循环对象.负责管理协程的上下文切换,已经运行周期的管理.
#### ***ASYNC_LOOP::run_until_complete(var task)***
并不推荐用户直接调用这个函数.建议用户调用asyncio.run函数来替代这个接口.
task可以是一个ASYNC_OBJECT或ASYNC_TASK,同样可以是一个列表.
用户可以并发执行这些任务直至所有任务结束.

```
var loop = asyncio.get_event_loop();

async var test1()
{
    await asyncio.sleep(100); //等待100毫秒后再执行,当test3执行完的时候我还没有休眠完成,在让给test2执行
    println("test1");
}

async var test2()
{
    yield; //先让其他协程执行.
    println("test2"); //当test3执行完并且test1没休眠完成时,test2就可以优先完成.
}

async var test3()
{
    println("test3"); //没人先执行完,那我先执行结束.
}

var tasks = [
    test1(),
    test2(),
    test3()
];

loop.run_until_complete(tasks);
结果:
test3
test2
test1
```
*****
### ***class asyncio::ASYNC_TASK***
ASYNC_TASK 为协程任务实体,它包含着ASYNC_OBJECT运行对象,以及上下文数据,还有返回值,以及运行状态等信息.在做协程相关操作时它起到了至关重要的作用.
#### ***ASYNC_TASK::result()***
返回ASYNC_OBJECT运行后的返回值.如果期间任务被任何情况打断时它返回为null.
*****
#### ***ASYNC_TASK::state()***
获取任务当前协程状态.
*****
#### ***ASYNC_TASK::cancelled()***
判断是否被取消,如果是取消状态则返回true,否则false.
*****
#### ***ASYNC_TASK::done()***
判断是否完成,如果是完成状态则返回true,否则false.
*****
#### ***ASYNC_TASK::timeout()***
判断是否超时,如果是超时状态则返回true,否则false.
*****
#### ***ASYNC_TASK::pending()***
判断是否未运行,如果是未运行状态则返回true,否则false.
*****
#### ***ASYNC_TASK::running()***
判断是否运行中,如果是运行中状态则返回true,否则false.
*****
#### ***ASYNC_TASK::set_name(var name)***
设置协程名称,可在callback时根据协程名称做一些逻辑判断.
*****
#### ***ASYNC_TASK::get_name()***
获取协程名称.
*****
#### ***ASYNC_TASK::add_done_callback(var func,var context = nil)***
增加完成回调函数,context可以在完成时传回到回调函数中.
*****
#### ***ASYNC_TASK::remove_done_callback()***
移除完成回调函数.

```
async var testasio()
{
    return 100;
}
var done_call_back(var task,var context)
{
    println(task.get_name());
    println(context);
}
async var main()
{
    var task1 = asyncio.create_task(testasio());
    task1.set_name("er gou zi");
    task1.add_done_callback(done_call_back,"gua wa zi");
    var ret = await task1;
    println(ret);

}
asyncio.run(main());
结果
er gou zi
gua wa zi
100
```
*****
# 5.内置扩展模块
## 5.1 compress
### ***zlib***
本扩展模块对zlib压缩库进行了封装.
#### ***zlib.compress(var data,var level = zlib.Z_DEFAULT_COMPRESSION)***
zip方式压缩,data可以是一个字符串格式或者是Buffer格式. level 默认为Z_DEFAULT_COMPRESSION压缩比.压缩成功则返回一个字符串类型的对象.否则为nil
*****
#### ***zlib.decompress(var data,var uncompresssize = 4096)***
zip方式解压缩.data可以是一个字符串格式或者是Buffer格式.uncompresssize 为未压缩时真实大小.如果不知道大小可以尽可能给一个大一些的数,否则可能会导致数据不全或解压缩失败.如果解压缩成功会返回一个字符串类型的对象,否则为nil
```

#import "compress"

 var srcstr = "123456789";
//zlib
var tarstr = zlib.compress(srcstr);
println(string.length(tarstr));
var srcstr2 = zlib.decompress(tarstr);

println(srcstr2);

```
*****
#### ***zlib.adler32(var data,var adler = 1)***
adler32校验算法.data可以是一个字符串格式或者是Buffer格式.
```
println(zlib.adler32("hello world"));
```
*****
#### ***zlib.crc32(var data,var crc = 1)***
crc32校验算法.data可以是一个字符串格式或者是Buffer格式.
```
println(zlib.crc32("hello world"));
```
*****
#### ***zlib.Z_NO_COMPRESSION = 0***
*****
#### ***zlib.Z_BEST_SPEED = 1***
*****
#### ***zlib.Z_BEST_COMPRESSION = 9***
*****
#### ***zlib.Z_DEFAULT_COMPRESSION = (-1)***
*****
#### ***zlib.ZLIB_VERSION***
编译时的zlib版本.
*****
#### ***zlib.ZLIB_RUNTIME_VERSION()***
当前动态库的zlib版本.
*****
### ***gzip***
本gzip是对zlib中的gzip压缩与解压缩方式进行的封装.
#### ***gzip.compress(var data,var level = gzip.Z_DEFAULT_COMPRESSION)***
gzip方式压缩,data可以是一个字符串格式或者是Buffer格式. level 默认为Z_DEFAULT_COMPRESSION压缩比.压缩成功则返回一个字符串类型的对象.否则为nil
*****
#### ***gzip.decompress(var data,var uncompresssize = 4096)***
gzip方式解压缩.data可以是一个字符串格式或者是Buffer格式.uncompresssize 为未压缩时真实大小.如果不知道大小可以尽可能给一个大一些的数,否则可能会导致数据不全或解压缩失败.如果解压缩成功会返回一个字符串类型的对象,否则为nil
```
#import "compress"

 var srcstr = "123456789";

var tarstr = gzip.compress(srcstr);
println(string.length(tarstr));
var srcstr2 = gzip.decompress(tarstr);

println(srcstr2);
```
*****
#### ***gzip.Z_NO_COMPRESSION = 0***
*****
#### ***gzip.Z_BEST_SPEED = 1***
*****
#### ***gzip.Z_BEST_COMPRESSION = 9***
*****
#### ***gzip.Z_DEFAULT_COMPRESSION = (-1)***
*****
### ***tarfile***
tarfile 模块可以用来读写 tar 归档，包括使用 gzip, (bz2 和 lzma 暂未实现) 压缩的归档。 请使用 zipfile 模块来读写 .zip 文件，或者使用 shutil 的高层级函数。
#### ***tarfile.open(var filepath,var mode = 'r',var bufsize = 10240)***

| 模式 | 动作 |
| --- | --- |
| `'r'or'r:*'` | 打开和读取使用透明压缩（推荐）。 |
| `'r:'` | 打开和读取不使用压缩。|
 | `'r:gz'` | 打开和读取使用gzip 压缩。 |
| `'r:bz2'` | 打开和读取使用bzip2 压缩。 (暂未实现)|
| `'r:xz'` | 打开和读取使用lzma 压缩。(暂未实现) |
| `'x'`或`'x:'` | 创建tarfile不进行压缩。如果文件已经存在，则返回空。 |
| `'x:gz'` | 使用gzip压缩创建tarfile。如果文件已经存在，则返回空 |
| `'x:bz2'` | 使用bzip2 压缩创建tarfile。如果文件已经存在，则返回空 (暂未实现)|
| `'x:xz'` | 使用lzma 压缩创建tarfile。如果文件已经存在，则返回空 (暂未实现)|
| `'a'or'a:'` | 打开以便在没有压缩的情况下追加。如果文件不存在，则创建该文件。 |
| `'w'or'w:'` | 打开用于未压缩的写入。 |
| `'w:gz'` | 打开用于 gzip 压缩的写入。 |
| `'w:bz2'` | 打开用于 bzip2 压缩的写入。(暂未实现) |
| `'w:xz'` | 打开用于 lzma 压缩的写入。 (暂未实现)|
*****
#### ***class tarfile::tarfile***
##### ***tarfile::open(var filepath,var mode = 'r',var bufsize = 10240)***
也可以自己new一个对象,然后open文件.推荐使用tarfile.open
*****
##### ***tarfile::getmembers()***
获取tarfile压缩文件中所有成员.
*****
##### ***tarfile::extract(var member,var path,var set_attrs = true,var numeric_owner = false)***
提取一个tarfile归档中的成员到指定目录.
*****
##### ***tarfile::extractall(var path,var membsers,var numeric_owner = false)***
将归档中的所有成员提取到指定目录,如果给定了members则只提取members中的成员信息,但前提是members的成员是tarfile中的子集.
*****
##### ***tarfile::close()***
关闭 tarfile 在写入模式下，会向归档添加两个表示结束的零数据块。
*****
##### ***tarfile::gettarinfo(var name,var arcname)***
name为指定文件路径,根据stat信息创建一个tarfile_info对象, archname则为添加到tarfile中的路径与名称.一般从name中提取.如果未指定archname,则会统一归档至根目录.
*****
##### ***tarfile::addfile(tarfile_info tarfileinfo)***
将gettarinfo获取的tarfile_info对象添加到tarfile归档中,在添加归档前你可以对tarfile_info对象进行一下调整与修改.
*****
##### ***tarfile::getmember(var name)***
获取指定name成员对象.
*****
#### ***class tarfile::tarfile_info***
##### ***tarfile_info::isblk()***
是否是一个块设备
*****
##### ***tarfile_info::ischr()***
是否是一个字符设备
*****
##### ***tarfile_info::isdev()***
如果为字符设备、块设备或 FIFO 之一则返回 true
*****
##### ***tarfile_info::isdir()***
是否是一个文件夹(目录)
*****
##### ***tarfile_info::isfifo()***
是否是一个FIFO文件
*****
##### ***tarfile_info::islnk()***
是否是一个连接
*****
##### ***tarfile_info::issym()***
是否是一个符号链接
*****
##### ***tarfile_info::isfile()***
是否是一个文件
*****
```
#import "compress"

/tarfile
//tar解压缩
var file = tarfile.open("test.tar","r",10240000);
file.extractall();

//tar.gz 解压缩
var file = tarfile.open("test.tar.gz","r:gz",10240000);
file.extractall();


//tar.gz 写入
var file = tarfile.open("test2.tar.gz","x:gz",10240000);

var fileinfo = file.gettarinfo("test.txt");

file.addfile(fileinfo);
file.close();

```
### ***zipfile***
ZIP 文件格式是一个常用的归档与压缩标准。 这个模块提供了创建、读取、写入、添加及列出 ZIP 文件的工具.
此模块目前不能处理分卷 ZIP 文件。它可以处理使用 ZIP64 扩展（超过 4 GB 的 ZIP 文件）的 ZIP 文件。
#### ***zipfile.open(var filepath,var pwd = "",var mode = "r",var level = zlib.Z_DEFAULT_COMPRESSION)***
此函数用来打开或创建一个ZIP文件格式的归档与压缩文件,如果打开成功则返回一个zipfile对象,如果打开失败则返回nil.
*****
#### ***class zipfile::zipfile***
##### ***zipfile::open(var filepath,var pwd = "",var mode = "r",var level = zlib.Z_DEFAULT_COMPRESSION)***
此函数用来打开或创建一个ZIP文件格式的归档与压缩文件,如果打开成功则返回true,否则返回false.
*****
##### ***zipfile::infolist()***
返回所有zipfile中成员对象列表,对象格式为zipfile_info.
*****
##### ***zipfile::namelist()***
返回所有zipfile中文件名称(路径)列表.
*****
##### ***zipfile::extract(var member,var path)***
将member一个成员提取到指定目录.
*****
##### ***zipfile::extractall(var path,var members)***
将zipfile中的所有成员对象提到path目录,如果给定members 则只将members中的成员提取到指定目录.但要保证members列表中的成员是zipfile中的成员.
*****
##### ***zipfile::close()***
关闭归档文件。 你必须在退出程序之前调用 close() 否则将不会写入关键记录数据。
*****
##### ***zipfile::read(var name)***
将读取指定文件到内存中,并返回. 返回类型为Buffer.
*****
##### ***zipfile::write(var name,var arcname)***
写入一个文件到zipfile中,如果给定arcname则以arcname为名称(路径)存入zipfile,否则读取name中的名称部分存到zipfile的根目录.
*****
##### ***zipfile::getinfo(var name)***
获取一个zipfile_info对象.
*****
#### ***class zipfile::zipfile_info***
##### ***zipfile_info::comment()***
获取文件注释
*****
##### ***zipfile_info::compress_size()***
获取压缩后的大小
*****
##### ***zipfile_info::compress_type()***
获取压缩类型
*****
##### ***zipfile_info::CRC()***
获取crc32校验值
*****
##### ***zipfile_info::create_system()***
获取创建文件时的系统
*****
##### ***zipfile_info::create_version()***
获取创建文件时的版本
*****
##### ***zipfile_info::data_time()***
获取文件添加zipfile时间.
*****
##### ***zipfile_info::extra()***
扩展字段数据。 PKZIP Application Note 包含一些保存于该 bytes 对象中的内部结构的注释
*****
##### ***zipfile_info::extract_version()***
需要用来提取归档的 PKZIP 版本。
*****
##### ***zipfile_info::file_size()***
获取文件原始大小
*****
##### ***zipfile_info::filename()***
获取文件名称
*****
##### ***zipfile_info::is_dir()***
是否为文件夹.
*****
```
#import "compress"

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
```
## 5.2 base64
### ***base64.encode(String strobj)***
将一个String类型的对象进行base64加密.并返回一个String类型.
*****
### ***base64.decode(String strobj)***
将一个base64加密字符串解密成一个String类型对象.
*****
## 5.3 encoding
### ***encoding.encode(String str,var encodingtype = encoding.UTF8)***
将str转成encodingtype类型字符串.
*****
### ***encoding.UTF8 = 0***
*****
### ***encoding.GBK = 1***
*****
## 5.4 hashlib
### ***class hashlib::md5(String strobj)***
MD5信息摘要算法,一种被广泛使用的密码散列函数，可以产生出一个128位（16字节）的散列值（hash value），用于确保信息传输完整一致。
构造函数相当于调用了update(strobj).
#### ***md5::update(String strobj)***
将strobj加入到MD5算法中.
*****
#### ***md5::hexdigest()***
输出一个16字节的散列值换算成16进制的32位字符串
*****
#### ***md5::hexdigest16()***
取hexdigest()中第9-24位字符串生成16位字符串.
*****
#### ***md5::digest()***
输出String类型16字节的散列值.(Hash value).
*****
#### ***md5::update_file(var filepath)***
讲一个文件加入到MD5算法中.
*****
### ***class hashlib::sha1(String strobj)***
SHA-1可以生成一个被称为消息摘要的160位（20字节）散列值，散列值通常的呈现形式为40个十六进制数
构造函数相当于调用了update(strobj).
#### ***sha1::update(String strobj)***
将strobj加入到SHA1算法中.
*****
#### ***sha1::hexdigest()***
输出一个20字节的散列值换算成16进制的40位字符串
*****
#### ***sha1::digest()***
输出String类型20字节的散列值.(Hash value).
*****
#### ***sha1::update_file(var filepath)***
将一个文件加入到SHA1算法中.
*****
### ***class hashlib::sha224(String strobj)***
SHA-2，名称来自于安全散列算法2（英语：Secure Hash Algorithm 2）的缩写，一种密码散列函数算法标准,其中包括SHA-224、SHA-256、SHA-384、SHA-512.
构造函数相当于调用了update(strobj).
#### ***sha224::update(String strobj)***
将strobj加入到sha224算法中.
*****
#### ***sha224::hexdigest()***
输出一个28字节的散列值换算成16进制的56位字符串
*****
#### ***sha224::digest()***
输出String类型28字节的散列值.(Hash value).
*****
#### ***sha224::update_file(var filepath)***
将一个文件加入到sha224算法中.
*****
### ***class hashlib::sha256(String strobj)***
构造函数相当于调用了update(strobj).
#### ***sha256::update(String strobj)***
将strobj加入到sha256算法中.
*****
#### ***sha256::hexdigest()***
输出一个32字节的散列值换算成16进制的64位字符串
*****
#### ***sha256::digest()***
输出String类型32字节的散列值.(Hash value).
*****
#### ***sha256::update_file(var filepath)***
将一个文件加入到sha256算法中.
*****
### ***class hashlib::sha384(String strobj)***
构造函数相当于调用了update(strobj).
#### ***sha384::update(String strobj)***
将strobj加入到sha384算法中.
*****
#### ***sha384::hexdigest()***
输出一个48字节的散列值换算成16进制的96位字符串
*****
#### ***sha384::digest()***
输出String类型48字节的散列值.(Hash value).
*****
#### ***sha384::update_file(var filepath)***
将一个文件加入到sha384算法中.
*****
### ***class hashlib::sha512(String strobj)***
构造函数相当于调用了update(strobj).
#### ***sha512::update(String strobj)***
将strobj加入到sha512算法中.
*****
#### ***sha512::hexdigest()***
输出一个64字节的散列值换算成16进制的128位字符串
*****
#### ***sha512::digest()***
输出String类型64字节的散列值.(Hash value).
*****
#### ***sha512::update_file(var filepath)***
将一个文件加入到sha512算法中.
*****
```
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
## 5.5 htmlparser
一个基础的html解释器.使用了http://github.com/rangerlee/htmlparser.git的开源项目.
### ***class htmlparser::parser***
#### ***parser::parse(var html)***
返回一个element对象root节点.如果解释失败则返回nil.
*****
### ***class htmlparser::element***
#### ***element::attr(var attrname)***
获取当前标签属性值.
*****
#### ***element::children(var id)***
通过id属性找到子层中的element对象.
*****
#### ***element::childrens_by_classname(var classname)***
通过class属性找到子层中的element对象列表.
*****
#### ***element::childrens_by_tagname()***
通过tagname标签找到子层中的element对象列表.
*****
#### ***element::childrens()***
获取所有子层节点列表.
*****
#### ***element::select(var rule)***
根据规则筛选子层需要查找的element对象节点.并返回一个符合条件的列表.
```
rule 规则我也还没太弄懂,等我稍后研究一下,在重新补文档吧.或者可以联系下作者.
```
*****
#### ***element::parent()***
获取父节点element对象
*****
#### ***element::value()***
获取当前节点中的值.如果存在.
*****
#### ***element::name()***
获取当前节点的名字.
*****
#### ***element::text()***
将当前节点转换为text文本格式
*****
#### ***element::html()***
取出当前节点内的html信息.
*****
## 5.6 http
### ***http.urlencode(String str)***
将字符串以URL编码，用于编码处理.
*****
### ***http.urldecode(String str)***
将字符串以URL解码，用于解码处理.
*****
### ***http.http_build_query(map data)***
用户把map对象转换成uri.
```
var data = {
    a:10,
    b:20
};
println(http.http_build_query(data));
结果:
a=10&b=20
```
*****
### ***class http::httprequest***
#### ***httprequest::setcookiefile(var cookisfilepath)***
设置cookie文件存放位置.
*****
#### ***httprequest::setproxy(var proxytype,String proxy)***
设置代理连接,proxy格式IP:PORT 例如127.0.0.1:12345
```
enum httpproxy
{
    HTTP = 0,
    HTTP1 = 1,
    HTTPS = 2,
    SOCKS4 = 4,
    SOCKS5 = 5,
    SOCKS4A = 6,
    SOCKS5_HOSTNAME = 7
}
```
*****
#### ***httprequest::setproxyaccount(var account,var pwd)***
设置代理账号密码,如果需要的话.
*****
#### ***httprequest::addheaders(map hearders)***
增加需要发送的协议头,格式为map.
```
//代码示例.
httpreq.addheaders({
    User-Agent: "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.25 Safari/537.36 Core/1.70.3776.400 QQBrowser/10.6.4212.400"
});
```
*****
#### ***httprequest::post(String url,String data)***
以POST形式发送一条http请求到url地址,并传递data内容.
*****
#### ***httprequest::get(String url)***
以GET形式发送一条http请求到url地址.
*****
#### ***httprequest::call(var reqeusttype,String url,String data)***
requesttype = 0 为Get方式
requesttype = 1 为Post方式 暂时未实现put delete方式,下个版本更新.
*****
#### ***httprequest::getcookies()***
获取服务器返回新增的cookie列表.
*****
#### ***httprequest::getheaders()***
获取服务器返回的HTTP协议头信息.
*****
#### ***httprequest::settimeout(var sec)***
设置超时时间(单位秒) 默认为0永不超时.
*****
#### ***httprequest::followlocation(var isfollow)***
设置是否跟随localtion跳转.如果为了得到中间层时则需要设置不跳转,根据header中的localtion信息手动跳转.默认状态为true跟随跳转.


*****

httprequest代码示例
```
#import "http"

var request = new http::httprequest();
request.setcookiesfile("cookies.txt");
//request.setproxy(httpproxy.SOCKS5,"192.168.1.166:25455");
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
### ***class http::downloader***
可以跟踪下载进度的下载器.
#### ***downloader::seturl(var url)***
设置要下载的url地址.
*****
#### ***downloader::setcookiefile(var cookiefile)***
设置cookie文件路径.
*****
#### ***downloader::setfilepath(var filepath)***
设置要文件存放地址,注意如果地址因权限或者目录不存在可能会导致下载失败.
*****
#### ***downloader::download(var progresscallbackfunc)***
progresscallbackfunc 格式为 var progresscallbackfunc(var filesize,var size,var filecurrsize); 
filesize 为文件总大小
size 为本次下载大小
filecurrsize 当前文件已下载大小.

调用次函数时才开始正式发起http请求开始下载文件.并根据网络消息回调progresscallbackfunc函数.
*****
### ***class http::uploader***
一个模拟HTTP表单模式上传器
#### ***uploader::addvalue(var name,var value)***
增加http表单参数.
*****
#### ***uploader::addfile(var name,var filepath,var fileminetype = "application/octet-stream")***
增加一个文件参数.
*****
#### ***uploader::setcookiefile(var cookiefile)***
设置cookie文件路径.
*****
#### ***uploader::upload(var url,var progresscallbackfunc)***
progresscallbackfunc 格式为 var progresscallbackfunc(var now,vcar total,var speed)
now 为当前已上传字节
total 为总上传字节
speed 为浮点类型 每秒与服务器通信的字节速度.
*****
## 5.7 json
### ***json.encode(var v,var encodeingtype = json.encode_utf8)***
将一个map或者vector序列化成JSON字符串. 默认字符为UTF8字符格式.
*****
### ***json.decode(var jsonv)***
将一个json字符串反序列化成map或者vector.
*****
### ***json.encode_ascii = 2***
*****
### ***json.encode_unicode = 3***
*****
### ***json.encode_utf8 = 1***
*****
## 5.8 logging
此日志模块提供了非常灵活的事件日志系统,使用标准库中的日志模块好处是所有的cpps模块都可能参与日志输出,包括你自己的日志消息和三方模块的日志消息.
### ***logging.create(var config) 或者 logging.create_with_config(var config)***
通过配置来创建日志系统. CPPS去掉了基础版日志 与 读取配置文件模式的方式创建日志.作者认为就一个接口是最好的.
```
//config 是一个map对象. 主要由version,disable_existing_loggers,formatters,filters,handlers,loggers几个参数组合而成.
//下面是一个示例
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
            color: [5,4,3,2,1] , // DEBUG,INFO,WARNING,ERROR,CRITICAL 颜色 1 红色2 绿色3 黄色4.蓝色5 紫色 6 青色
            formatter: 'simple'
        },
        fh: {
            level: 'DEBUG',
            class: 'logging.handlers.TimedRotatingFileHandler',  // 规则为:每5秒存一个文件. 可以根据自己需求修改.
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
```
*****
### ***logging.debug(String logstr)***
向默认日志对象输出DEBUG信息.
*****
### ***logging.info(String logstr)***
向默认日志对象输出INFO信息.
*****
### ***logging.warning(String logstr)***
向默认日志对象输出WARNING信息.
*****
### ***logging.error(String logstr)***
向默认日志对象输出ERROR信息.
*****
### ***logging.critical(String logstr)***
向默认日志对象输出CRITICAL信息.
*****
### ***logging.getlogger(String loggername)***
根据日志名字获取日志对象
*****
### ***logging.GBK = 1***
*****
### ***logging.UTF8 = 2***
*****
### ***logging.UNICODE = 3***
*****
### ***logging.DEBUG = 10***
*****
### ***logging.INFO = 20***
*****
### ***logging.WARNING = 30***
*****
### ***logging.ERROR = 40***
*****
### ***logging.CRITICAL = 50***
*****
### ***class logging::Logger***
#### ***Logger::addhandler(class<logging.Handler> handler)***
向日志对象中添加新的日志接口.
*****
#### ***Logger::removehandler(class<logging.Handler> handler)***
移除一个日志处理接口.
*****
#### ***Logger::debug(String logstr)***
向该日志对象输出DEBUG信息.
*****
#### ***Logger::info(String logstr)***
向该日志对象输出INFO信息.
*****
#### ***Logger::warning(String logstr)***
向该日志对象输出WARNING信息.
*****
#### ***Logger::error(String logstr)***
向该日志对象输出ERROR信息.
*****
#### ***Logger::critical(String logstr)***
向该日志对象输出CRITICAL信息.
*****
### ***class logging::Handler***
日志接口基类.并没有更多相关实现.
*****
### ***class logging::StreamHandler***
向控制台输出流信息的日志接口.
#### ***StreamHandler::setformatter(String fmt)***
添加输出日志的格式.
*****
#### ***StreamHandler::setcolor(vector colors)***
传入一个数组来修改每个等级中日志的颜色信息.
数组中
0位代表DEBUG
1位代表INFO
2位代表WARNING
3位代表ERROR
4位代表CRITICAL
颜色 1 红色2 绿色3 黄色4.蓝色5 紫色 6 青色
```
handler.setcolor([5,4,3,2,1]); //DEBUG 为紫色.
```
*****
#### ***StreamHandler::setlevel(var level)***
设置输出等级.
*****
### ***class logging::FileHandler***
向文件输出日志接口.它会在目标文件最后追加日志,如果没有管理日志它会无限增大文件大小.
#### ***FileHandler::setformatter(var fmt)***
添加输出日志的格式.
*****
#### ***FileHandler::setfile(var filepath)***
设置日志文件的路径.
*****
#### ***FileHandler::setdelay(var isdelay)***
暂时没有作用.
*****
#### ***FileHandler::setlevel(var level)***
设置输出等级.
*****
### ***class logging::RotatingFileHandler***
向文件输出日志接口.与FileHandler不同的是它有setmaxbytes接口.在大于设置阈值时系统将通过为原文件名添加扩展名 '.1', '.2' 等来保存旧日志文件。 例如，当 backupCount 为 5 而基本文件名为 app.log 时，你将得到 app.log, app.log.1, app.log.2 直至 app.log.5。 当前被写入的文件总是 app.log。 当此文件写满时，它会被关闭并重户名为 app.log.1，而如果文件 app.log.1, app.log.2 等存在，则它们会被分别重命名为 app.log.2, app.log.3 等等。因此你通常要设置 backupCount 至少为 1，而 maxBytes 不能为零。 否则会造成不能发生轮换.
#### ***RotatingFileHandler::setformatter(var fmt)***
添加输出日志的格式.
*****
#### ***RotatingFileHandler::setfile(var filepath)***
设置日志文件的路径.
*****
#### ***RotatingFileHandler::setdelay(var isdelay)***
暂时没有作用.
*****
#### ***RotatingFileHandler::setlevel(var level)***
设置输出等级.
*****
#### ***RotatingFileHandler::setmaxbytes(var maxbytes)***
单个文件最大字节数.
*****
#### ***RotatingFileHandler::setbackupcount(var backupcount)***
设置备份日志数量.
*****
### ***class logging::TimeRotatingFileHandler***
向文件输出日志接口.与FileHandler不同的是它有when接口.在when条件时系统将通过为原文件名添加扩展名 '.1', '.2' 等来保存旧日志文件。 例如，当 backupCount 为 5 而基本文件名为 app.log 时，你将得到 app.log, app.log.1, app.log.2 直至 app.log.5。 当前被写入的文件总是 app.log。 当此文件写满时，它会被关闭并重户名为 app.log.1，而如果文件 app.log.1, app.log.2 等存在，则它们会被分别重命名为 app.log.2, app.log.3 等等。因此你通常要设置 backupCount 至少为 1，而 when 条件不能为空。 否则会造成不能发生轮换.
#### ***TimeRotatingFileHandler::setformatter(var fmt)***
添加输出日志的格式.
*****
#### ***TimeRotatingFileHandler::setfile(var filepath)***
设置日志文件的路径.
*****
#### ***TimeRotatingFileHandler::setdelay(var isdelay)***
暂时没有作用.
*****
#### ***TimeRotatingFileHandler::setwhen(String when,var interval)***
设置when轮换条件.

| 值 | 间隔类型 | 如果/如何使用*when* |
| --- | --- | --- |
| `'S'` | 秒 | 忽略 |
| `'M'` | 分钟 | 忽略 |
| `'H'` | 小时 | 忽略 |
| `'D'` | 天 | 忽略 |
| `'W0'-'W6'` | 工作日(0=星期一) | 用于计算初始轮换时间 |
| `'midnight'` | 如果未指定*atTime*则在午夜执行轮换，否则将使用*atTime*。 | 用于计算初始轮换时间 |
*****
#### ***TimeRotatingFileHandler::setbackupcount(var backupcount)***
设置备份日志数量.
*****
#### ***TimeRotatingFileHandler::setlevel(var level)***
设置输出等级.
*****
## 5.9 mysql
### ***mysql.connect(class<MysqlOption> option)***
创建一个mysql连接. 如果连接成功则返回class<mysql::mysql> ,如果连接失败则返回nil.
下面可以看一下代码示例.
```
在bin/lib/mysql/main.cpp中有option的类定义
class MysqlOption
{
	var db;
	var host;
	var user;
	var passwd;
	var port;
	var isreconnect;
}

var conn = mysql.connect(new MysqlOption(){
    db = "testdb",
    host = "127.0.0.1",
    user = "root",
    passwd = "123456",
    port = 3306,
    isreconnect = true
});
if(conn == null) 
    println("mysql connected faild.");

```
*****
### ***class mysql::mysql***
此类是对libmysqlclient for c进行了封装.使用了mysql_stmt进行存储过程操作,所以不存在注入风险.可以放心使用.建议尽量或者不要使用execute语句执行.因为这是非常不安全的.
#### ***mysql::close()***
关闭mysql连接.
*****
#### ***mysql::call(String procname,vector params)***
调用存储过程,procname 为存储过程名称,vector对应参数列表.需要与存储过程中参数顺序一致.并不需要名称.
call会返回一个vector<class<mysql_result>> 的列表.mysql可以调用多个select语句并且返回多个结果集.每个结果集中包含多条record数据.
*****
#### ***mysql::execute(var sqlcmd)***
调用sql语句. sql语句存在注入风险,并不推荐使用.
*****
#### ***mysql::error()***
返回最后一次执行call或execute时mysql返回的错误码.
*****
#### ***mysql::isconnect()***
检测是否连接陈宫
*****
#### ***mysql::affected_rows()***
返回最后一次执行execute或call调用update 与 insert的影响行数.
*****
### ***class mysql::mysql_record***
#### ***mysql_record::fields()***
获取所有字段信息. 返回类型为map. 可以根据fields实例化对象,或者打印查看返回结果.
*****
#### ***mysql_record::get(var name)***
根据列名称获取本行的值.
*****
### ***class mysql::mysql_result***
#### ***mysql_result::records()***
获取当前result中所有的行数据.
*****
MYSQL模块代码示例
```

#import "mysql"


var option = new MysqlOption()
{
	db = "test",
	host = "127.0.0.1",
	user = "root",
	passwd = "123456",
	prot = 3306,
	isreconnect = true
}
var conn = mysql.connect(option);
if(conn == null)
{
	println("connect error");
}

var results = conn.call("selectuser",["johnson"])
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


var results = conn.execute("select * from users where username = 'johnson';")
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
## 5.10 quoteprintable
它是多用途互联网邮件扩展（MIME) 一种实现方式。其中MIME是一个互联网标准，它扩展了电子邮件标准，致力于使其能够支持非ASCII字符、二进制格式附件等多种格式的邮件消息。目前http协议中，很多采用MIME框架！quoted-printable 就是说用一些可打印常用字符，表示一个字节（8位）中所有非打印字符方法！
### ***quotedprintable.encode(String str)***
quotedprintable方式编码.
*****
### ***quotedprintable.decode(String str)***
quotedprintable方式解码.
*****
## 5.11 smtp
### ***stmp.makeboundary()***
系统自带生成boundary格式 源码在bin/lib/smtp/main.cpp中.
示例:
```
#import "smtp"

println(smtp.makeboundary());
结果:
__977_NextPart_44420295_=----
```
*****
### ***stmp.encodeutf8text(var str)***
smtp格式使用base64编码.
代码示例
```
#import "smtp"

println(smtp.encodeutf8text("helloworld"));
结果:
=?UTF-8?B?aGVsbG93b3JsZA==?=
```
*****
### ***stmp.sendmail(var opt)***
一个简单的发送邮件功能.
代码示例
```
#import "smtp"


//基础版本 够用
var code = smtp.sendmail({
			host:"smtp.cppscript.org",                     //必填
            port:25,                                    //必填
			username:"********@cppscript.org",            //必填
			pwd:"********",                            //必填
			content:"<a>hello</a>",                    //必填
            subject:"hello",                           //必填
			messageid:"{time.gettime()}@cppscript.org",      //按这个格式换下
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
// 			host:"smtp.cppscript.org",                     //必填
//             port:25,                                    //必填
// 			username:"********@cppscript.org",            //必填
// 			pwd:"********",                            //必填
//             ssl:false,
//             timeout:30,
// 			mailfrom:"********@cppscript.org",
// 			from:"********@cppscript.org",
// 			sender:"johnson",
// 			content:"<a>hello</a>",                    //必填
//             subject:"hello",                           //必填
//             verbose:true,    //是否输出调试信息
//             encoding:2,      // 0.8bit 1.base64 2.quoted-printable
// 			boundary:smtp.makeboundary(),
// 			messageid:"{time.gettime()}@cppscript.org",      //按这个格式换下
// 			charset:"UTF-8",
// 			tls:false,
// 			to:[
//                 ["88481106@qq.com","名字"],             //2种格式
//                 "14827@qq.com"
//             ],
// 			cc:[],  //抄送
// 			bcc:[], //密送
// 			headers:{
// 				GUID:"MakeGUID", //自定义headers
// 			},
// 			attachments:{
// 				aaa.txt:{
// 					type:"text/plain", //附件
// 					id:"content-id-test",
// 					content:"aaaa",
// 				},
//                 bbb.txt:{
// 					type:"application/x-msdownload",  //附件
// 					id:"content-id-test",
// 					content:"bbbb",
// 				}
// 			}
// 		});

```
*****
### ***class smtp::smtpclient***
当标准库提供的sendmail不能符合业务需求时可以通过smtpclient自定义发送邮件.
#### ***smtpclient::setsmtp(var host,var port)***
设置smtp地址与端口
*****
#### ***smtpclient::setmailfrom(var mailfrom)***
设置mail from信息
*****
#### ***smtpclient::setuserinfo(var username,var passwd)***
设置账号信息,如果需要的话.不设置则会使用直投方式.
*****
#### ***smtpclient::openssl(var isssl)***
是否开启ssl
*****
#### ***smtpclient::addrecipient(var rcptto)***
添加收件人地址
*****
#### ***smtpclient::setverbose(var verbose)***
是否开启调试信息.
*****
#### ***smtpclient::settimeout(var sec)***
设置邮件超时时间单位秒
*****
#### ***smtpclient::setreadcallback(var onreadfunc)***
设置服务器回信回调函数,回调函数原型 var onreadfunc(var msg);
*****
#### ***smtpclient::setcontent(var content)***
设置邮件详细内容.包含邮件头+内容+附件等信息.
*****
#### ***smtpclient::send()***
开始发送.
*****
## 5.12 socket
本扩展模块是对libevent的封装,Libevent 是一个用C语言编写的、轻量级的开源高性能事件通知库，主要有以下几个亮点：事件驱动（ event-driven），高性能;轻量级，专注于网络，不如 ACE 那么臃肿庞大；源代码相当精炼、易读；跨平台，支持 Windows、 Linux、 *BSD 和 Mac Os；支持多种 I/O 多路复用技术， epoll、 poll、 dev/poll、 select 和 kqueue 等；支持 I/O，定时器和信号等事件；注册事件优先级。本模块主要封装了Server,Client,HttpServer服务.
### ***class socket::server***
本类主要封装了socket中的server服务.作者特别在option里面增加自定义包头大小.和包头parser解析接口.使用它可以轻松简单的解决断包,黏包等问题.
#### ***server::setoption(class<ServerOption> option)***

```
ServerOption的定义在bin/lib/socket/main.cpp中.
class ServerOption
{
	var ip;             //绑定 IP.
	var accept;         //用户连接回调函数.
	var data;           //用户消息到达回调函数
	var close;          //用户关闭连接回调函数
	var parser;         //自定义协议头解析回调函数
	var headersize;     //自定义协议头大小
}
```
*****
#### ***server::listen(var port)***
开启监听服务.参数为监听端口号.
*****
#### ***server::run()***
事件驱动接口,需要用户调用run来驱动server的所有事件触发.
*****
#### ***server::send(var socketIndex,Buffer buf)***
发送给指定客户端,Buffer为数据流.为了更好的使用socket这里没有使用String.
*****
#### ***server::closesocket(var socketIndex)***
关闭一个客户端连接.
*****
#### ***server::stop()***
停止server服务.
*****
#### ***server::isrunning()***
返回server服务的运行状态.
*****

server服务的代码示例.
```
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

```
### ***class socket::client***
本类主要封装了socket中的client服务.作者特别在option里面增加自定义包头大小.和包头parser解析接口.使用它可以轻松简单的解决断包,黏包等问题.
#### ***client::setoption(class<ClientOption> option)***
```
ClientOption 定义在/lib/socket/main.cpp中.
class ClientOption
{
	var connected;          //客户端连接完成回调函数
	var data;               //服务端消息到达回调函数
	var close;              //客户端连接断开回调函数
	var parser;             //自定义协议头解析回调函数
	var headersize;         //自定义协议头大小
}
```
*****
#### ***client::run()***
事件驱动接口,需要用户调用run来驱动client的所有事件触发.
*****
#### ***client::send(Buffer buf)***
发送数据给服务端,Buffer为数据流.为了更好的使用socket这里没有使用String.
*****
#### ***client::closesocket()***
关闭与服务端的连接.
*****
#### ***client::connect(var ip,var port)***
开启一个对ip:port的连接.如果连接成功则返回true.失败则返回false. 由于是异步连接,真是连接状态应在connected回调函数中做详细判断.
*****
#### ***client::isconnect()***
返回连接状态.
*****
### ***class socket::httpserver***
httpserver 是一个非常强大的基于MVCT框架的Http服务,它有controller,Model,View,与Template.并且它有cookie,session,cache等网站系统应有的服务.您可以使用cpps订制自己需要的API接口甚至是一套网站.
本文档只是简单介绍一下接口,如果想了解更多httpserver中的使用方法,可以查看更详细的Httpserver使用.
#### ***httpserver::setoption(class<HttpServerOption> option)***
```
HttpServerOption 定义在/lib/socket/main.cpp中.
class HttpServerOption
{
	var ip;                 //绑定IP
	var exceptionfunc;      //当脚本运行总发生异常时的回调函数
	var notfoundfunc;       //当脚本找不到路由时,通常讲为404时的回调函数.
}
```
*****
#### ***httpserver::listen(var port)***
开启监听服务.参数为监听端口号.
*****
#### ***httpserver::add_type(var mime,var ext)***
添加支持的MimeType类型.支持的MimeType类型就可以跟IIS或者apache一样通过路径访问到.
mime为Mime-Type,ext为扩展名.
代码示例
```
add_type("text/html","html");
```
*****
#### ***socket.initmimetype(class<httpserver> srv)***
我们提供了一个基本上支持大部分mimetype的初始化函数.但是里面的缺点就是太全了.有一些并不想支持的也支持了.所以可以根据自己的需要从里面提取自己需要支持的mimetype.
*****
#### ***httpserver::get_type(var ext)***
根据扩展名获取已支持的mime-type.
*****
#### ***httpserver::run()***
事件驱动接口,需要用户调用run来驱动httpserver的所有事件触发.
*****
#### ***httpserver::register_handlefunc(var path,var func)***
重写路由中一个路径.
*****
#### ***httpserver::register_controller(var __class,var defaultset = false)***
注册controller,Controller会根据用户访问路径找到对应Controller 并创建一个对象访问其函数.这里有一点就是对函数名称的大小写是敏感的.所以在类中的函数名是大写则访问地址为大写,小写则为小写. 类名则不敏感.例如以下示例

```
//类名则对路径不敏感.
class Home
{
    //对应访问地址则是http://xxx/home/Index
    var Index(var request)
    {

    }
    //对应访问地址则是http://xxx/home/index
    var index(var request)
    {

    }
}


```
*****
#### ***httpserver::stop()***
停止httpserver服务.
*****
#### ***httpserver::isrunning()***
获取httpserver服务运行状态
*****
### ***class socket::httprequest***
httprequest是每次请求时对象的一个实体,可以通过它获取用户发送的协议头,参数,cookie以及session等信息.也是通过它返回信息给浏览器.
#### ***httprequest::addheader(map headers)***
添加返回协议头,header为map,分别通过kv方式添加.
*****
#### ***httprequest::append(String str)***
向返回数据流中添加数据.
*****
#### ***httprequest::send(var code,var msg)***
将数据流返回给浏览器.code 为返回码,正确为200,msg为OK,或者 302 跳转一类的.可以通过搜索引擎搜索HTTP状态码获取更多更详细的介绍..
*****
#### ***httprequest::getparam(var name)***
不区分get与post获取参数.
*****
#### ***httprequest::get(var name)***
只获取get部分的参数
*****
#### ***httprequest::post(var name)***
只获取post部分的参数
*****
#### ***httprequest::getheader(var name)***
获取浏览器发送来的协议头.
*****
#### ***httprequest::geturi()***
获取浏览器访问时的URI部分.
*****
#### ***httprequest::getpath()***
获取浏览器访问时的path部分.
*****
#### ***httprequest::getbuffer()***
当用户以POST形式访问时,获取post的整体数据部分.
*****
#### ***httprequest::setcookie(var key,var value, var path = "/",var domain = "",var max_age = nil)***
添加cookie,并返回给浏览器Set-Cookie
*****
#### ***httprequest::getcookie(var key)***
获取浏览器返回的cookie参数.
*****
#### ***httprequest::getfiledata(var name)***
当浏览器以表单形式上传文件,则需要使用getfiledata获取文件流.
*****
#### ***httprequest::paramslist()***
获取所有参数列表包含GET与POST.
*****
#### ***httprequest::getlist()***
获取GET方式中的所有参数.
*****
#### ***httprequest::postlist()***
获取POST方式中的所有参数.
*****
#### ***httprequest::session()***
获取SESSION对象
*****
### ***class socket::filedata***
#### ***filedata::name()***
返回参数名称
*****
#### ***filedata::filename()***
返回文件名称
*****
#### ***filedata::content_type()***
返回Mime-Type类型.
*****
#### ***filedata::getbuffer()***
返回文件数据流,String类型.
*****
### ***class socket::httpsession***
SESSION暂时只实现了一种内存型缓存机制.当程序被关闭则SESSION会全部失效.后续会开发db或者file版本的SESSION.以保证SESSION可以持久化.
#### ***httpsession::get(var key,var defaultval = "")***
获取session中的值.当不存在会添加并设置defaultval值.
*****
#### ***httpsession::set(var key,var value)***
设置session中的值
*****
#### ***httpsession::clear()***
清空session中的所有值.
*****
#### ***httpsession::remove()***
移除session.下次访问的时候会创建新的session对象.
*****
## 5.13 uuid 
在WINDOWS中称之为GUID,在UNIX系统中称之UUID.
### ***class uuid::UUID***
UUID对象.
#### ***UUID::tostring()***
将UUID对象转为字符串
*****
### ***uuid.create()***
创建一个UUID对象.
*****
### ***uuid.uuid4()***
创建一个UUID对象并直接转为字符串输出.(最基础的UUID生成.)
*****
# 6.如何嵌入CPPS到自己程序
嵌入cpps非常简单.只需要包含一个头文件#include <cpps/cpps.h>即可.
下面代码示例 包括嵌入cpps,并且一些cpps与c++调用的示例.
```
#include <iostream>
#include <cpps/cpps.h>

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
	if (chdir(cpps_io_getfilepath(path).c_str())) {}
#endif


	C* c = cpps::create(argc,argv);

	cpps::_module(c)[
		_class<CppClassTest>("CppClassTest") //注册CPP类给cpps中.
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
		cpps::object Agentvar = cpps::object::create_with_cppsclassvar(c, Agent);
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
	cpps::object cppclassvar = cpps::object::create_with_classvar< CppClassTest >(c, &ptr);
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

# 7.如何编写模块给CPPS
编写模块给cpps非常简单.只需要实现cpps_attach与cpps_detach接口就可以了.
源码在example\demo中.
```
#include <cpps/cpps.h> //Single Header

using namespace cpps;

void demotest()
{
	printf("hit demo function.\r\n");
}
cpps_export_void cpps_attach(cpps::C* c)
{
	printf("attach demo\r\n");

	cpps::cpps_init_cpps_class(c); //init cpps module

	cpps::_module(c)[
		def("demotest", demotest)
	];
}
cpps_export_void  cpps_detach(cpps::C* c)
{
	printf("detach demo\r\n");
}

cpps_export_finish	//unix export.
```