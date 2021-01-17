var upload(var option,var apikey){
    println("-- Uploading file...");
    
    var cl = ["/","-","\\","|"];
    var progress_max = 70;
    var kg = "                                                                       ";
	var dy = "=======================================================================";
	var filename = "{option["name"]}-{option["username"]}-{option["version"]}-{option["platfrom"]}";
    var uploader = new http::uploader();
    uploader.addfile("config","dist/{filename}.json");
    uploader.addfile("filezip","dist/{filename}.tar.gz");
    var ret = uploader.upload("http://c.cppscript.org:88/project/loadupdate?apikey={apikey}",[](var now,var total,var speed){
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
    println("");

    if(ret == "0"){
        println_color("-- Upload success.",2);
    }
    else{
        println_color("-- Upload faild.errcode:{ret}",1);
    }

}

var upload_check()
{
    println_color("-- Need login to cppscript.org.",COLOR_GREEN);
    print("username: ");
    var username = io.getline();
    if(username.empty()){
        println_color("UserName is empty.",COLOR_RED);
        exit(0);
    }
    var pwd = "";
    io.getpwd("password: ",pwd,"#");

    if(username.empty()){
        println_color("Password is empty.",COLOR_RED);
        exit(0);
    }
    var ret = http::post("http://c.cppscript.org:88/user/token","name={username}&pwd={pwd}");
    ret = json.decode(ret);
    if(!ismap(ret) || !ismap(ret["msg"])){
        println_color("Ops,Cpps server is crash.",COLOR_RED);
        exit(0);
    }
    if(ret["msg"]["code"] != 0){
        if(ret["msg"]["code"] == -1){
            println_color("Ops,Request exception.",COLOR_RED);
        }else if(ret["msg"]["code"] == -2){
            println_color("Ops,Request exception.",COLOR_RED);
        }else if(ret["msg"]["code"] == -3){
            println_color("The username not exist or wrong password",COLOR_RED);
        }else if(ret["msg"]["code"] == -4){
            println_color("The username not exist or wrong password",COLOR_RED);
        }
        exit(0);
    }
    println_color("-- Login suceess.",COLOR_GREEN);

    return ret["msg"]["token"];
}