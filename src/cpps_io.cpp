#include "cpps/cpps.h"

namespace cpps
{
	std::string cpps_string_real_join(std::string sep, std::vector<std::string>& vec);
	std::string	cpps_io_string_replace(std::string v, std::string v2, std::string v3)
	{
		std::string::size_type pos = 0;

		while ((pos = v.find(v2, pos)) != std::string::npos)
		{
			v.replace(pos, v2.length(), v3);
			pos += v3.length();
		}

		return v;
	}
	void cpps_io_string_split(std::vector<std::string> &vct, std::string v, std::string v2)
	{
		if (v.empty()) return ;

		const char* a = v.c_str(); const char* b;
		while (true)
		{
			b = strstr(a, v2.c_str());

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));

			vct.push_back( s );

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + v2.size();

		}
	}
	cpps_value cpps_io_getc(C *c)
	{
		std::string ret;

		std::cin >> ret;

		return cpps_value(c,ret);
	}
#ifndef _WIN32
#include <termios.h>
#endif
	cpps_integer cpps_io_getch() {
#ifdef _WIN32
		int ret = _getch();
#else
		struct termios stored_settings;
		struct termios new_settings;
		tcgetattr(0, &stored_settings);
		new_settings = stored_settings;
		new_settings.c_lflag &= (~ICANON);
		new_settings.c_cc[VTIME] = 0;
		new_settings.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &new_settings);

		if (system("stty -echo")) {}
		int ret = getchar();
		if (system("stty echo")) {}
		tcsetattr(0, TCSANOW, &stored_settings);
