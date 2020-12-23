#ifndef cpps_socket_h__
#define cpps_socket_h__

#include <cpps/cpps.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <string>

#include "cpps_socket_event_callback.h"

namespace cpps {

	class cpps_socket
	{
	public:
		cpps_socket();
		virtual ~cpps_socket();

		bool								create(event_base* evbase, evutil_socket_t nFd = 0);
		void								set_event_callback(cpps_socket_event_callback* cb);
		virtual void						close();
		virtual void						send(cpps::Buffer* buffer);


		static void							on_read_cb(struct bufferevent* bv, void* ptr);
		static void							on_write_cb(struct bufferevent* bv, void* ptr);
		static void							on_event_cb(struct bufferevent* bv, short e, void* ptr);

	public:
		evutil_socket_t						evsocket;
		struct bufferevent*					evbufferevent;
		cpps_socket_event_callback*			socket_event_callback;
		struct evbuffer*					socket_evbuffer;
	};
}


#endif // cpps_socket_h__
