var srv = null;
var initwebserver()
{
	var http_option = new HttpServerOption()
	{
		ip = globalconfig.httpserver["ip"],
		exceptionfunc = [](var request,var errmsg)
		{
			println(errmsg);
			request.addheader({
				Server:"John Server",
				"Content-Type":"text/plain; charset=UTF-8",
				Connection:"close"
			});
			request.append(errmsg);
			request.send(500,"SERVER ERROR");
		},
		notfoundfunc = [](var request)
		{
			request.addheader({
				Server:"John Server",
				"Content-Type":"text/plain; charset=UTF-8",
				Connection:"close"
			});
			request.append("there's nothing here.");
			request.send(404,"Not Found");
		}
	};
	srv = new socket::httpserver();
	srv.setoption(http_option);

	socket.initmimetype(srv);

	var port = globalconfig.httpserver["port"];
		//开启服务
	var ret = srv.listen(port); //配置端口
	if(!ret)
	{
		logger.error("httpserver start faild.");
		exit(0); //退出
	}
	logger.info("httpserver listen for {port} success.");
}

var register_controllers()
{	

	//注册contorlloers Home default
	srv.register_controller(Home,true);
	srv.register_controller(Admin);
}
