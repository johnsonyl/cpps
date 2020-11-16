#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <curl/curl.h>

class cpps_curl
{
public:
	cpps_curl();

	void	append_write_data(const char* data, size_t size);
	void	append_header(const char* data, size_t size);

	std::string getdata();
	std::string getheader();
	void		append_get_data(std::string s);
public:
	CURL* curl;
	std::vector<struct curl_slist* > slists;
	std::string data;
	std::string header;
	std::stringstream strstream;
};

