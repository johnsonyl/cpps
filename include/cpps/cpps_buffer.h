#ifndef CPPSBUFFER_CPPS_HEAD_
#define CPPSBUFFER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	CPPSBUFFER
//@Description	:	×Ö½ÚÁ÷IO
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	struct cppsbuffer_file
	{
		cppsbuffer_file();
		std::string filename;
		int32		begin;
		int32		end;
		int32		line;
	};
	class cppsbuffer
	{
	public:
		cppsbuffer(const char *_filename,const char* _buffer, int32 _buffersize);
		char							pop();
		char							realpop();
		char							cur();
		char							realcur();
		char							at(int32 off);
		int32							offset();
		void							seek(int32 off);
		bool							isend();
		int32							line();
		void							append(std::string _filename, const char* _buffer, int32 _buffersize);
		cppsbuffer_file&				getcurfile();
	public:
		std::vector< cppsbuffer_file >	files;
		std::string						buffer;
		int32							buffersize;
		int32							bufferoffset;
	};
}

#endif // CPPSBUFFER_CPPS_HEAD_