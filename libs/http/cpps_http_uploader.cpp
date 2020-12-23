#include "cpps_http_uploader.h"

namespace cpps
{

	cpps_http_uploader::cpps_http_uploader()
	{
		formpost = NULL;
		lastptr = NULL;
		headerlist = NULL;
	}
	
	cpps_http_uploader::~cpps_http_uploader()
	{
		
	}
	
	void cpps_http_uploader::addvalue(std::string name, std::string value)
	{
		curl_formadd(&formpost, &lastptr,
			CURLFORM_COPYNAME, name.c_str(),
			CURLFORM_COPYCONTENTS, value.c_str(),
			CURLFORM_END);
	}

	void cpps_http_uploader::addfile(std::string name, std::string file, object type /*= multipart/form-data */)
	{
		std::string ntype = "application/octet-stream";
		if (type.isstring()) ntype = type.tostring();
		curl_formadd(&formpost, &lastptr,
			CURLFORM_COPYNAME, name.c_str(),
			CURLFORM_FILE, file.c_str(),
			CURLFORM_CONTENTTYPE, ntype.c_str(),
			CURLFORM_END);
	}
	size_t write_data_upload(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		cpps_http_uploader* helper = (cpps_http_uploader*)stream;

		helper->append((const char*)ptr, size * nmemb);
	

		return size * nmemb;
	}

	std::string cpps_http_uploader::upload(C*cstate,std::string url, cpps::object _callbackfunc)
	{
		c = cstate;
		retpage = "";
		callbackfunc = _callbackfunc;
		CURL* curl;
		//CURLcode res;


		curl = curl_easy_init();

		headerlist = curl_slist_append(headerlist, buf);


		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

			if (!cookiesfile.empty())
			{
				curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiesfile.c_str()); // 指定cookie文件
				curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiesfile.c_str()); // 指定cookie文件
			}

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_upload); //将返回的http头输出到fp指向的文件

			/* Perform the request, res will get the return code */
			curl_easy_perform(curl);
			/* Check for errors */
			
			/* always cleanup */
			curl_easy_cleanup(curl);

			/* then cleanup the formpost chain */
			curl_formfree(formpost);
			/* free slist */
			curl_slist_free_all(headerlist);

			
		}
		return retpage;
	}

	void cpps_http_uploader::setcookie(std::string cookie)
	{
		cookiesfile = cookie;
	}

	void cpps_http_uploader::append(const char* data, size_t nmemb)
	{
		if (callbackfunc.isfunction()) {
			cpps::dofunction(c, callbackfunc, (cpps_integer)nmemb);
		}
		retpage.append(data, nmemb);
	}

}
