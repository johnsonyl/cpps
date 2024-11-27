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
		_shutdown = false;
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
		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

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
		if (ssl == NULL) return 1;
		if (write_bio == NULL) return 1 ;
		if (read_bio == NULL) return 0;

		return SSL_accept(ssl);
	}
	void	cpps_socket_server_client::on_shutdown_cb(uv_shutdown_t* req, int status)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)req->data;
		if (req)
			free(req);
		if (client) {
			client->close("shut down socket", cpps_socket_server::onClsoe);
		}
	}

	void	cpps_socket_server_client::shutdown() {
		if (uv_tcp == NULL) return;
		_shutdown = true;
		uv_shutdown_t* req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
		req->data = (void*)this;
		uv_shutdown(req, (uv_stream_t*)uv_tcp, on_shutdown_cb);
	}
	bool	cpps_socket_server_client::isShutdown() { return _shutdown; }

}	