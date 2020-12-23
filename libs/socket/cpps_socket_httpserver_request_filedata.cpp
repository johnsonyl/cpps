#include "cpps_socket_httpserver_request_filedata.h"

namespace cpps
{

	cpps_socket_httpserver_request_filedata::cpps_socket_httpserver_request_filedata()
	{
		
	}
	
	cpps_socket_httpserver_request_filedata::~cpps_socket_httpserver_request_filedata()
	{
		
	}
	
	std::string cpps_socket_httpserver_request_filedata::name()
	{
		return name_value;
	}

	std::string cpps_socket_httpserver_request_filedata::filename()
	{
		return filename_value;
	}

	std::string cpps_socket_httpserver_request_filedata::content_type()
	{
		return content_type_value;
	}

	std::string cpps_socket_httpserver_request_filedata::buffer()
	{
		return value_value;
	}

}
