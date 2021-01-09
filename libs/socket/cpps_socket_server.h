#ifndef cpps_socket_server_h__
#define cpps_socket_server_h__

#include <cpps/cpps.h>
#include <string>
#include <unordered_map>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <event2/buffer.h>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include "cpps_socket_event_callback.h"

namespace cpps {

	class cpps_socket_server_option
	{
	public:
		cpps_socket_server_option()
		{
			option_ip = "0.0.0.0";
			option_headsize = 0;
		}
		std::string			option_ip;
		cpps::object		option_accept;
		cpps::object		option_data;
		cpps::object		option_close;
		cpps_integer		option_headsize;
		cpps::object		option_parser;
	};

	class cpps_socket_server_client;
	typedef phmap::flat_hash_map<cpps_integer, cpps_socket_server_client*> socket_list;
	class cpps_socket_server : public cpps_socket_event_callback
	{
	public:
		cpps_socket_server();
		virtual ~cpps_socket_server();

		void									setcstate(cpps::C* cstate);
		cpps_socket_server*						setoption( cpps::object opt);
		int										get_addrinfo(const struct sockaddr* addr, std::string& ip, cpps::usint16& port);
		cpps_socket_server*						listen(cpps::C* cstate, cpps::usint16 port);
		virtual cpps_socket_server_client*		create_server_client();
		virtual void							free_server_client(cpps_socket_server_client* client);
		virtual	void							send(cpps_integer socketIndex, cpps::Buffer* buffer);
		cpps_socket_server_client*				getclient(cpps_integer socketIndex);
		virtual	void							run();
		virtual void							closesocket(cpps_integer socketIndex);
		virtual	void							stop();
		bool									isrunning();

	public:
		virtual void							onReadCallback(cpps_socket* sock, struct bufferevent* bv);
		virtual void							onWriteCallback(cpps_socket* sock, struct bufferevent* bv);
		virtual void							onEventCallback(cpps_socket* sock, short e);

	public:
		static void								cb_listener(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int len, void* ptr);
		static void								cb_listener_error(struct evconnlistener* pListener, void* pCtx);
	public:
		evutil_socket_t							ev_socket;
		cpps_socket_server_option				server_option;
		struct event_base*						ev_base;
		struct evconnlistener*					ev_listener;
		cpps::C*								c;
		socket_list								server_client_list;
		cpps_integer							inc_socket_index;
		bool									sever_running;
	};
}



#endif // cpps_socket_server_h__
