module http
{
    var http_build_query(var data)
	{
		var params = "";
		var pi = 0;
		foreach(var it: data) 
        {
			if (pi != 0)
			{
				params ..= "&";
			}
			pi++;
			params ..= it.first().."="..urlencode(it.second());
		};
		return params;
	}
	var get(var url)
	{
		var request = new http::httprequest();
		return request.get(url);
	}
	var post(var url,var data)
	{
		var request = new http::httprequest();
		return request.post(url,data);
	}
}

