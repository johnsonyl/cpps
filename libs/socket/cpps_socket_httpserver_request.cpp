#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_session.h"
namespace cpps {
	void cpps_string_real_split(std::vector<std::string>& vec, std::string& v, std::string v2, cpps_integer count);
	cpps_socket_httpserver_request::cpps_socket_httpserver_request()
	{
		ev_req = NULL;
		session = NULL;
	}

	cpps_socket_httpserver_request::~cpps_socket_httpserver_request()
	{
		for (auto filedata : filedataslist) {
			delete filedata.second;
		}
		filedataslist.clear();
	}

	void cpps_socket_httpserver_request::real_addheader(std::string& k, std::string& v)
	{
		struct evkeyvalq* out_headers = evhttp_request_get_output_headers(ev_req);
		evhttp_add_header(out_headers, k.c_str(), v.c_str());
	}
	void cpps_socket_httpserver_request::addheader(cpps::object list)
	{
		cpps_map* m = cpps_to_cpps_map(list.value);

		for (auto header : m->realmap())
		{
			std::string *k = cpps_get_string(header.first);
			std::string *v = cpps_get_string(header.second);
			real_addheader(*k, *v);
		}
	}

	void cpps_socket_httpserver_request::append(std::string s)
	{
		struct evbuffer* buf = evhttp_request_get_output_buffer(ev_req);
		evbuffer_add(buf, s.c_str(),s.size());
	}

	void cpps_socket_httpserver_request::send(cpps_integer code, std::string reason)
	{
		struct evbuffer* buf = evhttp_request_get_output_buffer(ev_req);
		evhttp_send_reply(ev_req, (int)code, reason.c_str(), buf);
	}

	cpps::cpps_value cpps_socket_httpserver_request::paramslistfunc(C*c)
	{
		cpps_map* m;
		cpps_value ret = newclass< cpps_map >(c, &m);
		for (auto p : paramslist) {
			m->insert(cpps_value(c, p.first), cpps_value(c, p.second));
		}
		return ret;
	}

	cpps::cpps_value cpps_socket_httpserver_request::getlistfunc(C* c)
	{
		cpps_map* m;
		cpps_value ret = newclass< cpps_map >(c, &m);
		for (auto p : getlist) {
			m->insert(cpps_value(c, p.first), cpps_value(c, p.second));
		}
		return ret;
	}

	cpps::cpps_value cpps_socket_httpserver_request::postlistfunc(C* c)
	{
		cpps_map* m;
		cpps_value ret = newclass< cpps_map >(c, &m);
		for (auto p : postlist) {
			m->insert(cpps_value(c, p.first), cpps_value(c, p.second));
		}
		return ret;
	}

	std::string cpps_socket_httpserver_request::getparam(std::string k)
	{
		return paramslist[k];
	}
	std::string cpps_socket_httpserver_request::get(std::string k)
	{
		return getlist[k];
	}
	
	std::string cpps_socket_httpserver_request::post(std::string k)
	{
		return postlist[k];
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
	
	std::string cpps_socket_httpserver_request::getcookie(std::string key)
	{
		std::string ret;
		std::string cookies = getheader("Cookie");
		if (!cookies.empty()) {
			/*
			a=value; b=value;
			*/
			std::vector<std::string> s;
			std::vector<std::string> s2;
			cpps_string_real_split(s, cookies, "; ", -1);
			for (auto singcookies : s)
			{
				s2.clear();
				cpps_string_real_split(s2, singcookies, "=", -1);
				if (s2.size() == 2 && s2[0] == key) {
					ret = s2[1];
					break;
				}
			}
		}
		return ret;
	}

	cpps::cpps_socket_httpserver_session* cpps_socket_httpserver_request::getsession()
	{
		return session;
	}

	void cpps_socket_httpserver_request::setsession(cpps_socket_httpserver_session* sess)
	{
		session = sess;
	}

	void	cpps_socket_httpserver_request::setcookie(std::string key, std::string value, object path, object domain, object max_age) {

		std::string set_cookie_value = key + "=" + value + "; ";
		if (path.isstring()) {
			set_cookie_value += "path=" + path.tostring();
		}
		else {
			set_cookie_value += "path=/;";
		}
		if (domain.isstring() && !domain.tostring().empty()) {
			set_cookie_value += "; domain=" + domain.tostring();
		}
		if (max_age.isint())
		{
			set_cookie_value += "; max-age=" + max_age.tostring();
		}
		std::string set_cookie = "Set-Cookie";
		real_addheader(set_cookie, set_cookie_value);
	}
	bool cpps_socket_httpserver_request::isformdata()
	{
		std::string content_type = getheader("Content-Type");
		size_t pos = content_type.find("multipart/form-data");
		if (pos != 0) return false;
		return true;
	}

	std::string cpps_socket_httpserver_request::getboundary()
	{
		std::string content_type = getheader("Content-Type");
		size_t pos = content_type.find("boundary=");
		if (pos == std::string::npos) return "";
		return content_type.substr(pos + strlen("boundary="));
	}

	cpps::cpps_socket_httpserver_request_filedata* cpps_socket_httpserver_request::getfiledata(std::string name)
	{
		cpps::cpps_socket_httpserver_request_filedata* ret = NULL;
		auto it = filedataslist.find(name);
		if (it != filedataslist.end()) {
			ret = it->second;
		}
		return ret;
	}

	std::string	cpps_string_real_strcut(std::string& v, std::string v2, std::string v3);
	void cpps_socket_httpserver_request::parse_form_data(std::string& input_buffer)
	{
		std::string boundary = std::string("--") + getboundary();
		if (boundary.empty()) return;
		size_t pos = 0;
		while (pos < input_buffer.size()) {
			pos = input_buffer.find(boundary, pos);
			if (pos == std::string::npos) break;
			pos += boundary.size();
			if (pos + 2 >= input_buffer.size()) break;
			std::string ends = input_buffer.substr(pos, 2);
			if (ends == "--") break;
			if (ends != "\r\n") break;
			pos += 2;

			size_t pos2 = input_buffer.find("\r\n\r\n", pos);
			if (pos2 == std::string::npos) break;

			

			std::string cfg = input_buffer.substr(pos, pos2 - pos + 2);
			pos = pos2 + strlen("\r\n\r\n");
			std::string name_value = cpps_string_real_strcut(cfg, "name=\"", "\"");
			std::string filename_value = cpps_string_real_strcut(cfg, "filename=\"", "\"");
			std::string content_type = cpps_string_real_strcut(cfg, "Content-Type: ", "\r\n");



			pos2 = input_buffer.find(boundary, pos);
			if (pos2 == std::string::npos) break;

			std::string value = input_buffer.substr(pos,pos2-pos);
			pos = pos2;


			if (!filename_value.empty()) {
				cpps_socket_httpserver_request_filedata* filedata = new cpps_socket_httpserver_request_filedata();
				filedata->content_type_value = content_type;
				filedata->filename_value = filename_value;
				filedata->name_value = name_value;
				filedata->value_value = value;
				filedataslist.insert(FILEDATASLIST::value_type(name_value, filedata));
			}
			else {
				paramslist.insert(PARAMSLIST::value_type(name_value, value));
				postlist.insert(PARAMSLIST::value_type(name_value, value));
			}

		}
	}

}