class program
{
	var nextsavefiletime = now() + 300;//5分钟存储一次.
	var isstop = false;
    var Main()
	{
		//创建日志
		initblogmanager();
		initlogging();
		initconfig();
		initwebserver();
		register_controllers();
		
		while(!isstop){
		
			Run();
			Sleep(1);
			
		}
		srv.stop();
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
