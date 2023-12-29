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
		socket_ip.clear();
		socket_port = 0;
		socket_index = 0;
		server = NULL;
		closemsg.clear();
	}

	void cpps_socket_server_client::setServerHandle(cpps_socket_server* srv)
	{
		server = srv;
		set_event_callback(server);
	}

	void cpps_socket_server_client::ssl_accept()
	{
		SSL_set_accept_state(ssl);  
		int ret = SSL_accept(ssl);
		if (ret != 1 && SSL_get_error(ssl, ret) != SSL_ERROR_WANT_READ) {
			on_error_event(-1);
			return;
		}
	}



	void cpps_socket_server_client::set_client_info(std::string ip, cpps::usint16 port)
	{
		socket_ip = ip;
		socket_port = port;
	}


	void cpps_socket_server_client::on_error_event(int type)
	{
		server->on_error_event(this,type);
	}
	int cpps_socket_server_client::ssl_continue()
	{
		return SSL_accept(ssl);
	}
}