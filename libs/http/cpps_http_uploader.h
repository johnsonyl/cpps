#ifndef cpps_cpps_http_uploader_h__
#define cpps_cpps_http_uploader_h__

#include <cpps/cpps.h>
#include <curl/curl.h>
static const char buf[] = "Expect:";

namespace cpps{

	class cpps_http_uploader
	{
	public:
		cpps_http_uploader();
		virtual ~cpps_http_uploader();
		
	public:
		void					addvalue(std::string name, std::string value);
		void					addfile(std::string name, std::string file, object type /*= text/plain */);
		std::string				upload(C* cstate, std::string url,cpps::object callbackfunc);
		void					setcookiefile(std::string cookie);
		void					uploadprogress(cpps_integer now, cpps_integer total, cpps_number speed);
		void					append(const char* data, size_t nmemb);
	public:
		struct					curl_httppost* formpost;
		struct					curl_httppost* lastptr;
		std::string				cookiesfile;
		struct curl_slist*		headerlist;
		std::string				retpage;
		cpps::object			callbackfunc;
		size_t					totalfilesize;
		C*						c;
		CURL* curl;

	};
	
}
#endif //cpps_cpps_http_uploader_h__