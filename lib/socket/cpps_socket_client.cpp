#include "stdafx.h"
#include "cpps_socket_client.h"

namespace cpps
{
	cpps_socket_client::cpps_socket_client()
	{
		client_connection = false;
		struct event_config* cfg = event_config_new();

#ifdef _WIN32
		evthread_use_windows_threads();
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
		}
	}

	cpps_socket_client::~cpps_socket_client()
	{
		if (ev_base) {
			event_base_free(ev_base);
			ev_base = NULL;
		}
	}

	void cpps_socket_client::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}

	cpps_socket_client* cpps_socket_client::setoption(cpps::C* cstate, cpps::object opt)
	{
		setcstate(cstate);
		client_option.option_connected = opt["connected"];
		client_option.option_data = opt["data"];
		client_option.option_close = opt["close"];
		client_option.option_parser = opt["parser"];
		if (cpps::type(opt["headersize"]) == CPPS_TINTEGER) client_option.option_headsize = object_cast<cpps_integer>(opt["headersize"]);
		client_option.isset = true;
		return this;
	}

	bool cpps_socket_client::connect(std::string ip, cpps::usint16 port)
	{
		if (client_option.isset == false) return false;
		if (ip.empty()) return false;
		if (port == 0) return false;

		dest_ip = ip;
		dest_port = port;

		

		if (!create(ev_base))
		{
			printf("create error...\r\n"); return false;
		}

		if (bufferevent_socket_connect_hostname(evbufferevent, NULL, AF_INET, dest_ip.c_str(), dest_port) != 0)
		{
			close();
			return false;
		}


		set_event_callback(this);
		return true;
	}

	void cpps_socket_client::run()
	{
		if (ev_base)	event_base_loop(ev_base, EVLOOP_NONBLOCK);
	}

	void cpps_socket_client::close()
	{
		client_connection = false;
		
		cpps_socket::close();
	}

	void cpps_socket_client::closesocket()
	{
		if (cpps::type(client_option.option_close) == CPPS_TFUNCTION)
		{
			cpps::dofunction(c, client_option.option_close, -1, "safe close the connction.");
		}
		close();
	}

	bool cpps_socket_client::isconnect()
	{
		return client_connection;
	}

	void cpps_socket_client::onReadCallback(cpps_socket* sock, struct bufferevent* bv)
	{
		cpps_socket_client* client = (cpps_socket_client*)sock;
		bufferevent_read_buffer(bv, client->socket_evbuffer);
		cpps_integer packetsize = evbuffer_get_length(client->socket_evbuffer);

		cpps_create_class_var(cpps::Buffer, c, buffer_var, buffer_ptr);

		if (client_option.option_headsize == 0)
		{
			buffer_ptr->realloc(packetsize);
			evbuffer_remove(client->socket_evbuffer, buffer_ptr->getbuffer(), packetsize);

			if (cpps::type(client_option.option_data) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, client_option.option_data,  buffer_var);
			}
		}
		else
		{
			while (packetsize >= client_option.option_headsize)
			{
				buffer_ptr->realloc(client_option.option_headsize);
				buffer_ptr->seek(0);
				evbuffer_copyout(client->socket_evbuffer, buffer_ptr->getbuffer(), client_option.option_headsize);
				if (cpps::type(client_option.option_parser) == CPPS_TFUNCTION)
				{
					cpps_integer size = object_cast<cpps_integer>(cpps::dofunction(c, client_option.option_parser, buffer_var));
					if (size == -1)
					{
						//说明包头异常关闭它.
						close();
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
						if (cpps::type(client_option.option_data) == CPPS_TFUNCTION)
						{
							cpps::dofunction(c, client_option.option_data, buffer_var);
						}
						packetsize = evbuffer_get_length(client->socket_evbuffer);
					}
				}
				else
				{
					close();
					return;
				}
			}
		}
	}

	void cpps_socket_client::onWriteCallback(cpps_socket* sock, struct bufferevent* bv)
	{

	}

	void cpps_socket_client::onEventCallback(cpps_socket* sock, short e)
	{
		if (e & BEV_EVENT_EOF)
		{
			if (cpps::type(client_option.option_close) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, client_option.option_close, 0, "server close the connection.");
			}
			close();
		}
		else if (e & BEV_EVENT_ERROR)
		{
			int err = EVUTIL_SOCKET_ERROR();

			if (cpps::type(client_option.option_close) == CPPS_TFUNCTION)
			{
#ifdef WIN32
				cpps::dofunction(c, client_option.option_close, err, cpps_socket_g2u(evutil_socket_error_to_string(err)));
#else
				cpps::dofunction(c, client_option.option_close, err, evutil_socket_error_to_string(err));
#endif
			}

			close();
		}
		else if (e & BEV_EVENT_CONNECTED)
		{
			client_connection = true;
			if (cpps::type(client_option.option_connected) == CPPS_TFUNCTION)
			{
				cpps::dofunction(c, client_option.option_connected);
			}
		}
		else if (e & BEV_EVENT_TIMEOUT)
		{
			close();
		}
	}

}