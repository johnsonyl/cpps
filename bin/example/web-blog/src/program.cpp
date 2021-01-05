class program
{
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

	}
	var Run(){
		LogicHandle();
		srv.run();
	}
}
