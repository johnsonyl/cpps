#ifndef CPPS_ERROR_CPPS_HEAD_
#define CPPS_ERROR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/23 (yy/mm/dd)
//@Module		:	CPPS_ERROR
//@Description	:	Cpps����
//@website		:	http://cppscript.org
//==================================
#include <stdarg.h>

enum
{
	cpps_error_noknow	=	0,
	cpps_error_unknow	=	1,
	cpps_error_varnotnumber = 2,//�������������ֿ�ͷ
	cpps_error_varerror = 3, //���������в��ܰ��������ַ���
	cpps_error_paramerror = 4, //���������в��ܰ��������ַ���
	cpps_error_deffuncrror = 5, //���庯������
	cpps_error_assembleerror = 6, //���弯����
	cpps_error_iferror = 7, //if �������
	cpps_error_normalerror = 8, //�������
	cpps_error_classerror = 9, //�������
	cpps_error_forerror = 10,
	cpps_error_whileerror = 11,
	cpps_error_arrayeerror = 12,
	cpps_error_trycatherror = 13,
	cpps_error_asyncerror = 14,
	cpps_error_moduleerror = 14,
};


struct cpps_error
{
	cpps_error(std::string f, cpps::usint32 l, cpps::int32 n, const char *format, ...)
	{
		char szString[4096];
		va_list ap;
		va_start(ap, format);
#ifdef _WIN32
		vsprintf_s(szString,4096, format, ap);
#else
		vsprintf(szString, format, ap);
#endif
		va_end(ap);

		_erron = n;
		_file = f;
		_line = l;
		s = szString;
	}
	std::string& what() { return s; }
	cpps::int32 error() { return _erron; }
	cpps::usint32 line() { return this->_line; }
	std::string& file() { return _file; }

	cpps::int32 _erron;
	std::string _file;
	cpps::usint32 _line;
	std::string s;
};

namespace cpps {
	inline void fail(std::string msg, int32 n = cpps_error_normalerror,const char* f = __FILE__, usint32 l = __LINE__);
	inline void error(C* c, const char* format, ...);
}


#endif // CPPS_ERROR_CPPS_HEAD_