#ifndef CPPS_IO_CPPS_HEAD_
#define CPPS_IO_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_IO
//@Description	:	IO¿â×¢²á
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{

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
			_write(_read(NULL, len), len);
		}
		Buffer			*write(Buffer *buf, cpps_integer len)
		{
			_write(_read(NULL, len), len);
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
			ret.append(_read(NULL, len), (usint32)len);
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