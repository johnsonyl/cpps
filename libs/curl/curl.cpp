// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include "cpps_curl.h"

using namespace cpps;
using namespace std;

void     cpps_curl_easy_cleanup(cpps_curl* curl)
{
    if (curl == NULL) return ;
    for (auto slist : curl->slists)
    {
        curl_slist_free_all(slist);
    }
    curl->slists.clear();

    if (curl->curl != NULL) {
        curl_easy_cleanup(curl->curl);
    }
}
cpps_integer cpps_curl_getinfo_type(cpps_integer info)
{
    cpps_integer ret = -1;
    if (info > CURLINFO_STRING && info < CURLINFO_LONG)
        ret = 1;
    else if (info > CURLINFO_LONG && info < CURLINFO_DOUBLE)
        ret = 2;
    else if (info > CURLINFO_DOUBLE && info < CURLINFO_SLIST)
        ret = 3;
    else if (info > CURLINFO_SLIST && info < CURLINFO_SOCKET)
        ret = 4;
    else if (info > CURLINFO_SOCKET && info < CURLINFO_OFF_T)
        ret = 5;
    else if (info > CURLINFO_OFF_T && info < CURLINFO_TYPEMASK)
        ret = 6;
    else if (info > CURLINFO_TYPEMASK )
        ret = 7;
    else if (info > CURLINFO_MASK && info < CURLINFO_STRING)
        ret = 8;

    return ret;
}
cpps_value cpps_curl_easy_getinfo(C*c,cpps_curl*curl, cpps_integer info)
{

    cpps_value ret;
    cpps_integer t = cpps_curl_getinfo_type(info);
    switch (t)
    {
    case 1: //string
    {
        std::string str_p;
        char buffer[4096] = { 0 };
        curl_easy_getinfo(curl->curl, (CURLINFO)info, &buffer);
        str_p.append(buffer);
        ret = cpps_value(c,str_p);
    }
        break;
    case 2: //int
    {
        cpps_integer int_v;
        curl_easy_getinfo(curl->curl, (CURLINFO)info, &int_v);
        ret = int_v;
    }
        break;
    case 3: //double
    {
        cpps_number number_v;
        curl_easy_getinfo(curl->curl, (CURLINFO)info, &number_v);
        ret = number_v;
    }
        break;
    case 4: //vector
    {
        cpps_create_class_var(cpps_vector, c, vec_v, vec_p)

        struct curl_slist* cookies = NULL;
        curl_easy_getinfo(curl->curl, (CURLINFO)info, &cookies);

        while (cookies) {
            vec_p->push_back(cpps_value(c, cookies->data));
            cookies = cookies->next;
        }
        curl_slist_free_all(cookies);

        ret = vec_v;
    }
        break;
    default:
        //暂未实现
        break;
    }
  
    return ret;
}
cpps_curl*     cpps_curl_easy_init()
{
    cpps_curl* ret = new cpps_curl();
    ret->curl = curl_easy_init();
    return ret;
}

size_t cpps_curl_write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    cpps_curl* curl = (cpps_curl*)stream;
    size_t len = size * nmemb;
    curl->append_write_data((const char*)ptr, len);
    return len;
}
size_t cpps_curl_header_callback(const char* ptr, size_t size, size_t nmemb, void* stream)
{
    cpps_curl* curl = (cpps_curl*)stream;
    size_t len = size * nmemb;
    curl->append_header(ptr, len);
    return len;
}
size_t cpps_curl_read_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t num_bytes = size * nmemb;
    char* data = (char*)ptr;
    cpps_curl* curl = (cpps_curl*)stream;

    curl->strstream.read(data, num_bytes);

    return  curl->strstream.gcount();
}
std::string     cpps_curl_version()
{
    return curl_version();
}
cpps_integer     cpps_curl_easy_perform(cpps_curl* curl)
{
    curl_easy_setopt(curl->curl, CURLOPT_WRITEDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_WRITEFUNCTION, cpps_curl_write_data);

    curl_easy_setopt(curl->curl, CURLOPT_HEADERDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_HEADERFUNCTION, cpps_curl_header_callback);

    curl_easy_setopt(curl->curl, CURLOPT_READDATA, curl);
    curl_easy_setopt(curl->curl, CURLOPT_READFUNCTION, cpps_curl_read_data);

    return curl_easy_perform(curl->curl);
}

