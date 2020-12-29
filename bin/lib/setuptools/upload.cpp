var upload(var option){
    println("---------------------------------------");
    println("uploading file...");
	println("");
	var filename = "{option["name"]}-{option["username"]}-{option["version"]}-{option["platfrom"]}";
    var uploader = new http::uploader();
    uploader.addfile("config","dist/{filename}.json");
    uploader.addfile("filezip","dist/{filename}.tar.gz");
    var ret = uploader.upload("http://192.168.31.124:8080/project/loadupdate",[](var now,var total,var speed){
     
        
	        var cl = ["/","-","\\","|"];
			var progress_max = 70;
	        var x = total / progress_max;
		    var cur = toint(now / x);
            var n = toint(now / total * 100 );
            if(n < 99) print("{cl[(cur)%4]} ");

            print("[");
            foreach(var j : xrange(1,cur)){
				print("=");	
			}
            print(">");
            foreach(var j: xrange(cur,progress_max)){
				print(" ");
			}
            print("] {n}%/100% {speed/1024} kb/sec");
            print("\r");
    });
	println("");
	println("");
    if(ret == "0"){
        println("upload success.");
    }
    else{
        println("upload faild.errcode:{ret}");
    }

}