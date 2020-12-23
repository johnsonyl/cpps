#include "cpps_socket_httpserver_controller.h"
namespace cpps
{

	cpps_socket_httpserver_controller::cpps_socket_httpserver_controller()
	{

	}

	cpps_socket_httpserver_controller::~cpps_socket_httpserver_controller()
	{

	}

	const cpps_socket_httpserver_controller cpps_socket_httpserver_parse_controlloer(std::string path)
	{
		//path like:/Home/Index/asdjkasd/asdasd/asdasd just get first and second.
		cpps_socket_httpserver_controller ret;
		size_t pos = 1; //pop first /
		size_t pos2 = path.find('/', pos);
		if (pos != std::string::npos)
		{

			ret.controllername = path.substr(pos, pos2 - pos);
			pos = pos2+1; //pop second /

			pos2 = path.find('/', pos);
			if (pos2 != std::string::npos)
			{
				ret.method = path.substr(pos, pos2 - pos);
			}
			else
			{
				ret.method = path.substr(pos);
			}
		}

		return ret;
	}

}