cpps_integer cpps_curl_option_type(cpps_integer option)
{
    cpps_integer ret = -1;
    if (option > 0 && option < CURLOPTTYPE_OBJECTPOINT)
    {
        ret = 1;
    }
    else if (option > CURLOPTTYPE_OBJECTPOINT && option < CURLOPTTYPE_FUNCTIONPOINT)
    {
        ret = 2;
        switch (option)
        {
        case CURLOPT_HTTPHEADER:
        case CURLOPT_MAIL_RCPT:
        case CURLOPT_QUOTE:
        case CURLOPT_POSTQUOTE:
        case CURLOPT_TELNETOPTIONS:
        case CURLOPT_PREQUOTE:
        case CURLOPT_HTTP200ALIASES:
        case CURLOPT_RESOLVE:
        case CURLOPT_PROXYHEADER:
        case CURLOPT_CONNECT_TO:
            ret = 3;
            break;
        default:
            break;
        }
    }
    else if (option > CURLOPTTYPE_FUNCTIONPOINT && option < CURLOPTTYPE_OFF_T)
    {
        ret = -1;
    }
    else if (option > CURLOPTTYPE_OFF_T && option < CURLOPTTYPE_BLOB)
    {
        ret = 4;
    }
    else if (option > CURLOPTTYPE_BLOB )
    {
        ret = 5;
    }
   
    return ret;
}
cpps_integer     cpps_curl_easy_setopt(cpps_curl* curl, cpps_integer option, cpps_value arg)
{
    cpps_integer ret = -1;
    cpps_integer t = cpps_curl_option_type(option);
    switch (t)
    {
    case 1: //int
        {
            cpps_integer v = cpps_to_integer(arg);
            curl_easy_setopt(curl->curl, (CURLoption)option, v);
        }
        break;
    case 2://string
        {
            std::string *v = cpps_get_string(arg);
            curl_easy_setopt(curl->curl, (CURLoption)option, v->c_str());
        }
        break;
    case 3://vector
        {
            cpps_vector* v = cpps_to_cpps_vector(arg);
            struct curl_slist* slist = NULL;
            for (size_t i = 0; i < v->realvector().size(); i++)
            {
                slist = curl_slist_append(slist, cpps_get_string(v->realvector()[i])->c_str());
            }
            curl_easy_setopt(curl->curl, (CURLoption)option, slist);
            curl->slists.push_back(slist);
        }
        break;
    default:

        break;
    }
    return ret;
}
cpps_export_void  cpps_attach(cpps::C* c)
{
    curl_global_init(CURL_GLOBAL_ALL);

	cpps::cpps_init_cpps_class(c);

    cpps::_module(c, "curl")[
        _class<cpps_curl>("CURL")
            .def("getdata", &cpps_curl::getdata)
            .def("getheader", &cpps_curl::getheader)
            .def("append", &cpps_curl::append_get_data),
        def("curl_easy_init", cpps_curl_easy_init),
        def("curl_easy_cleanup", cpps_curl_easy_cleanup),
        def("curl_easy_setopt", cpps_curl_easy_setopt),
        def_inside("curl_easy_getinfo", cpps_curl_easy_getinfo),
        def("curl_easy_perform", cpps_curl_easy_perform),
        def("curl_version", cpps_curl_version),
        defvar(c,"CURLOPT_WRITEDATA", (cpps_integer)CURLOPT_WRITEDATA),
        defvar(c,"CURLOPT_URL", (cpps_integer)CURLOPT_URL),
        defvar(c,"CURLOPT_PORT", (cpps_integer)CURLOPT_PORT),
        defvar(c,"CURLOPT_PROXY", (cpps_integer)CURLOPT_PROXY),
        defvar(c,"CURLOPT_USERPWD", (cpps_integer)CURLOPT_USERPWD),
        defvar(c,"CURLOPT_PROXYUSERPWD", (cpps_integer)CURLOPT_PROXYUSERPWD),
        defvar(c,"CURLOPT_RANGE", (cpps_integer)CURLOPT_RANGE),
        defvar(c,"CURLOPT_READDATA", (cpps_integer)CURLOPT_READDATA),
        defvar(c,"CURLOPT_ERRORBUFFER", (cpps_integer)CURLOPT_ERRORBUFFER),
        defvar(c,"CURLOPT_WRITEFUNCTION", (cpps_integer)CURLOPT_WRITEFUNCTION),
        defvar(c,"CURLOPT_READFUNCTION", (cpps_integer)CURLOPT_READFUNCTION),
        defvar(c,"CURLOPT_TIMEOUT", (cpps_integer)CURLOPT_TIMEOUT),
        defvar(c,"CURLOPT_INFILESIZE", (cpps_integer)CURLOPT_INFILESIZE),
        defvar(c,"CURLOPT_POSTFIELDS", (cpps_integer)CURLOPT_POSTFIELDS),
        defvar(c,"CURLOPT_REFERER", (cpps_integer)CURLOPT_REFERER),
        defvar(c,"CURLOPT_FTPPORT", (cpps_integer)CURLOPT_FTPPORT),
        defvar(c,"CURLOPT_USERAGENT", (cpps_integer)CURLOPT_USERAGENT),
        defvar(c,"CURLOPT_LOW_SPEED_LIMIT", (cpps_integer)CURLOPT_LOW_SPEED_LIMIT),
        defvar(c,"CURLOPT_LOW_SPEED_TIME", (cpps_integer)CURLOPT_LOW_SPEED_TIME),
        defvar(c,"CURLOPT_RESUME_FROM", (cpps_integer)CURLOPT_RESUME_FROM),
        defvar(c,"CURLOPT_COOKIE", (cpps_integer)CURLOPT_COOKIE),
        defvar(c,"CURLOPT_HTTPHEADER", (cpps_integer)CURLOPT_HTTPHEADER),
        defvar(c,"CURLOPT_HTTPPOST", (cpps_integer)CURLOPT_HTTPPOST),
        defvar(c,"CURLOPT_SSLCERT", (cpps_integer)CURLOPT_SSLCERT),
        defvar(c,"CURLOPT_KEYPASSWD", (cpps_integer)CURLOPT_KEYPASSWD),
        defvar(c,"CURLOPT_CRLF", (cpps_integer)CURLOPT_CRLF),
        defvar(c,"CURLOPT_QUOTE", (cpps_integer)CURLOPT_QUOTE),
        defvar(c,"CURLOPT_HEADERDATA", (cpps_integer)CURLOPT_HEADERDATA),
        defvar(c,"CURLOPT_COOKIEFILE", (cpps_integer)CURLOPT_COOKIEFILE),
        defvar(c,"CURLOPT_SSLVERSION", (cpps_integer)CURLOPT_SSLVERSION),
        defvar(c,"CURLOPT_TIMECONDITION", (cpps_integer)CURLOPT_TIMECONDITION),
        defvar(c,"CURLOPT_TIMEVALUE", (cpps_integer)CURLOPT_TIMEVALUE),
        defvar(c,"CURLOPT_CUSTOMREQUEST", (cpps_integer)CURLOPT_CUSTOMREQUEST),
        defvar(c,"CURLOPT_STDERR", (cpps_integer)CURLOPT_STDERR),
        defvar(c,"CURLOPT_POSTQUOTE", (cpps_integer)CURLOPT_POSTQUOTE),
        defvar(c,"CURLOPT_OBSOLETE40", (cpps_integer)CURLOPT_OBSOLETE40),
        defvar(c,"CURLOPT_VERBOSE", (cpps_integer)CURLOPT_VERBOSE),
        defvar(c,"CURLOPT_HEADER", (cpps_integer)CURLOPT_HEADER),
        defvar(c,"CURLOPT_NOPROGRESS", (cpps_integer)CURLOPT_NOPROGRESS),
        defvar(c,"CURLOPT_NOBODY", (cpps_integer)CURLOPT_NOBODY),
        defvar(c,"CURLOPT_FAILONERROR", (cpps_integer)CURLOPT_FAILONERROR),
        defvar(c,"CURLOPT_UPLOAD", (cpps_integer)CURLOPT_UPLOAD),
        defvar(c,"CURLOPT_POST", (cpps_integer)CURLOPT_POST),
        defvar(c,"CURLOPT_DIRLISTONLY", (cpps_integer)CURLOPT_DIRLISTONLY),
        defvar(c,"CURLOPT_APPEND", (cpps_integer)CURLOPT_APPEND),
        defvar(c,"CURLOPT_NETRC", (cpps_integer)CURLOPT_NETRC),
        defvar(c,"CURLOPT_FOLLOWLOCATION", (cpps_integer)CURLOPT_FOLLOWLOCATION),
        defvar(c,"CURLOPT_TRANSFERTEXT", (cpps_integer)CURLOPT_TRANSFERTEXT),
        defvar(c,"CURLOPT_PUT", (cpps_integer)CURLOPT_PUT),
        defvar(c,"CURLOPT_PROGRESSFUNCTION", (cpps_integer)CURLOPT_PROGRESSFUNCTION),
        defvar(c,"CURLOPT_XFERINFODATA", (cpps_integer)CURLOPT_XFERINFODATA),
        defvar(c,"CURLOPT_AUTOREFERER", (cpps_integer)CURLOPT_AUTOREFERER),
        defvar(c,"CURLOPT_PROXYPORT", (cpps_integer)CURLOPT_PROXYPORT),
        defvar(c,"CURLOPT_POSTFIELDSIZE", (cpps_integer)CURLOPT_POSTFIELDSIZE),
        defvar(c,"CURLOPT_HTTPPROXYTUNNEL", (cpps_integer)CURLOPT_HTTPPROXYTUNNEL),
        defvar(c,"CURLOPT_INTERFACE", (cpps_integer)CURLOPT_INTERFACE),
        defvar(c,"CURLOPT_KRBLEVEL", (cpps_integer)CURLOPT_KRBLEVEL),
        defvar(c,"CURLOPT_SSL_VERIFYPEER", (cpps_integer)CURLOPT_SSL_VERIFYPEER),
        defvar(c,"CURLOPT_CAINFO", (cpps_integer)CURLOPT_CAINFO),
        defvar(c,"CURLOPT_MAXREDIRS", (cpps_integer)CURLOPT_MAXREDIRS),
        defvar(c,"CURLOPT_FILETIME", (cpps_integer)CURLOPT_FILETIME),
        defvar(c,"CURLOPT_TELNETOPTIONS", (cpps_integer)CURLOPT_TELNETOPTIONS),
        defvar(c,"CURLOPT_MAXCONNECTS", (cpps_integer)CURLOPT_MAXCONNECTS),
        defvar(c,"CURLOPT_OBSOLETE72", (cpps_integer)CURLOPT_OBSOLETE72),
        defvar(c,"CURLOPT_FRESH_CONNECT", (cpps_integer)CURLOPT_FRESH_CONNECT),
        defvar(c,"CURLOPT_FORBID_REUSE", (cpps_integer)CURLOPT_FORBID_REUSE),
        defvar(c,"CURLOPT_RANDOM_FILE", (cpps_integer)CURLOPT_RANDOM_FILE),
        defvar(c,"CURLOPT_EGDSOCKET", (cpps_integer)CURLOPT_EGDSOCKET),
        defvar(c,"CURLOPT_CONNECTTIMEOUT", (cpps_integer)CURLOPT_CONNECTTIMEOUT),
        defvar(c,"CURLOPT_HEADERFUNCTION", (cpps_integer)CURLOPT_HEADERFUNCTION),
        defvar(c,"CURLOPT_HTTPGET", (cpps_integer)CURLOPT_HTTPGET),
        defvar(c,"CURLOPT_SSL_VERIFYHOST", (cpps_integer)CURLOPT_SSL_VERIFYHOST),
        defvar(c,"CURLOPT_COOKIEJAR", (cpps_integer)CURLOPT_COOKIEJAR),
        defvar(c,"CURLOPT_SSL_CIPHER_LIST", (cpps_integer)CURLOPT_SSL_CIPHER_LIST),
        defvar(c,"CURLOPT_HTTP_VERSION", (cpps_integer)CURLOPT_HTTP_VERSION),
        defvar(c,"CURLOPT_FTP_USE_EPSV", (cpps_integer)CURLOPT_FTP_USE_EPSV),
        defvar(c,"CURLOPT_SSLCERTTYPE", (cpps_integer)CURLOPT_SSLCERTTYPE),
        defvar(c,"CURLOPT_SSLKEY", (cpps_integer)CURLOPT_SSLKEY),
        defvar(c,"CURLOPT_SSLKEYTYPE", (cpps_integer)CURLOPT_SSLKEYTYPE),
        defvar(c,"CURLOPT_SSLENGINE", (cpps_integer)CURLOPT_SSLENGINE),
        defvar(c,"CURLOPT_SSLENGINE_DEFAULT", (cpps_integer)CURLOPT_SSLENGINE_DEFAULT),
        defvar(c,"CURLOPT_DNS_USE_GLOBAL_CACHE", (cpps_integer)CURLOPT_DNS_USE_GLOBAL_CACHE),
        defvar(c,"CURLOPT_DNS_CACHE_TIMEOUT", (cpps_integer)CURLOPT_DNS_CACHE_TIMEOUT),
        defvar(c,"CURLOPT_PREQUOTE", (cpps_integer)CURLOPT_PREQUOTE),
        defvar(c,"CURLOPT_DEBUGFUNCTION", (cpps_integer)CURLOPT_DEBUGFUNCTION),
        defvar(c,"CURLOPT_DEBUGDATA", (cpps_integer)CURLOPT_DEBUGDATA),
        defvar(c,"CURLOPT_COOKIESESSION", (cpps_integer)CURLOPT_COOKIESESSION),
        defvar(c,"CURLOPT_CAPATH", (cpps_integer)CURLOPT_CAPATH),
        defvar(c,"CURLOPT_BUFFERSIZE", (cpps_integer)CURLOPT_BUFFERSIZE),
        defvar(c,"CURLOPT_NOSIGNAL", (cpps_integer)CURLOPT_NOSIGNAL),
        defvar(c,"CURLOPT_SHARE", (cpps_integer)CURLOPT_SHARE),
        defvar(c,"CURLOPT_PROXYTYPE", (cpps_integer)CURLOPT_PROXYTYPE),
        defvar(c,"CURLOPT_ACCEPT_ENCODING", (cpps_integer)CURLOPT_ACCEPT_ENCODING),
        defvar(c,"CURLOPT_PRIVATE", (cpps_integer)CURLOPT_PRIVATE),
        defvar(c,"CURLOPT_HTTP200ALIASES", (cpps_integer)CURLOPT_HTTP200ALIASES),
        defvar(c,"CURLOPT_UNRESTRICTED_AUTH", (cpps_integer)CURLOPT_UNRESTRICTED_AUTH),
        defvar(c,"CURLOPT_FTP_USE_EPRT", (cpps_integer)CURLOPT_FTP_USE_EPRT),
        defvar(c,"CURLOPT_HTTPAUTH", (cpps_integer)CURLOPT_HTTPAUTH),
        defvar(c,"CURLOPT_SSL_CTX_FUNCTION", (cpps_integer)CURLOPT_SSL_CTX_FUNCTION),
        defvar(c,"CURLOPT_SSL_CTX_DATA", (cpps_integer)CURLOPT_SSL_CTX_DATA),
        defvar(c,"CURLOPT_FTP_CREATE_MISSING_DIRS", (cpps_integer)CURLOPT_FTP_CREATE_MISSING_DIRS),
        defvar(c,"CURLOPT_PROXYAUTH", (cpps_integer)CURLOPT_PROXYAUTH),
        defvar(c,"CURLOPT_FTP_RESPONSE_TIMEOUT", (cpps_integer)CURLOPT_FTP_RESPONSE_TIMEOUT),
        defvar(c,"CURLOPT_IPRESOLVE", (cpps_integer)CURLOPT_IPRESOLVE),
        defvar(c,"CURLOPT_MAXFILESIZE", (cpps_integer)CURLOPT_MAXFILESIZE),
        defvar(c,"CURLOPT_INFILESIZE_LARGE", (cpps_integer)CURLOPT_INFILESIZE_LARGE),
        defvar(c,"CURLOPT_RESUME_FROM_LARGE", (cpps_integer)CURLOPT_RESUME_FROM_LARGE),
        defvar(c,"CURLOPT_MAXFILESIZE_LARGE", (cpps_integer)CURLOPT_MAXFILESIZE_LARGE),
        defvar(c,"CURLOPT_NETRC_FILE", (cpps_integer)CURLOPT_NETRC_FILE),
        defvar(c,"CURLOPT_USE_SSL", (cpps_integer)CURLOPT_USE_SSL),
        defvar(c,"CURLOPT_POSTFIELDSIZE_LARGE", (cpps_integer)CURLOPT_POSTFIELDSIZE_LARGE),
        defvar(c,"CURLOPT_TCP_NODELAY", (cpps_integer)CURLOPT_TCP_NODELAY),
        defvar(c,"CURLOPT_FTPSSLAUTH", (cpps_integer)CURLOPT_FTPSSLAUTH),
        defvar(c,"CURLOPT_IOCTLFUNCTION", (cpps_integer)CURLOPT_IOCTLFUNCTION),
        defvar(c,"CURLOPT_IOCTLDATA", (cpps_integer)CURLOPT_IOCTLDATA),
        defvar(c,"CURLOPT_FTP_ACCOUNT", (cpps_integer)CURLOPT_FTP_ACCOUNT),
        defvar(c,"CURLOPT_COOKIELIST", (cpps_integer)CURLOPT_COOKIELIST),
        defvar(c,"CURLOPT_IGNORE_CONTENT_LENGTH", (cpps_integer)CURLOPT_IGNORE_CONTENT_LENGTH),
        defvar(c,"CURLOPT_FTP_SKIP_PASV_IP", (cpps_integer)CURLOPT_FTP_SKIP_PASV_IP),
        defvar(c,"CURLOPT_FTP_FILEMETHOD", (cpps_integer)CURLOPT_FTP_FILEMETHOD),
        defvar(c,"CURLOPT_LOCALPORT", (cpps_integer)CURLOPT_LOCALPORT),
        defvar(c,"CURLOPT_LOCALPORTRANGE", (cpps_integer)CURLOPT_LOCALPORTRANGE),
        defvar(c,"CURLOPT_CONNECT_ONLY", (cpps_integer)CURLOPT_CONNECT_ONLY),
        defvar(c,"CURLOPT_CONV_FROM_NETWORK_FUNCTION", (cpps_integer)CURLOPT_CONV_FROM_NETWORK_FUNCTION),
        defvar(c,"CURLOPT_CONV_TO_NETWORK_FUNCTION", (cpps_integer)CURLOPT_CONV_TO_NETWORK_FUNCTION),
        defvar(c,"CURLOPT_CONV_FROM_UTF8_FUNCTION", (cpps_integer)CURLOPT_CONV_FROM_UTF8_FUNCTION),
        defvar(c,"CURLOPT_MAX_SEND_SPEED_LARGE", (cpps_integer)CURLOPT_MAX_SEND_SPEED_LARGE),
        defvar(c,"CURLOPT_MAX_RECV_SPEED_LARGE", (cpps_integer)CURLOPT_MAX_RECV_SPEED_LARGE),
        defvar(c,"CURLOPT_FTP_ALTERNATIVE_TO_USER", (cpps_integer)CURLOPT_FTP_ALTERNATIVE_TO_USER),
        defvar(c,"CURLOPT_SOCKOPTFUNCTION", (cpps_integer)CURLOPT_SOCKOPTFUNCTION),
        defvar(c,"CURLOPT_SOCKOPTDATA", (cpps_integer)CURLOPT_SOCKOPTDATA),
        defvar(c,"CURLOPT_SSL_SESSIONID_CACHE", (cpps_integer)CURLOPT_SSL_SESSIONID_CACHE),
        defvar(c,"CURLOPT_SSH_AUTH_TYPES", (cpps_integer)CURLOPT_SSH_AUTH_TYPES),
        defvar(c,"CURLOPT_SSH_PUBLIC_KEYFILE", (cpps_integer)CURLOPT_SSH_PUBLIC_KEYFILE),
        defvar(c,"CURLOPT_SSH_PRIVATE_KEYFILE", (cpps_integer)CURLOPT_SSH_PRIVATE_KEYFILE),
        defvar(c,"CURLOPT_FTP_SSL_CCC", (cpps_integer)CURLOPT_FTP_SSL_CCC),
        defvar(c,"CURLOPT_TIMEOUT_MS", (cpps_integer)CURLOPT_TIMEOUT_MS),
        defvar(c,"CURLOPT_CONNECTTIMEOUT_MS", (cpps_integer)CURLOPT_CONNECTTIMEOUT_MS),
        defvar(c,"CURLOPT_HTTP_TRANSFER_DECODING", (cpps_integer)CURLOPT_HTTP_TRANSFER_DECODING),
        defvar(c,"CURLOPT_HTTP_CONTENT_DECODING", (cpps_integer)CURLOPT_HTTP_CONTENT_DECODING),
        defvar(c,"CURLOPT_NEW_FILE_PERMS", (cpps_integer)CURLOPT_NEW_FILE_PERMS),
        defvar(c,"CURLOPT_NEW_DIRECTORY_PERMS", (cpps_integer)CURLOPT_NEW_DIRECTORY_PERMS),
        defvar(c,"CURLOPT_POSTREDIR", (cpps_integer)CURLOPT_POSTREDIR),
        defvar(c,"CURLOPT_SSH_HOST_PUBLIC_KEY_MD5", (cpps_integer)CURLOPT_SSH_HOST_PUBLIC_KEY_MD5),
        defvar(c,"CURLOPT_OPENSOCKETFUNCTION", (cpps_integer)CURLOPT_OPENSOCKETFUNCTION),
        defvar(c,"CURLOPT_OPENSOCKETDATA", (cpps_integer)CURLOPT_OPENSOCKETDATA),
        defvar(c,"CURLOPT_COPYPOSTFIELDS", (cpps_integer)CURLOPT_COPYPOSTFIELDS),
        defvar(c,"CURLOPT_PROXY_TRANSFER_MODE", (cpps_integer)CURLOPT_PROXY_TRANSFER_MODE),
        defvar(c,"CURLOPT_SEEKFUNCTION", (cpps_integer)CURLOPT_SEEKFUNCTION),
        defvar(c,"CURLOPT_SEEKDATA", (cpps_integer)CURLOPT_SEEKDATA),
        defvar(c,"CURLOPT_CRLFILE", (cpps_integer)CURLOPT_CRLFILE),
        defvar(c,"CURLOPT_ISSUERCERT", (cpps_integer)CURLOPT_ISSUERCERT),
        defvar(c,"CURLOPT_ADDRESS_SCOPE", (cpps_integer)CURLOPT_ADDRESS_SCOPE),
        defvar(c,"CURLOPT_CERTINFO", (cpps_integer)CURLOPT_CERTINFO),
        defvar(c,"CURLOPT_USERNAME", (cpps_integer)CURLOPT_USERNAME),
        defvar(c,"CURLOPT_PASSWORD", (cpps_integer)CURLOPT_PASSWORD),
        defvar(c,"CURLOPT_PROXYUSERNAME", (cpps_integer)CURLOPT_PROXYUSERNAME),
        defvar(c,"CURLOPT_PROXYPASSWORD", (cpps_integer)CURLOPT_PROXYPASSWORD),
        defvar(c,"CURLOPT_NOPROXY", (cpps_integer)CURLOPT_NOPROXY),
        defvar(c,"CURLOPT_TFTP_BLKSIZE", (cpps_integer)CURLOPT_TFTP_BLKSIZE),
        defvar(c,"CURLOPT_SOCKS5_GSSAPI_SERVICE", (cpps_integer)CURLOPT_SOCKS5_GSSAPI_SERVICE),
        defvar(c,"CURLOPT_SOCKS5_GSSAPI_NEC", (cpps_integer)CURLOPT_SOCKS5_GSSAPI_NEC),
        defvar(c,"CURLOPT_PROTOCOLS", (cpps_integer)CURLOPT_PROTOCOLS),
        defvar(c,"CURLOPT_REDIR_PROTOCOLS", (cpps_integer)CURLOPT_REDIR_PROTOCOLS),
        defvar(c,"CURLOPT_SSH_KNOWNHOSTS", (cpps_integer)CURLOPT_SSH_KNOWNHOSTS),
        defvar(c,"CURLOPT_SSH_KEYFUNCTION", (cpps_integer)CURLOPT_SSH_KEYFUNCTION),
        defvar(c,"CURLOPT_SSH_KEYDATA", (cpps_integer)CURLOPT_SSH_KEYDATA),
        defvar(c,"CURLOPT_MAIL_FROM", (cpps_integer)CURLOPT_MAIL_FROM),
        defvar(c,"CURLOPT_MAIL_RCPT", (cpps_integer)CURLOPT_MAIL_RCPT),
        defvar(c,"CURLOPT_FTP_USE_PRET", (cpps_integer)CURLOPT_FTP_USE_PRET),
        defvar(c,"CURLOPT_RTSP_REQUEST", (cpps_integer)CURLOPT_RTSP_REQUEST),
        defvar(c,"CURLOPT_RTSP_SESSION_ID", (cpps_integer)CURLOPT_RTSP_SESSION_ID),
        defvar(c,"CURLOPT_RTSP_STREAM_URI", (cpps_integer)CURLOPT_RTSP_STREAM_URI),
        defvar(c,"CURLOPT_RTSP_TRANSPORT", (cpps_integer)CURLOPT_RTSP_TRANSPORT),
        defvar(c,"CURLOPT_RTSP_CLIENT_CSEQ", (cpps_integer)CURLOPT_RTSP_CLIENT_CSEQ),
        defvar(c,"CURLOPT_RTSP_SERVER_CSEQ", (cpps_integer)CURLOPT_RTSP_SERVER_CSEQ),
        defvar(c,"CURLOPT_INTERLEAVEDATA", (cpps_integer)CURLOPT_INTERLEAVEDATA),
        defvar(c,"CURLOPT_INTERLEAVEFUNCTION", (cpps_integer)CURLOPT_INTERLEAVEFUNCTION),
        defvar(c,"CURLOPT_WILDCARDMATCH", (cpps_integer)CURLOPT_WILDCARDMATCH),
        defvar(c,"CURLOPT_CHUNK_BGN_FUNCTION", (cpps_integer)CURLOPT_CHUNK_BGN_FUNCTION),
        defvar(c,"CURLOPT_CHUNK_END_FUNCTION", (cpps_integer)CURLOPT_CHUNK_END_FUNCTION),
        defvar(c,"CURLOPT_FNMATCH_FUNCTION", (cpps_integer)CURLOPT_FNMATCH_FUNCTION),
        defvar(c,"CURLOPT_CHUNK_DATA", (cpps_integer)CURLOPT_CHUNK_DATA),
        defvar(c,"CURLOPT_FNMATCH_DATA", (cpps_integer)CURLOPT_FNMATCH_DATA),
        defvar(c,"CURLOPT_RESOLVE", (cpps_integer)CURLOPT_RESOLVE),
        defvar(c,"CURLOPT_TLSAUTH_USERNAME", (cpps_integer)CURLOPT_TLSAUTH_USERNAME),
        defvar(c,"CURLOPT_TLSAUTH_PASSWORD", (cpps_integer)CURLOPT_TLSAUTH_PASSWORD),
        defvar(c,"CURLOPT_TLSAUTH_TYPE", (cpps_integer)CURLOPT_TLSAUTH_TYPE),
        defvar(c,"CURLOPT_TRANSFER_ENCODING", (cpps_integer)CURLOPT_TRANSFER_ENCODING),
        defvar(c,"CURLOPT_CLOSESOCKETFUNCTION", (cpps_integer)CURLOPT_CLOSESOCKETFUNCTION),
        defvar(c,"CURLOPT_CLOSESOCKETDATA", (cpps_integer)CURLOPT_CLOSESOCKETDATA),
        defvar(c,"CURLOPT_GSSAPI_DELEGATION", (cpps_integer)CURLOPT_GSSAPI_DELEGATION),
        defvar(c,"CURLOPT_DNS_SERVERS", (cpps_integer)CURLOPT_DNS_SERVERS),
        defvar(c,"CURLOPT_ACCEPTTIMEOUT_MS", (cpps_integer)CURLOPT_ACCEPTTIMEOUT_MS),
        defvar(c,"CURLOPT_TCP_KEEPALIVE", (cpps_integer)CURLOPT_TCP_KEEPALIVE),
        defvar(c,"CURLOPT_TCP_KEEPIDLE", (cpps_integer)CURLOPT_TCP_KEEPIDLE),
        defvar(c,"CURLOPT_TCP_KEEPINTVL", (cpps_integer)CURLOPT_TCP_KEEPINTVL),
        defvar(c,"CURLOPT_SSL_OPTIONS", (cpps_integer)CURLOPT_SSL_OPTIONS),
        defvar(c,"CURLOPT_MAIL_AUTH", (cpps_integer)CURLOPT_MAIL_AUTH),
        defvar(c,"CURLOPT_SASL_IR", (cpps_integer)CURLOPT_SASL_IR),
        defvar(c,"CURLOPT_XFERINFOFUNCTION", (cpps_integer)CURLOPT_XFERINFOFUNCTION),
        defvar(c,"CURLOPT_XOAUTH2_BEARER", (cpps_integer)CURLOPT_XOAUTH2_BEARER),
        defvar(c,"CURLOPT_DNS_INTERFACE", (cpps_integer)CURLOPT_DNS_INTERFACE),
        defvar(c,"CURLOPT_DNS_LOCAL_IP4", (cpps_integer)CURLOPT_DNS_LOCAL_IP4),
        defvar(c,"CURLOPT_DNS_LOCAL_IP6", (cpps_integer)CURLOPT_DNS_LOCAL_IP6),
        defvar(c,"CURLOPT_LOGIN_OPTIONS", (cpps_integer)CURLOPT_LOGIN_OPTIONS),
        defvar(c,"CURLOPT_SSL_ENABLE_NPN", (cpps_integer)CURLOPT_SSL_ENABLE_NPN),
        defvar(c,"CURLOPT_SSL_ENABLE_ALPN", (cpps_integer)CURLOPT_SSL_ENABLE_ALPN),
        defvar(c,"CURLOPT_EXPECT_100_TIMEOUT_MS", (cpps_integer)CURLOPT_EXPECT_100_TIMEOUT_MS),
        defvar(c,"CURLOPT_PROXYHEADER", (cpps_integer)CURLOPT_PROXYHEADER),
        defvar(c,"CURLOPT_HEADEROPT", (cpps_integer)CURLOPT_HEADEROPT),
        defvar(c,"CURLOPT_PINNEDPUBLICKEY", (cpps_integer)CURLOPT_PINNEDPUBLICKEY),
        defvar(c,"CURLOPT_UNIX_SOCKET_PATH", (cpps_integer)CURLOPT_UNIX_SOCKET_PATH),
        defvar(c,"CURLOPT_SSL_VERIFYSTATUS", (cpps_integer)CURLOPT_SSL_VERIFYSTATUS),
        defvar(c,"CURLOPT_SSL_FALSESTART", (cpps_integer)CURLOPT_SSL_FALSESTART),
        defvar(c,"CURLOPT_PATH_AS_IS", (cpps_integer)CURLOPT_PATH_AS_IS),
        defvar(c,"CURLOPT_PROXY_SERVICE_NAME", (cpps_integer)CURLOPT_PROXY_SERVICE_NAME),
        defvar(c,"CURLOPT_SERVICE_NAME", (cpps_integer)CURLOPT_SERVICE_NAME),
        defvar(c,"CURLOPT_PIPEWAIT", (cpps_integer)CURLOPT_PIPEWAIT),
        defvar(c,"CURLOPT_DEFAULT_PROTOCOL", (cpps_integer)CURLOPT_DEFAULT_PROTOCOL),
        defvar(c,"CURLOPT_STREAM_WEIGHT", (cpps_integer)CURLOPT_STREAM_WEIGHT),
        defvar(c,"CURLOPT_STREAM_DEPENDS", (cpps_integer)CURLOPT_STREAM_DEPENDS),
        defvar(c,"CURLOPT_STREAM_DEPENDS_E", (cpps_integer)CURLOPT_STREAM_DEPENDS_E),
        defvar(c,"CURLOPT_TFTP_NO_OPTIONS", (cpps_integer)CURLOPT_TFTP_NO_OPTIONS),
        defvar(c,"CURLOPT_CONNECT_TO", (cpps_integer)CURLOPT_CONNECT_TO),
        defvar(c,"CURLOPT_TCP_FASTOPEN", (cpps_integer)CURLOPT_TCP_FASTOPEN),
        defvar(c,"CURLOPT_KEEP_SENDING_ON_ERROR", (cpps_integer)CURLOPT_KEEP_SENDING_ON_ERROR),
        defvar(c,"CURLOPT_PROXY_CAINFO", (cpps_integer)CURLOPT_PROXY_CAINFO),
        defvar(c,"CURLOPT_PROXY_CAPATH", (cpps_integer)CURLOPT_PROXY_CAPATH),
        defvar(c,"CURLOPT_PROXY_SSL_VERIFYPEER", (cpps_integer)CURLOPT_PROXY_SSL_VERIFYPEER),
        defvar(c,"CURLOPT_PROXY_SSL_VERIFYHOST", (cpps_integer)CURLOPT_PROXY_SSL_VERIFYHOST),
        defvar(c,"CURLOPT_PROXY_SSLVERSION", (cpps_integer)CURLOPT_PROXY_SSLVERSION),
        defvar(c,"CURLOPT_PROXY_TLSAUTH_USERNAME", (cpps_integer)CURLOPT_PROXY_TLSAUTH_USERNAME),
        defvar(c,"CURLOPT_PROXY_TLSAUTH_PASSWORD", (cpps_integer)CURLOPT_PROXY_TLSAUTH_PASSWORD),
        defvar(c,"CURLOPT_PROXY_TLSAUTH_TYPE", (cpps_integer)CURLOPT_PROXY_TLSAUTH_TYPE),
        defvar(c,"CURLOPT_PROXY_SSLCERT", (cpps_integer)CURLOPT_PROXY_SSLCERT),
        defvar(c,"CURLOPT_PROXY_SSLCERTTYPE", (cpps_integer)CURLOPT_PROXY_SSLCERTTYPE),
        defvar(c,"CURLOPT_PROXY_SSLKEY", (cpps_integer)CURLOPT_PROXY_SSLKEY),
        defvar(c,"CURLOPT_PROXY_SSLKEYTYPE", (cpps_integer)CURLOPT_PROXY_SSLKEYTYPE),
        defvar(c,"CURLOPT_PROXY_KEYPASSWD", (cpps_integer)CURLOPT_PROXY_KEYPASSWD),
        defvar(c,"CURLOPT_PROXY_SSL_CIPHER_LIST", (cpps_integer)CURLOPT_PROXY_SSL_CIPHER_LIST),
        defvar(c,"CURLOPT_PROXY_CRLFILE", (cpps_integer)CURLOPT_PROXY_CRLFILE),
        defvar(c,"CURLOPT_PROXY_SSL_OPTIONS", (cpps_integer)CURLOPT_PROXY_SSL_OPTIONS),
        defvar(c,"CURLOPT_PRE_PROXY", (cpps_integer)CURLOPT_PRE_PROXY),
        defvar(c,"CURLOPT_PROXY_PINNEDPUBLICKEY", (cpps_integer)CURLOPT_PROXY_PINNEDPUBLICKEY),
        defvar(c,"CURLOPT_ABSTRACT_UNIX_SOCKET", (cpps_integer)CURLOPT_ABSTRACT_UNIX_SOCKET),
        defvar(c,"CURLOPT_SUPPRESS_CONNECT_HEADERS", (cpps_integer)CURLOPT_SUPPRESS_CONNECT_HEADERS),
        defvar(c,"CURLOPT_REQUEST_TARGET", (cpps_integer)CURLOPT_REQUEST_TARGET),
        defvar(c,"CURLOPT_SOCKS5_AUTH", (cpps_integer)CURLOPT_SOCKS5_AUTH),
        defvar(c,"CURLOPT_SSH_COMPRESSION", (cpps_integer)CURLOPT_SSH_COMPRESSION),
        defvar(c,"CURLOPT_MIMEPOST", (cpps_integer)CURLOPT_MIMEPOST),
        defvar(c,"CURLOPT_TIMEVALUE_LARGE", (cpps_integer)CURLOPT_TIMEVALUE_LARGE),
        defvar(c,"CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS", (cpps_integer)CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS),
        defvar(c,"CURLOPT_RESOLVER_START_FUNCTION", (cpps_integer)CURLOPT_RESOLVER_START_FUNCTION),
        defvar(c,"CURLOPT_RESOLVER_START_DATA", (cpps_integer)CURLOPT_RESOLVER_START_DATA),
        defvar(c,"CURLOPT_HAPROXYPROTOCOL", (cpps_integer)CURLOPT_HAPROXYPROTOCOL),
        defvar(c,"CURLOPT_DNS_SHUFFLE_ADDRESSES", (cpps_integer)CURLOPT_DNS_SHUFFLE_ADDRESSES),
        defvar(c,"CURLOPT_TLS13_CIPHERS", (cpps_integer)CURLOPT_TLS13_CIPHERS),
        defvar(c,"CURLOPT_PROXY_TLS13_CIPHERS", (cpps_integer)CURLOPT_PROXY_TLS13_CIPHERS),
        defvar(c,"CURLOPT_DISALLOW_USERNAME_IN_URL", (cpps_integer)CURLOPT_DISALLOW_USERNAME_IN_URL),
        defvar(c,"CURLOPT_DOH_URL", (cpps_integer)CURLOPT_DOH_URL),
        defvar(c,"CURLOPT_UPLOAD_BUFFERSIZE", (cpps_integer)CURLOPT_UPLOAD_BUFFERSIZE),
        defvar(c,"CURLOPT_UPKEEP_INTERVAL_MS", (cpps_integer)CURLOPT_UPKEEP_INTERVAL_MS),
        defvar(c,"CURLOPT_CURLU", (cpps_integer)CURLOPT_CURLU),
        defvar(c,"CURLOPT_TRAILERFUNCTION", (cpps_integer)CURLOPT_TRAILERFUNCTION),
        defvar(c,"CURLOPT_TRAILERDATA", (cpps_integer)CURLOPT_TRAILERDATA),
        defvar(c,"CURLOPT_HTTP09_ALLOWED", (cpps_integer)CURLOPT_HTTP09_ALLOWED),
        defvar(c,"CURLOPT_ALTSVC_CTRL", (cpps_integer)CURLOPT_ALTSVC_CTRL),
        defvar(c,"CURLOPT_ALTSVC", (cpps_integer)CURLOPT_ALTSVC),
        defvar(c,"CURLOPT_MAXAGE_CONN", (cpps_integer)CURLOPT_MAXAGE_CONN),
        defvar(c,"CURLOPT_SASL_AUTHZID", (cpps_integer)CURLOPT_SASL_AUTHZID),
        defvar(c,"CURLOPT_MAIL_RCPT_ALLLOWFAILS", (cpps_integer)CURLOPT_MAIL_RCPT_ALLLOWFAILS),
        defvar(c,"CURLOPT_SSLCERT_BLOB", (cpps_integer)CURLOPT_SSLCERT_BLOB),
        defvar(c,"CURLOPT_SSLKEY_BLOB", (cpps_integer)CURLOPT_SSLKEY_BLOB),
        defvar(c,"CURLOPT_PROXY_SSLCERT_BLOB", (cpps_integer)CURLOPT_PROXY_SSLCERT_BLOB),
        defvar(c,"CURLOPT_PROXY_SSLKEY_BLOB", (cpps_integer)CURLOPT_PROXY_SSLKEY_BLOB),
        defvar(c,"CURLOPT_ISSUERCERT_BLOB", (cpps_integer)CURLOPT_ISSUERCERT_BLOB),
        defvar(c,"CURLOPT_PROXY_ISSUERCERT", (cpps_integer)CURLOPT_PROXY_ISSUERCERT),
        defvar(c,"CURLOPT_PROXY_ISSUERCERT_BLOB", (cpps_integer)CURLOPT_PROXY_ISSUERCERT_BLOB),
        defvar(c,"CURLOPT_SSL_EC_CURVES", (cpps_integer)CURLOPT_SSL_EC_CURVES),
        //curl_proxytype
        defvar(c,"CURLPROXY_HTTP", (cpps_integer)CURLPROXY_HTTP),
        defvar(c,"CURLPROXY_HTTP_1_0", (cpps_integer)CURLPROXY_HTTP_1_0),
        defvar(c,"CURLPROXY_HTTPS", (cpps_integer)CURLPROXY_HTTPS),
        defvar(c,"CURLPROXY_SOCKS4", (cpps_integer)CURLPROXY_SOCKS4),
        defvar(c,"CURLPROXY_SOCKS5", (cpps_integer)CURLPROXY_SOCKS5),
        defvar(c,"CURLPROXY_SOCKS4A", (cpps_integer)CURLPROXY_SOCKS4A),
        defvar(c,"CURLPROXY_SOCKS5_HOSTNAME", (cpps_integer)CURLPROXY_SOCKS5_HOSTNAME),
        //CURLINFO
        defvar(c,"CURLINFO_EFFECTIVE_URL", (cpps_integer)CURLINFO_EFFECTIVE_URL),
        defvar(c,"CURLINFO_RESPONSE_CODE", (cpps_integer)CURLINFO_RESPONSE_CODE),
        defvar(c,"CURLINFO_TOTAL_TIME", (cpps_integer)CURLINFO_TOTAL_TIME),
        defvar(c,"CURLINFO_NAMELOOKUP_TIME", (cpps_integer)CURLINFO_NAMELOOKUP_TIME),
        defvar(c,"CURLINFO_CONNECT_TIME", (cpps_integer)CURLINFO_CONNECT_TIME),
        defvar(c,"CURLINFO_PRETRANSFER_TIME", (cpps_integer)CURLINFO_PRETRANSFER_TIME),
        defvar(c,"CURLINFO_SIZE_UPLOAD", (cpps_integer)CURLINFO_SIZE_UPLOAD),
        defvar(c,"CURLINFO_SIZE_UPLOAD_T", (cpps_integer)CURLINFO_SIZE_UPLOAD_T),
        defvar(c,"CURLINFO_SIZE_DOWNLOAD", (cpps_integer)CURLINFO_SIZE_DOWNLOAD),
        defvar(c,"CURLINFO_SIZE_DOWNLOAD_T", (cpps_integer)CURLINFO_SIZE_DOWNLOAD_T),
        defvar(c,"CURLINFO_SPEED_DOWNLOAD", (cpps_integer)CURLINFO_SPEED_DOWNLOAD),
        defvar(c,"CURLINFO_SPEED_DOWNLOAD_T", (cpps_integer)CURLINFO_SPEED_DOWNLOAD_T),
        defvar(c,"CURLINFO_SPEED_UPLOAD", (cpps_integer)CURLINFO_SPEED_UPLOAD),
        defvar(c,"CURLINFO_SPEED_UPLOAD_T", (cpps_integer)CURLINFO_SPEED_UPLOAD_T),
        defvar(c,"CURLINFO_HEADER_SIZE", (cpps_integer)CURLINFO_HEADER_SIZE),
        defvar(c,"CURLINFO_REQUEST_SIZE", (cpps_integer)CURLINFO_REQUEST_SIZE),
        defvar(c,"CURLINFO_SSL_VERIFYRESULT", (cpps_integer)CURLINFO_SSL_VERIFYRESULT),
        defvar(c,"CURLINFO_FILETIME", (cpps_integer)CURLINFO_FILETIME),
        defvar(c,"CURLINFO_FILETIME_T", (cpps_integer)CURLINFO_FILETIME_T),
        defvar(c,"CURLINFO_CONTENT_LENGTH_DOWNLOAD", (cpps_integer)CURLINFO_CONTENT_LENGTH_DOWNLOAD),
        defvar(c,"CURLINFO_CONTENT_LENGTH_DOWNLOAD_T", (cpps_integer)CURLINFO_CONTENT_LENGTH_DOWNLOAD_T),
        defvar(c,"CURLINFO_CONTENT_LENGTH_UPLOAD", (cpps_integer)CURLINFO_CONTENT_LENGTH_UPLOAD),
        defvar(c,"CURLINFO_CONTENT_LENGTH_UPLOAD_T", (cpps_integer)CURLINFO_CONTENT_LENGTH_UPLOAD_T),
        defvar(c,"CURLINFO_STARTTRANSFER_TIME", (cpps_integer)CURLINFO_STARTTRANSFER_TIME),
        defvar(c,"CURLINFO_CONTENT_TYPE", (cpps_integer)CURLINFO_CONTENT_TYPE),
        defvar(c,"CURLINFO_REDIRECT_TIME", (cpps_integer)CURLINFO_REDIRECT_TIME),
        defvar(c,"CURLINFO_REDIRECT_COUNT", (cpps_integer)CURLINFO_REDIRECT_COUNT),
        defvar(c,"CURLINFO_PRIVATE", (cpps_integer)CURLINFO_PRIVATE),
        defvar(c,"CURLINFO_HTTP_CONNECTCODE", (cpps_integer)CURLINFO_HTTP_CONNECTCODE),
        defvar(c,"CURLINFO_HTTPAUTH_AVAIL", (cpps_integer)CURLINFO_HTTPAUTH_AVAIL),
        defvar(c,"CURLINFO_PROXYAUTH_AVAIL", (cpps_integer)CURLINFO_PROXYAUTH_AVAIL),
        defvar(c,"CURLINFO_OS_ERRNO", (cpps_integer)CURLINFO_OS_ERRNO),
        defvar(c,"CURLINFO_NUM_CONNECTS", (cpps_integer)CURLINFO_NUM_CONNECTS),
        defvar(c,"CURLINFO_SSL_ENGINES", (cpps_integer)CURLINFO_SSL_ENGINES),
        defvar(c,"CURLINFO_COOKIELIST", (cpps_integer)CURLINFO_COOKIELIST),
        defvar(c,"CURLINFO_LASTSOCKET", (cpps_integer)CURLINFO_LASTSOCKET),
        defvar(c,"CURLINFO_FTP_ENTRY_PATH", (cpps_integer)CURLINFO_FTP_ENTRY_PATH),
        defvar(c,"CURLINFO_REDIRECT_URL", (cpps_integer)CURLINFO_REDIRECT_URL),
        defvar(c,"CURLINFO_PRIMARY_IP", (cpps_integer)CURLINFO_PRIMARY_IP),
        defvar(c,"CURLINFO_APPCONNECT_TIME", (cpps_integer)CURLINFO_APPCONNECT_TIME),
        defvar(c,"CURLINFO_CERTINFO", (cpps_integer)CURLINFO_CERTINFO),
        defvar(c,"CURLINFO_CONDITION_UNMET", (cpps_integer)CURLINFO_CONDITION_UNMET),
        defvar(c,"CURLINFO_RTSP_SESSION_ID", (cpps_integer)CURLINFO_RTSP_SESSION_ID),
        defvar(c,"CURLINFO_RTSP_CLIENT_CSEQ", (cpps_integer)CURLINFO_RTSP_CLIENT_CSEQ),
        defvar(c,"CURLINFO_RTSP_SERVER_CSEQ", (cpps_integer)CURLINFO_RTSP_SERVER_CSEQ),
        defvar(c,"CURLINFO_RTSP_CSEQ_RECV", (cpps_integer)CURLINFO_RTSP_CSEQ_RECV),
        defvar(c,"CURLINFO_PRIMARY_PORT", (cpps_integer)CURLINFO_PRIMARY_PORT),
        defvar(c,"CURLINFO_LOCAL_IP", (cpps_integer)CURLINFO_LOCAL_IP),
        defvar(c,"CURLINFO_LOCAL_PORT", (cpps_integer)CURLINFO_LOCAL_PORT),
        defvar(c,"CURLINFO_TLS_SESSION", (cpps_integer)CURLINFO_TLS_SESSION),
        defvar(c,"CURLINFO_ACTIVESOCKET", (cpps_integer)CURLINFO_ACTIVESOCKET),
        defvar(c,"CURLINFO_TLS_SSL_PTR", (cpps_integer)CURLINFO_TLS_SSL_PTR),
        defvar(c,"CURLINFO_HTTP_VERSION", (cpps_integer)CURLINFO_HTTP_VERSION),
        defvar(c,"CURLINFO_PROXY_SSL_VERIFYRESULT", (cpps_integer)CURLINFO_PROXY_SSL_VERIFYRESULT),
        defvar(c,"CURLINFO_PROTOCOL", (cpps_integer)CURLINFO_PROTOCOL),
        defvar(c,"CURLINFO_SCHEME", (cpps_integer)CURLINFO_SCHEME),
        defvar(c,"CURLINFO_TOTAL_TIME_T", (cpps_integer)CURLINFO_TOTAL_TIME_T),
        defvar(c,"CURLINFO_NAMELOOKUP_TIME_T", (cpps_integer)CURLINFO_NAMELOOKUP_TIME_T),
        defvar(c,"CURLINFO_CONNECT_TIME_T", (cpps_integer)CURLINFO_CONNECT_TIME_T),
        defvar(c,"CURLINFO_PRETRANSFER_TIME_T", (cpps_integer)CURLINFO_PRETRANSFER_TIME_T),
        defvar(c,"CURLINFO_STARTTRANSFER_TIME_T", (cpps_integer)CURLINFO_STARTTRANSFER_TIME_T),
        defvar(c,"CURLINFO_REDIRECT_TIME_T", (cpps_integer)CURLINFO_REDIRECT_TIME_T),
        defvar(c,"CURLINFO_APPCONNECT_TIME_T", (cpps_integer)CURLINFO_APPCONNECT_TIME_T),
        defvar(c,"CURLINFO_RETRY_AFTER", (cpps_integer)CURLINFO_RETRY_AFTER),
        defvar(c,"CURLINFO_EFFECTIVE_METHOD", (cpps_integer)CURLINFO_EFFECTIVE_METHOD),
        defvar(c,"CURLINFO_PROXY_ERROR", (cpps_integer)CURLINFO_PROXY_ERROR),
        //curl_usessl
        defvar(c,"CURLUSESSL_ALL", (cpps_integer)CURLUSESSL_ALL),
        defvar(c,"CURLUSESSL_NONE", (cpps_integer)CURLUSESSL_NONE),
        defvar(c,"CURLUSESSL_TRY", (cpps_integer)CURLUSESSL_TRY),
        defvar(c,"CURLUSESSL_CONTROL", (cpps_integer)CURLUSESSL_CONTROL),
        defvar(c,"CURLUSESSL_LAST", (cpps_integer)CURLUSESSL_LAST),
        //CURLcode
        defvar(c,"CURLE_OK", (cpps_integer)CURLE_OK),
        defvar(c,"CURLE_UNSUPPORTED_PROTOCOL", (cpps_integer)CURLE_UNSUPPORTED_PROTOCOL),
        defvar(c,"CURLE_FAILED_INIT", (cpps_integer)CURLE_FAILED_INIT),
        defvar(c,"CURLE_URL_MALFORMAT", (cpps_integer)CURLE_URL_MALFORMAT),
        defvar(c,"CURLE_NOT_BUILT_IN", (cpps_integer)CURLE_NOT_BUILT_IN),
        defvar(c,"CURLE_COULDNT_RESOLVE_PROXY", (cpps_integer)CURLE_COULDNT_RESOLVE_PROXY),
        defvar(c,"CURLE_COULDNT_RESOLVE_HOST", (cpps_integer)CURLE_COULDNT_RESOLVE_HOST),
        defvar(c,"CURLE_COULDNT_CONNECT", (cpps_integer)CURLE_COULDNT_CONNECT),
        defvar(c,"CURLE_WEIRD_SERVER_REPLY", (cpps_integer)CURLE_WEIRD_SERVER_REPLY),
        defvar(c,"CURLE_REMOTE_ACCESS_DENIED", (cpps_integer)CURLE_REMOTE_ACCESS_DENIED),
        defvar(c,"CURLE_FTP_ACCEPT_FAILED", (cpps_integer)CURLE_FTP_ACCEPT_FAILED),
        defvar(c,"CURLE_FTP_WEIRD_PASS_REPLY", (cpps_integer)CURLE_FTP_WEIRD_PASS_REPLY),
        defvar(c,"CURLE_FTP_ACCEPT_TIMEOUT", (cpps_integer)CURLE_FTP_ACCEPT_TIMEOUT),
        defvar(c,"CURLE_FTP_WEIRD_PASV_REPLY", (cpps_integer)CURLE_FTP_WEIRD_PASV_REPLY),
        defvar(c,"CURLE_FTP_WEIRD_227_FORMAT", (cpps_integer)CURLE_FTP_WEIRD_227_FORMAT),
        defvar(c,"CURLE_FTP_CANT_GET_HOST", (cpps_integer)CURLE_FTP_CANT_GET_HOST),
        defvar(c,"CURLE_HTTP2", (cpps_integer)CURLE_HTTP2),
        defvar(c,"CURLE_FTP_COULDNT_SET_TYPE", (cpps_integer)CURLE_FTP_COULDNT_SET_TYPE),
        defvar(c,"CURLE_PARTIAL_FILE", (cpps_integer)CURLE_PARTIAL_FILE),
        defvar(c,"CURLE_FTP_COULDNT_RETR_FILE", (cpps_integer)CURLE_FTP_COULDNT_RETR_FILE),
        defvar(c,"CURLE_OBSOLETE20", (cpps_integer)CURLE_OBSOLETE20),
        defvar(c,"CURLE_QUOTE_ERROR", (cpps_integer)CURLE_QUOTE_ERROR),
        defvar(c,"CURLE_HTTP_RETURNED_ERROR", (cpps_integer)CURLE_HTTP_RETURNED_ERROR),
        defvar(c,"CURLE_WRITE_ERROR", (cpps_integer)CURLE_WRITE_ERROR),
        defvar(c,"CURLE_OBSOLETE24", (cpps_integer)CURLE_OBSOLETE24),
        defvar(c,"CURLE_UPLOAD_FAILED", (cpps_integer)CURLE_UPLOAD_FAILED),
        defvar(c,"CURLE_READ_ERROR", (cpps_integer)CURLE_READ_ERROR),
        defvar(c,"CURLE_OUT_OF_MEMORY", (cpps_integer)CURLE_OUT_OF_MEMORY),
        defvar(c,"CURLE_OPERATION_TIMEDOUT", (cpps_integer)CURLE_OPERATION_TIMEDOUT),
        defvar(c,"CURLE_OBSOLETE29", (cpps_integer)CURLE_OBSOLETE29),
        defvar(c,"CURLE_FTP_PORT_FAILED", (cpps_integer)CURLE_FTP_PORT_FAILED),
        defvar(c,"CURLE_FTP_COULDNT_USE_REST", (cpps_integer)CURLE_FTP_COULDNT_USE_REST),
        defvar(c,"CURLE_OBSOLETE32", (cpps_integer)CURLE_OBSOLETE32),
        defvar(c,"CURLE_RANGE_ERROR", (cpps_integer)CURLE_RANGE_ERROR),
        defvar(c,"CURLE_HTTP_POST_ERROR", (cpps_integer)CURLE_HTTP_POST_ERROR),
        defvar(c,"CURLE_SSL_CONNECT_ERROR", (cpps_integer)CURLE_SSL_CONNECT_ERROR),
        defvar(c,"CURLE_BAD_DOWNLOAD_RESUME", (cpps_integer)CURLE_BAD_DOWNLOAD_RESUME),
        defvar(c,"CURLE_FILE_COULDNT_READ_FILE", (cpps_integer)CURLE_FILE_COULDNT_READ_FILE),
        defvar(c,"CURLE_LDAP_CANNOT_BIND", (cpps_integer)CURLE_LDAP_CANNOT_BIND),
        defvar(c,"CURLE_LDAP_SEARCH_FAILED", (cpps_integer)CURLE_LDAP_SEARCH_FAILED),
        defvar(c,"CURLE_OBSOLETE40", (cpps_integer)CURLE_OBSOLETE40),
        defvar(c,"CURLE_FUNCTION_NOT_FOUND", (cpps_integer)CURLE_FUNCTION_NOT_FOUND),
        defvar(c,"CURLE_ABORTED_BY_CALLBACK", (cpps_integer)CURLE_ABORTED_BY_CALLBACK),
        defvar(c,"CURLE_BAD_FUNCTION_ARGUMENT", (cpps_integer)CURLE_BAD_FUNCTION_ARGUMENT),
        defvar(c,"CURLE_OBSOLETE44", (cpps_integer)CURLE_OBSOLETE44),
        defvar(c,"CURLE_INTERFACE_FAILED", (cpps_integer)CURLE_INTERFACE_FAILED),
        defvar(c,"CURLE_OBSOLETE46", (cpps_integer)CURLE_OBSOLETE46),
        defvar(c,"CURLE_TOO_MANY_REDIRECTS", (cpps_integer)CURLE_TOO_MANY_REDIRECTS),
        defvar(c,"CURLE_UNKNOWN_OPTION", (cpps_integer)CURLE_UNKNOWN_OPTION),
        defvar(c,"CURLE_TELNET_OPTION_SYNTAX", (cpps_integer)CURLE_TELNET_OPTION_SYNTAX),
        defvar(c,"CURLE_OBSOLETE50", (cpps_integer)CURLE_OBSOLETE50),
        defvar(c,"CURLE_OBSOLETE51", (cpps_integer)CURLE_OBSOLETE51),
        defvar(c,"CURLE_GOT_NOTHING", (cpps_integer)CURLE_GOT_NOTHING),
        defvar(c,"CURLE_SSL_ENGINE_NOTFOUND", (cpps_integer)CURLE_SSL_ENGINE_NOTFOUND),
        defvar(c,"CURLE_SSL_ENGINE_SETFAILED", (cpps_integer)CURLE_SSL_ENGINE_SETFAILED),
        defvar(c,"CURLE_SEND_ERROR", (cpps_integer)CURLE_SEND_ERROR),
        defvar(c,"CURLE_RECV_ERROR", (cpps_integer)CURLE_RECV_ERROR),
        defvar(c,"CURLE_OBSOLETE57", (cpps_integer)CURLE_OBSOLETE57),
        defvar(c,"CURLE_SSL_CERTPROBLEM", (cpps_integer)CURLE_SSL_CERTPROBLEM),
        defvar(c,"CURLE_SSL_CIPHER", (cpps_integer)CURLE_SSL_CIPHER),
        defvar(c,"CURLE_PEER_FAILED_VERIFICATION", (cpps_integer)CURLE_PEER_FAILED_VERIFICATION),
        defvar(c,"CURLE_BAD_CONTENT_ENCODING", (cpps_integer)CURLE_BAD_CONTENT_ENCODING),
        defvar(c,"CURLE_LDAP_INVALID_URL", (cpps_integer)CURLE_LDAP_INVALID_URL),
        defvar(c,"CURLE_FILESIZE_EXCEEDED", (cpps_integer)CURLE_FILESIZE_EXCEEDED),
        defvar(c,"CURLE_USE_SSL_FAILED", (cpps_integer)CURLE_USE_SSL_FAILED),
        defvar(c,"CURLE_SEND_FAIL_REWIND", (cpps_integer)CURLE_SEND_FAIL_REWIND),
        defvar(c,"CURLE_SSL_ENGINE_INITFAILED", (cpps_integer)CURLE_SSL_ENGINE_INITFAILED),
        defvar(c,"CURLE_LOGIN_DENIED", (cpps_integer)CURLE_LOGIN_DENIED),
        defvar(c,"CURLE_TFTP_NOTFOUND", (cpps_integer)CURLE_TFTP_NOTFOUND),
        defvar(c,"CURLE_TFTP_PERM", (cpps_integer)CURLE_TFTP_PERM),
        defvar(c,"CURLE_REMOTE_DISK_FULL", (cpps_integer)CURLE_REMOTE_DISK_FULL),
        defvar(c,"CURLE_TFTP_ILLEGAL", (cpps_integer)CURLE_TFTP_ILLEGAL),
        defvar(c,"CURLE_TFTP_UNKNOWNID", (cpps_integer)CURLE_TFTP_UNKNOWNID),
        defvar(c,"CURLE_REMOTE_FILE_EXISTS", (cpps_integer)CURLE_REMOTE_FILE_EXISTS),
        defvar(c,"CURLE_TFTP_NOSUCHUSER", (cpps_integer)CURLE_TFTP_NOSUCHUSER),
        defvar(c,"CURLE_CONV_FAILED", (cpps_integer)CURLE_CONV_FAILED),
        defvar(c,"CURLE_CONV_REQD", (cpps_integer)CURLE_CONV_REQD),
        defvar(c,"CURLE_SSL_CACERT_BADFILE", (cpps_integer)CURLE_SSL_CACERT_BADFILE),
        defvar(c,"CURLE_REMOTE_FILE_NOT_FOUND", (cpps_integer)CURLE_REMOTE_FILE_NOT_FOUND),
        defvar(c,"CURLE_SSH", (cpps_integer)CURLE_SSH),
        defvar(c,"CURLE_SSL_SHUTDOWN_FAILED", (cpps_integer)CURLE_SSL_SHUTDOWN_FAILED),
        defvar(c,"CURLE_AGAIN", (cpps_integer)CURLE_AGAIN),
        defvar(c,"CURLE_SSL_CRL_BADFILE", (cpps_integer)CURLE_SSL_CRL_BADFILE),
        defvar(c,"CURLE_SSL_ISSUER_ERROR", (cpps_integer)CURLE_SSL_ISSUER_ERROR),
        defvar(c,"CURLE_FTP_PRET_FAILED", (cpps_integer)CURLE_FTP_PRET_FAILED),
        defvar(c,"CURLE_RTSP_CSEQ_ERROR", (cpps_integer)CURLE_RTSP_CSEQ_ERROR),
        defvar(c,"CURLE_RTSP_SESSION_ERROR", (cpps_integer)CURLE_RTSP_SESSION_ERROR),
        defvar(c,"CURLE_FTP_BAD_FILE_LIST", (cpps_integer)CURLE_FTP_BAD_FILE_LIST),
        defvar(c,"CURLE_CHUNK_FAILED", (cpps_integer)CURLE_CHUNK_FAILED),
        defvar(c,"CURLE_NO_CONNECTION_AVAILABLE", (cpps_integer)CURLE_NO_CONNECTION_AVAILABLE),
        defvar(c,"CURLE_SSL_PINNEDPUBKEYNOTMATCH", (cpps_integer)CURLE_SSL_PINNEDPUBKEYNOTMATCH),
        defvar(c,"CURLE_SSL_INVALIDCERTSTATUS", (cpps_integer)CURLE_SSL_INVALIDCERTSTATUS),
        defvar(c,"CURLE_HTTP2_STREAM", (cpps_integer)CURLE_HTTP2_STREAM),
        defvar(c,"CURLE_RECURSIVE_API_CALL", (cpps_integer)CURLE_RECURSIVE_API_CALL),
        defvar(c,"CURLE_AUTH_ERROR", (cpps_integer)CURLE_AUTH_ERROR),
        defvar(c,"CURLE_HTTP3", (cpps_integer)CURLE_HTTP3),
        defvar(c,"CURLE_QUIC_CONNECT_ERROR", (cpps_integer)CURLE_QUIC_CONNECT_ERROR),
        defvar(c,"CURLE_PROXY", (cpps_integer)CURLE_PROXY)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
    curl_global_cleanup();
}

cpps_export_finish