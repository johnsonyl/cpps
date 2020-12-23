#ifndef cpps_cpps_socket_httpserver_request_filedata_h__
#define cpps_cpps_socket_httpserver_request_filedata_h__

#include <cpps/cpps.h>

namespace cpps{

	class cpps_socket_httpserver_request_filedata
	{
	public:
		cpps_socket_httpserver_request_filedata();
		virtual ~cpps_socket_httpserver_request_filedata();
		std::string	name();
		std::string	filename();
		std::string content_type();
		std::string buffer();
	public:
		std::string name_value;
		std::string filename_value;
		std::string content_type_value;
		std::string value_value;
	};
	
}
#endif //cpps_cpps_socket_httpserver_request_filedata_h__