#include "stdafx.h"
#include "cpps_socket_server.h"
#include "cpps_socket_server_client.h"


namespace cpps {
	cpps_socket_server::cpps_socket_server()
	{
		ev_base = NULL;
		inc_socket_index = 1;
		ev_listener = NULL;
		ev_socket = 0;
		c = NULL;
	}

	cpps_socket_server::~cpps_socket_server()
	{
		stop();
	}

	void cpps_socket_server::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}

	cpps_socket_server* cpps_socket_server::setoption(cpps::C* cstate,cpps::object opt)
	{
		setcstate(cstate);
		if(cpps::type(opt["ip"]) == CPPS_TSTRING) server_option.option_ip =  object_cast<std::string>(opt["ip"]);
		server_option.option_accept = opt["accept"];
		server_option.option_data = opt["data"];
		server_option.option_close = opt["close"];
		server_option.option_parser = opt["parser"];
		if (cpps::type(opt["headersize"]) == CPPS_TINTEGER) server_option.option_headsize = object_cast<cpps_integer>(opt["headersize"]);
		server_option.isset = true;
		return this;
	}

	int cpps_socket_server::get_addrinfo(const struct sockaddr* addr, std::string& ip, cpps::usint16& port)
	{
		void* numeric_addr = NULL;
		char addr_buff[INET6_ADDRSTRLEN];

		if (AF_INET == addr->sa_family)
		{
			numeric_addr = &((struct sockaddr_in*)addr)->sin_addr;
			port = ntohs(((struct sockaddr_in*)addr)->sin_port);
		}
		else if (AF_INET6 == addr->sa_family)
		{
			numeric_addr = &((struct sockaddr_in6*)addr)->sin6_addr;
			port = ntohs(((struct sockaddr_in6*)addr)->sin6_port);
		}
		else
		{
			return -1;
		}

		if (NULL != inet_ntop(addr->sa_family, numeric_addr, addr_buff, sizeof(addr_buff)))
			ip = addr_buff;
		else
			return -1;

		return 0;
	}

	cpps_socket_server* cpps_socket_server::listen(cpps::usint16 port)
	{
		if (server_option.isset == false) return this;

		struct addrinfo SocketAddr, * res = NULL, * ressave = NULL;
		memset(&SocketAddr, 0, sizeof(SocketAddr));
		SocketAddr.ai_flags = AI_PASSIVE;
		SocketAddr.ai_family = AF_UNSPEC;
		SocketAddr.ai_socktype = SOCK_STREAM;
		SocketAddr.ai_protocol = IPPROTO_IP;
		int  ret;
		char s_port[256];
#ifdef WIN32
		_itoa_s(port, s_port, 10);

		if (0 != (ret = getaddrinfo(server_option.option_ip.empty() ? NULL : (PCSTR)server_option.option_ip.c_str(), s_port, &SocketAddr, &res)))
			return this;
#else
		sprintf(s_port, "%d", port);
		if (0 != (ret = getaddrinfo(server_option.option_ip.empty() ? NULL : server_option.option_ip.c_str(), s_port, &SocketAddr, &res)))
		{
			printf("getaddrinfo error...\r\n");
			return this;
		}
#endif

		struct event_config* cfg = event_config_new();

#ifdef _WIN32
		evthread_use_windows_threads();
#else

		if (event_config_require_features(cfg, EV_FEATURE_ET) == -1)
		{
			printf("event_config_require_features error...\r\n");
			return this;
		}
		event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
		evthread_use_pthreads();

#endif

		ev_base = event_base_new_with_config(cfg);
		if (!ev_base) {
			printf("event_base_new_with_config error...\r\n"); return NULL;
		}

		ressave = res;

		//尝试绑定端口
		while (res != NULL) {
			ev_listener = evconnlistener_new_bind(ev_base, cb_listener, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE | LEV_OPT_THREADSAFE, -1, (struct  sockaddr*) res->ai_addr, (int)res->ai_addrlen);
			if (!ev_listener)
			{
				res = res->ai_next;
				continue;
			}
			break;
		}
		freeaddrinfo(ressave);
		//绑定失败
		if (NULL == res || NULL == ev_listener)
		{
			printf("evconnlistener_new_bind error...\r\n");
			event_base_free(ev_base);
			ev_base = NULL;
			return this;
		}
		
		ev_socket = evconnlistener_get_fd(ev_listener);
		evconnlistener_set_error_cb(ev_listener, &cb_listener_error);
		sever_running = true;
		return this;
	}

	void cpps_socket_server::cb_listener(struct evconnlistener* ev_listener, evutil_socket_t fd, struct sockaddr* addr, int len, void* ptr)
	{
		cpps_socket_server* srv = (cpps_socket_server*)ptr;
		std::string ip;
		cpps::usint16 port;
		srv->get_addrinfo((struct sockaddr*) addr, ip, port);

		cpps_socket_server_client* client = srv->create_server_client(); //可能需要一个池呀.
		if (client == NULL) return;
		if (!client->create(srv->ev_base, fd)) {
			delete client;
			return;
		}
		client->set_client_info(ip, port);

		if (cpps::type(srv->server_option.option_accept) == CPPS_TFUNCTION)
		{
			cpps::dofunction(srv->c, srv->server_option.option_accept, client->socket_index);
		}


	}
	cpps_socket_server_client* cpps_socket_server::create_server_client()
	{
		cpps_socket_server_client* client = new cpps_socket_server_client();
		client->socket_index = inc_socket_index++;
		client->setServerHandle(this);
		server_client_list.insert(socket_list::value_type(client->socket_index,client));
		return client;
	}
	void cpps_socket_server::free_server_client(cpps_socket_server_client *client)
	{
		server_client_list.erase(client->socket_index);
		client->close();
		delete client;
	}

	void cpps_socket_server::send(cpps_integer socketIndex, cpps::Buffer* buffer)
	{
		cpps_socket_server_client* client = getclient(socketIndex);
		if (client)
		{
			client->send(buffer);
		}
	}

	cpps_socket_server_client* cpps_socket_server::getclient(cpps_integer socketIndex)
	{
		cpps_socket_server_client* client = NULL;
		socket_list::iterator it = server_client_list.find(socketIndex);
		if (it != server_client_list.end())
		{
			client = it->second;
		}
		return client;
	}

	void cpps_socket_server::run()
	{
		if(ev_base)	event_base_loop(ev_base, EVLOOP_NONBLOCK);
	}

	void cpps_socket_server::closesocket(cpps_integer socketIndex)
	{
		cpps_socket_server_client* client = getclient(socketIndex);
		if (client)
		{
			if (cpps::type(server_option.option_close) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, server_option.option_close, client->socket_index,-1,"server close the socket.");
			}

			free_server_client(client);
		}

	}

	void cpps_socket_server::stop()
	{
		for (auto client : server_client_list)
		{
			client.second->close();
			delete client.second;
		}
		server_client_list.clear();
		if (ev_base) {

			event_base_free(ev_base);
			ev_listener = NULL;
			inc_socket_index = 1;
			ev_socket = 0;
		}
		sever_running = false;

	}

	bool cpps_socket_server::isrunning()
	{
		return sever_running;
	}

	void cpps_socket_server::cb_listener_error(struct evconnlistener* ev_listener, void* pCtx)
	{
		struct event_base* ev_base = evconnlistener_get_base(ev_listener);
		int err = EVUTIL_SOCKET_ERROR();
		std::string error = evutil_socket_error_to_string(err);
		event_base_loopexit(ev_base, NULL);
	}
	void cpps_socket_server::onReadCallback(cpps_socket* sock, struct bufferevent* bv)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		bufferevent_read_buffer(bv, client->socket_evbuffer);
		cpps_integer packetsize = evbuffer_get_length(client->socket_evbuffer);

		cpps_create_class_var(cpps::Buffer,c, buffer_var, buffer_ptr);

		if (server_option.option_headsize == 0)
		{
			buffer_ptr->realloc(packetsize);
			evbuffer_remove(client->socket_evbuffer, buffer_ptr->getbuffer(), packetsize);

			if (cpps::type(server_option.option_data) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, server_option.option_data, client->socket_index, buffer_var);
			}
		}
		else
		{
			while (packetsize >= server_option.option_headsize)
			{
				buffer_ptr->realloc(server_option.option_headsize);
				buffer_ptr->seek(0);
				evbuffer_copyout(client->socket_evbuffer, buffer_ptr->getbuffer(), server_option.option_headsize);
				if (cpps::type(server_option.option_parser) == CPPS_TFUNCTION)
				{
					cpps_integer size = object_cast<cpps_integer>(cpps::dofunction(c, server_option.option_parser,  buffer_var));
					if (size == -1)
					{
						//说明包头异常关闭它.
						closesocket(client->socket_index);
						return;
					}
					else if (size == 0)
					{
						break; //说明包不全.
					}
					else if (size <= packetsize)
					{
						buffer_ptr->realloc(size);
						buffer_ptr->seek(0);
						evbuffer_remove(client->socket_evbuffer, buffer_ptr->getbuffer(), size);
						if (cpps::type(server_option.option_data) == CPPS_TFUNCTION)
						{
							cpps::dofunction(c, server_option.option_data, client->socket_index, buffer_var);
						}
						packetsize = evbuffer_get_length(client->socket_evbuffer);
					}
				}
				else
				{
					closesocket(client->socket_index);
					return;
				}
			}
		}

	}

	void cpps_socket_server::onWriteCallback(cpps_socket* sock, struct bufferevent* bv)
	{
		//do nothing..
	}

	void cpps_socket_server::onEventCallback(cpps_socket* sock, short e)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		if (e & BEV_EVENT_EOF)
		{
			if (cpps::type(server_option.option_close) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, server_option.option_close, client->socket_index,0,"normal close socket.");
			}

			free_server_client(client);
		}
		else if (e & BEV_EVENT_ERROR)
		{
			int err = EVUTIL_SOCKET_ERROR();

			if (cpps::type(server_option.option_close) == CPPS_TFUNCTION)
			{
#ifdef WIN32
				cpps::dofunction(c, server_option.option_close, client->socket_index,err, cpps_socket_g2u(evutil_socket_error_to_string(err)));
#else
				cpps::dofunction(c, server_option.option_close, client->socket_index,err, evutil_socket_error_to_string(err));
#endif
			}

			free_server_client(client);
		}
	}
}