#ifndef cpps_socket_h__
#define cpps_socket_h__

#include <cpps/cpps.h>
#include <uv.h>
#include <string>
#include <openssl/ssl.h>

#include "cpps_socket_event_callback.h"

namespace cpps {
	struct CPPS_SSL_CTX
	{
		SSL_CTX* ctx;
		CPPS_SSL_CTX() {
			ctx = NULL;
		}
		void release() {
			if(ctx != NULL)
				SSL_CTX_free(ctx);
			ctx = NULL;
		}
	};
	struct socket_msg_queue_t {
		socket_msg_queue_t() {
			data = NULL;
			real_size = 0;
			size = 0;
		}
		~socket_msg_queue_t() {
			release();
		}
		void realloc (const char *buffer,size_t _size) {
			if (real_size >= _size) {
				size = _size;
			}
			else {
				//release();
				char *new_data = (char*)::realloc(data,_size);
				if (new_data == NULL)
				{
					release();
					data = (char*)malloc(_size);
				}
				else {
					data = new_data;
				}
				real_size = _size;
				size = _size;
			}
			if (data != NULL) {
				memcpy(data, buffer, _size);
			}
		}
		void release() {
			if(data != NULL) free(data);
			size = 0;
			data = NULL;
		}
		char* data;
		size_t size;
		size_t real_size;
	};
	class cpps_socket
	{
	public:
		cpps_socket();
		virtual ~cpps_socket();

		bool								create( uv_tcp_t* nFd = 0);
		void								init_ssl_ctx(SSL_CTX* _ctx);
		bool								is_open_ssl();
		bool								read_start();
		void								set_event_callback(cpps_socket_event_callback* cb);
		void								close(std::string msg,uv_close_cb close_cb);
		void								send_write_real(const char* buffer, size_t size);
		void								send_write(const char* buffer, size_t size);
		virtual void						send(cpps::Buffer* buffer);
		virtual void						sends(std::string& buffer);

		virtual	void						readbuffer(const char* buf, ssize_t nread);
		virtual size_t						get_buffer_length();
		virtual	void						buffer_remove(char* out_buffer, size_t size);
		virtual	void						buffer_copyout(char* out_buffer, size_t size);
		void								on_read_do_cb(ssize_t nread, const char* buf);
		void								on_write_do_cb(ssize_t nread, const char* buf);
		bool								ssl_continue_wantwait(int status);
		void								ssl_read_cb(ssize_t nread, const uv_buf_t* buf);
		//ssl
		void								write_bio_to_socket();
		socket_msg_queue_t*					get_queue_front();
		void								queue_pop_front();
		void								queue_push_back(const char* buf, size_t);
		void								send_data_after_handshake();
		void								read_data_after_handshake();


		static void							on_read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		static void							on_write_cb(uv_write_t* req, int status);
		static void							on_event_cb(struct bufferevent* bv, short e, void* ptr);
		static void							on_alloc_cb(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);
		virtual void						on_error_event(int type) = 0;
		virtual int							ssl_continue() = 0;
		cpps_integer						get_send_count() { return send_count; }
		cpps_integer						get_write_count() { return write_count; }
		cpps_integer						get_recv_count() { return recv_count; }

	public:
		cpps_socket_event_callback*			socket_event_callback;
		uv_tcp_t*							uv_tcp;
		std::string							closemsg;
		std::string							buffer;
		//ssl
		SSL_CTX*							ctx;
		SSL*								ssl;
		BIO*								read_bio;
		BIO*								write_bio;
		std::vector<socket_msg_queue_t*>	msg_queue_list;
		cpps_object_pool<socket_msg_queue_t> msg_t_pool;

		cpps_integer						send_count;
		cpps_integer						write_count;
		cpps_integer						recv_count;
	};
}


#endif // cpps_socket_h__
