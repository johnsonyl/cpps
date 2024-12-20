#ifndef cpps_http_request_h__
#define cpps_http_request_h__
#include <cpps/cpps.h>
#include <curl/curl.h>
#include <unordered_map>

namespace cpps
{
	typedef phmap::flat_hash_map<std::string, std::string> HEADERSLIST;
	class cpps_http_request
	{
	public:
		cpps_http_request();
		virtual ~cpps_http_request();
	public:
		void				setcookiefile(std::string cf);
		void				setproxy(cpps_integer t, std::string pv);
		void				setproxyaccount(std::string username, std::string pass);
		void				addheaders(cpps::object h);
		std::string			get(C*c,std::string url);
		std::string			post(C* c, std::string url, std::string data);
		std::string			call(C* c, cpps_integer type, std::string url, std::string data);
		std::string			getcookies();
		std::string			getheaders();
		void				settimeout(cpps_integer sec);
		void				followlocation(bool b);
		void				setHeaderFunc(cpps::object _func);
		void				setBodyFunc(cpps::object _func);
	public:
		void				append(const char *data,size_t datasize);
	public:
		C*					m_c;
		std::string			cookiesfile;
		cpps_integer		proxytype;
		std::string			proxyvalue;
		std::string			proxyusername;
		std::string			proxypassword;
		HEADERSLIST			headerslist;
		cpps_integer		timeoutsec;
		bool				isfollowlocation;
		std::string			retpage;
		std::string			retheaders;
		std::string			retcookies;
		cpps::object		_headerfunc;
		cpps::object		_bodyfunc;
		cpps_integer		filesize;
	};
}


#endif // cpps_http_request_h__
