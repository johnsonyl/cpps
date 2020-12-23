#include "stdafx.h"
#include "cpps_socket_server_client.h"
#include "cpps_socket_server.h"

namespace cpps
{

	cpps_socket_server_client::cpps_socket_server_client()
	{
		socket_ip = "";
		socket_port = 0;
		socket_index = 0;
		server = NULL;
	}

	cpps_socket_server_client::~cpps_socket_server_client()
	{
		socket_ip = "";
		socket_port = 0;
		socket_index = 0;
		server = NULL;
	}

	void cpps_socket_server_client::setServerHandle(cpps_socket_server* srv)
	{
		server = srv;
		set_event_callback(server);
	}



	void cpps_socket_server_client::set_client_info(std::string ip, cpps::usint16 port)
	{
		socket_ip = ip;
		socket_port = port;
	}

}