
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
 
}