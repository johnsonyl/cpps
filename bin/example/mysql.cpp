system("chcp 65001"); //use utf8 string

#import "mysql"


var option = new MysqlOption()
{
	db = "mir2",
	host = "192.168.1.166",
	user = "root",
	passwd = "zheng008",
	prot = 3306,
	isreconnect = true
}
var conn = mysql.connect(option);
if(conn == null)
{
	println("connect error");
}

var results = conn.call("selectuser",["zheng"])
println(conn.error());
foreach(var result : results)
{
	foreach(var record : result.records())
	{
		println("----------------------------------------");
		foreach(var field: record.fields())
		{
			println("{field.first()}:{field.second()}");
		}
	}
}


var results = conn.execute("select * from users where username = 'jiang2';")
println(conn.error());
foreach(var result : results)
{
	foreach(var record : result.records())
	{
		println("id:"..record.get("id"));
		println("username:"..record.get("username"));
		println("userpwd:"..record.get("userpwd"));
	}
}

