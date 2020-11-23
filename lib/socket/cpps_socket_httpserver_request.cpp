#include "cpps_socket_httpserver_request.h"
namespace cpps {

	cpps_socket_httpserver_request::cpps_socket_httpserver_request()
	{
		ev_req = NULL;
	}

	cpps_socket_httpserver_request::~cpps_socket_httpserver_request()
	{

	}

	void cpps_socket_httpserver_request::addheader(cpps::object list)
	{
		cpps_map* m = cpps_to_cpps_map(list.value);
		struct evkeyvalq* out_headers = evhttp_request_get_output_headers(ev_req);

		for (auto header : m->realmap())
		{
			std::string *k = cpps_get_string(header.first);
			std::string *v = cpps_get_string(header.second);
			evhttp_add_header(out_headers, k->c_str(), v->c_str());
		}
	}

	void cpps_socket_httpserver_request::append(std::string s)
	{
		struct evbuffer* buf = evhttp_request_get_output_buffer(ev_req);
		evbuffer_add_printf(buf, "%s", s.c_str());
	}

	void cpps_socket_httpserver_request::send(cpps_integer code, std::string reason)
	{
		struct evbuffer* buf = evhttp_request_get_output_buffer(ev_req);
		evhttp_send_reply(ev_req, (int)code, reason.c_str(), buf);
	}

	std::string cpps_socket_httpserver_request::getparam(std::string k)
	{
		return paramslist[k];
	}

	std::string cpps_socket_httpserver_request::getheader(std::string k)
	{
		return input_headerslist[k];
	}
	std::string cpps_socket_httpserver_request::getpath()
	{
		return path;
	}

	std::string cpps_socket_httpserver_request::geturi()
	{
		return uri;
	}

	std::string cpps_socket_httpserver_request::getbuffer()
	{
		return input_buffer;
	}

}