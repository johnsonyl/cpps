#ifndef CPPS_DEF_CPPS_HEAD_
#define CPPS_DEF_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_DEF
//@Description	:	�궨��
//@website		:	http://cpps.wiki
//==================================

#if defined _WIN32
#ifdef _X86_
#define CPPS_CURRENT_PLANTFORM "win32"
#else
#define CPPS_CURRENT_PLANTFORM "win64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "windows"
#elif defined LINUX
#ifdef _X86_
#define CPPS_CURRENT_PLANTFORM "linux32"
#else
#define CPPS_CURRENT_PLANTFORM "linux64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "linux"
#elif defined __APPLE__
#ifdef _X86_
#define CPPS_CURRENT_PLANTFORM "macos32"
#else
#define CPPS_CURRENT_PLANTFORM "macos64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "macos"
#else
#define CPPS_CURRENT_PLANTFORM "unknow OS system."
#endif

#ifdef _X86_
#define CPPS_CURRENT_ARCH "x86"
#else
#define CPPS_CURRENT_ARCH "x64"
#endif

#if defined _MSC_VER
#define CPPS_BUILDER_VERSION_MAKE2(a,b) a # b " " CPPS_CURRENT_PLANTFORM
#define CPPS_BUILDER_VERSION_MAKE(a,b) CPPS_BUILDER_VERSION_MAKE2(a,b)
#define CPPS_BUILDER_VERSION	CPPS_BUILDER_VERSION_MAKE("MSC v.", _MSC_VER)
#elif defined __APPLE__
#define CPPS_BUILDER_VERSION_MAKE2(a,b,c,d) a # b "." # c "." # d " " CPPS_CURRENT_PLANTFORM
#define CPPS_BUILDER_VERSION_MAKE(a,b,c,d) CPPS_BUILDER_VERSION_MAKE2(a,b,c,d)
#define CPPS_BUILDER_VERSION CPPS_BUILDER_VERSION_MAKE("clang gcc-",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#elif defined __GNUC__
#define CPPS_BUILDER_VERSION_MAKE2(a,b,c,d,e) a # b "." # c "." # d " glibcxx-" # e " " CPPS_CURRENT_PLANTFORM
#define CPPS_BUILDER_VERSION_MAKE(a,b,c,d,e) CPPS_BUILDER_VERSION_MAKE2(a,b,c,d,e)
#define CPPS_BUILDER_VERSION CPPS_BUILDER_VERSION_MAKE("gcc-",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__,__GLIBCXX__)
#else
#define  CPPS_BUILDER_VERSION ""
#endif

#define CPPS_VER		"1.0.0 Build 20201209 [" CPPS_BUILDER_VERSION "]"
#define CPPS_VERN		10000
#define CPPS_NAME		"CPPS"
#ifndef M_PI
#define M_PI				3.14159265358979323846
#endif
#ifdef _DEBUG
#define CPPS_GEN1_CHECKSIZE			(512 * 1024)  // 0.5 M
#define CPPS_GEN0_CHECKSIZE			(CPPS_GEN1_CHECKSIZE / 8 * 3)
#define CPPS_GEN0_CHECKCOUNT		64
#else
#define CPPS_GEN1_CHECKSIZE			(1024 * 1024 * 128)  // 128 M
#define CPPS_GEN0_CHECKSIZE			(CPPS_GEN1_CHECKSIZE / 8 * 3)
#define CPPS_GEN0_CHECKCOUNT		512
#endif
#define CPPS_BARRIER_CHECKCOUNT		64
#define CPPS_ENCODE_CPPS_KEY	0x1f


#define CPPS_TNIL				0
#define CPPS_TINTEGER			1	
#define CPPS_TNUMBER			2
#define CPPS_TBOOLEAN			3
#define CPPS_TSTRING			4
#define	CPPS_TUSERDATA			5
#define CPPS_TFUNCTION			6
#define CPPS_TDOMAIN			7	//��ʱ�ڲ��ã���
#define CPPS_TREGVAR			8	//�������ã�ָ��)����
#define CPPS_TCLASS				9	//������
#define CPPS_TCLASSVAR			10  //��Ķ���
#define CPPS_TLAMBDAFUNCTION	11
#define CPPS_TMULTIRETURN		12
#define CPPS_TSTRINGV			13

#define CPPS_SINGLERET		0
#define CPPS_MUNITRET		1



