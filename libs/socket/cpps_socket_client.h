#ifndef cpps_socket_client_h__
#define cpps_socket_client_h__
#include "cpps_socket.h"

#include <cpps/cpps.h>
#include <string>
#include <unordered_map>
#include <uv.h>

namespace cpps {

	class cpps_socket_client_option
	{
	public:
		cpps_socket_client_option()
		{
			option_headsize = 0;
		}
		cpps::object		option_connected;
		cpps::object		option_data;
		cpps::object		option_write;
		cpps::object		option_close;
		cpps_integer		option_headsize;
		cpps::object		option_parser;
		cpps::object		option_ssl;
		cpps::object		userdata;
	};

	class cpps_socket_client : public cpps_socket , public cpps_socket_event_callback
	{
	public:
		cpps_socket_client();
		virtual ~cpps_socket_client();

		void									setcstate(cpps::C* cstate);
		cpps_socket_client*						setoption( cpps::object opt);
		bool									connect(cpps::C* cstate, std::string ip, cpps::usint16 port);
		virtual	void							run();
		void									close();
		virtual void							closesocket();
		bool									isconnect();
		void									closed();
		void									setuserdata(cpps::object userdata);
		cpps_value								getuserdata();
		void									on_connect_do_cb();
		void									ssl_connect();

	public:
		virtual void							onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf);
		virtual void							onWriteCallback(cpps_socket* sock, ssize_t nread, const char* buf);
		static  void							onClsoeCallback(uv_handle_t* handle);
		static  void							on_connect(uv_connect_t* req, int status);
		virtual void							on_error_event(int type);
		virtual int								ssl_continue();
	public:

		cpps::usint16							dest_port;
		std::string								dest_ip;
		uv_loop_t*								uv_loop;
		uv_connect_t*							uv_connect;
		cpps_socket_client_option				client_option;
		cpps::C*								c;
		bool									client_connection;
		cpps::object							user_data;
	};
}


#endif // cpps_socket_client_h__
