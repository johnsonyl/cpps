#include "cpps_curl.h"

cpps_curl::cpps_curl()
{
	curl = NULL;
}


void	cpps_curl::append_write_data(const char* d, size_t size)
{
	data.append(d, size);
}
void	cpps_curl::append_header(const char* d, size_t size)
{
	header.append(d, size);
}


std::string cpps_curl::getdata()
{
	return data;
}
std::string cpps_curl::getheader()
{
	return header;
}
void cpps_curl::append_get_data(std::string s)
{
	strstream << s;
}