#endif
		return (cpps_integer)ret;
	}
	cpps_value cpps_io_getline(C *c)
	{
		std::string ret;
		getline(std::cin, ret);
		return cpps_value(c,ret);
	}

	
	FILE *		cpps_io_open(std::string filepath, std::string mode)
	{
		FILE *ret = NULL;
#ifdef _WIN32
		fopen_s(&ret, filepath.c_str(), mode.c_str());
#else
		ret = fopen(filepath.c_str(), mode.c_str());
#endif
		return ret;
	}
	
	cpps_integer cpps_io_size(FILE *file)
	{
		long cur = ftell(file);
		fseek(file, 0, SEEK_END);
		cpps_integer ret = ftell(file);
		fseek(file, cur, SEEK_SET);
		return ret;
	}
	bool cpps_io_writefile(std::string filepath, std::string content) {
		FILE* file = cpps_io_open(filepath, "wb+");
		if (file)
		{
			fwrite(content.c_str(), content.size(), 1, file);
			fclose(file);
			return true;
		}
		return false;
	}
	bool cpps_io_appendfile(std::string filepath, std::string content) {
		FILE* file = cpps_io_open(filepath, "ab+");
		if (file)
		{
			fwrite(content.c_str(), content.size(), 1, file);
			fclose(file);
			return true;
		}
		return false;
	}
	cpps_integer cpps_io_filesize(std::string filepath) {
		FILE* file = cpps_io_open(filepath, "rb");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			cpps_integer ret = ftell(file);
			fclose(file);
			return ret;
		}
		return 0;
	}
	std::string cpps_io_readfile(std::string filepath)
	{
		std::string ret;
		FILE* file = cpps_io_open(filepath, "rb");
		if (file)
		{
			size_t size = (size_t)cpps_io_size(file);
			char* buf = (char*)CPPSMALLOC(size);
			memset(buf, 0, size);
			if (fread(buf, size, 1, file)) {};
			fclose(file);
			ret.append(buf, size);
			CPPSFREE( buf);
			buf = NULL;
		}
		return ret;
	}
	cpps_integer	cpps_io_read(FILE *file, Buffer *buf, cpps_integer len)
	{
		buf->realloc(buf->length() + len);
		return fread(buf->getbuffer() + buf->length(), 1, (size_t)len, file);
	}
	std::string cpps_io_getlines(FILE *file)
	{
		std::string ret = "";
		char s[sizeof(size_t)];
		while (true)
		{
			memset(s, 0, sizeof(size_t));
			size_t result = fread(s, 1, 1, file);
			if (result != 1) break;
			if (s[0] == '\n') break;
			if (s[0] != '\r') ret += s;
		}
		return ret;
	}
	void		cpps_io_write(FILE *file, Buffer *buf)
	{
		fwrite(buf->getbuffer(), (size_t)buf->length(), 1, file);
	}
	void		cpps_io_writes(FILE *file, std::string buf)
	{
		fwrite(buf.c_str(), buf.size(), 1, file);
	}
		
	void		cpps_io_seek(FILE *file, cpps_integer sk)
	{
		if (sk == -1)
			fseek(file, 0, SEEK_END);
		else
			fseek(file, (long)sk, SEEK_SET);
	}
	void	cpps_io_fflush(FILE *file)
	{
		fflush(file);
	}
	void		cpps_io_close(FILE *file)
	{
		fclose(file);
	}
	std::string cpps_io_getfileext(std::string str)
	{
		size_t pos = str.find(".");
		if (pos != std::string::npos)
		{
			return str.substr(pos + 1);
		}

		return "";
	}
	std::string cpps_io_getfilepath(std::string str)
	{
		str = cpps_io_string_replace(str, "\\", "/");

		size_t pos = str.find_last_of("/");
		if (pos != std::string::npos)
		{
			return str.substr(0,pos);
		}

		return "";
	}
	std::string cpps_io_getfilename(std::string str)
	{
		str = cpps_io_string_replace(str, "\\", "/");

		size_t pos = str.find_last_of("/");
		if (pos != std::string::npos)
		{
			return str.substr(pos + 1);
		}

		return str;
	}
	std::string getfilenamenotext(std::string str)
	{
		str = cpps_io_getfilename(str);
		size_t pos = str.find(".");
		if (pos != std::string::npos)
		{
			return str.substr(0,pos);
		}
		return str;
	}
	cpps_integer cpps_io_remove(std::string filepath)
	{
		return remove(filepath.c_str());
	}
	cpps_integer cpps_io_rename(std::string o, std::string n)
	{
		return rename(o.c_str(),n.c_str());
	}
	cpps_integer cpps_io_mkdir(std::string p)
	{
		return _mkdir(p.c_str());
	}

	
	cpps_integer cpps_io_mkdirs(std::string szdir)
	{
#ifdef _WIN32
		if (-1 != _access(szdir.c_str(), 0))
			return 0;
#else

		if (-1 != access(szdir.c_str(), 0))
			return 0;
#endif

		std::string strdir = szdir;
		cpps_integer iret = 0;
		size_t index = strdir.find_last_of('/');
		if ( index != std::string::npos) //存在多级目录
		{
			strdir.erase(index, strdir.length() - index);

#ifdef _WIN32
			if (-1 == _access(strdir.c_str(), 0)) 
			{
				iret = cpps_io_mkdirs(strdir.c_str());
			}
#else
		
			if (-1 == access(strdir.c_str(), 0)) 
			{
				iret = cpps_io_mkdirs(strdir.c_str());
			}
#endif
		}

		if (0 == iret)
		{
#ifdef _WIN32
			iret = _mkdir(szdir.c_str());
#else
			iret = mkdir(szdir.c_str(), 0755);
#endif
		}

		return iret;
	}
	bool cpps_io_file_exists(std::string path)
	{
#ifdef _WIN32
		if (_access(path.c_str(), 0) == -1)
#else
		if (access(path.c_str(), 0) == -1)
#endif
			return false;
		else
			return true;
	}
	std::string cpps_getcwd()
	{
		char buffer[4096];
#ifdef _WIN32
		_getcwd(buffer, 4096);
#else
		if (getcwd(buffer, 4096)) {}
#endif
		return buffer;
	}

	//using namespace std::literals::chrono_literals;
	cpps_integer cpps_io_last_write_time( std::string path) {
		/*auto ftime = std::filesystem::last_write_time(path);
		auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
		auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count() - elapse;*/

#ifdef _WIN32
		struct _stat64 statinfo;
		_stati64(path.c_str(), &statinfo);
#else 
		struct stat statinfo;
		lstat(path.c_str(), &statinfo);
#endif
		
		return (cpps_integer)statinfo.st_mtime;
	}
	cpps_integer cpps_io_getctime( std::string path) {
		/*auto ftime = std::filesystem::last_write_time(path);
		auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
		auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count() - elapse;*/

#ifdef _WIN32
		struct _stat64 statinfo;
		_stati64(path.c_str(), &statinfo);
#else 
		struct stat statinfo;
		lstat(path.c_str(), &statinfo);
#endif
		
		return (cpps_integer)statinfo.st_ctime;
	}
	cpps_integer cpps_io_getatime( std::string path) {
		/*auto ftime = std::filesystem::last_write_time(path);
		auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
		auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count() - elapse;*/

#ifdef _WIN32
		struct _stat64 statinfo;
		_stati64(path.c_str(), &statinfo);
#else 
		struct stat statinfo;
		lstat(path.c_str(), &statinfo);
#endif
		
		return (cpps_integer)statinfo.st_atime;
	}
#ifdef _WIN32
#include <Shlobj.h>

	std::string cpps_io_getappdata(cpps_value v) {
		char m_lpszDefaultDir[MAX_PATH];
		char szDocument[MAX_PATH] = { 0 };
		memset(m_lpszDefaultDir, 0, _MAX_PATH);

		LPITEMIDLIST pidl = NULL;
		SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
		if (pidl && SHGetPathFromIDList(pidl, szDocument))
		{
			GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
		}
		if (cpps_isnull(v)) {
			return m_lpszDefaultDir;
		}
		else if (cpps_isstring(v)){
			std::string path = m_lpszDefaultDir;
			path.append("\\");
			path.append(*cpps_get_string(v));

			if (_access(path.c_str(), 0) == -1)
			{
				_mkdir(path.c_str());
			}
			return path;
		}
		return m_lpszDefaultDir;
	}
