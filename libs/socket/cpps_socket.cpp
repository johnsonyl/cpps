#include "cpps_socket.h"

namespace cpps {
	cpps_socket::cpps_socket()
	{
		socket_event_callback = NULL;
		uv_tcp = NULL;
		ctx = NULL;
		send_count = 0;
		recv_count = 0;
		write_count = 0;
		ssl = NULL;
		read_bio = NULL;
		write_bio = NULL;
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
	void cpps_socket::init_ssl_ctx(SSL_CTX* _ctx)
	{
		ctx = _ctx;
	}
	bool cpps_socket::is_open_ssl()
	{
		return ctx != NULL;
	}
	bool cpps_socket::read_start()
	{
		int err = uv_read_start((uv_stream_t*)uv_tcp, &cpps_socket::on_alloc_cb, cpps_socket::on_read_cb);
		if (err) return false;
		if (ctx != NULL) {
			//初始化ssl
			ssl = SSL_new(ctx);
			read_bio = BIO_new(BIO_s_mem());
			write_bio = BIO_new(BIO_s_mem());
			SSL_set_bio(ssl, read_bio, write_bio);
		}

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

		if (is_open_ssl()) {
			SSL_shutdown(ssl);
			SSL_free(ssl);

			//BIO_free(read_bio);
			//BIO_free(write_bio); //这个不需要释放吗？

			ssl = NULL;
			read_bio = NULL;
			write_bio = NULL;
		}
	}
	typedef struct {
		uv_write_t req;
		uv_buf_t buf;
		cpps_socket* sock;
	} write_req_t;
	void cpps_socket::send_write_real(const char* buffer, size_t size)
	{
		if (uv_tcp == NULL) return;
		write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));

