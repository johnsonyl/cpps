#ifndef CPPSBUFFER_CPPS_HEAD_
#define CPPSBUFFER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	CPPSBUFFER
//@Description	:	字节流IO
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	class cppsbuffer
	{
	public:
		cppsbuffer(const char *_filename,const char* _buffer, int32 _buffersize)
		{
			filename = _filename;
			buffer = _buffer;
			buffersize = _buffersize;
			bufferoffset = 0;
			l = 1;
		}
		char		pop()
		{
			//检测是不是注释
			if (realcur() == '/')
			{
				if (at(offset() + 1) == '/')
				{
					realpop(); //pop first /
					realpop(); //pop second /

					while (!isend() && cur() != '\n')
						realpop();


				}
				else if (at(offset() + 1) == '*')
				{
					realpop();
					realpop();
					int  a = 10;
					while (!isend())
					{
						if (realcur() == '*')
						{
							if (at(offset() + 1) == '/')
							{
								realpop();
								realpop();
								break;
							}
						}
						realpop();
					}

				}
			}

			int32 ret = bufferoffset;
			
			

			return realpop();
		}

		char realpop()
		{
			if (isend())
			{
				throw(cpps_error(filename.c_str(), l, 0, "Unknown end of file."));
			}
			int32 ret = bufferoffset++;

			if (buffer[ret] == '\n')
				l++;
			return buffer[ret];
		}

		char		cur()
		{
			//检测是不是注释
			if (realcur() == '/')
			{
				if (at(offset() + 1) == '/')
				{
					realpop(); //pop first /
					realpop(); //pop second /

					while (!isend() && cur() != '\n')
						realpop();


				}
				else if (at(offset() + 1) == '*')
				{
					realpop();
					realpop();
					int  a = 10;
					while (!isend())
					{
						if (realcur() == '*')
						{
							if (at(offset() + 1) == '/')
							{
								realpop();
								realpop();
								break;
							}
						}
						realpop();
					}

				}
			}

			return realcur();
		}

		char		realcur()
		{
			return buffer[bufferoffset];
		}

		char		at(int32 off)
		{
			return buffer[off];
		}
		int32			offset()
		{
			return bufferoffset;
		}
		void		seek(int32 off)
		{
			bufferoffset = off;
		}
		bool		isend()
		{
			return bufferoffset == buffersize || (realcur() == EOF) || (realcur() == 0);
		}
		int32			line()
		{
			return l;
		}
	public:
		const char *buffer;
		std::string filename;
		int32			buffersize;
		int32			bufferoffset;
		int32			l;
	};
}

#endif // CPPSBUFFER_CPPS_HEAD_