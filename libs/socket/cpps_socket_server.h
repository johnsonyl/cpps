#ifndef cpps_socket_server_h__
#define cpps_socket_server_h__

#include <cpps/cpps.h>
#include <string>
#include <unordered_map>
#include <uv.h>

#ifdef _WIN32
#include <ws2def.h>
#include <windows.h>
#include <time.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif
#include <openssl/ssl.h>

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
		cpps::object		option_ssl;
		cpps::object		option_certificate_file;
		cpps::object		option_privatekey_file;
	};

	class cpps_socket_server_client;
	typedef phmap::flat_hash_map<cpps_integer, cpps_socket_server_client*> socket_list;
	class cpps_socket_server : public cpps_socket_event_callback
	{
	public:
		cpps_socket_server();
		virtual ~cpps_socket_server();

		virtual void							setcstate(cpps::C* cstate);
		cpps_socket_server*						setoption( cpps::object opt);
		bool									is_open_ssl();
		int										get_addrinfo(const struct sockaddr* addr, std::string& ip, cpps::usint16& port);
		cpps_socket_server*						listen(cpps::C* cstate, cpps::usint16 port);
		virtual cpps_socket_server_client*		create_server_client();
		virtual void							free_server_client(cpps_socket_server_client* client);
		virtual	void							sends(cpps_integer socketIndex,std::string& buffer);
		virtual	void							send(cpps_integer socketIndex, cpps::Buffer* buffer);
		virtual cpps_socket_server_client*		getclient(cpps_integer socketIndex);
		virtual	void							run();
		virtual void							closesocket(cpps_integer socketIndex);
		virtual	void							stop();
		void									stoped();
		bool									isrunning();

	public:
		virtual void							onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf);
		static  void							onClsoeCallback(uv_handle_t* handle);
		virtual void							on_error_event(cpps_socket_server_client* client,int type);

	public:
		static void								stop_cb(uv_handle_t* handle);
		static void								write_task_cb(uv_async_t* handle);
		static void								cb_listener(uv_stream_t* server, int status);
	public:
		uv_tcp_t 								uv_socket;
		uv_async_t								async_send_msg;
		cpps_socket_server_option				server_option;
		uv_loop_t*								uv_loop;
		cpps::C*								c;
		socket_list								server_client_list;
		cpps_integer							inc_socket_index;
		bool									sever_running;
		SSL_CTX*								ctx;

	};
}



#endif // cpps_socket_server_h__
