system("chcp 65001"); //use utf8 string

#import "socket"

class Home
{
	var name;
	Home()
	{
		name = "I'm John ,32 years old."
	}
	//http://127.0.0.1:8080 or http://127.0.0.1:8080/Home or http://127.0.0.1:8080/Home/index
	var index(var request)
	{
		println("Home/Index..");
		println(name);
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(name.."\n");

		request.send(200,"OK");
	}
}

class Student
{
	var name;
	var age;
	Student()
	{
		name = "xiaoming";
		age = 18;
	}
	//http://127.0.0.1:8080/Student/ or http://127.0.0.1:8080/Student/Index
	var index(var request)
	{
		
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append("i'm {name}, {age} years old.");

		request.send(200,"OK");
	}
	//http://127.0.0.1:8080/Student/Name
	var name(var request)
	{
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(name);

		request.send(200,"OK");
	}
	//http://127.0.0.1:8080/Student/Age
	var age(var request)
	{
		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(age);

		request.send(200,"OK");
	}
}
//http://127.0.0.1:8080/Home/test?kkk=100
var test(var request)
{
	request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

	request.append("test page.\n");
	request.append("path:"..request.getpath().."\n");
	request.append("uri:"..request.geturi().."\n");
	request.append("kkk:"..request.getparam("kkk").."\n");
	request.append("User-Agent header:"..request.getheader("User-Agent").."\n");
	request.append("getbuffer():"..request.getbuffer().."\n");

	request.send(200,"OK");
}

var option = new HttpServerOption()
{
	exceptionfunc = [](var request,var errmsg)
	{
		println(errmsg);

		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append(errmsg);

		request.send(500,"SERVER ERROR");
	},
	notfoundfunc = [](var request)
	{

		request.addheader({
			Server:"John Server",
			Content-Type:"text/plain; charset=UTF-8",
			Connection:"close"
		});

		request.append("there's nothing here.");

		request.send(404,"Not Found");
	}
}

var http = new socket::httpserver().setoption(option).listen(8080);


http.register_controller(Home,true);
http.register_controller(Student);
http.register_handlefunc("/Home/test",test);

while (true){
	http.run();
}
