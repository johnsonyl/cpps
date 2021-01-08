system("chcp 65001"); //use utf8 string

#import "http"

var request = new http::httprequest();
request.setcookiesfile("cookies.txt");
//request.setproxy(httpproxy.SOCK5,"192.168.1.166:25455");
//request.setproxyaccount("johnsonyl","mima");
request.addheaders({User-Agent:"cppsrequest/1.1.0"});
var ret = request.get("http://127.0.0.1:8080/Home/test?kkk=100");


println("-------------------GET-------------------------");
println("get:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");


println("-------------------POST-------------------------");
ret = request.post("http://127.0.0.1:8080/Home/test","kkk=100");
println("post:{ret}");
var cookies = request.getcookies();
println("cookies:{cookies}");

var headers = request.getheaders();
println("headers:{headers}");
println("-------------------END-------------------------");
