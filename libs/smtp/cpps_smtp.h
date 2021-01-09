#ifndef cpps_cpps_smtp_h__
#define cpps_cpps_smtp_h__

#include <cpps/cpps.h>

namespace cpps{

	class cpps_smtp
	{
	public:
		cpps_smtp();
		virtual ~cpps_smtp();
		
	public:
		
		void			setsmtp(std::string host, cpps_integer port);
		void			setmailfrom(std::string mf);
		void			setuserinfo(std::string un, std::string pwd);
		void			openssl(bool b);
		void			addrecipient(std::string rcpt);
		void			setverbose(bool b);
		void			settimeout(cpps_integer timeout);
		void			setreadcallback(cpps_value func);
		void			setcontent(std::string content);
		cpps_integer	send(C *cstate);

		std::string		smtp_host;
		cpps_integer	smtp_port;
		cpps_integer	smtp_timeout;
		std::string		smtp_mail_from;
		std::string		smtp_username;
		std::string		smtp_pwd;
		std::vector<std::string>	smtp_recipients;
		bool			smtp_ssl;
		bool			smtp_verbose;
		cpps_value		smtp_read_callback_func;
		std::stringstream	smtp_content;
		C*				c;

	};
	
}
#endif //cpps_cpps_smtp_h__