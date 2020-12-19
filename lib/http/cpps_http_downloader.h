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

		bool			download(cpps::C* c, cpps::object func);
		void			close();
		void			append(const char* page, size_t size);

	public:
		std::string		httpurl;
		std::string		filepath;
		cpps_integer	fileSize;
		cpps_integer	fileCurrSize;
		cpps::C* m_c;
		cpps::object									m_funcCallback;

		//std::string										m_strProxy;
		//std::string										m_strCookieFile;
		//std::string										m_strDomain;
		//int												m_nPort;
		//int												m_nType;
		//std::string										m_strUrl;
		//std::string										m_strData;
		//std::string										m_strPage;
		//std::vector<std::string>						m_vctHeaderList;
		//std::string										m_HeaderDatas;
	};
	
}
#endif //cpps_cpps_http_downloader_h__