#import "http"

var func()
{
	return { "content": http.get("http://www.baidu.com").substr(0,10),
	"other":"haha"};
}

async var test(){
	
	while(true){
		println(1);
		await asyncio.sleep(1000);
	}
	return "done";
}

async var main() {
	try{
		test().then([](var ret)=>println(ret););
		var ret = await asyncio.call(func);
		println(ret);
	}
	catch(var e){
		println(e);
	}
	
	while(true){
		println(2);
		await asyncio.sleep(1000);
	}
}

asyncio.run(main());