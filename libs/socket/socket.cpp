﻿
#include <cpps/cpps.h>
#ifdef _WIN32
#pragma warning(disable: 4098) 
#include <Windows.h>
#include <WinSock2.h>
#endif
using namespace cpps;
using namespace std;
#include "cpps_socket.h"
#include "cpps_socket_client.h"
#include "cpps_socket_server.h"
#include "cpps_socket_server_client.h"
#include "cpps_socket_httpserver.h"
#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_session.h"

#include <openssl/ssl.h>

namespace cpps {
	std::string cpps_getcwd();
	std::string cpps_io_readfile(std::string filepath); bool cpps_io_file_exists(std::string path);
}

std::string cpps_socket_prasehtml2str2(cpps::C* c, int32 & __htmltextblockidx, cpps::object::vector &vec,std::string& __s, std::string& take, std::string& __html, size_t& pos, char startsep1, char startsep2, std::string endsep)
{
	std::string ret;
	char chr = __html[pos];

	if (chr == startsep1)
	{
		char chr2 = __html[pos + 1];
		if (chr2 == startsep2)
		{
			if (!take.empty()) {
				vec.push_back(cpps::object::create(c, take));
				char fmtstr[1024];
				sprintf(fmtstr, "echo __htmltextblock[%d];\n", __htmltextblockidx);
				__s += fmtstr;
				++__htmltextblockidx;
				take.clear();
			}
			size_t pos2 = __html.find(endsep, pos+2);
			if (pos2 == std::string::npos) {
				return ret;
			}
			pos += 2;

			ret = __html.substr(pos, pos2 - pos);
			pos = pos2 + 2;
		}
	}
	return ret;
}

inline std::string& lTrim(std::string& ss)
{
	std::string::iterator   p = find_if(ss.begin(), ss.end(), [](char code) { return !isspace(code); });
	ss.erase(ss.begin(), p);
	return  ss;
}

inline  std::string& rTrim(std::string & ss)
{
	std::string::reverse_iterator  p = find_if(ss.rbegin(), ss.rend(), [](char code) { return !isspace(code); });
	ss.erase(p.base(), ss.end());
	return   ss;
}

