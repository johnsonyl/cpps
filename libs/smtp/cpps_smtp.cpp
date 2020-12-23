#include "cpps_smtp.h"
#include <curl/curl.h>
namespace cpps
{

	cpps_smtp::cpps_smtp()
	{
		smtp_port = 25;
		smtp_timeout = 30;
		smtp_ssl = false;
		smtp_verbose = false;
	}
	
	cpps_smtp::~cpps_smtp()
	{
		
	}
	
	void cpps_smtp::setsmtp(std::string host, cpps_integer port)
	{
		smtp_host = host;
		smtp_port = port;
	}

	void cpps_smtp::setmailfrom(std::string mf)
	{
		smtp_mail_from = mf;
	}

	void cpps_smtp::setuserinfo(std::string un, std::string pwd)
	{
		smtp_username = un;
		smtp_pwd = pwd;
	}

	void cpps_smtp::openssl(bool b)
	{
		smtp_ssl = b;
	}

	void cpps_smtp::addrecipient(std::string rcpt)
	{
		smtp_recipients.push_back(rcpt);
	}

	void cpps_smtp::setverbose(bool b)
	{
		smtp_verbose = b;
	}

	void cpps_smtp::settimeout(cpps_integer timeout)
	{
		smtp_timeout = timeout;
	}

	void cpps_smtp::setreadcallback(cpps_value func)
	{
		smtp_read_callback_func = func;
	}

	void cpps_smtp::setcontent(std::string content)
	{
		smtp_content.write(content.c_str(),content.size());
	}
	size_t cpps_smtp_payload_source(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t num_bytes = size * nmemb;
		char* data = (char*)ptr;
		std::stringstream* strstream = (std::stringstream*)stream;

		strstream->read(data, num_bytes);

		return strstream->gcount();
	}
	size_t cpps_smtp_header_callback(const char* ptr, size_t size, size_t nmemb, cpps_smtp* smtp_ptr)
	{
		size_t len = size * nmemb;
		std::string stream;
		stream.append(ptr, len);

		if (smtp_ptr->smtp_read_callback_func.tt == CPPS_TFUNCTION || smtp_ptr->smtp_read_callback_func.tt == CPPS_TLAMBDAFUNCTION) {
			dofunction(smtp_ptr->c, smtp_ptr->smtp_read_callback_func, stream);
		}

		return len;
	}
	cpps_integer cpps_smtp::send(C*cstate)
	{
		c = cstate;
		if (smtp_host.empty()) return false;
		if (smtp_recipients.empty()) return false;
		if (smtp_content.str().empty()) return false;

		std::string tmp = "smtp://";
		if (smtp_port == 465)
			tmp = "smtps://";
		char tmphost[255];
		sprintf(tmphost, "%s:%d", smtp_host.c_str(), (int)smtp_port);
		tmp += tmphost;

		CURL* curl;
		CURLcode resCode = CURLE_OK;
		struct curl_slist* recipients = NULL;

		curl = curl_easy_init();
		if (curl) {

			if (!smtp_username.empty()) {
				curl_easy_setopt(curl, CURLOPT_USERNAME, smtp_username.c_str());
				curl_easy_setopt(curl, CURLOPT_PASSWORD, smtp_pwd.c_str());
			}

			curl_easy_setopt(curl, CURLOPT_URL, tmp.c_str());
			curl_easy_setopt(curl, CURLOPT_MAIL_FROM, smtp_mail_from.c_str());

			if (smtp_ssl || smtp_port == 465) {
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
				curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
			}

			
			for (auto& rcpt : smtp_recipients) {
				recipients = curl_slist_append(recipients, rcpt.c_str());
			}
			curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);


			curl_easy_setopt(curl, CURLOPT_READFUNCTION, cpps_smtp_payload_source);
			curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&smtp_content);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, smtp_timeout);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, smtp_verbose);


			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, cpps_smtp_header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
			resCode = curl_easy_perform(curl);
			curl_slist_free_all(recipients);
			curl_easy_cleanup(curl);

			return (cpps_integer)resCode;
		}
		return -1;
	}

}