#endif
	cpps_value cpps_io_get_stat(C* c, std::string path) {

		cpps_io_stat* st = NULL;
		cpps_value ret;
		newclass<cpps_io_stat>(c, &st,&ret);

#ifdef _WIN32
		_stati64(path.c_str(), &st->statinfo);
#else
		lstat(path.c_str(), &st->statinfo);
#endif
		return ret;
	}

	bool cpps_io_isdir(std::string p)
	{
		p = cpps_io_string_replace(p, "\\", "/");

#ifdef _WIN32
		struct _stat64 statinfo;
		_stati64(p.c_str(), &statinfo);
#else 
		struct stat statinfo;
		lstat(p.c_str(), &statinfo);
#endif
		return S_ISDIR(statinfo.st_mode);
	}
	
	void cpps_io_real_splitdrive(std::vector<std::string> &vct,std::string path) {

		const char* sep = "\\";
		const char* altsep = "/";
		const char colon = ':';
		if (path.size() >= 2) {

			std::string normp = cpps_io_string_replace(path, altsep, sep);
			if (normp.find("\\\\") == 0 && normp.substr(2, 1) != sep)
			{
				size_t index = normp.find(sep, 2);
				if (index == std::string::npos) {
					vct.push_back(path.substr(0, 1));
					vct.push_back(path);
					return;
				}
				size_t index2 = normp.find(sep, index);
				if (index2 == index + 1) {
					vct.push_back(path.substr(0, 1));
					vct.push_back(path);
					return;
				}
				if (index == std::string::npos) {
					index2 = path.size();
				}
				vct.push_back(path.substr(0, index2));
				vct.push_back(path.substr(index2 ));
				return;
			}
			if (normp[1] == colon) {
				vct.push_back(path.substr(0, 2));
				vct.push_back(path.substr(2));
				return;
			}
		}
		if (path[0] == '/')
			vct.push_back(path.substr(0, 1));
		else
			vct.push_back("");
		vct.push_back(path);
	}
	cpps_value cpps_io_splitdrive(C* c, std::string path) {

		std::vector<std::string> realvct;
		cpps_io_real_splitdrive(realvct, path);

		cpps_vector* vct = NULL;
		cpps_value ret;
		newclass<cpps_vector>(c, &vct,&ret);
		vct->push_back(cpps_value(c, realvct[0]));
		vct->push_back(cpps_value(c, realvct[1]));
		return ret;
	}
	bool cpps_io_isabspath(std::string path)
	{
		std::vector<std::string> realvct;
		cpps_io_real_splitdrive(realvct, path);

		return (realvct.size() == 2 && !realvct[0].empty());
	}
	std::string cpps_io_normpath(std::string path)
	{

#ifdef _WIN32
		const char* sep = "\\";
		const char* altsep = "/";
		const char* special_prefixes[2] = { "\\\\.\\" ,"\\\\?\\" };
#else
		const char* sep = "/";
		const char* altsep = "\\";
		const char* special_prefixes[2] = { "//./" ,"//?/" };
#endif
		const char* curdir = ".";
		const char* pardir = "..";

		for (int i = 0; i < 2; i++) {

			if (path.find(special_prefixes[i]) == 0) {
				return path;
			}
		}

		path = cpps_io_string_replace(path, altsep, sep);

		std::vector<std::string> realvct;
		cpps_io_real_splitdrive(realvct, path);

		std::string prefix = realvct[0];
		path = realvct[1];

		if (path.find(sep) == 0)
		{
			prefix += sep;
			path.erase(path.begin());
		}

		realvct.clear();
		cpps_io_string_split(realvct, path, sep);

		std::vector<std::string>::iterator i = realvct.begin();
		while (i != realvct.end()) {
			std::string& s = *i;
			if (s == curdir) {
				i = realvct.erase(i);
			}
			else if (s == pardir) //   home/abd/../abcd  -> home/abcd
			{
				if (i != realvct.begin() && (*(i - 1)) != pardir)
					i = realvct.erase(i - 1, i+1);
				else if (i == realvct.begin() && prefix[prefix.size() - 1] == sep[0])
					i = realvct.erase(i);
				else
					++i;
			}
			else
				++i;
		}
		if(prefix.empty() && realvct.empty())
			realvct.push_back(curdir);

		return prefix + cpps_string_real_join(sep,realvct);

	}
	bool cpps_io_isfile(std::string p)
	{
		p = cpps_io_string_replace(p, "\\", "/");

#ifdef _WIN32
		struct _stat64 statinfo;
		_stati64(p.c_str(), &statinfo);
#else 
		struct stat statinfo;
		lstat(p.c_str(), &statinfo);
#endif
		return S_ISREG(statinfo.st_mode);
	}
	void cpps_cpp_real_walk(std::vector<std::string>& vct,std::string path, bool bfindchildren) {
		char dirNew[4096];


		path = cpps_io_string_replace(path, "\\", "/");

		strcpy(dirNew, path.c_str());
#if defined _WIN32
		strcat(dirNew, "/*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
		intptr_t handle;
		_finddata_t findData;

		handle = _findfirst(dirNew, &findData);
		if (handle == -1)        // 检查是否成功
			return;



		do
		{
			strcpy(dirNew, path.c_str());
			strcat(dirNew, "/");
			strcat(dirNew, findData.name);

			if (findData.attrib & _A_SUBDIR)
			{
				if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
					continue;

				vct.push_back( dirNew);

				if(bfindchildren)
					cpps_cpp_real_walk( vct,dirNew, bfindchildren);
			}
			else
				vct.push_back(dirNew);

		} while (_findnext(handle, &findData) == 0);

		_findclose(handle);    // 关闭搜索句柄
#else
		struct dirent* filename;    // return value for readdir()
		DIR* dir;                   // return value for opendir()
		dir = opendir(dirNew);
		if (NULL == dir)
		{
			return;
		}

		/* read all the files in the dir ~ */
		while ((filename = readdir(dir)) != NULL)
		{

			strcpy(dirNew, path.c_str());
			strcat(dirNew, "/");
			strcat(dirNew, filename->d_name);

			// get rid of "." and ".."
			if (strcmp(filename->d_name, ".") == 0 ||
				strcmp(filename->d_name, "..") == 0)
				continue;

			
			vct.push_back( dirNew );

			struct stat s;
			lstat(dirNew, &s);
			if (S_ISDIR(s.st_mode))
			{
				if (bfindchildren)
					cpps_cpp_real_walk(vct, dirNew, bfindchildren);
			}
		}
#endif
	}
	bool cpps_io_real_filename_filter(char const* haystack, char const* needle) {
		for (; *needle != '\0'; ++needle) {
			switch (*needle) {
			case '?':
				if (*haystack == '\0')
					return false;
				++haystack;
				break;
			case '.':
				if (*haystack != '.')
					return false;
				++haystack;
				break;
			case '*': {
				if (needle[1] == '\0')
					return true;
				size_t max = strlen(haystack);
				for (size_t i = 0; i < max; i++)
					if (cpps_io_real_filename_filter(haystack + i, needle + 1))
						return true;
				return false;
			}
			default:
				if (*haystack != *needle)
					return false;
				++haystack;
			}
		}
		return *haystack == '\0';
	}
	bool cpps_io_filename_filter(std::string filepath, std::string filter)
	{
		std::string filename = cpps_io_getfilename(filepath);
		/*
			filter :
			*.*
			a*.*
			*a.*
			*.a*
			*.*a             =  
			*a*b*c*.*a*b*c*  =  xaxbxc.xaxbxc 
		*/
		return cpps_io_real_filename_filter(filename.c_str(),filter.c_str());
	}

	void cpps_real_walk(C*c,cpps_vector* vct,std::string path, bool bfindchildren = false,bool onlydir = false,std::string filter = "") {
		char dirNew[200];


		path = cpps_io_string_replace(path, "\\", "/");

		strcpy(dirNew, path.c_str());
#if defined _WIN32
		strcat(dirNew, "/*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
		intptr_t handle;
		_finddata_t findData;

		handle = _findfirst(dirNew, &findData);
		if (handle == -1)        // 检查是否成功
			return;



		do
		{
			strcpy(dirNew, path.c_str());
			if(path[path.size() - 1] != '/') strcat(dirNew, "/");
			strcat(dirNew, findData.name);

			if (findData.attrib & _A_SUBDIR)
			{
				if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
					continue;

				if(filter.empty())
					vct->push_back(cpps_value(c, dirNew));

				if(bfindchildren)
					cpps_real_walk(c, vct,dirNew, bfindchildren,onlydir,filter);
			}
			else if (!onlydir) {
				if(filter.empty() || cpps_io_filename_filter(dirNew,filter))
					vct->push_back(cpps_value(c, dirNew));
			}

		} while (_findnext(handle, &findData) == 0);

		_findclose(handle);    // 关闭搜索句柄
#else
		struct dirent* filename;    // return value for readdir()
		DIR* dir;                   // return value for opendir()
		dir = opendir(dirNew);
		if (NULL == dir)
		{
			return;
		}

		/* read all the files in the dir ~ */
		while ((filename = readdir(dir)) != NULL)
		{

			strcpy(dirNew, path.c_str());
			if (path[path.size() - 1] != '/') strcat(dirNew, "/");
			strcat(dirNew, filename->d_name);

			// get rid of "." and ".."
			if (strcmp(filename->d_name, ".") == 0 ||
				strcmp(filename->d_name, "..") == 0)
				continue;

			

			struct stat s;
			lstat(dirNew, &s);
			if (S_ISDIR(s.st_mode))
			{
				if (filter.empty())
					vct->push_back(cpps_value(c, dirNew));
				if (bfindchildren)
					cpps_real_walk(c, vct, dirNew, bfindchildren,onlydir,filter);
			}
			else if (!onlydir)
			{
				if (filter.empty() || cpps_io_filename_filter(dirNew, filter))
					vct->push_back(cpps_value(c, dirNew));
			}
		}
#endif
	}
	cpps_value cpps_io_walk( C *c,std::string path,cpps_value findchildren,cpps_value filter) {
		
		cpps_vector* vct = NULL;
		cpps_value ret;
		newclass<cpps_vector>(c, &vct,&ret);
		
		bool bfindchildren = findchildren.tt == CPPS_TBOOLEAN ? findchildren.value.b : true;
		std::string sfilter = "";
		if (cpps_isstring(filter)) sfilter = cpps_to_string(filter);
		cpps_real_walk(c, vct, path, bfindchildren,false, sfilter);
		return ret;
	}
	cpps_value cpps_io_listdir(C* c, std::string path, cpps_value findchildren) {

		cpps_vector* vct = NULL;
		cpps_value ret;
		newclass<cpps_vector>(c, &vct,&ret);

		bool bfindchildren = findchildren.tt == CPPS_TBOOLEAN ? findchildren.value.b : true;
		cpps_real_walk(c, vct, path, bfindchildren,true);
		return ret;
	}
	std::string cpps_real_path()
	{
		char abs_path[4096];
		memset(abs_path, 0, 4096);
#if defined _WIN32
		GetModuleFileNameA(NULL, abs_path, 4096);
		size_t cnt = strlen(abs_path);
		if (cnt != 0) {
			for (int64 i = (int64)cnt; i >= 0; --i){
				if (abs_path[i] == '\\')
				{
					abs_path[i + 1] = '\0';
					break;
				}
			}
		}
		
#elif defined LINUX
		size_t cnt = readlink("/proc/self/exe", abs_path, 4096);//获取可执行程序的绝对路径
		if (cnt < 0 || cnt >= 4096)
		{
			return "";
		}
		
		//最后一个'/' 后面是可执行程序名，去掉devel/lib/m100/exe，只保留前面部分路径

		for (size_t i = cnt; i >= 0; --i)
		{
			if (abs_path[i] == '/')
			{
				abs_path[i + 1] = '\0';
				break;
			}
		}
#elif defined __APPLE__
		usint32 size = 4096;
		_NSGetExecutablePath(abs_path, &size);

		//mac os 下返回的不是真实地址而是软连接地址.
		//需要读取软连接的地址真实地址
		struct stat statinfo;
		lstat(abs_path, &statinfo);
		if (S_ISLNK(statinfo.st_mode))
		{
			size = readlink(abs_path, abs_path, 4096);
			if (size < 0 || size >= 4096)
			{
				return "";
			}
		}

		for (size_t i = (size_t)size; i >= 0; --i)
		{
			if (abs_path[i] == '/')
			{
				abs_path[i + 1] = '\0';
				break;
			}
		}
		


#endif
		
		return abs_path;
	}

	cpps_integer cpps_io_copy(std::string sourcefile, std::string targetfile) {
		cpps_io_mkdirs(cpps_io_getfilepath(targetfile));

		// int c to store one char at a time
		sourcefile = cpps_io_string_replace(sourcefile, "\\", "/");
		targetfile = cpps_io_string_replace(targetfile, "\\", "/");
		
		// declare and open files for copy
		FILE* in_ptr = fopen(sourcefile.c_str(), "rb");
		FILE* out_ptr = fopen(targetfile.c_str(), "wb+");

		if (!in_ptr) {
			return -1;
		}

		if (!out_ptr) {
			return -1;
		}
		char tmpbuf[4096];
		
		size_t filesize = 0;
		while (true) {
			size_t size = fread(tmpbuf,1 , 4096, in_ptr);
			if (size == 0) break;
			fwrite(tmpbuf, size, 1, out_ptr);
			filesize += size;
		}
		// close files
		fclose(in_ptr);
		fclose(out_ptr);
		return filesize;
	}
	cpps_integer cpps_io_move(std::string sourcefile, std::string targetfile) {
		cpps_integer ret = cpps_io_copy(sourcefile, targetfile);
		if (ret >= 0) {
			cpps_io_remove(sourcefile);
			return 0;
		}
		return -1;
	}
	bool	cpps_io_chdir(std::string path) {
#ifdef WIN32
		return SetCurrentDirectoryA(path.c_str());
#else
		return chdir(path.c_str());
#endif
	}
	
	cpps_integer cpps_io_rmdir(std::string sourcepath) {
		sourcepath = cpps_io_string_replace(sourcepath, "\\", "/");

		std::vector<std::string> files;
		cpps_cpp_real_walk(files, sourcepath, true);
		auto it = files.rbegin();
		auto end = files.rend();
		cpps_integer ret = 0;
		for (; it != end; ++it) {
			auto file = *it;
			if (cpps_io_isfile(file))
				ret = cpps_io_remove(file);
			else if (cpps_io_isdir(file))
				ret = _rmdir(file.c_str());
			if (ret < 0) return ret;
		}
		return _rmdir(sourcepath.c_str());
	}
	cpps_integer cpps_io_copydir(std::string sourcepath, std::string targetpath) {
		sourcepath = cpps_io_string_replace(sourcepath, "\\", "/");
		targetpath = cpps_io_string_replace(targetpath, "\\", "/");

		std::vector<std::string> files;
		cpps_cpp_real_walk(files, sourcepath, true);
		cpps_integer ret = cpps_io_mkdirs(targetpath);
		for (auto file : files) {
			ret = 0;
			std::string tarfile = cpps_io_string_replace(file, sourcepath, targetpath);
			if (cpps_io_isfile(file))
				ret = cpps_io_copy(file, tarfile);
			else if (cpps_io_isdir(file))
				cpps_io_mkdirs(tarfile);
			else
				return -1;
			if (ret < 0) return -1;
		}
		return 0;
	}
	cpps_integer cpps_io_movedir(std::string sourcepath, std::string targetpath) {
		sourcepath = cpps_io_string_replace(sourcepath, "\\", "/");
		targetpath = cpps_io_string_replace(targetpath, "\\", "/");
		cpps_integer ret = 0;
		ret = cpps_io_copydir(sourcepath, targetpath);
		if (ret == 0) {
			ret = cpps_io_rmdir(sourcepath);
		}
		return ret;
	}
#ifndef _WIN32
#include <fcntl.h>
	int _kbhit(void)
	{
		struct termios oldt, newt;
		int ch;
		int oldf;
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fcntl(STDIN_FILENO, F_SETFL, oldf);
		if (ch != EOF)
		{
			ungetc(ch, stdin);
			return 1;
		}
		return 0;
	}
#endif
	cpps_integer cpps_io_kbhit()
	{
		return _kbhit();
	}
	void cpps_regio(C *c)
	{
		cpps::_module(c,"io")[
			def_inside("getc",cpps_io_getc),
			def_inside("getch",cpps_io_getch),
			def_inside("getline",cpps_io_getline),
			def("kbhit", cpps_io_kbhit),
			def("fopen",cpps_io_open),
			def("writefile",cpps_io_writefile),
			def("appendfile",cpps_io_appendfile),
			def("readfile",cpps_io_readfile),
			def("filesize",cpps_io_filesize),
			def("fsize", cpps_io_size),
			def("fread", cpps_io_read),
			def("getlines", cpps_io_getlines),
			def("fwrite", cpps_io_write),
			def("fwrites", cpps_io_writes),
			def("fseek", cpps_io_seek),
			def("fclose", cpps_io_close),
			def("fflush", cpps_io_fflush),
			def("remove", cpps_io_remove),
			def("copy", cpps_io_copy),
			def("move", cpps_io_move),
			def("copydir", cpps_io_copydir),
			def("movedir", cpps_io_movedir),
			def("rename", cpps_io_rename),
			def("getfileext", cpps_io_getfileext),
			def("getfilepath", cpps_io_getfilepath),
			def("getfilename", cpps_io_getfilename),
			def("getfilenamenotext", getfilenamenotext),
			def("getcwd", cpps_getcwd),
			def("mkdir",cpps_io_mkdir),
			def("rmdir",cpps_io_rmdir),
			def("mkdirs",cpps_io_mkdirs),
			def("chdir",cpps_io_chdir),
			def("isdir",cpps_io_isdir),
			def("isfile",cpps_io_isfile),
			def("normpath",cpps_io_normpath),
			def("isabspath", cpps_io_isabspath),
			def_inside("splitdrive", cpps_io_splitdrive),
			def("getrealpath", cpps_real_path),
			def("file_exists",cpps_io_file_exists),
			def("access",cpps_io_file_exists),
			def_inside("walk",cpps_io_walk),
			def_inside("listdir",cpps_io_listdir),
			def_inside("stat",cpps_io_get_stat),
			def("last_write_time",cpps_io_last_write_time),
			def("getmtime",cpps_io_last_write_time),
			def("getctime",cpps_io_getctime),
			def("getatime",cpps_io_getatime),
			def("match", cpps_io_filename_filter),
#ifdef _WIN32
			def("getappdata",cpps_io_getappdata),
#endif
			defvar(c,"SEEK_END", SEEK_END),
			defvar(c,"SEEK_CUR", SEEK_CUR),
			defvar(c,"SEEK_SET", SEEK_SET),
#ifdef _WIN32
			defvar(c, "sep", "\\"),
			defvar(c, "linesep", "\r\n")
#else
			defvar(c, "sep", "/"),
			defvar(c, "linesep", "\n")
#endif
		];

		cpps::_module(c)[
			_class<Buffer>("Buffer")
				.def("read", &Buffer::read)
				.def("write", &Buffer::write)
				.def("tostring", &Buffer::tostring)
				.def("tointeger", &Buffer::tointeger)
				.def("tonumber", &Buffer::tonumber)
				.def("readInt8", &Buffer::readint8)
				.def("readInt16", &Buffer::readint16)
				.def("readInt32", &Buffer::readint32)
				.def("readInt", &Buffer::readint)
				.def("readUInt8", &Buffer::readuint8)
				.def("readUInt16", &Buffer::readuint16)
				.def("readUInt32", &Buffer::readuint32)
				.def("readUInt", &Buffer::readuint)
				.def("readNumber", &Buffer::readnumber)
				.def("readFloat", &Buffer::readfloat)
				.def("readString", &Buffer::readstring)
				.def("readBool", &Buffer::readbool)
				.def("writeInt8", &Buffer::writeint8)
				.def("writeInt16", &Buffer::writeint16)
				.def("writeInt32", &Buffer::writeint32)
				.def("writeInt", &Buffer::writeint)
				.def("writeUInt8", &Buffer::writeuint8)
				.def("writeUInt16", &Buffer::writeuint16)
				.def("writeUInt32", &Buffer::writeuint32)
				.def("writeUInt", &Buffer::writeuint)
				.def("writeNumber", &Buffer::writenumber)
				.def("writeFloat", &Buffer::writefloat)
				.def("writeString", &Buffer::writestring)
				.def("writeBool", &Buffer::writebool)
				.def("seek", &Buffer::seek)
				.def("clear", &Buffer::clear)
				.def("endian", &Buffer::endian)
				.def("writefile", &Buffer::writefile)
				.def("length", &Buffer::length),
			_enum(c,"BUFFERENDIAN")
				.value("LITTLE_ENDIAN",0)
				.value("BIG_ENDIAN",1),
			_class<FILE>("FILE"),
			_class< cpps_io_stat>("statinfo")
				.def("dev",&cpps_io_stat::dev)
				.def("ino",&cpps_io_stat::ino)
				.def("mode",&cpps_io_stat::mode)
				.def("nlink",&cpps_io_stat::nlink)
				.def("uid",&cpps_io_stat::uid)
				.def("gid",&cpps_io_stat::gid)
				.def("rdev",&cpps_io_stat::rdev)
				.def("size",&cpps_io_stat::size)
				.def("atime",&cpps_io_stat::atime)
				.def("mtime",&cpps_io_stat::mtime)
				.def("ctime",&cpps_io_stat::ctime)
				.def("isdir",&cpps_io_stat::isdir)
				.def("isreg",&cpps_io_stat::isreg)
				.def("isblk",&cpps_io_stat::isblk)
				.def("ischr",&cpps_io_stat::ischr)
				.def("isfifo",&cpps_io_stat::isfifo)
				.def("islink",&cpps_io_stat::islink)
				.def("issock",&cpps_io_stat::issock)
		];
	}

	Buffer::Buffer()
	{
		buff = NULL;
		offset = 0;
		buffsize = 0;
		_endian = 0;
		_length = 0;
	}

	Buffer::~Buffer()
	{
		clear();
	}

	void Buffer::read(Buffer* out, cpps_integer len)
	{
		char _tmpendian = _endian;
		_endian = 0;
		out->_write(getbuffer(), length());
		_endian = _tmpendian;
	}

	cpps::Buffer* Buffer::write(Buffer* buf, cpps_integer len)
	{
		char _tmpendian = _endian;
		_endian = 0;
		_write(buf->getbuffer(), len);
		_endian = _tmpendian;
		return this;
	}

	char* Buffer::_read(char* out, cpps_integer len)
	{
		if (length() + len > _length) return NULL;

		char* ret = getbuffer() + length();
		if (out)
		{
			if (_endian == 0)
				memcpy(out, ret, (size_t)len);
			else
				_rmemcpy(out, ret, (size_t)len);

		}
		seek(length() + len);
		return ret;
	}

	void Buffer::_rmemcpy(char*dest,const char* buf, size_t len) {
		for (size_t i = len,i2 = 0; i > 0; i--,i2++)
			dest[i2] = buf[i-1];
	}
	void Buffer::_write(const char* buf, cpps_integer len)
	{
		realloc(length() + len);
		if (_endian == 0)
			memcpy(getbuffer() + length(), buf, (size_t)len);
		else
			_rmemcpy(getbuffer() + length(), buf, (size_t)len);

		seek(length() + len);
	}

	std::string Buffer::tostring()
	{
		std::string ret;
		ret.append(getbuffer(),(size_t)_length);
		return ret;
	}

	cpps_integer Buffer::tointeger()
	{
		std::string str = tostring();
		cpps_integer ret;
		cpps_str2i64(str.c_str(), &ret);
		return ret;
	}

	cpps_number Buffer::tonumber()
	{
		std::string str = tostring();
		cpps_number ret;
		cpps_str2d(str.c_str(), &ret);
		return ret;
	}

	cpps_integer Buffer::readint8()
	{
		signed char ret = 0;
		_read((char*)&ret, sizeof(signed char));
		return ret;
	}

	cpps_integer Buffer::readint16()
	{
		short ret = 0;
		_read((char*)&ret, sizeof(short));
		return ret;
	}

	cpps_integer Buffer::readint32()
	{
		int32 ret = 0;
		_read((char*)&ret, sizeof(int32));
		return ret;
	}

	cpps_integer Buffer::readint()
	{
		cpps_integer ret = 0;
		_read((char*)&ret, sizeof(cpps_integer));
		return ret;
	}

	cpps_uinteger Buffer::readuint8()
	{
		unsigned char ret = 0;
		_read((char*)&ret, sizeof(unsigned char));
		return ret;
	}

	cpps_uinteger Buffer::readuint16()
	{
		usint16 ret = 0;
		_read((char*)&ret, sizeof(usint16));
		return ret;
	}

	cpps_uinteger Buffer::readuint32()
	{
		usint32 ret = 0;
		_read((char*)&ret, sizeof(usint32));
		return ret;
	}

	cpps_uinteger Buffer::readuint()
	{
		cpps_uinteger ret = 0;
		_read((char*)&ret, sizeof(cpps_uinteger));
		return ret;
	}

	cpps_number Buffer::readnumber()
	{
		cpps_number ret = 0;
		_read((char*)&ret, sizeof(cpps_number));
		return ret;
	}

	cpps_number Buffer::readfloat()
	{
		float ret = 0;
		_read((char*)&ret, sizeof(float));
		return (float)ret;
	}

	std::string Buffer::readstring(cpps_integer len)
	{
		std::string ret;
		ret.resize(size_t(len));
		char _tmpendian = _endian;
		_endian = 0;
		_read((char*)ret.c_str(), len);
		_endian = _tmpendian;
		return ret;
	}

	bool Buffer::readbool()
	{
		bool ret = false;
		_read((char*)&ret, sizeof(bool));
		return ret;
	}

	cpps::Buffer* Buffer::writeint8(int8 i)
	{
		_write((char*)&i, sizeof(int8));
		return this;
	}

	cpps::Buffer* Buffer::writeint16(int16 i)
	{
		_write((char*)&i, sizeof(short));
		return this;
	}

	cpps::Buffer* Buffer::writeint32(int32 i)
	{
		_write((char*)&i, sizeof(int32));
		return this;
	}

	cpps::Buffer* Buffer::writeint(cpps_integer i)
	{
		_write((char*)&i, sizeof(cpps_integer));
		return this;
	}

	cpps::Buffer* Buffer::writeuint8(usint8 i)
	{
		_write((char*)&i, sizeof(usint8));
		return this;
	}

	cpps::Buffer* Buffer::writeuint16(usint16 i)
	{
		_write((char*)&i, sizeof(usint16));
		return this;
	}

	cpps::Buffer* Buffer::writeuint32(usint32 i)
	{
		_write((char*)&i, sizeof(usint32));
		return this;
	}

	cpps::Buffer* Buffer::writeuint(cpps_uinteger i)
	{
		_write((char*)&i, sizeof(cpps_uinteger));
		return this;
	}

	cpps::Buffer* Buffer::writenumber(cpps_number i)
	{
		_write((char*)&i, sizeof(cpps_number));
		return this;
	}

	cpps::Buffer* Buffer::writefloat(cpps_number i)
	{
		_write((char*)&i, sizeof(float));
		return this;
	}

	cpps::Buffer* Buffer::writestring(std::string s)
	{
		char _tmpendian = _endian;
		_endian = 0;
		_write(s.c_str(), s.size());
		_endian = _tmpendian;
		return this;
	}

	cpps::Buffer* Buffer::writebool(bool b)
	{
		_write((char*)&b, sizeof(bool));
		return this;
	}

	void Buffer::seek(cpps_integer s)
	{
		if (s == -1)
			offset = buffsize;
		else
			offset = s;
	}

	cpps_integer Buffer::length()
	{
		return offset;
	}

	char* Buffer::getbuffer()
	{
		return buff;
	}

	void Buffer::clear()
	{
		if (buff) CPPSFREE( buff);
		buff = NULL;
		buffsize = 0;
		offset = 0;
	}
	void Buffer::endian(cpps::object v)
	{
		cpps_integer _e = v.toint();
		_endian = (char)_e;
	}

	void Buffer::realloc(cpps_integer s)
	{
		_length = s;
		if (buffsize >= s)
		{
			return;
		}

		size_t newsize = buffsize == 0 ? static_cast<size_t>(s) : static_cast<size_t>(fmax(buffsize,s)*2);
		char* newbuff = (char*) CPPSMALLOC(newsize + 1);
		memset(newbuff, 0, (size_t)newsize + 1);

		if (buff)
		{
			memcpy(newbuff, buff, (size_t)buffsize);
			CPPSFREE( buff);
		}

		buff = newbuff;
		buffsize = newsize;
	}

	void Buffer::writefile( std::string path)
	{
		FILE* _f = fopen(path.c_str(), "wb+");
		if (_f) {

			fwrite(getbuffer(), length(), 1, _f);
			fclose(_f);
		}
	}
}
