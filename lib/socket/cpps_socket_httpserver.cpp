#include "cpps_socket_httpserver.h"
#include "cpps_socket_httpserver_request.h"
#include "cpps_socket_httpserver_controller.h"
namespace cpps {

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
			printf("evhttp_new error...\r\n"); return this;
		}

		int ret = evhttp_bind_socket(ev_http, http_option.option_ip.c_str(), port);
		if (ret != 0)
		{
			evhttp_free(ev_http);
			ev_http = NULL;
			return this;
		}
		evhttp_set_gencb(ev_http, generic_handler, this);

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
		if (userinfo) cpps_request_ptr->scheme = userinfo;

		if (query)
		{
			cpps_request_ptr->uri = query;

			char* decode_query = evhttp_decode_uri(query);
			struct evkeyvalq params;
			evhttp_parse_query_str(decode_query, &params);

			for (struct evkeyval* header = params.tqh_first; header; header = header->next.tqe_next) {
				cpps_request_ptr->paramslist.insert(PARAMSLIST::value_type(header->key, header->value));
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

			struct evkeyvalq post_params;
			evhttp_parse_query_str(ib_buffer, &post_params);

			for (struct evkeyval* header = post_params.tqh_first; header; header = header->next.tqe_next) {
				cpps_request_ptr->paramslist.insert(PARAMSLIST::value_type(header->key, header->value));
			}

			delete[] ib_buffer;
		}
		cpps_stack* takestack = httpserver->c->getcallstack()->empty() ? NULL : httpserver->c->getcallstack()->at(httpserver->c->getcallstack()->size() - 1);

		try
		{
			//1.先找路由
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
					std::string method = "Index";
					if (!controller.method.empty()) method = controller.method;
					cpps_domain* leftdomain = NULL;
					cpps_regvar *var = cppsclass->getvar(method, leftdomain, true);
					if (var) //找到method了.
					{
						//创建变量.
						cpps_cppsclassvar* cppsclassvar = newcppsclasvar(httpserver->c, cppsclass);
						doclassfunction(httpserver->c, cppsclassvar, var->getval(), cpps_request_var);
						return;
					}
				}
			}
			//3.找不到执行notfound
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
		http_route_list[path] = func;
	}

	void cpps_socket_httpserver::register_controller(cpps::object cls)
	{
		if (cpps::type(cls) == CPPS_TCLASS)
		{
			cpps::cpps_cppsclass* cppsclass = cpps_to_cpps_cppsclass(cls.value);
			if (cppsclass->iscppsclass())
			{
				http_class_route_list[cppsclass->getclassname()] = cls;
			}
		}
	}

	void cpps_socket_httpserver::register_controller_default(cpps::object cls)
	{
		http_default_class_route = cls;
		register_controller(cls);
	}

	bool cpps_socket_httpserver::isrunning()
	{
		return http_running;
	}

	void cpps_socket_httpserver::run()
	{
		if (ev_base)	event_base_loop(ev_base, EVLOOP_NONBLOCK);
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

	cpps::object cpps_socket_httpserver::gethandlefunc(std::string path)
	{
		cpps::object ret = http_route_list[path];
		return ret;
	}

	cpps::object cpps_socket_httpserver::getcontroller(std::string controllername)
	{
		cpps::object ret = http_class_route_list[controllername];
		return ret;
	}

}