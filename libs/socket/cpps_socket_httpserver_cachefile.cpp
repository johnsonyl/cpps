#include "cpps_socket_httpserver_cachefile.h"

namespace cpps
{

	cpps_socket_httpserver_cachefile::cpps_socket_httpserver_cachefile()
	{
		lastchangetime = 0;
	}
	
	cpps_socket_httpserver_cachefile::~cpps_socket_httpserver_cachefile()
	{
		
	}
	
	void cpps_socket_httpserver_cachefile::setfilepath(std::string& path)
	{
		filepath = path;
	}

	std::string& cpps_socket_httpserver_cachefile::getfilepath()
	{
		return filepath;
	}

	std::string& cpps_socket_httpserver_cachefile::getcontent()
	{
		return filecontent;
	}

	void cpps_socket_httpserver_cachefile::setcontent(std::string &content)
	{
		filecontent = content;
	}

	cpps_integer cpps_socket_httpserver_cachefile::getlast_write_time()
	{
		return lastchangetime;
	}

	void cpps_socket_httpserver_cachefile::setlast_write_time(cpps_integer time)
	{
		lastchangetime = time;
	}

}
