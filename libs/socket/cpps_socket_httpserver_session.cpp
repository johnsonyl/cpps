#include "cpps_socket_httpserver_session.h"

namespace cpps
{

	cpps_socket_httpserver_session::cpps_socket_httpserver_session()
	{
		needremove = false;
		session_expire = 0;
	}
	
	cpps_socket_httpserver_session::~cpps_socket_httpserver_session()
	{
		
	}
	
	void cpps_socket_httpserver_session::set(std::string key, std::string value)
	{
		session_value[key] = value;
	}

	std::string cpps_socket_httpserver_session::get(std::string key,object defaultvalue)
	{
		auto it = session_value.find(key);
		if (it == session_value.end() && defaultvalue.isstring()) {
			session_value.insert(session_values::value_type(key, defaultvalue.tostring()));
		}
		return session_value[key];
	}

	void cpps_socket_httpserver_session::set_expire(cpps_integer time)
	{
		session_expire = time;
	}

	void cpps_socket_httpserver_session::clear()
	{
		session_value.clear();
	}

	void cpps_socket_httpserver_session::remove()
	{
		clear();
	}

}
