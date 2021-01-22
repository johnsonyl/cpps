#include "cpps_socket.h"

namespace cpps {
	cpps_socket::cpps_socket()
	{
		socket_event_callback = NULL;
		uv_tcp = NULL;
	}

	cpps_socket::~cpps_socket()
	{
		socket_event_callback = NULL;
		if (uv_tcp) CPPSDELETE(uv_tcp);
		uv_tcp = NULL;
	}

	bool cpps_socket::create( uv_tcp_t* nFd )
	{
		uv_tcp = nFd;
		
		return true;
	}
	bool cpps_socket::read_start()
	{
		int err = uv_read_start((uv_stream_t*)uv_tcp, &cpps_socket::on_alloc_cb, cpps_socket::on_read_cb);
		if (err) return false;
		return true;
	}
	void cpps_socket::set_event_callback(cpps_socket_event_callback* cb)
	{
		socket_event_callback = cb;
	}

	void cpps_socket::close(std::string msg, uv_close_cb close_cb)
	{
		if (uv_tcp == NULL) return;

		closemsg = msg;
		if(uv_tcp)uv_close((uv_handle_t*)uv_tcp, close_cb);
		uv_tcp = NULL;
	}
	typedef struct {
		uv_write_t req;
		uv_buf_t buf;
	} write_req_t;

	void cpps_socket::send(cpps::Buffer* buffer)
	{

		write_req_t* req = (write_req_t*)CPPSMALLOC(sizeof(write_req_t));

		req->buf = uv_buf_init((char*)CPPSMALLOC((size_t)buffer->length()), (usint32)buffer->length());
		req->req.data = req->buf.base;
		memcpy(req->buf.base, buffer->getbuffer(), size_t(buffer->length()));
		int err = uv_write(&req->req,(uv_stream_t*) uv_tcp, &req->buf, 1, on_write_cb);
		if (err)
		{
#ifdef _DEBUG
			fprintf(stderr, "Write error %s\n", uv_strerror(err));
#endif
		}

	}

	void cpps_socket::sends(std::string& buffer)
	{
		write_req_t* req = (write_req_t*)CPPSMALLOC(sizeof(write_req_t));

		req->buf = uv_buf_init((char*)CPPSMALLOC((size_t)buffer.size()), (usint32)buffer.size());
		req->req.data = req->buf.base;
		memcpy(req->buf.base, buffer.data(), buffer.size());
		int err = uv_write(&req->req, (uv_stream_t*)uv_tcp, &req->buf, 1, on_write_cb);
		if (err)
		{
#ifdef _DEBUG
			fprintf(stderr, "Write error %s\n", uv_strerror(err));
#endif
		}
	}

	void cpps_socket::readbuffer(const uv_buf_t* buf, ssize_t nread)
	{
		buffer.append(buf->base, nread);
	}

	size_t cpps_socket::get_buffer_length()
	{
		return buffer.size();
	}

	void cpps_socket::buffer_remove(char* out_buffer, size_t size)
	{
		if (buffer.size() < size) abort();

		memcpy(out_buffer, buffer.data(), size);
		if (size >= buffer.size()) {
			buffer.clear();
		}
		else {
			memmove((void*)buffer.data(), buffer.data() + size, buffer.size() - size);
			buffer.resize(buffer.size() - size);
		}
	}

	void cpps_socket::buffer_copyout(char* out_buffer, size_t size)
	{
		if (buffer.size() < size) abort();
		memcpy(out_buffer, buffer.data(), size);
	}

	void cpps_socket::on_read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
	{
		cpps_socket* sock = (cpps_socket*)client->data;
		if (sock == NULL) return;
		if (sock->socket_event_callback) sock->socket_event_callback->onReadCallback(sock, nread, buf);
		if(buf && buf->base) CPPSFREE(buf->base);
	}

	

	void cpps_socket::on_write_cb(uv_write_t* req, int status)
	{
		if (status) {
#ifdef _DEBUG
			fprintf(stderr, "Write error %s\n", uv_strerror(status));
#endif
		}
		write_req_t* wr;

		/* Free the read/write buffer and the request */
		wr = (write_req_t*)req;
		if (wr && wr->buf.base) {
			CPPSFREE(wr->buf.base);
			CPPSFREE(wr);
		}

	}

	void cpps_socket::on_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		*buf = uv_buf_init((char*)CPPSMALLOC(suggested_size),(usint32) suggested_size);
	}

}