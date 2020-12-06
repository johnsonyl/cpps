system("chcp 65001"); //use utf8 string

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


