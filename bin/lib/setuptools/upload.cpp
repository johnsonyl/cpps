var upload(var option){
    println("-- Uploading file...");
    
    var cl = ["/","-","\\","|"];
    var progress_max = 70;
    var kg = "                                                                       ";
	var dy = "=======================================================================";
	var filename = "{option["name"]}-{option["username"]}-{option["version"]}-{option["platfrom"]}";
    var uploader = new http::uploader();
    uploader.addfile("config","dist/{filename}.json");
    uploader.addfile("filezip","dist/{filename}.tar.gz");
    var ret = uploader.upload("http://192.168.31.124:8080/project/loadupdate",[](var now,var total,var speed){
	        var x = total / progress_max;
		    var cur = toint(now / x);
            var n = toint(now / total * 100 );
            if(n < 99) print("{cl[(cur)%4]} ");

            print("[");
            if(cur > 0)
			    print(dy[0:cur]);
            print(">");
            if(cur < progress_max)
		    	print(kg[cur:-1]);
            print("]");
            print_color(" {n}%/100% {speed/1024} kb/sec",2);
            print("\r");
    });
    if(ret == "0"){
        println_color("-- Upload success.",2);
    }
    else{
        println_color("-- Upload faild.errcode:{ret}",1);
    }

}