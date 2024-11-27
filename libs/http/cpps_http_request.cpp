#include "cpps_http_request.h"
namespace cpps
{

	cpps_http_request::cpps_http_request()
	{
		proxytype = -1; //不代理
		timeoutsec = 10; //不超时
		isfollowlocation = true;
	}

	cpps_http_request::~cpps_http_request()
	{

	}


	void cpps_http_request::setcookiefile(std::string cf)
	{
		cookiesfile = cf;
	}

	void cpps_http_request::setproxy(cpps_integer t, std::string pv)
	{
		proxytype = t;
		proxyvalue = pv;
	}

	void cpps_http_request::setproxyaccount(std::string username, std::string pass)
	{
		proxyusername = username;
		proxypassword = pass;
	}

	void cpps_http_request::addheaders(cpps::object h)
	{
		cpps_map* m = cpps_to_cpps_map(h.value);
		for (auto v : m->realmap())
		{
			headerslist.insert(HEADERSLIST::value_type(cpps_to_string(v.first), cpps_to_string( v.second)));
		}
	}
	size_t http_get_size_struct(void* ptr, size_t size, size_t nmemb, void* data) {
		return (size_t)(size * nmemb);
	}

	cpps_integer http_get_download_size(std::string url, std::string cookiesfile) {
		CURL* curl;
		CURLcode res;
		double size = 0.0;

		curl = curl_easy_init();
		if (url.find("https") == 0)
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		}

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_get_size_struct);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		if (!cookiesfile.empty())
		{
			curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiesfile.c_str()); // 指定cookie文件
			curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiesfile.c_str()); // 指定cookie文件
		}

		res = curl_easy_perform(curl);
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);

		return static_cast<cpps_integer>(size);
	}
	std::string cpps_http_request::get(C* c, std::string url)
	{
		return call(c,0, url, "");
	}

	std::string cpps_http_request::post(C*c,std::string url, std::string data)
	{
		return call(c,1, url, data);
	}
	size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		cpps_http_request* request = (cpps_http_request*)stream;
		request->append((const char*)ptr, size * nmemb);
		if (request->_bodyfunc.isfunction()) {
			std::string _s((const char*)ptr, size * nmemb);
			cpps::dofunction(request->m_c, request->_bodyfunc, _s);
		}

		return size * nmemb;
	}
	size_t header_callback(const char* ptr, size_t size, size_t nmemb, void*  stream)
	{
		cpps_http_request* request = (cpps_http_request*)stream;
		size_t len = size * nmemb;
		request->retheaders.append(ptr, len);
		if (request->retheaders.find("\r\n\r\n") != std::string::npos) {
			if (request->_headerfunc.isfunction()) {
				cpps::dofunction(request->m_c, request->_headerfunc, request->retheaders);
			}
		}
		return len;
	}
	std::string cpps_http_request::call(C* c, cpps_integer type,std::string url, std::string data)
	{
		m_c = c;
		retpage.clear();
		retcookies.clear();
		retheaders.clear();
		CURL* curl = curl_easy_init();
		if (!curl) return "";

		//filesize = http_get_download_size(url, cookiesfile);
		if (url.find("https") == 0)
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			if (proxytype != -1)
			{
				curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1L);
			}
		}

		struct curl_slist* headers = NULL;
		//headers
		for (auto header : headerslist)
		{
			std::string h = header.first + ": " + header.second;
			headers = curl_slist_append(headers, h.c_str());
		}
		headers = curl_slist_append(headers, "Cache-Control: no-cache");
		headers = curl_slist_append(headers, "Pragma: no-cache");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//关闭调试信息

		if (proxytype != -1) {
			curl_easy_setopt(curl, CURLOPT_PROXY, proxyvalue.c_str());// 代理
			curl_easy_setopt(curl, CURLOPT_PROXYTYPE, proxytype);
			if (!proxyusername.empty())
			{
				curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, proxyusername.c_str());// 代理
			}
			if (!proxypassword.empty())
			{
				curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, proxypassword.c_str());// 代理
			}

		}

		if (type == 1)
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());    // 指定post内容
		}

		if (!cookiesfile.empty())
		{
			curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiesfile.c_str()); // 指定cookie文件
			curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiesfile.c_str()); // 指定cookie文件
		}
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");// 改协议头
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //将返回的http头输出到fp指向的文件
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, isfollowlocation);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutsec*1000);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeoutsec*500);
		//curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

		/*CURLcode res = */curl_easy_perform(curl);   // 执行

		struct curl_slist* cookies = NULL;
		curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);		//获得cookie数据

		int i = 1;
		while (cookies) {
			retcookies += cookies->data;
			retcookies += "\r\n";
			cookies = cookies->next;
			i++;
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		return retpage;
	}

	std::string cpps_http_request::getcookies()
	{
		return retcookies;
	}

	std::string cpps_http_request::getheaders()
	{
		return retheaders;
	}

	void cpps_http_request::settimeout(cpps_integer sec)
	{
		timeoutsec = sec;
	}

	void cpps_http_request::followlocation(bool b)
	{
		isfollowlocation = b;
	}

	void cpps_http_request::setHeaderFunc(cpps::object _func)
	{
		_headerfunc = _func;
	}

	void cpps_http_request::setBodyFunc(cpps::object _func)
	{
		_bodyfunc = _func;
	}

	void cpps_http_request::append(const char* data, size_t datasize)
	{
		retpage.append(data, datasize);
	}

}