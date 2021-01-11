#ifndef cpps_socket_h__
#define cpps_socket_h__

#include <cpps/cpps.h>
#include <uv.h>
#include <string>

#include "cpps_socket_event_callback.h"

namespace cpps {

	class cpps_socket
	{
	public:
		cpps_socket();
		virtual ~cpps_socket();

		bool								create( uv_tcp_t* nFd = 0);
		bool								read_start();
		void								set_event_callback(cpps_socket_event_callback* cb);
		void								close(std::string msg,uv_close_cb close_cb);
		virtual void						send(cpps::Buffer* buffer);
		virtual void						sends(std::string& buffer);

		virtual	void						readbuffer(const uv_buf_t* buf, ssize_t nread);
		virtual size_t						get_buffer_length();
		virtual	void						buffer_remove(char* out_buffer, size_t size);
		virtual	void						buffer_copyout(char* out_buffer, size_t size);

		static void							on_read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		static void							on_write_cb(uv_write_t* req, int status);
		static void							on_event_cb(struct bufferevent* bv, short e, void* ptr);
		static void							on_alloc_cb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);

	public:
		cpps_socket_event_callback*			socket_event_callback;
		uv_tcp_t*							uv_tcp;
		std::string							closemsg;
		std::string							buffer;
	};
}


#endif // cpps_socket_h__
