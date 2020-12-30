#ifndef cpps_cpps_http_downloader_h__
#define cpps_cpps_http_downloader_h__

#include <cpps/cpps.h>
#include <curl/curl.h>

namespace cpps{

	class cpps_http_downloader
	{
	public:
		cpps_http_downloader();
		virtual ~cpps_http_downloader();
		
	public:
		bool			seturl(std::string url);
		void			setfilepath(std::string filepath);
		void			setcookiefile(std::string cookie);

		bool			download(cpps::C* c, cpps::object func);
		void			close();
		void			append(const char* page, size_t size);

	public:
		FILE* file;
		std::string		httpurl;
		std::string		filepath;
		std::string		cookiesfile;
		cpps_integer	fileSize;
		cpps_integer	fileCurrSize;
		cpps::C* m_c;
		cpps::object									m_funcCallback;

	};
	
}
#endif //cpps_cpps_http_downloader_h__