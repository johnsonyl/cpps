#ifndef cpps_cpps_socket_httpserver_cachefile_h__
#define cpps_cpps_socket_httpserver_cachefile_h__

#include <cpps/cpps.h>

namespace cpps{

	class cpps_socket_httpserver_cachefile
	{
	public:
		cpps_socket_httpserver_cachefile();
		virtual ~cpps_socket_httpserver_cachefile();
		
		void					setfilepath(std::string& path);
		std::string&			getfilepath();
		std::string&			getcontent();
		void					setcontent(std::string& content);
		cpps_integer			getlast_write_time();
		void					setlast_write_time(cpps_integer time);

	public:
		std::string				filecontent;
		cpps_integer			lastchangetime;
		std::string				filepath;
	};
	
}
#endif //cpps_cpps_socket_httpserver_cachefile_h__