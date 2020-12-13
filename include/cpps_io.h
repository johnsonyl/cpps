#ifndef CPPS_IO_CPPS_HEAD_
#define CPPS_IO_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_IO
//@Description	:	IO¿â×¢²á
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{

	struct cpps_io_stat 
	{
		cpps_integer dev() { return statinfo.st_dev; }
		cpps_integer ino() { return statinfo.st_ino; }
		cpps_integer mode() { return statinfo.st_mode; }
		cpps_integer nlink() { return statinfo.st_nlink; }
		cpps_integer uid() { return statinfo.st_uid; }
		cpps_integer gid() { return statinfo.st_gid; }
		cpps_integer rdev() { return statinfo.st_rdev; }
		cpps_integer size() { return statinfo.st_size; }
		cpps_integer atime() { return statinfo.st_atime; }
		cpps_integer mtime() { return statinfo.st_mtime; }
		cpps_integer ctime() { return statinfo.st_ctime; }
		bool		 isdir() { return S_ISDIR(statinfo.st_mode); }
		bool		 isreg() { return S_ISREG(statinfo.st_mode); }
		bool		 isblk() { return S_ISBLK(statinfo.st_mode); }
		bool		 ischr() { return S_ISCHR(statinfo.st_mode); }
		bool		 isfifo() { return S_ISFIFO(statinfo.st_mode); }
		bool		 islink() { return S_ISLNK(statinfo.st_mode); }
		bool		 issock() { return S_ISSOCK(statinfo.st_mode); }
#ifdef _WIN32
		struct _stat64 statinfo;
#else 
		struct stat statinfo;
#endif
	};

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
			clear();
		}
		void			read(Buffer *out, cpps_integer len)
		{
			out->_write(getbuffer(), length());
		}
		Buffer			*write(Buffer *buf, cpps_integer len)
		{
			_write(buf->getbuffer(), len);
			return this;
		}
		char *			_read(char *out, cpps_integer len)
		{
			if (length() + len > buffsize) return NULL;

			char *ret = getbuffer() + length();
			if (out)
			{
				memcpy(out, ret, (size_t)len);
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
			ret.append(getbuffer(), buffsize);
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
		cpps_integer	readint8()
		{
			signed char ret = 0;
			_read((char *)&ret, sizeof(signed char));
			return ret;
		}
		cpps_integer	readint16()
		{
			short ret = 0;
			_read((char *)&ret, sizeof(short));
			return ret;
		}
		cpps_integer	readint32()
		{
			int32 ret = 0;
			_read((char *)&ret, sizeof(int32));
			return ret;
		}
		cpps_integer	readint()
		{
			cpps_integer ret = 0;
			_read((char *)&ret, sizeof(cpps_integer));
			return ret;
		}
		cpps_number		readnumber()
		{
			cpps_number ret = 0;
			_read((char *)&ret, sizeof(cpps_number));
			return ret;
		}
		std::string		readstring(cpps_integer len)
		{
			std::string ret;
			ret.resize(len);
			_read((char*)ret.c_str(), len);
			return ret;
		}
		bool			readbool()
		{
			bool ret = false;
			_read((char *)&ret, sizeof(bool));
			return ret;
		}
		Buffer*			writeint8(signed char i)
		{
			_write((char *)&i, sizeof(signed char));
			return this;
		}
		Buffer*			writeint16(short i)
		{
			_write((char *)&i, sizeof(short));
			return this;
		}
		Buffer*			writeint32(int32 i)
		{
			_write((char *)&i, sizeof(int32));
			return this;
		}
		Buffer*			writeint(cpps_integer i)
		{
			_write((char *)&i, sizeof(cpps_integer));
			return this;
		}
		Buffer*			writenumber(cpps_number i)
		{
			_write((char *)&i, sizeof(cpps_number));
			return this;
		}
		Buffer*			writestring(std::string s)
		{
			_write(s.c_str(), s.size());
			return this;
		}
		Buffer*			writebool(bool b)
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
		void			clear()
		{
			if (buff) delete[] buff;
			buff = NULL;
			buffsize = 0;
			offset = 0;
		}
		void			realloc(cpps_integer s)
		{
			if (buffsize >= s)
			{
				buffsize = s;
				return;
			}
			size_t newsize = static_cast<size_t>(s);
			char *newbuff = new char[newsize+1];
			memset(newbuff, 0, (size_t)newsize+1);

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

	void	cpps_regio(C *c);
}

#endif