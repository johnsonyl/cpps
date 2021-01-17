
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