inline   std::string& trim(std::string & st)
{
	lTrim(rTrim(st));
	return   st;
}
std::string  cpps_socket_prasehtml2str(cpps::C* c,cpps_socket_httpserver_request*request, std::string path, object __htmltextblock)
{
	cpps::object::vector vct(__htmltextblock);
	
	std::string __html = cpps_io_readfile(path);
	std::string __s = "";
	size_t pos = 0;
	size_t size = __html.size();
	int32 __htmltextblockidx = 0;
	std::string take;
	while (pos < size) {
		
		char chr = __html[pos];
		if (chr == '{') {
			char chr2 = __html[pos+1];
			if (chr2 == '{')
			{
				std::string r = cpps_socket_prasehtml2str2(c, __htmltextblockidx, vct, __s, take, __html, pos, '{', '{', "}}");
				if (!r.empty()) {
					__s += "echo ";
					__s += r;
					__s += ";\n";
					continue;
				}
			}
			else if (chr2 == '%')
			{
				std::string r = cpps_socket_prasehtml2str2(c, __htmltextblockidx, vct, __s, take, __html, pos, '{', '%', "%}");
				if (!r.empty()) {
					trim(r);
					if (r[0] == '@')
					{
						if (r.find("@page(") == 0) {
							size_t pos2 = r.rfind(')');
							if (pos2 != std::string::npos) {
								std::string path = cpps_getcwd() + "/" +  r.substr(strlen("@page("), pos2 - strlen("@page("));
								if (cpps_io_file_exists(path)) {
									std::string content = cpps_io_readfile(path);
									size += content.size();
									__html.insert(pos, content);
								}
								
							}
						}
						else if (r.find("@csrf_token") == 0)
						{
							object csrftoken = (request && request->getsession()) ? request->getsession()->get("csrftoken",nil) : object();
							std::string csrfmiddlewaretoken = "<input type='hidden' name='csrfmiddlewaretoken' value='" + csrftoken.tostring() + "' />";
							size += csrfmiddlewaretoken.size();
							__html.insert(pos, csrfmiddlewaretoken);

						}
					}
					else 
						__s += r;
					
					continue;
				}
			}
		}
		
		++pos;
		take.append(1, chr);
	}

	if (!take.empty())
	{
		vct.push_back(cpps::object::create(c, take));
		char fmtstr[1024];
		sprintf(fmtstr, "echo __htmltextblock[%d];\n", __htmltextblockidx);
		__s += fmtstr;
		++__htmltextblockidx;
		take.clear();
	}

	return __s;
}
cpps_value cpps_ssl_ctx_new(C* c, std::string certificate_file, std::string privatekey_file)
{
	cpps_create_class_var(CPPS_SSL_CTX, c, ret, _ctx);
	_ctx->ctx = SSL_CTX_new(TLSv1_2_server_method());
	if (_ctx->ctx == NULL) {
		return cpps::nil;
	}
	/* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(_ctx->ctx, certificate_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
		return cpps::nil;
	}
	/* 载入用户私钥 */
	if (SSL_CTX_use_PrivateKey_file(_ctx->ctx, privatekey_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
		return cpps::nil;
	}
	/* 检查用户私钥是否正确 */
	if (!SSL_CTX_check_private_key(_ctx->ctx)) {
		return cpps::nil;
	}
	return ret;
}
cpps_export_void  cpps_attach(cpps::C* c)
{
#ifdef _WIN32
	WSADATA wsadata;
	WSAStartup(0x0202, &wsadata);
#endif
	cpps::cpps_init_cpps_class(c);

	cpps::_module(c,"socket")[
		def_inside("prasehtml2str",cpps_socket_prasehtml2str),
		def_inside("SSL_CTX_NEW", cpps_ssl_ctx_new),
		_class<CPPS_SSL_CTX>("SSL_CTX")
			.def("release",&CPPS_SSL_CTX::release),
		_class<cpps_socket_server>("server")
			.def("setoption", &cpps_socket_server::setoption)
			.def_inside("listen", &cpps_socket_server::listen)
			.def("run", &cpps_socket_server::run)
			.def("send", &cpps_socket_server::send)
			.def("closesocket", &cpps_socket_server::closesocket)
			.def("stop", &cpps_socket_server::stop)
			.def("isrunning", &cpps_socket_server::isrunning)
			.def("get_send_count", &cpps_socket_server::get_send_count)
			.def("get_write_count", &cpps_socket_server::get_write_count)
			.def("get_recv_count", &cpps_socket_server::get_recv_count),
		_class<cpps_socket_httpserver>("httpserver")
			.def("setoption", &cpps_socket_httpserver::setoption)
			.def_inside("listen", &cpps_socket_httpserver::listen)
			.def("add_type", &cpps_socket_httpserver::add_type)
			.def("get_type", &cpps_socket_httpserver::get_type)
			.def("run", &cpps_socket_httpserver::run)
			.def("register_handlefunc", &cpps_socket_httpserver::register_handlefunc)
			.def("register_controller", &cpps_socket_httpserver::register_controller)
			.def("stop", &cpps_socket_httpserver::stop)
			.def("close", &cpps_socket_httpserver::closesocket)
			.def("closesocket", &cpps_socket_httpserver::closesocket)
			.def("shutdown", &cpps_socket_httpserver::shutdown)
			.def("isrunning", &cpps_socket_httpserver::isrunning)
			.def("get_send_count", &cpps_socket_server::get_send_count)
			.def("get_write_count", &cpps_socket_server::get_write_count)
			.def("get_request", &cpps_socket_httpserver::get_request)
			.def("get_recv_count", &cpps_socket_server::get_recv_count),
		_class<cpps_socket_client>("client")
			.def("setoption", &cpps_socket_client::setoption)
			.def("run", &cpps_socket_client::run)
			.def_inside("connect", &cpps_socket_client::connect)
			.def("send", &cpps_socket_client::send)
			.def("isconnect", &cpps_socket_client::isconnect)
			.def("closesocket", &cpps_socket_client::closesocket)
			.def("close", &cpps_socket_client::closesocket)
			.def("setuserdata", &cpps_socket_client::setuserdata)
			.def("getuserdata", &cpps_socket_client::getuserdata)
			.def("get_send_count", &cpps_socket_client::get_send_count)
			.def("get_write_count", &cpps_socket_client::get_write_count)
			.def("get_recv_count", &cpps_socket_client::get_recv_count),
		_class< cpps_socket_httpserver_request>("httprequest")
			.def("addheader", &cpps_socket_httpserver_request::addheader)
			.def("append", &cpps_socket_httpserver_request::append)
			.def("send", &cpps_socket_httpserver_request::send)
			.def("getparam", &cpps_socket_httpserver_request::getparam)
			.def("get", &cpps_socket_httpserver_request::get)
			.def("post", &cpps_socket_httpserver_request::post)
			.def("getheader", &cpps_socket_httpserver_request::getheader)
			.def("geturi", &cpps_socket_httpserver_request::geturi)
			.def("getpath", &cpps_socket_httpserver_request::getpath)
			.def("getbuffer", &cpps_socket_httpserver_request::getbuffer)
			.def("setcookie", &cpps_socket_httpserver_request::setcookie)
			.def("getcookie", &cpps_socket_httpserver_request::getcookie)
			.def("createsession", &cpps_socket_httpserver_request::createsession)
			.def("getfiledata", &cpps_socket_httpserver_request::getfiledata)
			.def_inside("paramslist", &cpps_socket_httpserver_request::paramslistfunc)
			.def_inside("getlist", &cpps_socket_httpserver_request::getlistfunc)
			.def_inside("postlist", &cpps_socket_httpserver_request::postlistfunc)
			.def("send_header", &cpps_socket_httpserver_request::send_header)
			.def("send_body", &cpps_socket_httpserver_request::send_body)
			.def("getheaderdata", &cpps_socket_httpserver_request::getheaderdata)
			.def("getmethod", &cpps_socket_httpserver_request::getmethod)
			.def_inside("getheaders", &cpps_socket_httpserver_request::getheaders)
			.def("session", &cpps_socket_httpserver_request::getsession)
			.def("close", &cpps_socket_httpserver_request::close)
			.def("getsocketindex", &cpps_socket_httpserver_request::getsocketindex)
			.def("get_ip_address", &cpps_socket_httpserver_request::get_ip_address)
			.def("get_port", &cpps_socket_httpserver_request::get_port)
			.def("shutdown", &cpps_socket_httpserver_request::shutdown),
		_class< cpps_socket_httpserver_request_filedata>("filedata")
			.def("name", &cpps_socket_httpserver_request_filedata::name)
			.def("filename", &cpps_socket_httpserver_request_filedata::filename)
			.def("content_type", &cpps_socket_httpserver_request_filedata::content_type)
			.def("getfilesize", &cpps_socket_httpserver_request_filedata::getfilesize)
			.def("getbuffer", &cpps_socket_httpserver_request_filedata::buffer),
		_class< cpps_socket_httpserver_session>("httpsession")
			.def("get",&cpps_socket_httpserver_session::get)
			.def("set",&cpps_socket_httpserver_session::set)
			.def("clear",&cpps_socket_httpserver_session::clear)
			.def("remove",&cpps_socket_httpserver_session::remove)
	];

}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "socket");
#ifdef _WIN32
	WSACleanup();
#endif
}

cpps_export_finish