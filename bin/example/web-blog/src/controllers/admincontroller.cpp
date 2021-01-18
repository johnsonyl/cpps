class Admin : Controller
{
	Admin(){

	}

	var index(var request)
	{
		
		var key = request.get("key");
		if(key != globalconfig.admin_key){
			var loginstate = request.session().get("loginstate","");
			if(loginstate != globalconfig.admin_key){
				forbidden(request,"forbidden! you are not administrator.");
				return;
			}
		}
		request.createsession();
		request.session().set("loginstate",globalconfig.admin_key);
		var page = request.get("page");
		if(isnull(page) || page == "") page = 1;
		
		var [prepageid,nextpageid,list] = blogmanager.getbloglist(int(page));

		render(request,"view/admin/index.html",{prepageid:prepageid,nextpageid:nextpageid,bloglist:list});
	}

	var post(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
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
		render(request,"view/admin/post.html",{blog:blog});
	}
	var remove(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
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
		blogmanager.remove(id);
		blogmanager.save();
		redirect(request,"/admin");
	}
	var add(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
		render(request,"view/admin/add.html",{});
	}
	var add2(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
		var easycontent = request.post("easycontent");
		var markdown = request.post("markdown");
		var title = request.post("title");
		var image = request.post("image");

		var blog = new models::blog();
		blog.id = hashlib.sha1encode(time.gettime());
		blog.content = "/data/md/{blog.id}.md";
		blog.easycontent = http.urldecode(easycontent).substr(0,301);
		var mdpath = "{io.getcwd()}{blog.content}";
		io.writefile(mdpath,http.urldecode(markdown))
		blog.title = http.urldecode(title);
		blog.image = http.urldecode(image);
		blog.readcount = 0;
		blog.createdate = time.gettime();

		blogmanager.bloglist.push_back(blog);
		blogmanager.bloglist_id.insert(blog.id,blog);
		blogmanager.save();

		sendmsg(request,blog.id);
	}
	var save(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
		var id = request.post("id");
		var easycontent = request.post("easycontent");
		var markdown = request.post("markdown");
		var title = request.post("title");
		var image = request.post("image");

		
		var blog = blogmanager.getblog(id);
		if(blog == null){
			sendmsg(request,"1");
			return;
		}

		blog.easycontent = http.urldecode(easycontent).substr(0,301);
		var mdpath = "{io.getcwd()}{blog.content}";
		io.writefile(mdpath,http.urldecode(markdown))
		blog.title = http.urldecode(title);
		blog.image = http.urldecode(image);
		blogmanager.save();

		sendmsg(request,"0");
	}
	var upload(var request)
	{
		var loginstate = request.session() ? request.session().get("loginstate","") : "";
		if(loginstate != globalconfig.admin_key){
			forbidden(request,"forbidden! you are not administrator.");
			return;
		}
		var filedata = request.getfiledata("editormd-image-file");
		if(filedata == null){
			var ret = {
				success : 0,
				message: "上传失败",
				url : ""
			}
			send(request,ret);
			return;
		}
		var ext = io.getfileext(filedata.filename());
		var supportext = ["jpg", "jpeg", "gif", "png", "bmp", "webp"];
		if(!supportext.has(ext)){
			var ret = {
				success : 2,
				message: "不支持此扩展类型",
				url : ""
			}
			send(request,ret);
			return;
		}
		var imagepath = hashlib::sha1encode(time.gettime());
		var filepath = "/upload/{imagepath}.{ext}";
		io.writefile("{io.getcwd()}{filepath}",filedata.getbuffer());
		var ret = {
			success : 1,
			message: "上传成功",
			url : filepath
		};
		send(request,ret);
	}
}
