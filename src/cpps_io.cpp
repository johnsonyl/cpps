#include "cpps.h"
namespace cpps
{
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
	cpps_value cpps_io_getc(C *c)
	{
		std::string ret;

		std::cin >> ret;

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
	void		cpps_io_read(FILE *file, Buffer *buf, cpps_integer len)
	{
		buf->realloc(buf->length() + len);
		fread(buf->getbuffer() + buf->length(), (size_t)len, 1, file);
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
	cpps_integer cpps_io_rmdir(std::string p)
	{
		return _rmdir(p.c_str());
	}
	cpps_integer cpps_io_mkdirs(std::string szdir)
	{
		std::string strdir = szdir;
		cpps_integer iret = 0;
		cpps_integer index = strdir.find_last_of('/');
		if (0 < index) //存在多级目录
		{
			strdir.erase(index, strdir.length() - index);

#ifdef WIN32
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
#ifdef WIN32
			iret = _mkdir(szdir.c_str());
#else
			iret = mkdir(szdir.c_str(), 0755);
#endif
		}

		return iret;
	}
	bool cpps_io_file_exists(std::string path)
	{
#ifdef WIN32
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
#ifdef WIN32
		_getcwd(buffer, 4096);
#else
		getcwd(buffer, 4096);
#endif
		return buffer;
	}
	cpps_value cpps_io_get_stat(C* c, std::string path) {

		cpps_io_stat* st = NULL;
		cpps_value ret = newclass<cpps_io_stat>(c, &st);

#ifdef WIN32
		_stati64(path.c_str(), &st->statinfo);
#else
		stat(path.c_str(), &st->statinfo);
#endif
		return ret;
	}
	void cpps_real_walk(C*c,cpps_vector* vct,std::string path, bool bfindchildren) {
		char dirNew[200];


		path = cpps_io_string_replace(path, "\\", "/");

		strcpy(dirNew, path.c_str());
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
			else
				vct->push_back(cpps_value(c, dirNew));

		} while (_findnext(handle, &findData) == 0);

		_findclose(handle);    // 关闭搜索句柄
	}
	cpps_value cpps_io_walk( C *c,std::string path,cpps_value findchildren) {
		
		cpps_vector* vct = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vct);
		
		bool bfindchildren = findchildren.tt == CPPS_TBOOLEAN ? findchildren.value.b : true;
		cpps_real_walk(c, vct, path, bfindchildren);
		return ret;
	}
	std::string cpps_real_path()
	{
		char abs_path[1024];
		memset(abs_path, 0, 1024);
#ifdef WIN32
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
		
#else
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
#endif
		
		return abs_path;
	}
	void cpps_regio(C *c)
	{
		module(c,"io")[
			def_inside("getc",cpps_io_getc),
			def("fopen",cpps_io_open),
			def("fsize", cpps_io_size),
			def("fread", cpps_io_read),
			def("getlines", cpps_io_getlines),
			def("fwrite", cpps_io_write),
			def("fwrites", cpps_io_writes),
			def("fseek", cpps_io_seek),
			def("fclose", cpps_io_close),
			def("fflush", cpps_io_fflush),
			def("remove", cpps_io_remove),
			def("rename", cpps_io_rename),
			def("getfileext", cpps_io_getfileext),
			def("getfilepath", cpps_io_getfilepath),
			def("getfilename", cpps_io_getfilename),
			def("getfilenamenotext", getfilenamenotext),
			def("getcwd", cpps_getcwd),
			def("mkdir",cpps_io_mkdir),
			def("rmdir",cpps_io_rmdir),
			def("mkdirs",cpps_io_mkdirs),
			def("getrealpath", cpps_real_path),
			def("file_exists",cpps_io_file_exists),
			def_inside("walk",cpps_io_walk),
			def_inside("stat",cpps_io_get_stat)
		];

		module(c)[
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
}
