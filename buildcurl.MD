### libcurl编译-windows-x64 

文章时间：2020-11-16

网上找了一堆资料，五花八门，几个文章弄下来才编译过。这里做一个记录，也顺带让想编译curl库的朋友，做个借鉴。

首先编译curl需要 libcurl,openssl,zlib

我放的目录是 cpps/deps下。
![](images/screenshot_1605508433057.png)
自己可以调整自己所需要的目录。
libcurl下载地址：https://curl.se/download.html
openssl下载地址：https://www.openssl.org/source/
zlib下载地址：http://www.zlib.net/

需要安装的软件都知道 perl,python, NASM 其他什么我就不复数，自行百度吧。
![](images/screenshot_1605508622342.png)
我使用的版本号都是目前最新的。
\
\
首先打开VS命令工具 我这里只编译64位。所以选择X64
![](images/screenshot_1605508717889.png)

第一步编译 zlib
-
进入到zlib 目录
\
输入
```
nmake -f win32/Makefile.msc AS=ml64 LOC="-DASMV -DASMINF -I." OBJA="inffasx64.obj gvmat64.obj inffas8664.obj"
```
\
![](images/screenshot_1605508840585.png)
编译好后，进入下一步。
\
\
第二步 编译openssl
-
同样进入 openssl目录
![](images/screenshot_1605508985189.png)
输入
```
perl Configure VC-WIN64A --prefix=D:\work\code\apiserver\cpps\deps\openssl-1.1.1h\build no-asm
nmake
nmake install
```
最后可能会有错误，提示找不到什么文件，无所谓忽视他。
确定文件夹里面多了一个 build文件夹就可以了 （--prefix= 地址\build）我用的地址+build文件夹 所以是build 换其他文件夹就是其他文件夹名
编译好，下一步。
\
\
第三步 编译libcurl
-
编译好 zlib 与 openssl 后，这里需要把头文件和lib文件分别拷贝到相应目录
openssl目录下：build的include与lib 拷贝到 deps/deps目录里 （需要手动建立）
![](images/screenshot_1605509489466.png)
zlib目录下：zconf.h , zlib.h , zutil.h 拷贝到 deps/deps/include里面
                    zlib.lib zdll.lib 拷贝到 deps/deps/lib 里
![](images/screenshot_1605509586207.png)
\
开始编译
-
同样进入 curl目录 在进入 winbuild

![](images/screenshot_1605509215529.png)

输入
```
cd winbuild
nmake /f Makefile.vc mode=dll MACHINE=x64 VC=15 ENABLE_IPV6=yes WITH_SSL=dll WITH_ZLIB=dll DEBUG=no
```
编译完成后
![](images/screenshot_1605509725959.png)
有个builds文件夹 
进入 libcurl-vc15-x64-release-dll-ssl-dll-zlib-dll-ipv6-sspi
分别把include 与 lib拷贝到 deps/deps里面
\
**使用libcurl记得添加宏定义：BUILDING\_LIBCURL;HTTP\_ONLY**
(curl我已经添加好了)
\
至此编译结束。 就可以开始编译 demo/curl 啦。


尾语： 我觉得编译就是干货，网上的文章乱七八糟的。 最后说下 dll 部分 openssl 和 libcurl 都生成了 bin目录 自己拷贝下dll就好了。zlib在根目录有个zlib1.dll

![](images/screenshot_1605510131235.png)
****

