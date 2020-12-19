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



	public:
		void								set_client_info(std::string ip, cpps::usint16 port);
		cpps_integer						socket_index;
		std::string							socket_ip;
		cpps::int16							socket_port;
		cpps_socket_server*					server;
	};
}

#endif // cpps_socket_server_client_h__
