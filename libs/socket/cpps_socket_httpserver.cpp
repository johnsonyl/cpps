#include "cpps_socket_httpserver.h"
#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_controller.h"
#include "cpps_socket_httpserver_session.h"
#include "cpps_socket_httpserver_cachefile.h"
#include "cpps_socket_server_client.h"
#include "http_parser.h"
namespace cpps {
	void cpps_string_real_tolower(std::string& s);
	bool cpps_io_file_exists(std::string path);
	std::string cpps_io_getfileext(std::string str);
	std::string cpps_io_readfile(std::string filepath);
	std::string cpps_getcwd();
	cpps_integer	cpps_time_gettime();
	cpps_integer cpps_io_last_write_time(std::string path);
	cpps_socket_httpserver::cpps_socket_httpserver()
	{
		http_running = false;
	}

	cpps_socket_httpserver::~cpps_socket_httpserver()
	{
		stop();
	}

	void cpps_socket_httpserver::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}

	cpps_socket_httpserver* cpps_socket_httpserver::setoption(cpps::object opt)
	{
		if (cpps::type(opt["ip"]) == CPPS_TSTRING) http_option.option_ip = object_cast<std::string>(opt["ip"]);
		http_option.exceptionfunc = opt["exceptionfunc"];
		http_option.notfoundfunc = opt["notfoundfunc"];
		http_option.writefunc = opt["writefunc"];
		http_option.userdata = opt["userdata"];
		server_option.option_ip = http_option.option_ip;
		server_option.option_ssl = opt["ssl"];
		server_option.option_certificate_file = opt["certificate_file"];
		server_option.option_privatekey_file = opt["privatekey_file"];
		server_option.option_servername_callback = opt["servername_callback"];
		if (server_option.option_ssl.tobool()) {
			SSL_library_init();
			OpenSSL_add_all_algorithms();
			SSL_load_error_strings();
		}
		return this;
	}

	cpps_socket_httpserver* cpps_socket_httpserver::listen(cpps::C* cstate, cpps::usint16 port, cpps::object _ssl_port)
	{
		setcstate(cstate);
		cpps_socket_server::listen(cstate, port, _ssl_port);
		if (!sever_running) return this;

		createuuidfunc = cpps::object::globals(c)["__socket_httpserver_createuuid"];
		if(!createuuidfunc.isfunction()) 
			throw cpps_error(__FILE__, __LINE__, cpps_error_normalerror, "__socket_httpserver_createuuid not found...\r\n");

		SESSION_ENABLED = cpps::object::globals(c)["SESSION_ENABLED"].tobool();
		SESSION_ENGINE = cpps::object::globals(c)["SESSION_ENGINE"].tostring();
		SESSION_FILE_PATH = cpps::object::globals(c)["SESSION_FILE_PATH"].tostring();
		SESSION_COOKIE_NAME = cpps::object::globals(c)["SESSION_COOKIE_NAME"].tostring();
		SESSION_COOKIE_PATH = cpps::object::globals(c)["SESSION_COOKIE_PATH"].tostring();
		SESSION_COOKIE_DOMAIN = cpps::object::globals(c)["SESSION_COOKIE_DOMAIN"].tostring();
		SESSION_COOKIE_SECURE = cpps::object::globals(c)["SESSION_COOKIE_SECURE"].tobool();
		SESSION_COOKIE_HTTPONLY = cpps::object::globals(c)["SESSION_COOKIE_HTTPONLY"].tobool();
		SESSION_EXPIRE_AT_BROWSER_CLOSE = cpps::object::globals(c)["SESSION_EXPIRE_AT_BROWSER_CLOSE"].tobool();
		SESSION_SAVE_EVERY_REQUEST = cpps::object::globals(c)["SESSION_SAVE_EVERY_REQUEST"].tobool();
		SESSION_COOKIE_AGE = cpps::object::globals(c)["SESSION_COOKIE_AGE"].toint();


		http_running = true;

		return this;
	}

	unsigned char ToHex(unsigned char x)
	{
		return  x > 9 ? x + 55 : x + 48;
	}

	unsigned char FromHex(unsigned char x)
	{
		unsigned char y = 0;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		return y;
	}

	std::string urlencode(std::string str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((unsigned char)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
			{
				strTemp += '%';
				strTemp += '2';
				strTemp += '0';
			}
			else
			{
				strTemp += '%';
				strTemp += ToHex((unsigned char)str[i] >> 4);
				strTemp += ToHex((unsigned char)str[i] % 16);
			}
		}
		return strTemp;
	}

	std::string urldecode(std::string str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (str[i] == '+') strTemp += ' ';
			else if (str[i] == '%')
			{
				unsigned char high = FromHex((unsigned char)str[++i]);
				unsigned char low = FromHex((unsigned char)str[++i]);
				strTemp += high * 16 + low;
			}
			else strTemp += str[i];
		}
		return strTemp;
	}

	void http_parse_query_str(std::string& uri, PARAMSLIST& list)
	{

		//a=10&b=20&c=&d=&=10
		size_t i = 0;
		
		while (i != std::string::npos && i < uri.size()) {
			size_t ii = uri.find('=',i);
			if (ii == std::string::npos) {
				break;
			}
			if (ii == i + 1) {
				break; //&= is wrong.need keys.
			}
			std::string field = uri.substr(i, ii - i);
			ii++;
			size_t iii = uri.find("&", ii);
			if (iii == std::string::npos) {
				list[field] = urldecode(uri.substr(ii));
				break;
			}
			list[field] = urldecode(uri.substr(ii, iii - ii));
			i = iii + 1;
		}
	}
	void cpps_socket_httpserver::generic_handler(struct http_request& req, void* handler)
	{
		cpps_socket_httpserver* httpserver = (cpps_socket_httpserver*)handler;

		cpps_create_class_var(cpps_socket_httpserver_request, httpserver->c, cpps_request_var, cpps_request_ptr);

		cpps_request_ptr->server = httpserver;
		cpps_request_ptr->socket_index = req.socket_index;
		cpps_request_ptr->path = req.path;
		cpps_request_ptr->keepalive = req.keepalive;
		cpps_request_ptr->support_gzip = req.support_gzip;
		cpps_request_ptr->method = req.method;
		cpps_request_ptr->ip_address = req.ip_address;
		cpps_request_ptr->port = (cpps_integer)req.port;
		
		httpserver->request_list.insert(http_request_list::value_type(req.socket_index, cpps_request_var));

		if (!req.uri.empty())
		{
			cpps_request_ptr->uri = req.uri;

			http_parse_query_str(req.uri, cpps_request_ptr->getlist);

			for (auto item : cpps_request_ptr->getlist)
				cpps_request_ptr->paramslist[item.first] = item.second;
		}
		
		for (auto item : req.headers)
			cpps_request_ptr->input_headerslist.insert(PARAMSLIST::value_type(item.first, item.second));

		size_t ibsize = req.body_len;
		if (ibsize > 0)
		{
			cpps_request_ptr->input_buffer.append(req.body_buf, req.body_len);

			if (cpps_request_ptr->isformdata()) {
				cpps_request_ptr->parse_form_data(cpps_request_ptr->input_buffer);
			}
			else {
				http_parse_query_str(cpps_request_ptr->input_buffer, cpps_request_ptr->postlist);

				for (auto item : cpps_request_ptr->postlist)
					cpps_request_ptr->paramslist[item.first] = item.second;
			}
		}
		ibsize = req.header_len;
		if (ibsize > 0) {
			cpps_request_ptr->header_buffer.append(req.header_buf, req.header_len);
		}
		cpps_stack* takestack = httpserver->c->getcallstack()->empty() ? NULL : httpserver->c->getcallstack()->at(httpserver->c->getcallstack()->size() - 1);


		try
		{
			//1.先找路由
			std::string path = cpps_request_ptr->path;
			cpps_string_real_tolower(path);
			cpps::object func = httpserver->gethandlefunc(path);
			if (cpps::type(func) == CPPS_TFUNCTION)
			{
				cpps_socket_httpserver_bindsession(httpserver, cpps_request_ptr);
				dofunction(httpserver->c, func, cpps_request_var);
				return;
			}
			//2.找controller.
			cpps::object controller_object;
			const cpps_socket_httpserver_controller controller = cpps_socket_httpserver_parse_controlloer(path);
			if (path == "/") //找默认controller.
			{
				controller_object = httpserver->http_default_class_route;
			}
			else
			{
				controller_object = httpserver->getcontroller(controller.controllername);
			}
			if (cpps::type(controller_object) == CPPS_TCLASS)
			{
				cpps::cpps_cppsclass* cppsclass = cpps_to_cpps_cppsclass(controller_object.value);
				if (cppsclass->iscppsclass())
				{
					std::string method = "index";
					if (!controller.method.empty()) method = controller.method;
					cpps_domain* leftdomain = NULL;
					cpps_regvar *var = cppsclass->getvar(method, leftdomain, true,true);
					if (var) //找到method了.
					{
						//创建变量.
						cpps_socket_httpserver_bindsession(httpserver, cpps_request_ptr);
						cpps::object cppsclassvar;
						newcppsclasvar(httpserver->c, cppsclass, &cppsclassvar.getval());
						doclassfunction(httpserver->c, cppsclassvar, var->getval(), cpps_request_var);
						return;
					}
				}
			}
			//3.找本地文件(cache么?)
			std::string filepath = cpps_getcwd() + httpserver->getwwwroot() + path;
			std::string ext = cpps_io_getfileext(path);
			if (ext.empty()) {
				filepath += "/index.html";
				ext = "html";
			}
			bool b = cpps_io_file_exists(filepath);
			if (b)
			{
				const std::string& mime_type = httpserver->get_type_ref(ext);
				if (!mime_type.empty()) {

					cpps_request_ptr->real_addheader(CACHE_CONTROL, "max-age=2592000, public");
					cpps_request_ptr->real_addheader(CONTENT_TYPE, mime_type.c_str());
					cpps_request_ptr->real_addheader(SERVER_HEADER, "Cpps Server");
					cpps_request_ptr->real_addheader(CONNECTION, "close");

					cpps_integer last_write_time = cpps_io_last_write_time(filepath);
					auto cachefile = httpserver->get_cachefile(path);
					if (cachefile == NULL )
					{
						std::string content = cpps_io_readfile(filepath);
						cachefile = httpserver->create_cachefile(path, content, last_write_time);
					}
					if (cachefile->getlast_write_time() != last_write_time) {
						std::string content = cpps_io_readfile(filepath);
						cachefile->setcontent(content);
					}

					cpps_request_ptr->append(cachefile->getcontent());

					cpps_request_ptr->send(200, "OK");

					return;
				}
				
			}
			//4.找不到执行notfound
			func = httpserver->http_option.notfoundfunc;
			if (cpps::type(func) == CPPS_TFUNCTION)
			{
				dofunction(httpserver->c, func, cpps_request_var);
				return;
			}

			//4.没有绑定notfound的话...
			cpps_request_ptr->append("Sorry,not found.\n");
			cpps_request_ptr->send( 404, "NOT FOUND");
		}
		catch (cpps_error e)
		{

			//5.脚本异常的话...
			std::string errmsg;
			char errbuffer[8192];
			sprintf(errbuffer, "error: %d : %s file:%s line:%d \nError stack information:\n", e.error(), e.what().c_str(), e.file().c_str(), e.line());
			errmsg.append(errbuffer);
			std::vector<cpps_stack*>* stacklist = httpserver->c->getcallstack();
			for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)
			{
				cpps::cpps_stack* stack = *it;
				sprintf(errbuffer, "file:%s [%d] %s\n", stack->f, stack->l, stack->func);
				errmsg.append(errbuffer);
			}
			cpps_pop_stack_to_here(httpserver->c, takestack); //清栈

			cpps::object func = httpserver->http_option.exceptionfunc;
			if (cpps::type(func) == CPPS_TFUNCTION)
			{
				dofunction(httpserver->c, func, cpps_request_var, errmsg);
				return;
			}

			//6.没有找到脚本异常的话...

			cpps_request_ptr->append("Sorry,SERVER ERROR.\n");
			cpps_request_ptr->send(500, "SERVER ERROR");
		}
	}

	void cpps_socket_httpserver::register_handlefunc(std::string path, cpps::object func)
	{
		cpps_string_real_tolower(path);
		http_route_list[path] = func;
	}

	void cpps_socket_httpserver::register_controller(cpps::object cls, cpps_value defaultset)
	{
		bool bdefaultset = defaultset.tt == CPPS_TBOOLEAN ? defaultset.value.b : false;
		if (cpps::type(cls) == CPPS_TCLASS)
		{
			cpps::cpps_cppsclass* cppsclass = cpps_to_cpps_cppsclass(cls.value);
			if (cppsclass->iscppsclass())
			{
				std::string s = cppsclass->getclassname();
				cpps_string_real_tolower(s);
				http_class_route_list[s] = cls;
				if(bdefaultset)
					http_default_class_route = cls;
			}
		}
	}

	bool cpps_socket_httpserver::isrunning()
	{
		return http_running;
	}

	void cpps_socket_httpserver::run()
	{
		cpps_socket_server::run();
		update_session();
	}

	void cpps_socket_httpserver::stop()
	{
		cpps_socket_server::stop();

		for (auto session : session_list)
		{
			cpps_socket_httpserver_session* sess = session.second;
			CPPSDELETE(sess);
		}
		session_list.clear();
		auto it = cachefile_list.begin();
		for (; it != cachefile_list.end(); ++it) {
			cpps_socket_httpserver_cachefile* file = it->second;
			CPPSDELETE( file);

		}
		cachefile_list.clear();
		http_running = false;
	}

	void cpps_socket_httpserver::add_type(std::string mime, std::string ext)
	{
		mime_types.insert(http_mime_type::value_type(ext, mime));
	}

	const std::string& cpps_socket_httpserver::get_type_ref(std::string& ext)
	{
		return mime_types[ext];
	}
	std::string cpps_socket_httpserver::get_type(std::string ext)
	{
		return mime_types[ext];
	}

	cpps::object cpps_socket_httpserver::gethandlefunc(std::string path)
	{
		cpps::object ret;
		auto it = http_route_list.find(path);
		if (it != http_route_list.end()) {
			ret = it->second;
		}
		return ret;
	}

	cpps::object cpps_socket_httpserver::getcontroller(std::string controllername)
	{
		cpps::object ret;
		auto it = http_class_route_list.find(controllername);
		if (it != http_class_route_list.end()) {
			ret = it->second;
		}
		return ret;
	}
	

	cpps::cpps_socket_httpserver_session* cpps_socket_httpserver::create_seesion(cpps::C* c)
	{
		std::string sesionid = object_cast<std::string>(dofunction(c, createuuidfunc));
		std::string csrf_token = object_cast<std::string>(dofunction(c, createuuidfunc));
		if (sesionid.empty()) return NULL;

		cpps_socket_httpserver_session* session = CPPSNEW( cpps_socket_httpserver_session)();
		session->session_id = sesionid;
		session->set_expire(cpps_time_gettime() + SESSION_COOKIE_AGE);
		session->set("csrftoken", cpps_value(c,csrf_token));
		session_list.insert(http_session_list::value_type(sesionid, session));
		return session;
	}

	cpps::cpps_socket_httpserver_session* cpps_socket_httpserver::get_session(std::string session_id)
	{
		cpps_socket_httpserver_session* ret = NULL;
		auto it = session_list.find(session_id);
		if (it != session_list.end())
		{
			ret = it->second;
		}
		return ret;
	}

	cpps::cpps_socket_httpserver_cachefile* cpps_socket_httpserver::create_cachefile(std::string& filepath, std::string& content,cpps_integer last_write_time)
	{
		cpps_socket_httpserver_cachefile* cachefile = CPPSNEW( cpps_socket_httpserver_cachefile)();
		cachefile->setfilepath(filepath);
		cachefile->setcontent(content);
		cachefile->setlast_write_time(last_write_time);

		cachefile_list.insert(http_cachefile_list::value_type(filepath, cachefile));
		return cachefile;
	}

	cpps::cpps_socket_httpserver_cachefile* cpps_socket_httpserver::get_cachefile(std::string filepath)
	{
		cpps_socket_httpserver_cachefile* ret = NULL;
		auto it = cachefile_list.find(filepath);
		if (it != cachefile_list.end())
		{
			ret = it->second;
		}
		return ret;
	}

	int header_field_cb(http_parser* p, const char* buf, size_t len)
	{
		http_request* req = (struct http_request*)p->data;
		req->field.clear();
		req->field.append(buf, len);
		return 0;
	}

	int header_value_cb(http_parser* p, const char* buf, size_t len)
	{
		http_request* req = (struct http_request*)p->data;
		std::string value;
		value.append(buf, len);
		if ("Accept-Encoding" == req->field) {
			req->support_gzip = value.find("gzip") != std::string::npos;
		}
		req->headers[req->field] = value;
		return 0;
	}

	int request_url_cb(http_parser* p, const char* buf, size_t len)
	{
		http_request* req = (struct http_request*)p->data;
		std::string url;
		url.append(buf, len);
		size_t pos = url.find('?');
		req->path.append(url.substr(0,pos));
		if (pos != std::string::npos) 
			req->uri.append(url.substr(pos + 1));
		return 0;
	}



	int body_cb(http_parser* p, const char* buf, size_t len)
	{
		return 0;
	}

	int headers_complete_cb(http_parser* p)
	{
		http_request* req = (struct http_request*)p->data;
		req->keepalive = http_should_keep_alive(p);
		return -1;
	}

	int message_begin_cb(http_parser* p)
	{
		http_request* req = (struct http_request*)p->data;
		req->method = http_method_str( (http_method)p->method );
		return 0;
	}
	int message_complete_cb(http_parser* p)
	{
		return 0;
	}

	int chunk_header_cb(http_parser* parser)
	{
		return 0;
	}

	int chunk_complete_cb(http_parser* p)
	{
		return 0;
	}
	
	void cpps_socket_httpserver::onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		if (nread > 0)
		{
			client->readbuffer(buf, nread);
			http_request request;
			http_parser parser;
			request.support_gzip = false;
			request.body_len = 0;
			request.header_len = 0;
			parser.data = (void*)&request;
			http_parser_init(&parser, HTTP_REQUEST);
			http_parser_settings settings = { message_begin_cb, request_url_cb, nullptr, header_field_cb, header_value_cb,
				headers_complete_cb, body_cb, message_complete_cb, chunk_header_cb, chunk_complete_cb };

			size_t parsed = http_parser_execute(&parser, &settings, client->buffer.c_str(), client->buffer.size());
			if (!request.headers["Content-Length"].empty())
			{
				auto content_length = atoll(request.headers["Content-Length"].c_str());
				if ((client->buffer.size() - parsed) < (size_t)content_length)
				{
					return;
				}
				request.body_buf = client->buffer.c_str() + parsed + 1;
				request.body_len = size_t(content_length);

			}
			else if ((parsed + 1) != client->buffer.size()) {
				return;
			}

			request.header_buf = client->buffer.c_str();
			request.header_len = parsed + 1;

			request.ip_address = client->socket_ip;
			request.port = (usint16)client->socket_port;

			//request.body.append(request.body_buf, request.body_len);
			request.socket_index = client->socket_index;
			client->recv_count++;
			generic_handler(request, this);
			client->buffer.clear();
		}
		else if (nread < 0)
		{
			client->close("normal close.", onClsoe);
		}
	}

	void cpps_socket_httpserver::onWriteCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		if (cpps::type(http_option.writefunc) == CPPS_TFUNCTION)
		{
			cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
			std::string _s(buf, nread);
			cpps::dofunction(c, http_option.writefunc, _s, this, client->socket_index);

			//close
			/*if (client->isShutdown() && client->get_send_count() == client->get_write_count()) {
				closesocket(client->socket_index);
			}*/
		}
	}

	void cpps_socket_httpserver::onClsoeCallback(cpps_socket* sock)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		request_list.erase(client->socket_index);
	}

	void cpps_socket_httpserver::update_session()
	{
		auto it = session_list.begin();
		for (; it != session_list.end();) {
			auto session = it->second;
			if (cpps_time_gettime() >= session->session_expire || session->needremove) {
				CPPSDELETE(session);
				it = session_list.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void cpps_socket_httpserver::cpps_socket_httpserver_bindsession(cpps_socket_httpserver* httpserver, cpps_socket_httpserver_request* cpps_request_ptr,bool create_session)
	{
		if (httpserver->SESSION_ENABLED) {
			std::string sessionid = cpps_request_ptr->getcookie(httpserver->SESSION_COOKIE_NAME);
			cpps_socket_httpserver_session* session = NULL;
			if (!sessionid.empty()) {
				//check session;
				session = httpserver->get_session(sessionid);
				if (session) {
					session->set_expire(cpps_time_gettime() + httpserver->SESSION_COOKIE_AGE);//加时.
				}
			}
			//不应该直接创建session.应该由后台创建.
			if (!session && create_session) {
				session = httpserver->create_seesion(httpserver->c);
				cpps_request_ptr->setcookie(httpserver->SESSION_COOKIE_NAME, session->session_id, cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_PATH), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_DOMAIN), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_AGE));
				object csrftoken = session->get("csrftoken", nil);
				cpps_request_ptr->setcookie("csrftoken", csrftoken.tostring(), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_PATH), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_DOMAIN), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_AGE));
			}
			cpps_request_ptr->setsession(session);
		}
	}

	std::string cpps_socket_httpserver::getwwwroot()
	{
		return _wwwroot ;
	}
	void cpps_socket_httpserver::setwwwroot(std::string wwwroot)
	{
		_wwwroot = wwwroot + "/";
	}

	void cpps_socket_httpserver::setuserdata(cpps::object userdata)
	{
		http_option.userdata = userdata;
	}

	cpps_value cpps_socket_httpserver::getuserdata()
	{
		return http_option.userdata.getval();
	}
	cpps_value cpps_socket_httpserver::get_request(cpps_integer socket_index)
	{
		cpps_value ret;
		http_request_list::iterator it = request_list.find(socket_index);
		if (it != request_list.end()) {
			ret = it->second;
		}
		return ret;
	}
}