class program
{
    var Main()
	{
		//创建日志
		initlogging();
		initconfig();
		initwebserver();
		register_controllers();
		
		while(true){
			try{

				Run();
				Sleep(1);
			}
			catch(var e)
			{
				logger.error("{e.geterrstr()} line:{e.getline()} file:{e.getfile()}");
				logger.error(e.callstack());
			}
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
