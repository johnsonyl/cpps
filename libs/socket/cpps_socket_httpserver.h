#ifndef cpps_socket_httpserver_h__
#define cpps_socket_httpserver_h__

#include <cpps/cpps.h>
#include <uv.h>
#include <unordered_map>
#include <string>
#include "cpps_socket_server.h"


namespace cpps {
	typedef phmap::flat_hash_map<std::string, std::string> http_request_header;
	struct http_request {
		std::string path;
		std::string uri;
		bool support_gzip;
		http_request_header headers;
		cpps_integer socket_index;
		std::string chunk;
		std::string body;
		std::string field;
		int32 keepalive;
		const char* body_buf;
		size_t body_len;

	};
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
		cpps::object		option_ssl;
		cpps::object		option_certificate_file;
		cpps::object		option_privatekey_file;
	};
	class cpps_socket_httpserver_session;
	class cpps_socket_httpserver_cachefile;
	typedef phmap::flat_hash_map<std::string, cpps::object> http_route;
	typedef phmap::flat_hash_map<std::string, std::string> http_mime_type;
	typedef phmap::flat_hash_map<std::string, cpps_socket_httpserver_session*> http_session_list;
	typedef phmap::flat_hash_map<std::string, cpps_socket_httpserver_cachefile*> http_cachefile_list;
	class cpps_socket_httpserver_request;
	class cpps_socket_httpserver : public cpps_socket_server
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

		void update_session();

		void									stop();
		void									add_type(std::string mime, std::string ext);
		std::string								get_type(std::string ext);

		cpps::object							gethandlefunc(std::string path);
		cpps::object							getcontroller(std::string controllername);

		cpps_socket_httpserver_session*			create_seesion(cpps::C* c);
		cpps_socket_httpserver_session*			get_session(std::string session_id);

		cpps_socket_httpserver_cachefile*		create_cachefile(std::string &filepath,std::string &content, cpps_integer last_write_time);
		cpps_socket_httpserver_cachefile*		get_cachefile(std::string filepath);
		virtual void							onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf);
		std::string								getwwwroot();
		void									setwwwroot(std::string wwwroot);
	public:
		static void								cb_listener(uv_stream_t* server, int status);
		static void								generic_handler(struct http_request& req, void* handler);
		static void								cpps_socket_httpserver_bindsession(cpps_socket_httpserver* httpserver, cpps_socket_httpserver_request* cpps_request_ptr, bool create_session=false);

	public:
		cpps::C*								c;
		http_route								http_route_list;
		http_route								http_class_route_list;
		cpps::object							http_default_class_route;
		cpps_socket_httpserver_option			http_option;
		bool									http_running;
		http_mime_type							mime_types;
		http_session_list						session_list;
		http_cachefile_list						cachefile_list;
		std::string								_wwwroot;
	private:
		object									createuuidfunc;

		bool									SESSION_ENABLED;
		std::string								SESSION_ENGINE;
		std::string								SESSION_FILE_PATH;
		std::string								SESSION_COOKIE_NAME;
		std::string								SESSION_COOKIE_PATH;
		std::string								SESSION_COOKIE_DOMAIN;
		bool									SESSION_COOKIE_SECURE;
		bool									SESSION_COOKIE_HTTPONLY;
		cpps_integer							SESSION_COOKIE_AGE;
		bool									SESSION_EXPIRE_AT_BROWSER_CLOSE;
		bool									SESSION_SAVE_EVERY_REQUEST;
	};
}


#endif // cpps_socket_httpserver_h__
