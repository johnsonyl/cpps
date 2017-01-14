#include "cpps.h"
namespace cpps
{
	std::string	cpps_string_replace(std::string v, std::string v2, std::string v3);
	cpps_value cpps_io_getc()
	{
		std::string ret;

		std::cin >> ret;

		return ret;
	}

	struct Buffer
	{
		Buffer()
		{
			buff = NULL;
			offset = 0;
			buffsize = 0;
		}
		~Buffer()
		{
			if (buff) delete[] buff; 
			buff = NULL;
			buffsize = 0;
			offset = 0;
		}
		void			read(Buffer *out, cpps_integer len)
		{
			_write(_read(NULL, len),len);
		}
		Buffer			*write(Buffer *buf, cpps_integer len)
		{
			_write(_read(NULL,len),len);
			return this;
		}
		char *			_read(char *out, cpps_integer len)
		{
			char *ret = getbuffer() + length();
			if (out)
			{
				memcpy(ret, out, (size_t)len);
			}
			seek(length() + len);
			return ret;
		}
		void			_write(const char *buf, cpps_integer len)
		{
			realloc(length() + len);
			memcpy(getbuffer() + length(), buf, (size_t)len);
			seek(length() + len);
		}
		std::string		tostring()
		{
			std::string ret;
			ret = getbuffer();
			return ret;
		}
		cpps_integer	tointeger()
		{
			std::string str = tostring();
			cpps_integer ret;
			cpps_str2i64(str.c_str(), &ret);
			return ret;
		}
		cpps_number		tonumber()
		{
			std::string str = tostring();
			cpps_number ret;
			cpps_str2d(str.c_str(), &ret);
			return ret;
		}
		cpps_integer	readInt8()
		{
			signed char ret = 0;
			_read((char *)&ret, sizeof(signed char));
			return ret;
		}
		cpps_integer	readInt16()
		{
			short ret = 0;
			_read((char *)&ret, sizeof(short));
			return ret;
		}
		cpps_integer	readInt32()
		{
			int32 ret = 0;
			_read((char *)&ret, sizeof(int32));
			return ret;
		}
		cpps_integer	readInt()
		{
			cpps_integer ret = 0;
			_read((char *)&ret, sizeof(cpps_integer));
			return ret;
		}
		cpps_number		readNumber()
		{
			cpps_number ret = 0;
			_read((char *)&ret, sizeof(cpps_number));
			return ret;
		}
		std::string		readString(cpps_integer len)
		{
			std::string ret;
			ret.append(_read(NULL, len),(usint32) len);
			return ret;
		}
		bool			readBool()
		{
			bool ret = false;
			_read((char *)&ret, sizeof(bool));
			return ret;
		}
		Buffer*			writeInt8(signed char i)
		{
			_write((char *)&i, sizeof(signed char));
			return this;
		}
		Buffer*			writeInt16(short i)
		{
			_write((char *)&i, sizeof(short));
			return this;
		}
		Buffer*			writeInt32(int32 i)
		{
			_write((char *)&i, sizeof(int32));
			return this;
		}
		Buffer*			writeInt(cpps_integer i)
		{
			_write((char *)&i, sizeof(cpps_integer));
			return this;
		}
		Buffer*			writeNumber(cpps_number i)
		{
			_write((char *)&i, sizeof(cpps_number));
			return this;
		}
		Buffer*			writeString(std::string s)
		{
			_write(s.c_str(), s.size());
			return this;
		}
		Buffer*			writeBool(bool b)
		{
			_write((char *)&b, sizeof(bool));
			return this;
		}
		void			seek(cpps_integer s)
		{
			if (s == -1)
				offset = buffsize;
			else
				offset = s;
		}
		cpps_integer	length()
		{
			return offset;
		}
		char *			getbuffer()
		{
			return buff;
		}
		void			realloc(cpps_integer s)
		{
			if (buffsize >= s)
			{
				buffsize = s;
				return;
			}
			usint32 newsize = static_cast<usint32>(s);
			char *newbuff = new char[newsize+1];
			memset(newbuff, 0, (size_t)s+1);

			if (buff)
			{
				memcpy(newbuff, buff, (size_t)buffsize);
				delete[] buff;
			}

			buff = newbuff;
			buffsize = s;
		}
	public:
		cpps_integer	offset;
		char*			buff;
		cpps_integer	buffsize; //×î´ósize
	};
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
	void		cpps_io_write(FILE *file, Buffer *buf)
	{
		fwrite(buf->getbuffer(), (size_t)buf->length(), 1, file);
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
		str = cpps_string_replace(str, "\\", "/");

		size_t pos = str.find_last_of("/");
		if (pos != std::string::npos)
		{
			return str.substr(0,pos);
		}

		return "";
	}
	std::string cpps_io_getfilename(std::string str)
	{
		str = cpps_string_replace(str, "\\", "/");

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
	void cpps_regio(C *c)
	{
		module(c,"io")[
			def("getc",cpps_io_getc),
			def("fopen",cpps_io_open),
			def("fsize", cpps_io_size),
			def("fread", cpps_io_read),
			def("fwrite", cpps_io_write),
			def("fseek", cpps_io_seek),
			def("fclose", cpps_io_close),
			def("fflush", cpps_io_fflush),
			def("remove", cpps_io_remove),
			def("rename", cpps_io_rename),
			def("getfileext", cpps_io_getfileext),
			def("getfilepath", cpps_io_getfilepath),
			def("getfilename", cpps_io_getfilename),
			def("getfilenamenotext", getfilenamenotext)
		];

		module(c)[
			_class<Buffer>("Buffer")
				.def("read", &Buffer::read)
				.def("write", &Buffer::write)
				.def("tostring", &Buffer::tostring)
				.def("tointeger", &Buffer::tointeger)
				.def("tonumber", &Buffer::tonumber)
				.def("readInt8", &Buffer::readInt8)
				.def("readInt16", &Buffer::readInt16)
				.def("readInt32", &Buffer::readInt32)
				.def("readInt", &Buffer::readInt)
				.def("readNumber", &Buffer::readNumber)
				.def("readString", &Buffer::readString)
				.def("readBool", &Buffer::readBool)
				.def("writeInt8", &Buffer::writeInt8)
				.def("writeInt16", &Buffer::writeInt16)
				.def("writeInt32", &Buffer::writeInt32)
				.def("writeInt", &Buffer::writeInt)
				.def("writeNumber", &Buffer::writeNumber)
				.def("writeString", &Buffer::writeString)
				.def("writeBool", &Buffer::writeBool)
				.def("seek", &Buffer::seek)
				.def("length", &Buffer::length),
			_class<FILE>("FILE")
		];
	}
}