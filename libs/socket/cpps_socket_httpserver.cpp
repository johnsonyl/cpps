#include "cpps_socket_httpserver.h"
#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_controller.h"
#include "cpps_socket_httpserver_session.h"
#include "cpps_socket_httpserver_cachefile.h"
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
		ev_base = NULL;
		ev_http = NULL;
		c = NULL;
		http_running = false;
		struct event_config* cfg = event_config_new();

#ifdef _WIN32
		evthread_use_windows_threads();
		event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);
		//根据CPU实际数量配置libEvent的CPU数
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		event_config_set_num_cpus_hint(cfg, si.dwNumberOfProcessors);
#else

		if (event_config_require_features(cfg, EV_FEATURE_ET) == -1)
		{
			printf("event_config_require_features error...\r\n");
			return;
		}
		event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
		evthread_use_pthreads();

#endif

		ev_base = event_base_new_with_config(cfg);
		if (!ev_base) {
			printf("event_base_new_with_config error...\r\n");
			return;
		}
		event_config_free(cfg);
	}

	cpps_socket_httpserver::~cpps_socket_httpserver()
	{
		stop();
		if (ev_base) {

			event_base_free(ev_base);
			ev_base = NULL;
		}
		for (auto session: session_list)
		{
			delete session.second;
		}
		session_list.clear();
		for (auto cachefile: cachefile_list)
		{
			delete cachefile.second;
		}
		cachefile_list.clear();
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
		return this;
	}

	cpps_socket_httpserver* cpps_socket_httpserver::listen(cpps::C* cstate, cpps::usint16 port)
	{
		setcstate(cstate);

		ev_http = evhttp_new(ev_base);
		if (!ev_http)
		{
			throw cpps_error(__FILE__,__LINE__,cpps_error_normalerror,"evhttp_new error...\r\n");
			return this;
		}

		int ret = evhttp_bind_socket(ev_http, http_option.option_ip.c_str(), port);
		if (ret != 0)
		{
			evhttp_free(ev_http);
			ev_http = NULL;
			return this;
		}
		evhttp_set_gencb(ev_http, generic_handler, this);

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

	void cpps_socket_httpserver::generic_handler(struct evhttp_request* req, void* handler)
	{
		cpps_socket_httpserver* httpserver = (cpps_socket_httpserver*)handler;

		cpps_create_class_var(cpps_socket_httpserver_request, httpserver->c, cpps_request_var, cpps_request_ptr);

		const struct evhttp_uri* uri = evhttp_request_get_evhttp_uri(req);
		const char* path = evhttp_uri_get_path(uri);
		const char* query = evhttp_uri_get_query(uri);
		const char* scheme = evhttp_uri_get_scheme(uri);
		const char* userinfo = evhttp_uri_get_userinfo(uri);


		cpps_request_ptr->ev_req = req;

		
		

		if (path) cpps_request_ptr->path = path;
		if (scheme) cpps_request_ptr->scheme = scheme;
		if (userinfo) cpps_request_ptr->userinfo = userinfo;

		if (query)
		{
			cpps_request_ptr->uri = query;

			char* decode_query = evhttp_decode_uri(query);
			struct evkeyvalq params;
			evhttp_parse_query_str(decode_query, &params);

			for (struct evkeyval* header = params.tqh_first; header; header = header->next.tqe_next) {
				cpps_request_ptr->paramslist.insert(PARAMSLIST::value_type(header->key, header->value));
				cpps_request_ptr->getlist.insert(PARAMSLIST::value_type(header->key, header->value));
			}
		}
		

		struct evkeyvalq* headers = evhttp_request_get_input_headers(req);
		for (struct evkeyval* header = headers->tqh_first; header; header = header->next.tqe_next) {
			cpps_request_ptr->input_headerslist.insert(PARAMSLIST::value_type(header->key, header->value));

		}
		struct evbuffer* ib = evhttp_request_get_input_buffer(req);
		size_t ibsize = evbuffer_get_length(ib);
		if (ibsize > 0)
		{
			char* ib_buffer = new char[ibsize + 1];
			memset(ib_buffer, 0, ibsize + 1);
			evbuffer_remove(ib, ib_buffer, ibsize);
			cpps_request_ptr->input_buffer.append(ib_buffer, ibsize);

			if (cpps_request_ptr->isformdata()) {
				cpps_request_ptr->parse_form_data(cpps_request_ptr->input_buffer);
			}
			else {
				struct evkeyvalq post_params;
				evhttp_parse_query_str(ib_buffer, &post_params);

				for (struct evkeyval* header = post_params.tqh_first; header; header = header->next.tqe_next) {
					cpps_request_ptr->paramslist.insert(PARAMSLIST::value_type(header->key, header->value));
					cpps_request_ptr->postlist.insert(PARAMSLIST::value_type(header->key, header->value));
				}

			}

			
			delete[] ib_buffer;
		}
		cpps_stack* takestack = httpserver->c->getcallstack()->empty() ? NULL : httpserver->c->getcallstack()->at(httpserver->c->getcallstack()->size() - 1);

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
			if (!session) {
				session = httpserver->create_seesion(httpserver->c);
				cpps_request_ptr->setcookie(httpserver->SESSION_COOKIE_NAME, session->session_id, cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_PATH), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_DOMAIN), cpps::object::create(httpserver->c,httpserver->SESSION_COOKIE_AGE));
				object csrftoken = session->get("csrftoken", nil);
				cpps_request_ptr->setcookie("csrftoken", csrftoken.tostring(), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_PATH), cpps::object::create(httpserver->c, httpserver->SESSION_COOKIE_DOMAIN), cpps::object::create(httpserver->c,httpserver->SESSION_COOKIE_AGE));
			}
			cpps_request_ptr->setsession(session);
		}
		try
		{
			//1.先找路由
			cpps_string_real_tolower(cpps_request_ptr->path);
			cpps::object func = httpserver->gethandlefunc(cpps_request_ptr->path);
			if (cpps::type(func) == CPPS_TFUNCTION)
			{
				dofunction(httpserver->c, func, cpps_request_var);
				return;
			}
			//2.找controller.
			cpps::object controller_object;
			const cpps_socket_httpserver_controller controller = cpps_socket_httpserver_parse_controlloer(cpps_request_ptr->path);
			if (cpps_request_ptr->path == "/") //找默认controller.
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
					cpps_regvar *var = cppsclass->getvar(method, leftdomain, true);
					if (var) //找到method了.
					{
						//创建变量.
						cpps::object cppsclassvar = newcppsclasvar(httpserver->c, cppsclass);
						doclassfunction(httpserver->c, cppsclassvar, var->getval(), cpps_request_var);
						return;
					}
				}
			}
			//3.找本地文件(cache么?)
			std::string filepath = cpps_getcwd() + cpps_request_ptr->path;
			std::string ext = cpps_io_getfileext(cpps_request_ptr->path);
			if (ext.empty()) {
				filepath += "/index.html";
				ext = "html";
			}
			bool b = cpps_io_file_exists(filepath);
			if (b)
			{
				std::string mime_type = httpserver->get_type(ext);
				if (!mime_type.empty()) {
					std::string content_type = "Content-Type";
					std::string Server = "Server";
					std::string ServerName = "Cpps Server";
					std::string Connection = "Connection";
					std::string ConnectionType = "close";
					cpps_request_ptr->real_addheader(content_type, mime_type);
					cpps_request_ptr->real_addheader(Server, ServerName);
					cpps_request_ptr->real_addheader(Connection, ConnectionType);

					cpps_integer last_write_time = cpps_io_last_write_time(filepath);
					auto cachefile = httpserver->get_cachefile(cpps_request_ptr->path);
					if (cachefile == NULL )
					{
						std::string content = cpps_io_readfile(filepath);
						cachefile = httpserver->create_cachefile(cpps_request_ptr->path, content, last_write_time);
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
			struct evbuffer* buf = evhttp_request_get_output_buffer(req);
			evbuffer_add_printf(buf, "sorry,not found.\n");
			evhttp_send_reply(req, HTTP_NOTFOUND, "NOT FOUND", buf);
		}
		catch (cpps_error e)
		{

			//5.脚本异常的话...
			std::string errmsg;
			char errbuffer[1024];
			sprintf(errbuffer, "error: %d : %s file:%s line:%d \nError stack information:\n", e.erron, e.s.c_str(), e.file.c_str(), e.line);
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
			struct evbuffer* buf = evhttp_request_get_output_buffer(req);
			evbuffer_add_printf(buf, "It Error Page!\n%s", errmsg.c_str());
			evhttp_send_reply(req, HTTP_INTERNAL, "SERVER ERROR", buf);
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
		if (ev_base)	event_base_loop(ev_base, EVLOOP_NONBLOCK);
		update_session();
	}

	void cpps_socket_httpserver::stop()
	{
		if (ev_http)
		{
			evhttp_free(ev_http);
			ev_http = NULL;
		}
		http_running = false;
	}

	void cpps_socket_httpserver::add_type(std::string mime, std::string ext)
	{
		mime_types.insert(http_mime_type::value_type(ext, mime));
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

		cpps_socket_httpserver_session* session = new cpps_socket_httpserver_session();
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
		cpps_socket_httpserver_cachefile* cachefile = new cpps_socket_httpserver_cachefile();
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

	void cpps_socket_httpserver::update_session()
	{
		auto it = session_list.begin();
		for (; it != session_list.end();) {
			auto session = it->second;
			if (cpps_time_gettime() >= session->session_expire || session->needremove) {
				delete session;
				it = session_list.erase(it);
			}
			else {
				++it;
			}
		}
	}

}