		req->buf = uv_buf_init((char*)malloc(size), (usint32)size);
		req->req.data = req->buf.base;
		req->sock = this;
		memcpy(req->buf.base, buffer, size);
		int err = uv_write(&req->req, (uv_stream_t*)uv_tcp, &req->buf, 1, on_write_cb);
		if (err)
		{
			
			on_error_event(-1);
		}
	}
	void cpps_socket::send_write(const char *buffer,size_t size)
	{
		send_count++;
		if (is_open_ssl()) {
			queue_push_back(buffer, size);
			send_data_after_handshake();
		}
		else {
			send_write_real(buffer, size);
		}
	}
	void cpps_socket::send(cpps::Buffer* buffer)
	{
		send_write(buffer->getbuffer(), (size_t)buffer->length());
	}

	void cpps_socket::sends(std::string& buffer)
	{
		send_write(buffer.data(), buffer.size());
	}

	void cpps_socket::readbuffer(const char* buf, ssize_t nread)
	{
		buffer.append(buf, nread);
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
	void cpps_socket::on_read_do_cb(ssize_t nread, const char* buf) {
		if (socket_event_callback) socket_event_callback->onReadCallback(this, nread, buf);
	}
	void cpps_socket::on_write_do_cb(ssize_t nread, const char* buf) {
		if (socket_event_callback) socket_event_callback->onWriteCallback(this, nread, buf);
	}
	bool cpps_socket::ssl_continue_wantwait(int status) {

		if (ssl == NULL) return true;
		if (write_bio == NULL) return true;
		if (read_bio == NULL) return true;
		if (status == 1) return true;

		write_bio_to_socket();
		int err = SSL_get_error(ssl, status);
		if (err == SSL_ERROR_WANT_READ) {
			// 在read回调函数中读取数据
		}
		else if (err == SSL_ERROR_WANT_WRITE) {
			write_bio_to_socket();
		}
		return false;
	}
	void cpps_socket::ssl_read_cb(ssize_t nread, const uv_buf_t* buf)
	{
		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

		BIO_write(read_bio, buf->base, (int)nread);
		if (!SSL_is_init_finished(ssl)) {
			// 我们还没有完成ssl的初始化，继续进行握手。
			int ret = ssl_continue();
			
			if(ssl_continue_wantwait(ret)) {
				// 握手完成，发送数据。
				send_data_after_handshake();
				read_data_after_handshake();
			}
		}
		else {
			// ssl已经初始化好了, 我们可以从BIO中读取已经解密的数据。
			read_data_after_handshake();
		}

	}
	void cpps_socket::on_read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
	{

		cpps_socket* sock = (cpps_socket*)client->data;
		if (sock == NULL) return;
		if (sock->is_open_ssl()) {
			sock->ssl_read_cb(nread, buf);
		}
		else {
			sock->on_read_do_cb(nread, buf->base);
		}

		if (buf && buf->base) {
			free(buf->base);
		}
	}


	void cpps_socket::on_write_cb(uv_write_t* req, int status)
	{
		if (status) {
			//fprintf(stderr, "Write error %s\n", uv_strerror(status));
		}
		write_req_t* wr;
		/* Free the read/write buffer and the request */
		wr = (write_req_t*)req;
		if (wr->sock) {
			wr->sock->write_count++;
			wr->sock->on_write_do_cb(wr->buf.len, wr->buf.base);
		}
		if (wr && wr->buf.base) {
			free(wr->buf.base);
			free(wr);
		}

	}

	void cpps_socket::on_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		*buf = uv_buf_init((char*)malloc(suggested_size),(usint32) suggested_size);
	}

	void cpps_socket::write_bio_to_socket()
	{
		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

		std::string _real_buffer;
		while (true) {
			char buf[4096];
			memset(buf, 0, 4096);
			int hasread = BIO_read(write_bio, buf, sizeof(buf));
			if (hasread <= 0) {
				break;
			}
			_real_buffer.append(buf, hasread);
		}
		if(!_real_buffer.empty())
			send_write_real(_real_buffer.data(), _real_buffer.size());

		if (SSL_is_init_finished(ssl)) {
			send_data_after_handshake();
		}
	}
	socket_msg_queue_t* cpps_socket::get_queue_front() {
		std::vector<socket_msg_queue_t*>::iterator it = msg_queue_list.begin();
		if (it == msg_queue_list.end()) return NULL;
		socket_msg_queue_t* t = *it;
		return t;
	}
	void cpps_socket::queue_pop_front() {
		std::vector<socket_msg_queue_t*>::iterator it = msg_queue_list.begin();
		socket_msg_queue_t* t = *it;
		msg_t_pool.free(t);
		msg_queue_list.erase(it);
	}
	void cpps_socket::queue_push_back(const char* buf, size_t size)
	{
		socket_msg_queue_t* t= msg_t_pool.alloc();
		t->realloc(buf, size);
		msg_queue_list.emplace_back(t);
	}
	void cpps_socket::send_data_after_handshake()
	{
		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

		//还需要一个队列来处理
		socket_msg_queue_t* t = get_queue_front();
		if (t == NULL) return;
		if (ssl == NULL) return;

		int ret = SSL_write(ssl, t->data, (int)t->size);   // data中存放了要发送的数据
		if (ret > 0) {
			// 写入socket
			queue_pop_front();
			write_bio_to_socket();
		}
		else if (ret == 0) {
			// 连接关闭了？？
			on_error_event(-1);
		}
		else {
			// 需要读取或写入数据。
			ssl_continue_wantwait(ret);
		}
	}

	void cpps_socket::read_data_after_handshake()
	{

		if (ssl == NULL) return;
		if (write_bio == NULL) return;
		if (read_bio == NULL) return;

		//printf("read_data_after_handshake\n");
		std::string _real_buffer;
		while (true) {

			if (ssl == NULL) return;
			if (write_bio == NULL) return;
			if (read_bio == NULL) return;

			char buf[4096];
			memset(buf, 0, 4096);
			int ret = SSL_read(ssl, buf, sizeof(buf));
			//printf("ret:%d\n", ret);
			if (ret < 0) {
				ssl_continue_wantwait(ret);
				return;//?
			}
			else if (ret > 4096) {
				on_error_event(-1);
				return;
			}
			else if (ret > 0) {
				_real_buffer.append(buf, ret);
				int pendingData = SSL_pending(ssl);
				if (pendingData == 0) {
					on_read_do_cb(_real_buffer.size(), _real_buffer.data());
					break;
				}
			}
			else if (ret == 0) {
				on_error_event(-1);
				return;
			}
		}
	}
}