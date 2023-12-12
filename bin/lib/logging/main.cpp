#import "http"

class HTTPHandler
{
	var _url;
	//virsual
	var create(var config){
		println(config);
	}
	//virsual
	var pop(var level,var msg){
		println(level,msg);
		//http.post(_url,"level={level}&text={http.urlencode(msg)}";
	}
	var set_url(var url){
		_url = url;
	}
}