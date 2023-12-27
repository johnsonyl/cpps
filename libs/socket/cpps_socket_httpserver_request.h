#ifndef cpps_socket_httpserver_request_h__
#define cpps_socket_httpserver_request_h__

#include <cpps/cpps.h>
#include <unordered_map>
#include <string>
#include "cpps_socket_httpserver_request_filedata.h"


namespace cpps {

	typedef phmap::flat_hash_map<std::string, std::string> PARAMSLIST;
	typedef phmap::flat_hash_map<std::string, cpps_socket_httpserver_request_filedata*> FILEDATASLIST;
	class cpps_socket_httpserver_session;
	class cpps_socket_httpserver;
	class cpps_socket_httpserver_request
	{
	public:
		cpps_socket_httpserver_request();
		virtual ~cpps_socket_httpserver_request();

	public:
		void		real_addheader(std::string& k, const char*  v);
		void		addheader(cpps::object list);
		void		append(std::string s);
		void		send(cpps_integer n,std::string msg);
		cpps_value paramslistfunc(C* c);
		cpps_value getlistfunc(C* c);
		cpps_value postlistfunc(C* c);
		std::string getparam(std::string k);
		std::string get(std::string k);
		std::string post(std::string k);
		std::string getheader(std::string k);
		std::string getpath();
		std::string geturi();
		std::string getbuffer();
		std::string getcookie(std::string key);
		cpps_socket_httpserver_session* getsession();
		void							setsession(cpps_socket_httpserver_session*sess);
		void		createsession();
		/*
		* ·µ»Ø:void
		* std::string key
		* std::string value
		* object path   default /
		* object domain default empty
		*/
		void		setcookie(std::string key, std::string value, object path, object domain, object max_age);
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
		std::string	output_headerslist;
		std::string scheme;
		std::string userinfo;
		std::string input_buffer;
		std::string output_buffer;
		cpps_integer socket_index;
		cpps_socket_httpserver* server;
		FILEDATASLIST filedataslist;
		cpps_socket_httpserver_session* session;
		bool support_gzip;
		int32 keepalive;
	private:
		bool gzip_compress(std::string& output_buffer);
	};
}


#endif // cpps_socket_httpserver_request_h__
