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

		return "";
	}
	std::string getfilenamenotext(std::string str)
	{
		str = cpps_io_getfilename(str);
		size_t pos = str.find_last_of(".");
		if (pos != std::string::npos)
		{
			return str.substr(0,pos-1);
		}
		return "";
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
	std::string cpps_real_path()
	{
		char abs_path[1024];
		memset(abs_path, 0, 1024);
#ifdef WIN32
		GetModuleFileNameA(NULL, abs_path, 1024);
		size_t cnt = strlen(abs_path);
		for (size_t i = cnt; i >= 0; --i)
		{
			if (abs_path[i] == '\\')
			{
				abs_path[i + 1] = '\0';
				break;
	}
		}
#else
		size_t cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
		if (cnt < 0 || cnt >= 1024)
		{
			return NULL;
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
			def("getrealpath", cpps_real_path),
			def("file_exists",cpps_io_file_exists)
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
			_class<FILE>("FILE")
		];
	}
}