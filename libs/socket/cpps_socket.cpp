#include "stdafx.h"
#include "cpps_socket.h"

namespace cpps {
	cpps_socket::cpps_socket()
	{
		socket_event_callback = NULL;
		evsocket = 0;
		evbufferevent = NULL;
		socket_evbuffer = NULL;
	}

	cpps_socket::~cpps_socket()
	{
		if (socket_evbuffer)
		{
			evbuffer_free(socket_evbuffer);
		}
		if (evbufferevent != NULL)
		{
			bufferevent_free(evbufferevent);
			evbufferevent = NULL;
			evsocket = 0;
		}
		socket_event_callback = NULL;
	}

	bool cpps_socket::create(event_base* evbase, evutil_socket_t nFd /*= 0*/)
	{
		if (nFd == 0) evsocket = -1;
		else evsocket = nFd;

		evbufferevent = bufferevent_socket_new(evbase, evsocket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

		if (evbufferevent == NULL)
		{
			int err = EVUTIL_SOCKET_ERROR();
			std::string err_str = evutil_socket_error_to_string(err);
			return false;
		}

		if (nFd == 0)
			evsocket = bufferevent_getfd(evbufferevent);

		socket_evbuffer = evbuffer_new();

		bufferevent_setcb(evbufferevent, &cpps_socket::on_read_cb, &cpps_socket::on_write_cb, &cpps_socket::on_event_cb, this);
		bufferevent_enable(evbufferevent, EV_READ | EV_WRITE | EV_PERSIST);

		int nOptVal = 1;
		int nOptLen = sizeof(int);
		setsockopt(evsocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nOptVal, nOptLen);

		nOptVal = 1024 * 1024;
		nOptLen = sizeof(int);
		setsockopt(evsocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nOptVal, nOptLen);
		setsockopt(evsocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nOptVal, nOptLen);

		return true;
	}

	void cpps_socket::set_event_callback(cpps_socket_event_callback* cb)
	{
		socket_event_callback = cb;
	}

	void cpps_socket::close()
	{
		if (evbufferevent != NULL)
		{
			bufferevent_free(evbufferevent);
			evbufferevent = NULL;
			evsocket = 0;
		}
		if (socket_evbuffer)
		{
			evbuffer_free(socket_evbuffer);
			socket_evbuffer = NULL;
		}
	}

	void cpps_socket::send(cpps::Buffer* buffer)
	{
		bufferevent_write(evbufferevent, buffer->getbuffer(), buffer->length());
	}

	void cpps_socket::on_read_cb(struct bufferevent* bv, void* ptr)
	{
		cpps_socket* sock = (cpps_socket*)ptr;
		if (sock == NULL) return;
		if (sock->socket_event_callback) sock->socket_event_callback->onReadCallback(sock,bv);
	}

	void cpps_socket::on_write_cb(struct bufferevent* bv, void* ptr)
	{
		cpps_socket* sock = (cpps_socket*)ptr;
		if (sock == NULL) return;
		if (sock->socket_event_callback) sock->socket_event_callback->onWriteCallback(sock, bv);
	}

	void cpps_socket::on_event_cb(struct bufferevent* bv, short e, void* ptr)
	{
		cpps_socket* sock = (cpps_socket*)ptr;
		if (sock == NULL) return;
		if (sock->socket_event_callback) sock->socket_event_callback->onEventCallback(sock, e);
	}
}