#define CPPS_ONIL				0 //�� Լ����NULL
#define CPPS_ODOFUNCTION		1 //ִ�к���
#define CPPS_ONUMPARAM			2 //���ֲ���
#define CPPS_OSTRPARAM			3 //�ַ�������
#define CPPS_OSTR				4 //�ַ���
#define CPPS_ROOT				5 //���ڵ�
#define CPPS_ORETLIST			6 //����ֵ�б�
#define CPPS_OPARAMLIST			7 //�����б�
#define CPPS_ODEFVAR			8 //�������
#define CPPS_VARNAME			9 //��������
#define CPPS_ODEFVAR_VAR		10	//����ȷʵΪ��������
#define CPPS_ODEFVAR_FUNCTION	11	//����Ϊ����
#define CPPS_OVARPARAM			12 //��������
#define CPPS_FUNCNAME			13 //��������
#define CPPS_ONUMBER			14 //����(��������
#define CPPS_OINTEGER			15 //���֣�������
#define CPPS_ODEFVAR_FUNC		16 //���庯��
#define CPPS_ODEFVAR_NIL		17 // NIL
#define CPPS_OBOOLPARAM			18 //�ַ�������
#define CPPS_OBOOL				19 //�ַ�������
#define CPPS_OASSEMBLE			20 //ִ�м�
#define CPPS_OIF				21	//ִ��if else
#define CPPS_ORETURN			22	//ִ��if else
#define CPPS_OBRACKET			23	//����
#define CPPS_OFOR				24	//forѭ��
#define CPPS_QUOTEVARNAME		25 //���ñ�������
#define CPPS_OEXPRESSION		26 //���ʽ
#define CPPS_OGETOBJECT			27	//��ȡ�Ӷ���
#define CPPS_CHILDVARNAME		28	//�Ӷ�������
#define	CPPS_OWHILE				29	//whileѭ��
#define	CPPS_OCLASS				30	//class��
#define CPPS_ONEWVAR			31	//��������
#define CPPS_QUOTEGETOBJECT		32  //���ñ�������
#define	CPPS_ODOFILE			33	//dofile
#define	CPPS_ODOSTRING			34	//dostring
#define	CPPS_OBREAK				35	//break
#define	CPPS_OCONTINUE			36	//continue
#define CPPS_OGETCHIILD			37	//ִ�� [ ] ����
#define CPPS_QUOTEGETCHIILD		38  //���ñ�������
#define	CPPS_OINCLUDE			39	//include
#define CPPS_VARNAME_LAMBDA		40	//lambda������
#define CPPS_OARRAY				41	//����{}
#define	CPPS_ONAMESPACE			42	//���ռ�
#define CPPS_OINTEGER16			43 //���֣�������
#define CPPS_OTRYCATCH			44 //trycatch
#define CPPS_OTHROW				45	//ִ��if else
#define CPPS_OOBJECT			46	//����{}
#define CPPS_ONAMESPANCE_CHILD	47	//namespace::class
#define CPPS_OCLASS_CONSTRUCTOR	48	//new class(XX)
#define CPPS_ODEFCONSTVAR		49 //��������const
#define CPPS_ONEW_SETV			50	//new Class(){ this }
#define CPPS_OFOREACH			51	//foreachѭ��
#define	CPPS_OIMPORT			52	//import�����
#define	CPPS_OOFFSET			53	//offset��Զ�λ����Ҹ�����ִ��
#define	CPPS_QUOTEOFFSET		54	//offset����
#define CPPS_OCATCH				55 //catch
#define CPPS_ODEFVAR_LAMBDA_FUNC 56 //���庯��
#define CPPS_ODEFASYNCVAR		 57 //����Э�̺���
#define CPPS_OAWAIT				58 //await �ȴ��첽����
#define CPPS_OMODULE			59 //ģ��
#define CPPS_VARNAMES			60 //ģ��
#define CPPS_MULTIVAR			61 //ģ��
#define CPPS_OTHIS				62 //this�ؼ��ֵ���д
#define CPPS_OTHISPARAM			63 //�ַ�������
#define CPPS_OYIELD				64 //�ַ�������


#define CPPS_NOERROR			0	//��������û�д���


#define CPPS_NOT_DEFASSEMBLE	1 //�����Զ��弯
#define CPPS_NOT_DEFFUNCTION	2 //�����Զ��庯��
#define CPPS_NOT_USEBUILTIN		4	//������ʹ�ùؼ���
#define CPPS_NOT_DEFVAR			8 //�����Ը���������ֵ
#define CPPS_NOT_DONTDELETEEND	16 //����Ҫ�Ƴ�;
#define CPPS_NOT_DEFCLASS		32 //��Ҫ������
#define CPPS_NOT_DEFSYSTEM		32 //������ʹ��#

//regvatlist
#define CPPS_OFFSET_TYPE_GLOBAL 0		//_G 
#define CPPS_OFFSET_TYPE_LEFTDOMAIN 1	//leftdomain + N
#define	CPPS_OFFSET_TYPE_SELF 2			// SELF + N;
#define	CPPS_OFFSET_TYPE_LEFTCLASS 3	// LEFT CLASS + N;

