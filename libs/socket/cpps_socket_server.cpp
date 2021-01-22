#include "cpps_socket_server.h"
#include "cpps_socket_server_client.h"


namespace cpps {
	cpps_socket_server::cpps_socket_server()
	{
		
		inc_socket_index = 1;
		c = NULL;
		sever_running = false;
		uv_loop = CPPSNEW( uv_loop_t)();
		uv_loop_init(uv_loop);

	}

	cpps_socket_server::~cpps_socket_server()
	{
		if (uv_loop)
		{
			uv_loop_close(uv_loop);
			CPPSDELETE( uv_loop );
			uv_loop = NULL;
		}
		for (auto client : server_client_list) {
			CPPSDELETE(client.second);
		}
		server_client_list.clear();
	}

	void cpps_socket_server::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}

	cpps_socket_server* cpps_socket_server::setoption(cpps::object opt)
	{
		if(cpps::type(opt["ip"]) == CPPS_TSTRING) server_option.option_ip =  object_cast<std::string>(opt["ip"]);
		server_option.option_accept = opt["accept"];
		server_option.option_data = opt["data"];
		server_option.option_close = opt["close"];
		server_option.option_parser = opt["parser"];
		if (cpps::type(opt["headersize"]) == CPPS_TINTEGER) server_option.option_headsize = object_cast<cpps_integer>(opt["headersize"]);
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

	cpps_socket_server* cpps_socket_server::listen(cpps::C* cstate, cpps::usint16 port)
	{
		if (sever_running) return this;

		setcstate(cstate);

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
	

		uv_tcp_init(uv_loop, &uv_socket);

		ressave = res;
		int err = 0;
		//尝试绑定端口
		while (res != NULL) {

			err = uv_tcp_bind(&uv_socket, (const struct sockaddr*)res->ai_addr, 0);
			if (err != 0)
			{
				res = res->ai_next;
				continue;
			}
			break;
		}
		freeaddrinfo(ressave);
		//绑定失败
		if (NULL == res )
		{
			fprintf(stderr, "Listen error %s\n", uv_strerror(err));
			return this;
		}
		uv_socket.data = (void*)this;
		err = uv_listen((uv_stream_t*)&uv_socket, 5000, cb_listener);
		if (err) {
			fprintf(stderr, "Listen error %s\n", uv_strerror(err));
			return this;
		}
		//暂不做异步.异步的话整个结构都需要修改.
// 		async_send_msg.data = (void*)this;
// 		uv_async_init(uv_loop, &async_send_msg, write_task_cb);


		sever_running = true;
		return this;
	}

	void cpps_socket_server::cb_listener(uv_stream_t* server, int status)
	{
		cpps_socket_server* srv = (cpps_socket_server*)server->data;
		if (status < 0) {
			fprintf(stderr, "New connection error %s\n", uv_strerror(status));
			return;
		}

		cpps_socket_server_client* client = srv->create_server_client(); //可能需要一个池呀.
		uv_tcp_t* fd = CPPSNEW( uv_tcp_t)();
		uv_tcp_init(srv->uv_loop, fd);
		fd->data = (void*)client;
		client->create(fd);
		if (uv_accept(server, (uv_stream_t*)fd) != 0) {
			client->close("server accept error.", onClsoeCallback);
			return;
		}
		struct sockaddr  peername;
		int namelen;
		uv_tcp_getpeername(fd, &peername, &namelen);


		std::string ip;
		cpps::usint16 port;
		srv->get_addrinfo((struct sockaddr*) &peername, ip, port);

		if (client == NULL) return;
		if (!client->read_start()) {
			client->close("client create faild.", onClsoeCallback);
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
		cpps_socket_server_client* client = CPPSNEW( cpps_socket_server_client)();
		client->socket_index = inc_socket_index++;
		client->setServerHandle(this);
		server_client_list.insert(socket_list::value_type(client->socket_index,client));
		return client;
	}
	void cpps_socket_server::free_server_client(cpps_socket_server_client *client)
	{
		server_client_list.erase(client->socket_index);
		CPPSDELETE( client);
	}

	void cpps_socket_server::sends(cpps_integer socketIndex, std::string& buffer)
	{
		cpps_socket_server_client* client = getclient(socketIndex);
		if (client)
		{
			client->sends(buffer);
		}
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
		if (uv_loop)	uv_run(uv_loop, UV_RUN_NOWAIT);
	}

	void cpps_socket_server::closesocket(cpps_integer socketIndex)
	{
		cpps_socket_server_client* client = getclient(socketIndex);
		if (client)
		{
			client->close("server close the socket", onClsoeCallback);
		}

	}
	void cpps_socket_server::stop_cb(uv_handle_t* handle)
	{
		cpps_socket_server* server = (cpps_socket_server*)handle->data;
		server->stoped();
	}
	void cpps_socket_server::stoped()
	{
		for (auto client : server_client_list)
		{
			client.second->close("server close", onClsoeCallback);
		}
	}
	void cpps_socket_server::stop()
	{
		if (!sever_running) return;
		uv_tcp_close_reset(&uv_socket, stop_cb);
		sever_running = false;
	}

	bool cpps_socket_server::isrunning()
	{
		return sever_running;
	}


	void cpps_socket_server::onReadCallback(cpps_socket* sock, ssize_t nread, const uv_buf_t* buf)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		if (nread > 0)
		{
			client->readbuffer( buf, nread);
			cpps_integer packetsize = (cpps_integer)client->get_buffer_length();
			cpps_create_class_var(cpps::Buffer, c, buffer_var, buffer_ptr);
			if (server_option.option_headsize == 0)
			{
				buffer_ptr->clear();
				buffer_ptr->realloc(packetsize);
				client->buffer_remove( buffer_ptr->getbuffer(), packetsize);

				if (cpps::type(server_option.option_data) == CPPS_TFUNCTION)
				{
					cpps::dofunction(c, server_option.option_data, client->socket_index, buffer_var);
				}
			}
			else {
				
				while (packetsize >= server_option.option_headsize)
				{
					buffer_ptr->clear();
					buffer_ptr->realloc(server_option.option_headsize);
					buffer_ptr->seek(0);
					client->buffer_copyout( buffer_ptr->getbuffer(), server_option.option_headsize);
					if (cpps::type(server_option.option_parser) == CPPS_TFUNCTION)
					{
						cpps_integer size = object_cast<cpps_integer>(cpps::dofunction(c, server_option.option_parser, buffer_var));
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
							buffer_ptr->clear();
							buffer_ptr->realloc(size);
							buffer_ptr->seek(0);
							client->buffer_remove( buffer_ptr->getbuffer(), size);
							if (cpps::type(server_option.option_data) == CPPS_TFUNCTION)
							{
								cpps::dofunction(c, server_option.option_data, client->socket_index, buffer_var);
							}
							packetsize = client->get_buffer_length();
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
		else if (nread <= 0)
		{
			client->close("normal close.", onClsoeCallback);
		}
	}
	void cpps_socket_server::onClsoeCallback(uv_handle_t* handle)
	{
		uv_tcp_t* source = (uv_tcp_t*)handle;
		cpps_socket_server_client* client = (cpps_socket_server_client*)handle->data;
	
		if (cpps::type(client->server->server_option.option_close) == CPPS_TFUNCTION)
		{
			cpps::dofunction(client->server->c, client->server->server_option.option_close, client->socket_index, 0, client->closemsg);
		}

		CPPSDELETE(source);
		client->server->free_server_client(client);
	}
}