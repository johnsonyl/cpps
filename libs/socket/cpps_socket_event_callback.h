#ifndef cpps_socket_event_callback_h__
#define cpps_socket_event_callback_h__

namespace cpps
{

	class cpps_socket;
	class cpps_socket_event_callback
	{
	public:
		virtual void						onReadCallback(cpps_socket* sock, ssize_t nread, const uv_buf_t* buf) = 0;
	};
}
#endif // cpps_socket_event_callback_h__