enum
{
	cpps_domain_type_root = 1, //��
	cpps_domain_type_ass,		//��
	cpps_domain_type_func,     //����
	cpps_domain_type_if,		//if
	cpps_domain_type_for,		//for
	cpps_domain_type_while,		//while
	cpps_domain_type_switch,	//switch
	cpps_domain_type_exec,		//ִ������ʱ��
	cpps_domain_type_class,		//�����
	cpps_domain_type_classvar,		//�����
	cpps_domain_type_namespace,		//���ռ�
	cpps_domain_type_module,		//���ռ�
	cpps_domain_type_trycatch,		//trycatch
	cpps_domain_type_foreach,		//foreach
	//cpps_domain_type_

};

#ifndef _WIN32
typedef long long __int64;
#endif

#define cpps_time_leap(year) (((year%4==0)&&(year%100!=0))||((year%4==0)&&(year%400==0))) 


#define cpps_try try {

#define _CPPS_TRY try {

#define cpps_catch } catch (cpps_error e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
	\
}\
catch(cpps_trycatch_error e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
}\
	catch (const char* s)\
{\
	printf("error: %s \n", s); \
}
#define _CPPS_CATCH } catch (cpps_error e)\
{\
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
	std::vector<cpps_stack*> *stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f << "	The " << stack->l << " line	function:" << stack->func << std::endl; \
}\
	c->resume();\
}\
catch(cpps_trycatch_error e)\
{\
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
	printf("%s\n",e.callstackstr.c_str());\
	std::vector<cpps_stack*> *stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f << "	The " << stack->l << " line	function:" << stack->func << std::endl; \
}\
	c->resume();\
}\
catch (const char* s)\
{\
	printf("error: %s \nnError stack information:\n", s); \
	std::vector<cpps_stack*> *stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f << "	The " << stack->l << " line	function:" << stack->func << std::endl; \
}\
	c->resume();\
}


typedef double cpps_number;
typedef __int64 cpps_integer;
//typedef unsigned char byte;

//long ,unsigned long,float ,double

namespace cpps
{
	typedef char				int8;
	typedef unsigned char		usint8;
	typedef short				int16;
	typedef unsigned short		usint16;
	typedef	int					int32;
	typedef	unsigned int		usint32;
	typedef long				long32;
	typedef unsigned long		uslong32;
	typedef	long long			int64;
	typedef	unsigned long long	usint64;
}




#if defined LINUX
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/io.h>
#include <dlfcn.h>
#include <dirent.h>



#elif defined __APPLE__

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/uio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <mach-o/dyld.h>

#elif defined _WIN32 

#ifdef __cplusplus
#pragma warning(disable:26495)   //disable unicode code page warning
#pragma warning(disable:6011)   //disable NULL PTR USED warning
#pragma warning(disable:4819)   //disable unicode code page warning
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <Process.h>
#include <corecrt_io.h>
#ifdef _WIN32
#define S_ISDIR(m) m & S_IFDIR
#define S_ISREG(m) m & S_IFREG
#define S_ISBLK(m) false
#define S_ISCHR(m) m & S_IFCHR
#define S_ISFIFO(m) m & _S_IFIFO
#define S_ISLNK(m)  false
#define S_ISSOCK(m)  false
#endif
#define cpps_export_void extern "C" _declspec(dllexport) void __stdcall
#define cpps_export_finish 

#endif //_WIN32

//һЩ��windows�¶���
#ifndef _WIN32
#ifndef FALSE  
#define FALSE   0
#endif
#ifndef TRUE  
#define TRUE    1
#endif
#ifndef NULL  
#define NULL    0
#endif

#define MAXUINT64   ((usint64)~((int64)0))
#define MAXINT64    ((cpps_integer)(MAXUINT64 >> 1))
#define MININT64    ((cpps_integer)~MAXINT64)

typedef void* HMODULE;

#define _mkdir(p) mkdir(p,S_IRWXU)
#define _rmdir(p) rmdir(p)

#define  cpps_export_void extern "C" void
#define cpps_export_finish extern "C" const CPPS_ST_API  LIBAPI = {\
cpps_attach,\
cpps_detach,\
};


namespace cpps { struct C; }

typedef struct {
	void   (*cpps_attach)(cpps::C*);
	void   (*cpps_detach)(cpps::C*);
} CPPS_ST_API;

#endif

#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include<math.h>
#include <regex>
#include "phmap.h"
//lib
namespace cpps
{
	struct C;

	struct cpps_value;
	typedef std::vector<cpps_value> cpps_std_vector;
}
#ifdef _WIN32
typedef void(__stdcall*cpps_attach_func)(cpps::C *c);
typedef void(__stdcall*cpps_detach_func)(cpps::C *c);
#else
typedef void(*cpps_attach_func)(cpps::C *c);
typedef void(*cpps_detach_func)(cpps::C *c);
#endif
//////////////////////////////////////////////////////////////////////////


//�ýű���vs����ÿ�
typedef  void var;
void println(std::string asd);
void printfln(std::string asd);
void sleep(int time);
typedef void math;
void random(int min, int max);
void gettime();
void gettickcount();

#endif // CPPS_DEF_CPPS_HEAD_