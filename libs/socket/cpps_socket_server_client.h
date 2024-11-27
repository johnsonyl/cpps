#ifndef cpps_socket_server_client_h__
#define cpps_socket_server_client_h__
#include <cpps/cpps.h>
#include "cpps_socket.h"

namespace cpps {
	class cpps_socket_server;
	class cpps_socket_server_client : public cpps_socket
	{
	public:
		cpps_socket_server_client();
		virtual ~cpps_socket_server_client();

		void								setServerHandle(cpps_socket_server* srv);
		void								ssl_accept();
	public:
		void								set_client_info(std::string ip, cpps::usint16 port);
		void								on_error_event(int type);
		virtual int							ssl_continue();
		static void							on_shutdown_cb(uv_shutdown_t* req, int status);
		void								shutdown();
		bool								isShutdown();
		cpps_integer						socket_index;
		std::string							socket_ip;
		cpps::usint16						socket_port;
		cpps_socket_server*					server;
		bool								_shutdown;
	};
}

#endif // cpps_socket_server_client_h__
