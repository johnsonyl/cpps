#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_session.h"
#include "cpps_socket_httpserver.h"
#include "cpps_socket_server_client.h"
#include <zlib.h>
namespace cpps {
	void cpps_string_real_split(std::vector<std::string>& vec, std::string& v, std::string v2, cpps_integer count);
	cpps_socket_httpserver_request::cpps_socket_httpserver_request()
	{
		session = NULL;
		_close = false;
	}

	cpps_socket_httpserver_request::~cpps_socket_httpserver_request()
	{
		for (auto filedata : filedataslist) {
			CPPSDELETE( filedata.second);
		}
		filedataslist.clear();
	}

	void cpps_socket_httpserver_request::real_addheader(std::string& k, const char* v)
	{
		output_headerslist += k + ": " + v + "\r\n";
	}
	void cpps_socket_httpserver_request::addheader(cpps::object list)
	{
		cpps_map* m = cpps_to_cpps_map(list.value);

		for (auto header : m->realmap())
		{
			std::string *k = cpps_get_string(header.first);
			std::string *v = cpps_get_string(header.second);
			real_addheader(*k, v->c_str());
		}
	}

	void cpps_socket_httpserver_request::append(std::string s)
	{
		output_buffer.append(s);
	}
	void cpps_socket_httpserver_request::send_header(cpps_integer code, std::string reason,cpps_integer size)
	{
		std::string build_buffer;

		//1.code
		char tmp[4096];
		sprintf(tmp, "HTTP/1.1 %d %s\r\n", (int32)code, reason.c_str());
		build_buffer.append(tmp);
		
		//3.keepalive
	/*	if (!keepalive) {
			build_buffer.append("Connection: close\r\n");
		}*/
		//4.Content-Length
	/*	if (size > 0) {
			sprintf(tmp, "Content-Length: %d\r\n", (usint32)size);
			build_buffer.append(tmp);
			build_buffer.append("Content-Encoding: gzip\r\n");
		}*/

		//build_buffer.append("Connection: close\r\n");
		//5. headers
		build_buffer.append(output_headerslist);
		build_buffer.append("\r\n");



		server->sends(socket_index, build_buffer);
	}
	void cpps_socket_httpserver_request::send_body(std::string content)
	{
		//gzip_compress(content);
		server->sends(socket_index, content);
	}
	void cpps_socket_httpserver_request::send(cpps_integer code, std::string reason)
	{
		std::string build_buffer;

		//1.code
		char tmp[4096];
		sprintf(tmp, "HTTP/1.1 %d %s\r\n", (int32)code,reason.c_str());
		build_buffer.append(tmp);

		//2.gzip
		if (support_gzip) {
			if(gzip_compress(output_buffer))
				build_buffer.append("Content-Encoding: gzip\r\n");
		}
		//3.keepalive
		if (!keepalive){
			build_buffer.append("Connection: close\r\n");
		}
		//4.Content-Length
		sprintf(tmp, "Content-Length: %d\r\n", (usint32)output_buffer.size());
		build_buffer.append(tmp);

		//5. headers
		build_buffer.append(output_headerslist);
		build_buffer.append("\r\n");
		
		//6. body
		build_buffer.append(output_buffer);


		server->sends(socket_index, build_buffer);
	}

	cpps::cpps_value cpps_socket_httpserver_request::paramslistfunc(C*c)
	{
		cpps_map* m;
		cpps_value ret;
		newclass< cpps_map >(c, &m,&ret);
		for (auto p : paramslist) {
			m->insert(cpps_value(c, p.first), cpps_value(c, p.second));
		}
		return ret;
	}

	cpps::cpps_value cpps_socket_httpserver_request::getlistfunc(C* c)
	{
		cpps_map* m;
		cpps_value ret;
		newclass< cpps_map >(c, &m,&ret);
		for (auto p : getlist) {
			m->insert(cpps_value(c, p.first), cpps_value(c, p.second));
		}
		return ret;
	}

	cpps::cpps_value cpps_socket_httpserver_request::postlistfunc(C* c)
	{
		cpps_map* m;
		cpps_value ret;
		newclass< cpps_map >(c, &m,&ret);
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
	cpps::cpps_value cpps_socket_httpserver_request::getheaders(C*c)
	{
		cpps_create_class_var(cpps::cpps_map, c, cpps_map_var, cpps_map_ptr);
		for (auto _v : input_headerslist) {
			cpps_map_ptr->insert(cpps_value(c, _v.first), cpps_value(c, _v.second));
		}
		return cpps_map_var;
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
	std::string cpps_socket_httpserver_request::getmethod() {
		return method;
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

	std::string cpps_socket_httpserver_request::getheaderdata()
	{
		return header_buffer;
	}

	cpps::cpps_socket_httpserver_session* cpps_socket_httpserver_request::getsession()
	{
		return session;
	}
	void cpps_socket_httpserver_request::close()
	{
		server->closesocket(socket_index);
	}

	void cpps_socket_httpserver_request::shutdown()
	{
		auto client = server->getclient(socket_index);
		if (client) client->shutdown();
	}

	void cpps_socket_httpserver_request::setsession(cpps_socket_httpserver_session* sess)
	{
		session = sess;
	}

	void cpps_socket_httpserver_request::createsession()
	{
		cpps_socket_httpserver::cpps_socket_httpserver_bindsession(server, this,true);
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
		real_addheader(set_cookie, set_cookie_value.c_str());
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
				cpps_socket_httpserver_request_filedata* filedata = CPPSNEW( cpps_socket_httpserver_request_filedata)();
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
	/* Compress gzip data */
	/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
	int gzcompress(Bytef* zdata, uLong* nzdata, Bytef* data, uLong ndata, cpps_integer level)
	{
		z_stream c_stream;
		int err = 0;

		if (data && ndata > 0) {
			c_stream.zalloc = NULL;
			c_stream.zfree = NULL;
			c_stream.opaque = NULL;
			//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
			if (deflateInit2(&c_stream, (int)level, Z_DEFLATED,
				MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
			c_stream.next_in = data;
			c_stream.avail_in = ndata;
			c_stream.next_out = zdata;
			c_stream.avail_out = *nzdata;
			while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
				if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
			}
			if (c_stream.avail_in != 0) return c_stream.avail_in;
			for (;;) {
				if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
				if (err != Z_OK) return -1;
			}
			if (deflateEnd(&c_stream) != Z_OK) return -1;
			*nzdata = c_stream.total_out;
			return 0;
		}
		return -1;
	}
	bool cpps_socket_httpserver_request::gzip_compress(std::string& output_buffer)
	{
		Bytef* buf = NULL;
		uLong len = 0;

		buf = (Bytef*)output_buffer.c_str();
		len = (uLong)output_buffer.size();

		uLongf destlen = (uLongf)len + 20; /*不知道这个协议头到底是多大*/
		Bytef* dest = (Bytef * )malloc(destlen);

		int32 err = gzcompress(dest, &destlen, buf, len, Z_DEFAULT_COMPRESSION);
		if (err != Z_OK) {
			free(dest);
			dest = NULL;
			return false;
		}

		output_buffer.clear();
		output_buffer.append((const char*)dest, destlen);

		free(dest);
		dest = NULL;
		return true;
	}

}