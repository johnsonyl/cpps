#include "cpps/cpps.h"

namespace cpps
{

	
	cppsbuffer::cppsbuffer(const char* _filename, const char* _buffer, int32 _buffersize)
	{
		cppsbuffer_file file;
		file.filename = _filename;
		file.begin = 0;
		file.end = _buffersize;
		file.line = 1;
		files.push_back(file);

		buffer.append(_buffer, (size_t)_buffersize);
		buffersize = _buffersize;
		bufferoffset = 0;
	}

	char cppsbuffer::pop()
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

	char cppsbuffer::realpop()
	{
		if (isend())
		{
			throw(cpps_error(getcurfile().filename.c_str(), getcurfile().line, 0, "Unknown end of file."));
		}
		int32 ret = bufferoffset++;


		//退出最后一个
		if (ret > (getcurfile().end+1)) {
			if (files.size() > 1) files.pop_back();
		}


		if (buffer[(size_t)ret] == '\n')
			getcurfile().line++;

		return buffer[(size_t)ret];
	}

	char cppsbuffer::cur()
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

	char cppsbuffer::realcur()
	{
		return buffer[(size_t)bufferoffset];
	}

	char cppsbuffer::at(int32 off)
	{
		return buffer[(size_t)off];
	}

	cpps::int32 cppsbuffer::offset()
	{
		return bufferoffset;
	}

	void cppsbuffer::seek(int32 off)
	{
		bufferoffset = off;
	}

	bool cppsbuffer::isend()
	{
		return bufferoffset == buffersize || (realcur() == EOF) || (realcur() == 0);
	}

	cpps::int32 cppsbuffer::line()
	{
		return getcurfile().line;
	}

	void cppsbuffer::append(std::string _filename, const char* _buffer, int32 _buffersize)
	{
		cppsbuffer_file file;
		file.filename = _filename;
		file.begin = bufferoffset;
		file.end = bufferoffset + _buffersize;
		file.line = 1;
		for (auto f : files)
		{
			f.end += _buffersize;
		}
		files.push_back(file);

		buffer.insert((size_t)bufferoffset, _buffer, (size_t)_buffersize);
		buffersize += _buffersize;
	}

	cpps::cppsbuffer_file& cppsbuffer::getcurfile()
	{
		return files.back();
	}

}
