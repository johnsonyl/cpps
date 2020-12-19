#ifndef cpps_socket_httpserver_h__
#define cpps_socket_httpserver_h__

#include <cpps/cpps.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/thread.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <unordered_map>
#include <string>


namespace cpps {
	class cpps_socket_httpserver_option
	{
	public:
		cpps_socket_httpserver_option()
		{
			option_ip = "0.0.0.0";
		}
		std::string			option_ip;
		cpps::object		exceptionfunc;
		cpps::object		notfoundfunc;
	};
	typedef phmap::flat_hash_map<std::string, cpps::object> http_route;
	class cpps_socket_httpserver
	{
	public:
		cpps_socket_httpserver();
		virtual ~cpps_socket_httpserver();

		void									setcstate(cpps::C* cstate);
		cpps_socket_httpserver*					setoption(cpps::object opt);
		cpps_socket_httpserver*					listen(cpps::C* cstate, cpps::usint16 port);
		void									register_handlefunc(std::string path, cpps::object func);
		void									register_controller(cpps::object cls,cpps_value defaultset); 
		bool									isrunning();
		virtual	void							run();
		void									stop();

		cpps::object							gethandlefunc(std::string path);
		cpps::object							getcontroller(std::string controllername);
	public:
		static void								generic_handler(struct evhttp_request* req, void* handler);
	public:
		cpps::C*								c;
		struct event_base*						ev_base;
		struct evhttp*							ev_http;
		http_route								http_route_list;
		http_route								http_class_route_list;
		cpps::object							http_default_class_route;
		cpps_socket_httpserver_option			http_option;
		bool									http_running;
	};
}


#endif // cpps_socket_httpserver_h__
