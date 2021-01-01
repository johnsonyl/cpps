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
	
	void cpps_socket_httpserver_session::set(std::string key, object value)
	{
		session_value[key] = value.value;
	}

	cpps_value cpps_socket_httpserver_session::get(std::string key,object defaultvalue)
	{
		auto it = session_value.find(key);
		if (it == session_value.end()) {
			session_value.insert(session_values::value_type(key, defaultvalue.value));
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
		needremove = true;
	}

}
