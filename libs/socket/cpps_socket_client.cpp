#include "cpps_socket_client.h"

namespace cpps
{
	cpps_socket_client::cpps_socket_client()
	{
		client_connection = false;
		c = NULL;
		uv_connect = NULL;
		dest_port = 0;
		uv_loop = CPPSNEW( uv_loop_t)();
		uv_loop_init(uv_loop);
	}

	cpps_socket_client::~cpps_socket_client()
	{
		if (uv_loop) {
			uv_loop_close(uv_loop);
			CPPSDELETE( uv_loop);
			uv_loop = NULL;
		}
	}

	void cpps_socket_client::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}

	cpps_socket_client* cpps_socket_client::setoption( cpps::object opt)
	{
		client_option.option_connected = opt["connected"];
		client_option.option_data = opt["data"];
		client_option.option_write = opt["write"];
		client_option.option_close = opt["close"];
		client_option.option_parser = opt["parser"];
		client_option.option_ssl = opt["ssl"];
		client_option.userdata = opt["userdata"];
		if (cpps::type(opt["headersize"]) == CPPS_TINTEGER || cpps::type(opt["headersize"]) == CPPS_TUINTEGER) client_option.option_headsize = object_cast<cpps_integer>(opt["headersize"]);
		if (client_option.option_ssl.tobool()) {
			SSL_library_init();
			OpenSSL_add_all_algorithms();
			SSL_load_error_strings();
			
		}
		return this;
	}

	bool cpps_socket_client::connect(cpps::C* cstate, std::string ip, cpps::usint16 port)
	{
		if (ip.empty()) return false;
		if (port == 0) return false;
		setcstate(cstate);

		dest_ip = ip;
		dest_port = port;


		uv_tcp_t* fd = CPPSNEW( uv_tcp_t)();
		uv_tcp_init(uv_loop, fd);
		fd->data = (void*)this;

	
		uv_connect = CPPSNEW( uv_connect_t)();
		uv_connect->data = (void* )this;


// 		struct sockaddr_in dest;
// 		uv_ip4_addr(dest_ip.c_str(), dest_port, &dest);
// 		uv_ip4_addr(dest_ip.c_str(), dest_port, &dest);

		struct addrinfo SocketAddr, * res = NULL, * ressave = NULL;
		memset(&SocketAddr, 0, sizeof(SocketAddr));
		SocketAddr.ai_flags = AI_ALL;
		SocketAddr.ai_family = AF_UNSPEC;
		SocketAddr.ai_socktype = 0;
		SocketAddr.ai_protocol = 0;
		int  ret;
		char s_port[256];
#ifdef WIN32
		_itoa_s(dest_port, s_port, 10);

		if (0 != (ret = getaddrinfo(dest_ip.c_str(), s_port, &SocketAddr, &res)))
			return false;
#else
		sprintf(s_port, "%d", dest_port);
		if (0 != (ret = getaddrinfo(dest_ip.c_str(), s_port, &SocketAddr, &res)))
		{
			printf("getaddrinfo error...\r\n");
			return false;
		}
#endif
		create(fd);
		ressave = res;

		if (client_option.option_ssl.tobool()) {
			ctx = SSL_CTX_new(TLSv1_2_client_method());
			if (ctx == NULL) {
				return NULL;
			}
		}

		
		//尝试绑定端口
		while (res != NULL) {
			int result = uv_tcp_connect(uv_connect, fd, (const struct sockaddr*)res->ai_addr, on_connect);
			
			if (result) {
				res = res->ai_next;
				continue;
			}
			break;
		}

		freeaddrinfo(ressave);
		//绑定失败
		if (NULL == res)
		{
			cpps_socket::close("connection error..", onClsoeCallback);
			return false;
		}

	

		set_event_callback(this);
		return true;
	}

	void cpps_socket_client::run()
	{
		if (uv_loop != NULL) {
			uv_run(uv_loop, UV_RUN_NOWAIT);
		}
	}

	void cpps_socket_client::close()
	{
		if (!client_connection) return;
		cpps_socket::close("normal close.", onClsoeCallback);

	}

	void cpps_socket_client::closesocket()
	{
		close();
	}

	bool cpps_socket_client::isconnect()
	{
		return client_connection;
	}

	void cpps_socket_client::onReadCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		if (nread > 0)
		{
			readbuffer(buf, nread);
			cpps_integer packetsize = (cpps_integer)get_buffer_length();
			cpps_create_class_var(cpps::Buffer, c, buffer_var, buffer_ptr);

			if (client_option.option_headsize == 0)
			{
				buffer_ptr->clear();
				buffer_ptr->realloc(packetsize);
				buffer_remove(buffer_ptr->getbuffer(), size_t(packetsize));

				if (cpps::type(client_option.option_data) == CPPS_TFUNCTION)
				{
					cpps::dofunction(c, client_option.option_data, buffer_var,this);
				}
			}
			else
			{
				while (packetsize >= client_option.option_headsize)
				{
					buffer_ptr->clear();
					buffer_ptr->realloc(client_option.option_headsize);
					buffer_ptr->seek(0);
					buffer_copyout(buffer_ptr->getbuffer(), size_t(client_option.option_headsize));
					if (cpps::type(client_option.option_parser) == CPPS_TFUNCTION)
					{
						cpps_integer size = object_cast<cpps_integer>(cpps::dofunction(c, client_option.option_parser, buffer_var,this));
						if (size == -1)
						{
							//说明包头异常关闭它.
							cpps_socket::close("Illegal header.", onClsoeCallback);
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
							buffer_remove(buffer_ptr->getbuffer(), size_t(size));
							recv_count++;
							if (cpps::type(client_option.option_data) == CPPS_TFUNCTION)
							{
								cpps::dofunction(c, client_option.option_data, buffer_var,this);
							}
							packetsize = (cpps_integer)get_buffer_length();
						}
					}
					else
					{
						cpps_socket::close("not found parser function..", onClsoeCallback);
						return;
					}
				}
			}
		}
		else {
			cpps_socket::close("server closed the connection.", onClsoeCallback);
		}
	}

	void cpps_socket_client::onWriteCallback(cpps_socket* sock, ssize_t nread, const char* buf)
	{
		if (cpps::type(client_option.option_write) == CPPS_TFUNCTION)
		{
			std::string _s(buf, nread);
			cpps::dofunction(c, client_option.option_write, _s, this);
		}
	}

	void cpps_socket_client::closed() {
		client_connection = false;
		if (cpps::type(client_option.option_close) == CPPS_TFUNCTION)
		{
			cpps::dofunction(c, client_option.option_close, 0, closemsg,this);
		}
	}

	void cpps_socket_client::setuserdata(cpps::object userdata)
	{
		client_option.userdata = userdata;
	}

	cpps_value cpps_socket_client::getuserdata()
	{
		return client_option.userdata.getval();
	}

	void cpps_socket_client::onClsoeCallback(uv_handle_t* handle)
	{
		cpps_socket_client* client = (cpps_socket_client*)handle->data;
		client->closed();
		uv_tcp_t* fd = (struct uv_tcp_s*)handle;
		CPPSDELETE( fd);
		SSL_CTX_free(client->ctx);
	}
	void cpps_socket_client::on_connect_do_cb()
	{
		client_connection = true;
		if (cpps::type(client_option.option_connected) == CPPS_TFUNCTION)
		{
			cpps::dofunction(c, client_option.option_connected, this);
		}
	}

	void cpps_socket_client::ssl_connect()
	{

		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

		SSL_set_connect_state(ssl);     // 这是个客户端连接
		int ret = SSL_connect(ssl);
		ssl_continue_wantwait(ret);
	}

	void cpps_socket_client::on_connect(uv_connect_t* req, int status)
	{
		cpps_socket_client* client = (cpps_socket_client*)req->data;
		if (!client->read_start()) {
			client->close();
			return ;
		}
		if (client->is_open_ssl()) {
			client->ssl_connect();
		}

		client->on_connect_do_cb();
		
	}

	void cpps_socket_client::on_error_event(int type)
	{
		cpps_socket::close("server closed the connection.", onClsoeCallback);
	}

	int cpps_socket_client::ssl_continue()
	{
		if (ssl == NULL) return 0;
		if (write_bio == NULL) return 0;
		if (read_bio == NULL) return 0;

		return SSL_connect(ssl);
	}


}