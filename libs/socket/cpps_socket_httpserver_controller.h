#ifndef cpps_socket_httpserver_controller_h__
#define cpps_socket_httpserver_controller_h__

#include <string>

namespace cpps
{
	class cpps_socket_httpserver_controller
	{
	public:
		cpps_socket_httpserver_controller();
		~cpps_socket_httpserver_controller();
		
		std::string controllername;
		std::string method;
	};

	const cpps_socket_httpserver_controller cpps_socket_httpserver_parse_controlloer(std::string path);
}


#endif // cpps_socket_httpserver_controller_h__
