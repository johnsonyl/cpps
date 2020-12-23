#ifndef cpps_cpps_socket_httpserver_session_h__
#define cpps_cpps_socket_httpserver_session_h__

#include <cpps/cpps.h>

namespace cpps{
	typedef phmap::flat_hash_map<std::string, std::string> session_values;
	class cpps_socket_httpserver_session
	{
	public:
		cpps_socket_httpserver_session();
		virtual ~cpps_socket_httpserver_session();
		
		void				set(std::string key, std::string value);
		std::string			get(std::string key, object defaultvalue);
		void				set_expire(cpps_integer time);
		void				clear();
		void				remove();
		bool				isneedremove();

	public:
		std::string			session_id;
		session_values		session_value;
		cpps_integer		session_expire; //���ڵ�unixʱ���
		bool				needremove;
	};
	
}
#endif //cpps_cpps_socket_httpserver_session_h__