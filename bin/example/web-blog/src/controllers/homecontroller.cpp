class Home : Controller
{
	Home(){

	}

	var index(var request)
	{
		var page = request.get("page");
		if(isnull(page) || page == "") page = 1;
		
		var [prepageid,nextpageid,list] = blogmanager.getbloglist(int(page));

		render(request,"view/index.html",{prepageid:prepageid,nextpageid:nextpageid,bloglist:list});
	}

	var post(var request)
	{
		var id = request.get("id");
		if(isnull(id) || id == "") {
			sendmsg(request,"not found id param.");
			return;
		}
		var blog = blogmanager.getblog(id);
		if(blog == null){
			sendmsg(request,"not found blogpage.");
			return;
		}
		blog.readcount++;
		render(request,"view/post.html",{blog:blog});
	}
	
	var about(var request)
	{
		render(request,"view/about.html",{});
	}
	var quit(var request)
	{
		sendmsg(request,"ok.");
		programhandle.isstop = true;
	}
}