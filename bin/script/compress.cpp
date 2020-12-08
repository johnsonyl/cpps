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

