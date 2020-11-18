#ifndef cpps_socket_event_callback_h__
#define cpps_socket_event_callback_h__

namespace cpps
{

	class cpps_socket;
	class cpps_socket_event_callback
	{
	public:
		virtual void						onReadCallback(cpps_socket* sock, struct bufferevent* bv) = 0;
		virtual void						onWriteCallback(cpps_socket* sock, struct bufferevent* bv) = 0;
		virtual void						onEventCallback(cpps_socket* sock, short e) = 0;
	};
}
#endif // cpps_socket_event_callback_h__
