#ifndef CPPSBUFFER_CPPS_HEAD_
#define CPPSBUFFER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	CPPSBUFFER
//@Description	:	字节流IO
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	struct cppsbuffer_file
	{
		cppsbuffer_file() {
			begin = 0;
			end = 0;
			line = 0;
		}
		std::string filename;
		int32		begin;
		int32		end;
		int32		line;
	};
	class cppsbuffer
	{
	public:
		cppsbuffer(const char *_filename,const char* _buffer, int32 _buffersize)
		{
			cppsbuffer_file file;
			file.filename = _filename;
			file.begin = 0;
			file.end = _buffersize;
			file.line = 1;
			files.push_back(file);

			buffer.append(_buffer,(size_t)_buffersize);
			buffersize = _buffersize;
			bufferoffset = 0;
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

			
			

			return realpop();
		}

		char realpop()
		{
			if (isend())
			{
				throw(cpps_error(getcurfile().filename.c_str(), getcurfile().line, 0, "Unknown end of file."));
			}
			int32 ret = bufferoffset++;

			if (buffer[(size_t)ret] == '\n')
				getcurfile().line++;

			//退出最后一个
			if (  bufferoffset > getcurfile().end) {
				if(files.size() > 1) files.pop_back();
			}

			return buffer[(size_t)ret];
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
			return buffer[(size_t)bufferoffset];
		}

		char		at(int32 off)
		{
			return buffer[(size_t)off];
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
			return getcurfile().line;
		}
		void		append(std::string _filename,const char* _buffer, int32 _buffersize)
		{
			cppsbuffer_file file;
			file.filename = _filename;
			file.begin = bufferoffset;
			file.end = bufferoffset+_buffersize;
			file.line = 1;
			for (auto f : files)
			{
				f.end += _buffersize;
			}
			files.push_back(file);

			buffer.insert((size_t)bufferoffset,_buffer, (size_t)_buffersize);
			buffersize += _buffersize;
		}
		cppsbuffer_file& getcurfile() {
			return files.back();
		}
	public:
		std::vector< cppsbuffer_file > files;
		std::string	buffer;
		int32			buffersize;
		int32			bufferoffset;
	};
}

#endif // CPPSBUFFER_CPPS_HEAD_