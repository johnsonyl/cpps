#ifndef cpps_socket_httpserver_request_h__
#define cpps_socket_httpserver_request_h__

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
#include "cpps_socket_httpserver_request_filedata.h"


namespace cpps {

	typedef phmap::flat_hash_map<std::string, std::string> PARAMSLIST;
	typedef phmap::flat_hash_map<std::string, cpps_socket_httpserver_request_filedata*> FILEDATASLIST;
	class cpps_socket_httpserver_request
	{
	public:
		cpps_socket_httpserver_request();
		virtual ~cpps_socket_httpserver_request();

	public:
		void		addheader(cpps::object list);
		void		append(std::string s);
		void		send(cpps_integer n,std::string msg);
		std::string getparam(std::string k);
		std::string get(std::string k);
		std::string post(std::string k);
		std::string getheader(std::string k);
		std::string getpath();
		std::string geturi();
		std::string getbuffer();
		bool		isformdata();
		std::string getboundary();
		cpps_socket_httpserver_request_filedata* getfiledata(std::string name);
		void parse_form_data(std::string& input_buffer);
	public:
		std::string	path;
		std::string uri;
		PARAMSLIST	paramslist;
		PARAMSLIST	getlist;
		PARAMSLIST	postlist;
		PARAMSLIST	input_headerslist;
		std::string scheme;
		std::string userinfo;
		std::string input_buffer;
		struct evhttp_request* ev_req;
		FILEDATASLIST filedataslist;
	};
}


#endif // cpps_socket_httpserver_request_h__
