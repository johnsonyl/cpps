class program
{
	var nextsavefiletime = now() + 300;//5分钟存储一次.
    var Main()
	{
		//创建日志
		initblogmanager();
		initlogging();
		initconfig();
		initwebserver();
		register_controllers();
		
		while(true){
		
			Run();
			Sleep(1);
			
		}
	}
	var LogicHandle()
	{
		if(now() > nextsavefiletime){
			blogmanager.save();
			saveconfig();
			nextsavefiletime = now() + 300;
		}
	}
	var Run(){
		LogicHandle();
		srv.run();
	}
}
