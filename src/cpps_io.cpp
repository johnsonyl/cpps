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
			cpps_integer size = cpps_io_size(file);
			char* buf = new char[size];
			memset(buf, 0, size);
			if (fread(buf, size, 1, file)) {};
			fclose(file);
			ret.append(buf, size);
			delete[] buf;
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
		char s[100];
		while (true)
		{
			memset(s, 0, 100);
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
		size_t pos = str.find_last_of(".");
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
		size_t pos = str.find_last_of(".");
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
		std::string strdir = szdir;
		cpps_integer iret = 0;
		cpps_integer index = strdir.find_last_of('/');
		if (0 < index) //存在多级目录
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

	using namespace std::literals::chrono_literals;
	cpps_integer cpps_io_last_write_time( std::string path) {
		auto ftime = std::filesystem::last_write_time(path);
		auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
		auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count() - elapse;
		return (cpps_integer)systemTime;
	}
	cpps_value cpps_io_get_stat(C* c, std::string path) {

		cpps_io_stat* st = NULL;
		cpps_value ret = newclass<cpps_io_stat>(c, &st);

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
		cpps_value ret = newclass<cpps_vector>(c, &vct);
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

		const char* sep = "\\";
		const char* altsep = "/";
		const char* curdir = ".";
		const char* pardir = "..";
		const char* special_prefixes[2] = { "\\\\.\\" ,"\\\\?\\" };

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
				else if (i == realvct.begin() && prefix[prefix.size() - 1] == '\\')
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
	void cpps_real_walk(C*c,cpps_vector* vct,std::string path, bool bfindchildren = false,bool onlydir = false) {
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
			strcat(dirNew, "/");
			strcat(dirNew, findData.name);

			if (findData.attrib & _A_SUBDIR)
			{
				if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
					continue;

				vct->push_back(cpps_value(c, dirNew));

				if(bfindchildren)
					cpps_real_walk(c, vct,dirNew, bfindchildren);
			}
			else if(!onlydir)
				vct->push_back(cpps_value(c, dirNew));

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

			

			struct stat s;
			lstat(dirNew, &s);
			if (S_ISDIR(s.st_mode))
			{
				vct->push_back(cpps_value(c, dirNew));
				if (bfindchildren)
					cpps_real_walk(c, vct, dirNew, bfindchildren);
			}
			else if (!onlydir)
			{
				vct->push_back(cpps_value(c, dirNew));
			}
		}
#endif
	}
	cpps_value cpps_io_walk( C *c,std::string path,cpps_value findchildren) {
		
		cpps_vector* vct = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vct);
		
		bool bfindchildren = findchildren.tt == CPPS_TBOOLEAN ? findchildren.value.b : true;
		cpps_real_walk(c, vct, path, bfindchildren);
		return ret;
	}
	cpps_value cpps_io_listdir(C* c, std::string path, cpps_value findchildren) {

		cpps_vector* vct = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vct);

		bool bfindchildren = findchildren.tt == CPPS_TBOOLEAN ? findchildren.value.b : true;
		cpps_real_walk(c, vct, path, bfindchildren,true);
		return ret;
	}
	std::string cpps_real_path()
	{
		char abs_path[1024];
		memset(abs_path, 0, 1024);
#if defined _WIN32
		GetModuleFileNameA(NULL, abs_path, 1024);
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
		size_t cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
		if (cnt < 0 || cnt >= 1024)
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
		usint32 size = 1024;
		_NSGetExecutablePath(abs_path, &size);

		//mac os 下返回的不是真实地址而是软连接地址.
		//需要读取软连接的地址真实地址
		struct stat statinfo;
		lstat(abs_path, &statinfo);
		if (S_ISLNK(statinfo.st_mode))
		{
			size = readlink(abs_path, abs_path, 1024);
			if (size < 0 || size >= 1024)
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
			std::string tarfile = cpps_io_string_replace(file, sourcepath, targetpath);
			if (cpps_io_isfile(file))
				ret = cpps_io_copy(file, tarfile);
			else if (cpps_io_isdir(file))
				cpps_io_mkdir(tarfile);
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
	void cpps_regio(C *c)
	{
		cpps::_module(c,"io")[
			def_inside("getc",cpps_io_getc),
			def_inside("getline",cpps_io_getline),
			def("fopen",cpps_io_open),
			def("writefile",cpps_io_writefile),
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
			def_inside("walk",cpps_io_walk),
			def_inside("listdir",cpps_io_listdir),
			def_inside("stat",cpps_io_get_stat),
			def("last_write_time",cpps_io_last_write_time),
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
				.def("readNumber", &Buffer::readnumber)
				.def("readString", &Buffer::readstring)
				.def("readBool", &Buffer::readbool)
				.def("writeInt8", &Buffer::writeint8)
				.def("writeInt16", &Buffer::writeint16)
				.def("writeInt32", &Buffer::writeint32)
				.def("writeInt", &Buffer::writeint)
				.def("writeNumber", &Buffer::writenumber)
				.def("writeString", &Buffer::writestring)
				.def("writeBool", &Buffer::writebool)
				.def("seek", &Buffer::seek)
				.def("clear", &Buffer::clear)
				.def("length", &Buffer::length),
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
	}

	Buffer::~Buffer()
	{
		clear();
	}

	void Buffer::read(Buffer* out, cpps_integer len)
	{
		out->_write(getbuffer(), length());
	}

	cpps::Buffer* Buffer::write(Buffer* buf, cpps_integer len)
	{
		_write(buf->getbuffer(), len);
		return this;
	}

	char* Buffer::_read(char* out, cpps_integer len)
	{
		if (length() + len > buffsize) return NULL;

		char* ret = getbuffer() + length();
		if (out)
		{
			memcpy(out, ret, (size_t)len);
		}
		seek(length() + len);
		return ret;
	}

	void Buffer::_write(const char* buf, cpps_integer len)
	{
		realloc(length() + len);
		memcpy(getbuffer() + length(), buf, (size_t)len);
		seek(length() + len);
	}

	std::string Buffer::tostring()
	{
		std::string ret;
		ret.append(getbuffer(), buffsize);
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

	cpps_number Buffer::readnumber()
	{
		cpps_number ret = 0;
		_read((char*)&ret, sizeof(cpps_number));
		return ret;
	}

	std::string Buffer::readstring(cpps_integer len)
	{
		std::string ret;
		ret.resize(len);
		_read((char*)ret.c_str(), len);
		return ret;
	}

	bool Buffer::readbool()
	{
		bool ret = false;
		_read((char*)&ret, sizeof(bool));
		return ret;
	}

	cpps::Buffer* Buffer::writeint8(signed char i)
	{
		_write((char*)&i, sizeof(signed char));
		return this;
	}

	cpps::Buffer* Buffer::writeint16(short i)
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

	cpps::Buffer* Buffer::writenumber(cpps_number i)
	{
		_write((char*)&i, sizeof(cpps_number));
		return this;
	}

	cpps::Buffer* Buffer::writestring(std::string s)
	{
		_write(s.c_str(), s.size());
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
		if (buff) delete[] buff;
		buff = NULL;
		buffsize = 0;
		offset = 0;
	}

	void Buffer::realloc(cpps_integer s)
	{
		if (buffsize >= s)
		{
			buffsize = s;
			return;
		}
		size_t newsize = static_cast<size_t>(s);
		char* newbuff = new char[newsize + 1];
		memset(newbuff, 0, (size_t)newsize + 1);

		if (buff)
		{
			memcpy(newbuff, buff, (size_t)buffsize);
			delete[] buff;
		}

		buff = newbuff;
		buffsize = s;
	}

}
