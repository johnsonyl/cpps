
class Controller
{
 	Controller()
 	{

 		
 	}
	var send(var request,var msg)
	{
		request.addheader({
			Server:"cpps Server",
			Content-Type:"application/json; charset=UTF-8",
			Connection:"close"
		});
		request.append(json.encode(msg));
		request.send(200,"OK");
	}
	var forbidden(var request,var msg)
	{
		request.addheader({
			Server:"cpps Server",
			Content-Type:"application/html; charset=UTF-8",
			Connection:"close"
		});
		request.append(msg);
		request.send(403,"forbidden");
	}
	var sendmsg(var request,var msg)
	{
		request.addheader({
			Server:"cpps Server",
			Content-Type:"text/html; charset=UTF-8",
			Connection:"close"
		});
		request.append(msg);
		request.send(200,"OK");
	}
	var redirect(var request,var url)
	{
		request.addheader({
			Server:"cpps Server",
			Content-Type:"text/html; charset=UTF-8",
			Location:url,
			Connection:"close",
			Access-Control-Allow-Origin:"*"
		});
		request.send(302,"redirect");
	}
 
}