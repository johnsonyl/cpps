#ifndef CPPS_IO_CPPS_HEAD_
#define CPPS_IO_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_IO
//@Description	:	IO��ע��
//@website		:	http://cppscript.org
//==================================


namespace cpps
{

	struct cpps_io_stat 
	{
		virtual ~cpps_io_stat(){}
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
		Buffer();
		virtual ~Buffer();
		void			read(Buffer *out, cpps_integer len);
		Buffer			*write(Buffer *buf, cpps_integer len);
		char *			_read(char *out, cpps_integer len);
		void			_rmemcpy(char* dest, const char* buf, size_t len);
		void			_write(const char *buf, cpps_integer len);
		std::string		tostring();
		cpps_integer	tointeger();
		cpps_number		tonumber();
		cpps_integer	readint8();
		cpps_integer	readint16();
		cpps_integer	readint32();
		cpps_integer	readint();
		cpps_uinteger	readuint8();
		cpps_uinteger	readuint16();
		cpps_uinteger	readuint32();
		cpps_uinteger	readuint();
		cpps_number		readnumber();
		cpps_number		readfloat();
		std::string		readstring(cpps_integer len);
		bool			readbool();
		Buffer*			writeint8(int8  i);
		Buffer*			writeint16(int16  i);
		Buffer*			writeint32(int32 i);
		Buffer*			writeint(cpps_integer i);
		Buffer*			writeuint8(usint8 i);
		Buffer*			writeuint16(usint16 i);
		Buffer*			writeuint32(usint32 i);
		Buffer*			writeuint(cpps_uinteger i);
		Buffer*			writenumber(cpps_number i);
		Buffer*			writefloat(cpps_number i);
		Buffer*			writestring(std::string s);
		Buffer*			writebool(bool b);
		void			seek(cpps_integer s);
		cpps_integer	length();
		char *			getbuffer();
		void			clear();
		void			endian(cpps::object v);
		void			realloc(cpps_integer s);
		void			writefile(std::string path);
	public:
		cpps_integer	offset;
		char*			buff;
		cpps_integer	buffsize; //���size
		cpps_integer	_length; //ʵ��length
		char			_endian;
	};

	void	cpps_regio(C *c);
}

#endif