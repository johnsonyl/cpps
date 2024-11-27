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
		ctx = NULL;
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
		server_option.option_write = opt["write"];
		server_option.option_close = opt["close"];
		server_option.option_parser = opt["parser"];
		server_option.option_ssl = opt["ssl"];
		server_option.userdata = opt["userdata"];
		server_option.option_certificate_file = opt["certificate_file"];
		server_option.option_privatekey_file = opt["privatekey_file"];
		if (cpps::type(opt["headersize"]) == CPPS_TINTEGER || cpps::type(opt["headersize"]) == CPPS_TUINTEGER) server_option.option_headsize = object_cast<cpps_integer>(opt["headersize"]);
		if (server_option.option_ssl.tobool()) {
			SSL_library_init();
			OpenSSL_add_all_algorithms();
			SSL_load_error_strings();
		}
		
		return this;
	}

	bool cpps_socket_server::is_open_ssl()
	{
		return ctx != NULL;
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
			return -2;
		}

		if (NULL != inet_ntop(addr->sa_family, numeric_addr, addr_buff, sizeof(addr_buff)))
			ip = addr_buff;
		else
			return -1;

		return 0;
	}
	static int ServerNameCallback(SSL* ssl, int* ad, void* arg)
	{
		cpps_socket_server* server = (cpps_socket_server*)arg;
		if (ssl == NULL)
			return SSL_TLSEXT_ERR_NOACK;
		const char *_servername = SSL_get_servername(ssl, TLSEXT_NAMETYPE_host_name);
		if(_servername == NULL || _servername[0] == '\0')
			return SSL_TLSEXT_ERR_NOACK;
		std::string servername = _servername;
		if (servername.empty())
			return SSL_TLSEXT_ERR_NOACK;

		if (!server->server_option.option_servername_callback.isfunction())
			return SSL_TLSEXT_ERR_NOACK;

		cpps::object ctx = cpps::dofunction(server->c, server->server_option.option_servername_callback, servername);
		if(ctx.isnull())
			return SSL_TLSEXT_ERR_NOACK;
		CPPS_SSL_CTX* _ctx = cpps::object_cast<CPPS_SSL_CTX*>(ctx);

		SSL_CTX* v = SSL_set_SSL_CTX(ssl, _ctx->ctx);
		if (v != _ctx->ctx)
			return SSL_TLSEXT_ERR_NOACK;

		return SSL_TLSEXT_ERR_OK;
	}
	cpps_socket_server* cpps_socket_server::listen(cpps::C* cstate, cpps::usint16 port,cpps::object _ssl_port)
	{
		if (sever_running) return this;
		ssl_port = port;
		if (_ssl_port.isint()) ssl_port = (cpps::usint16)_ssl_port.toint();

		setcstate(cstate);

		// port
		if (!server_option.option_ssl.tobool() || ssl_port != port)
		{
			uv_tcp_init(uv_loop, &uv_socket);
			uv_socket.data = (void*)this;
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
			if (NULL == res)
			{
				fprintf(stderr, "Listen error %s\n", uv_strerror(err));
				return this;
			}

			err = uv_listen((uv_stream_t*)&uv_socket, 5000, cb_listener);
			if (err) {
				fprintf(stderr, "Listen error %s\n", uv_strerror(err));
				return this;
			}
		}

		if(server_option.option_ssl.tobool())
		{

			uv_tcp_init(uv_loop, &uv_socket_ssl);
			uv_socket_ssl.data = (void*)this;

			struct addrinfo SocketAddr, * res = NULL, * ressave = NULL;
			memset(&SocketAddr, 0, sizeof(SocketAddr));
			SocketAddr.ai_flags = AI_PASSIVE;
			SocketAddr.ai_family = AF_UNSPEC;
			SocketAddr.ai_socktype = SOCK_STREAM;
			SocketAddr.ai_protocol = IPPROTO_IP;
			int  ret;
			char s_port[256];
#ifdef WIN32
			_itoa_s(ssl_port, s_port, 10);

			if (0 != (ret = getaddrinfo(server_option.option_ip.empty() ? NULL : (PCSTR)server_option.option_ip.c_str(), s_port, &SocketAddr, &res)))
				return this;
#else
			sprintf(s_port, "%d", ssl_port);
			if (0 != (ret = getaddrinfo(server_option.option_ip.empty() ? NULL : server_option.option_ip.c_str(), s_port, &SocketAddr, &res)))
			{
				printf("getaddrinfo error...\r\n");
				return this;
			}
#endif


			ressave = res;
			int err = 0;
			//尝试绑定端口
			while (res != NULL) {

				err = uv_tcp_bind(&uv_socket_ssl, (const struct sockaddr*)res->ai_addr, 0);
				if (err != 0)
				{
					res = res->ai_next;
					continue;
				}
				break;
			}
			freeaddrinfo(ressave);
			//绑定失败
			if (NULL == res)
			{
				fprintf(stderr, "Listen error %s\n", uv_strerror(err));
				return this;
			}

			err = uv_listen((uv_stream_t*)&uv_socket_ssl, 5000, cb_listener_ssl);
			if (err) {
				fprintf(stderr, "Listen error %s\n", uv_strerror(err));
				return this;
			}
		}

		//暂不做异步.异步的话整个结构都需要修改.
// 		async_send_msg.data = (void*)this;
// 		uv_async_init(uv_loop, &async_send_msg, write_task_cb);

		if (server_option.option_ssl.tobool()) {
			ctx = SSL_CTX_new(TLSv1_2_server_method());
			if (ctx == NULL) {
				return NULL;
			}

		
			/* 是否需要多域名多证书 */
			if (server_option.option_servername_callback.isfunction()) {

				SSL_CTX_clear_mode(ctx, SSL_MODE_AUTO_RETRY);

				SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, [](int preverify_ok, X509_STORE_CTX* x509_ctx)->int {
					return 1;
				});
				

				SSL_CTX_set_tlsext_servername_callback(ctx, ServerNameCallback);
				SSL_CTX_set_tlsext_servername_arg(ctx, this);
			}
			else {
				/* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
				if (SSL_CTX_use_certificate_file(ctx, server_option.option_certificate_file.tostring().c_str(), SSL_FILETYPE_PEM) <= 0) {
					return NULL;
				}
				/* 载入用户私钥 */
				if (SSL_CTX_use_PrivateKey_file(ctx, server_option.option_privatekey_file.tostring().c_str(), SSL_FILETYPE_PEM) <= 0) {
					return NULL;
				}
				/* 检查用户私钥是否正确 */
				if (!SSL_CTX_check_private_key(ctx)) {
					return NULL;
				}


			}
		}

		sever_running = true;
		return this;
	}

	void cpps_socket_server::cb_listener_ssl(uv_stream_t* server, int status)
	{
		cpps_socket_server* srv = (cpps_socket_server*)server->data;
		if (status < 0) {
			fprintf(stderr, "New connection error %s\n", uv_strerror(status));
			return;
		}

		cpps_socket_server_client* client = srv->create_server_client(); //可能需要一个池呀.
		uv_tcp_t* fd = CPPSNEW(uv_tcp_t)();
		uv_tcp_init(srv->uv_loop, fd);
		fd->data = (void*)client;
		client->create(fd);
		if (uv_accept(server, (uv_stream_t*)fd) != 0) {
			client->close("server accept error.", onClsoe);
			return;
		}
		struct sockaddr_storage  peername;
		int namelen = sizeof(peername);
		int err = uv_tcp_getpeername(fd, (struct sockaddr*) & peername, &namelen);
		if (err < 0)
		{
			printf("uv_tcp_getpeername failed: %s\n", uv_strerror(err));
		}
		client->ctx = srv->ctx;

		std::string ip;
		cpps::usint16 port = 9999;
		err = srv->get_addrinfo((struct sockaddr*)&peername, ip, port);
		if (err != 0)
			printf("get_addrinfo err : %d\n", err);


		if (client == NULL) return;
		if (!client->read_start()) {
			client->close("client create faild.", onClsoe);
			return;
		}

		client->set_client_info(ip, port);

		if (client->is_open_ssl()) {
			client->ssl_accept();
		}


		if (cpps::type(srv->server_option.option_accept) == CPPS_TFUNCTION)
		{
			cpps::dofunction(srv->c, srv->server_option.option_accept, client->socket_index);
		}
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
			client->close("server accept error.", onClsoe);
			return;
		}
		struct sockaddr_storage  peername;
		int namelen = sizeof(peername);
		int err = uv_tcp_getpeername(fd, (struct sockaddr*)&peername, &namelen);
		if (err < 0)
		{
			printf("uv_tcp_getpeername failed: %s\n", uv_strerror(err));
		}

		std::string ip;
		cpps::usint16 port = 9999;
		err = srv->get_addrinfo((struct sockaddr*)&peername, ip, port);
		if (err != 0)
			printf("get_addrinfo err : %d\n", err);

		if (client == NULL) return;
		if (!client->read_start()) {
			client->close("client create faild.", onClsoe);
			return;
		}

		client->set_client_info(ip, port);

		if (client->is_open_ssl()) {
			client->ssl_accept();
		}
		

		if (cpps::type(srv->server_option.option_accept) == CPPS_TFUNCTION)
		{
			cpps::dofunction(srv->c, srv->server_option.option_accept, client->socket_index);
		}
	}
	cpps_integer cpps_socket_server::get_send_count(cpps_integer socketindex)
	{
		cpps_socket_server_client* client = getclient(socketindex);
		if (client)
		{
			return client->get_send_count();
		}
		return 0;
	}
	cpps_integer cpps_socket_server::get_write_count(cpps_integer socketindex)
	{
		cpps_socket_server_client* client = getclient(socketindex);
		if (client)
		{
			return client->get_write_count();
		}
		return 0;
	}
	cpps_integer cpps_socket_server::get_recv_count(cpps_integer socketindex)
	{
		cpps_socket_server_client* client = getclient(socketindex);
		if (client)
		{
			return client->get_recv_count();
		}
		return 0;
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
			client->close("server close the socket", onClsoe);
		}

	}
	void cpps_socket_server::shutdown(cpps_integer socketIndex)
	{
		cpps_socket_server_client* client = getclient(socketIndex);
		if (client)
		{
			client->shutdown();
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
			client.second->close("server close", onClsoe);
		}
	}
	void cpps_socket_server::stop()
	{
		if (!sever_running) return;
		uv_tcp_close_reset(&uv_socket, stop_cb);
		uv_tcp_close_reset(&uv_socket_ssl, stop_cb);
		sever_running = false;
	}

	bool cpps_socket_server::isrunning()
	{
		return sever_running;
	}


	void cpps_socket_server::onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
		if (nread > 0)
		{
			client->readbuffer(buf, nread);
			cpps_integer packetsize = (cpps_integer)client->get_buffer_length();
			cpps_create_class_var(cpps::Buffer, c, buffer_var, buffer_ptr);
			if (server_option.option_headsize == 0)
			{
				buffer_ptr->clear();
				buffer_ptr->realloc(packetsize);
				client->buffer_remove( buffer_ptr->getbuffer(), size_t(packetsize));

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
					client->buffer_copyout( buffer_ptr->getbuffer(), size_t(server_option.option_headsize));
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
							client->buffer_remove( buffer_ptr->getbuffer(), size_t(size));
							client->recv_count++;
							if (cpps::type(server_option.option_data) == CPPS_TFUNCTION)
							{
								cpps::dofunction(c, server_option.option_data, client->socket_index, buffer_var);
							}
							packetsize = client->get_buffer_length();
						}
						else if(size > packetsize) {
							return;
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
			client->close("normal close.", onClsoe);
		}
	}
	void cpps_socket_server::onWriteCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		if (cpps::type(server_option.option_write) == CPPS_TFUNCTION)
		{
			cpps_socket_server_client* client = (cpps_socket_server_client*)sock;
			std::string _s(buf, nread);
			cpps::dofunction(c, server_option.option_write, _s, this, client->socket_index);
		}
	}
	void cpps_socket_server::onClsoeCallback(cpps_socket* sock)
	{

	}
	void cpps_socket_server::onClsoe(uv_handle_t* handle)
	{
		uv_tcp_t* source = (uv_tcp_t*)handle;
		cpps_socket_server_client* client = (cpps_socket_server_client*)handle->data;
		
		client->server->onClsoeCallback((cpps_socket*)handle->data);

		if (cpps::type(client->server->server_option.option_close) == CPPS_TFUNCTION)
		{
			cpps::dofunction(client->server->c, client->server->server_option.option_close, client->socket_index, 0, client->closemsg);
		}

		CPPSDELETE(source);
		client->server->free_server_client(client);
	}
	void cpps_socket_server::on_error_event(cpps_socket_server_client* client,int type)
	{
		client->close("client error_event.", onClsoe);
	}

	void cpps_socket_server::setuserdata(cpps::object userdata)
	{
		server_option.userdata = userdata;
	}

	cpps_value cpps_socket_server::getuserdata()
	{
		return server_option.userdata.getval();
	}

}