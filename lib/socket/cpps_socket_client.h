#ifndef cpps_socket_client_h__
#define cpps_socket_client_h__
#include "cpps_socket.h"

#include <cpps.h>
#include <string>
#include <unordered_map>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <event2/buffer.h>

namespace cpps {

	class cpps_socket_client_option
	{
	public:
		cpps_socket_client_option()
		{
			option_headsize = 0;
			isset = false;
		}
		cpps::object		option_connected;
		cpps::object		option_data;
		cpps::object		option_close;
		cpps_integer		option_headsize;
		cpps::object		option_parser;
		bool				isset;
	};

	class cpps_socket_client : public cpps_socket , public cpps_socket_event_callback
	{
	public:
		cpps_socket_client();
		virtual ~cpps_socket_client();

		void									setcstate(cpps::C* cstate);
		cpps_socket_client*						setoption(cpps::C* cstate, cpps::object opt);
		bool									connect(std::string ip, cpps::usint16 port);
		virtual	void							run();
		virtual void							close();
		virtual void							closesocket();
		bool									isconnect();

	public:
		virtual void							onReadCallback(cpps_socket* sock, struct bufferevent* bv);
		virtual void							onWriteCallback(cpps_socket* sock, struct bufferevent* bv);
		virtual void							onEventCallback(cpps_socket* sock, short e);
	public:

		cpps::usint16							dest_port;
		std::string								dest_ip;
		struct event_base*						ev_base;
		cpps_socket_client_option				client_option;
		cpps::C*								c;
		bool									client_connection;

	};
}


#endif // cpps_socket_client_h__
