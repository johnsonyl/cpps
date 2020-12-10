#include "cpps_http_downloader.h"
namespace cpps
{

	cpps_http_downloader::cpps_http_downloader()
	{
		
	}
	
	cpps_http_downloader::~cpps_http_downloader()
	{
		
	}
	
	bool cpps_http_downloader::seturl(std::string url)
	{
		httpurl = url;
		return true;
	}

	void cpps_http_downloader::setfilepath(std::string f)
	{
		filepath = f;
		remove(filepath.c_str());
	}
	size_t get_size_struct(void* ptr, size_t size, size_t nmemb, void* data) {
		return (size_t)(size * nmemb);
	}

	cpps_integer get_download_size(const char* url) {
		CURL* curl;
		CURLcode res;
		double size = 0.0;

		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, get_size_struct);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		res = curl_easy_perform(curl);
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);

		return static_cast<cpps_integer>(size);
	}
	size_t cpps_http_downloader_write_data(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		cpps_http_downloader* helper = (cpps_http_downloader*)stream;

		helper->append((const char*)ptr, size * nmemb);

		return size * nmemb;
	}
	
	bool cpps_http_downloader::download(cpps::C* c, cpps::object func)
	{
		m_funcCallback = func;
		m_c = c;


		CURL* curl;
		CURLcode res;



		fileSize = get_download_size(httpurl.c_str());


		curl = curl_easy_init();    // ��ʼ��
		if (curl)
		{
			if (httpurl.find("https") == 0)
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			}


			curl_easy_setopt(curl, CURLOPT_URL, httpurl.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); //�����ص�httpͷ�����fpָ����ļ�
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cpps_http_downloader_write_data); //�����ص�httpͷ�����fpָ����ļ�
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//������Ϣ��
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);



			res = curl_easy_perform(curl);   // ִ��
			if (res != 0) {
				switch (res)
				{
				case CURLE_UNSUPPORTED_PROTOCOL:
					printf("��֧�ֵ�Э��,��URL��ͷ��ָ��\n");
				case CURLE_COULDNT_CONNECT:
					printf("�������ӵ�remote�������ߴ���\n");
				case CURLE_HTTP_RETURNED_ERROR:
					printf("http���ش���\n");
				case CURLE_READ_ERROR:
					printf("�������ļ�����\n");
				default:
					printf("����ֵ:%d\n", res);
				}
				curl_easy_cleanup(curl);
				return false;
			}


			curl_easy_cleanup(curl);
			return true;
		}
		return false;
	}

	void cpps_http_downloader::close()
	{

	}

	void cpps_http_downloader::append(const char* page, size_t size)
	{
		fileCurrSize += size;
		//д���ļ��ɡ�
		FILE* file = fopen(filepath.c_str(), "ab+");
		if (file)
		{
			fwrite(page, size, 1, file);
			fclose(file);
		}

		cpps::dofunction(m_c, m_funcCallback, (__int64)fileSize, (__int64)size, (__int64)fileCurrSize);
	}

}