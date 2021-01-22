#ifndef CPPS_DEF_CPPS_HEAD_
#define CPPS_DEF_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_DEF
//@Description	:	�궨��
//@website		:	http://cppscript.org
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
#define CPPS_GEN1_CHECKSIZE			(1024 * 1024 * 8)  // 128 M
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
#define CPPS_TREF			8	//�������ã�ָ��)����
#define CPPS_TCLASS				9	//������
#define CPPS_TCLASSVAR			10  //��Ķ���
#define CPPS_TLAMBDAFUNCTION	11
#define CPPS_TTUPLE				12
#define CPPS_TSTRINGV			13
#define CPPS_TELLIPSIS			14

#define CPPS_SINGLERET		0
#define CPPS_MUNITRET		1

//step
#define CPPS_OEXPRESSION		100 //���ʽ
#define CPPS_ODEFVAR			101 //�������
#define CPPS_ODEFCONSTVAR		102 //��������const
#define CPPS_ODEFASYNCVAR		103 //����Э�̺���
#define CPPS_OASSEMBLE			104 //ִ�м�
#define CPPS_ORETURN			105	//ִ��if else
#define CPPS_OTHROW				106	//ִ��if else
#define CPPS_OECHO				107 //ECHO.
#define	CPPS_OBREAK				108	//break
#define	CPPS_OCONTINUE			109	//continue
#define CPPS_OYIELD				110 //�ַ�������
#define CPPS_OIF				111	//ִ��if else
#define CPPS_OFOR				112	//forѭ��
#define CPPS_OFOREACH			113	//foreachѭ��
#define	CPPS_OWHILE				114	//whileѭ��
#define	CPPS_OCLASS				115	//class��
#define	CPPS_ONAMESPACE			116	//���ռ�
#define CPPS_OMODULE			117 //ģ��
#define CPPS_OENUM				118 //ENUM ö��.
#define CPPS_OTRYCATCH			119 //trycatch
#define	CPPS_ODOFILE			120	//dofile
#define	CPPS_OINCLUDE			121	//include
#define	CPPS_OIMPORT			122	//import�����
#define	CPPS_ODOSTRING			123	//dostring

//expression
#define	CPPS_OOFFSET			200	//offset��Զ�λ����Ҹ�����ִ��
#define CPPS_FUNCNAME			201 //��������
#define	CPPS_QUOTEOFFSET		202	//offset����
#define CPPS_VARNAME			203 //��������
#define CPPS_ODOFUNCTION		204 //ִ�к���
#define CPPS_OSTR				205 //�ַ���
#define CPPS_ONUMBER			206 //����(��������
#define CPPS_OINTEGER			207 //���֣�������
#define CPPS_OINTEGER16			208 //���֣�������
#define CPPS_ONEWVAR			209	//��������
#define CPPS_OBRACKET			210	//����
#define CPPS_OARRAY				211	//����{}
#define CPPS_OOBJECT			212	//����{}
#define CPPS_OBOOL				213 //�ַ�������
#define CPPS_OTHIS				214 //this�ؼ��ֵ���д
#define CPPS_OAWAIT				215 //await �ȴ��첽����
#define CPPS_VARNAME_LAMBDA		216	//lambda������
#define CPPS_OELLIPSIS			217 //...
#define CPPS_OGETCHIILD			218	//ִ�� [ ] ����
#define CPPS_OSLICE				219 //��Ƭ.
#define CPPS_OGETOBJECT			220	//��ȡ�Ӷ���
#define CPPS_QUOTEVARNAME		221 //���ñ�������
#define CPPS_QUOTEGETCHIILD		222  //���ñ�������
#define CPPS_QUOTEGETOBJECT		223  //���ñ�������




#define CPPS_ONIL				0 //�� Լ����NULL
#define CPPS_ONUMPARAM			2 //���ֲ���
#define CPPS_OSTRPARAM			3 //�ַ�������
#define CPPS_ROOT				5 //���ڵ�
#define CPPS_ORETLIST			6 //����ֵ�б�
#define CPPS_OPARAMLIST			7 //�����б�
#define CPPS_ODEFVAR_VAR		10	//����ȷʵΪ��������
#define CPPS_ODEFVAR_FUNCTION	11	//����Ϊ����
#define CPPS_OVARPARAM			12 //��������
#define CPPS_ODEFVAR_FUNC		16 //���庯��
#define CPPS_ODEFVAR_NIL		17 // NIL
#define CPPS_OBOOLPARAM			18 //�ַ�������
#define CPPS_CHILDVARNAME		28	//�Ӷ�������
#define CPPS_ONAMESPANCE_CHILD	47	//namespace::class
#define CPPS_OCLASS_CONSTRUCTOR	48	//new class(XX)
#define CPPS_ONEW_SETV			50	//new Class(){ this }
#define CPPS_OCATCH				55 //catch
#define CPPS_ODEFVAR_LAMBDA_FUNC 56 //���庯��
#define CPPS_VARNAMES			60 //ģ��
#define CPPS_MULTIVAR			61 //ģ��
#define CPPS_OTHISPARAM			63 //�ַ�������
#define CPPS_ODEFVAR_HOTUPDATE  65 //�Ӻ���.

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
	cpps_domain_type_module,		//ģ��
	cpps_domain_type_enum,		//ö��
	cpps_domain_type_trycatch,		//trycatch
	cpps_domain_type_foreach,		//foreach
	//cpps_domain_type_

};
#define cpps_def_regfunction			1
#define cpps_def_regclass				2
#define cpps_def_regvar					3
#define cpps_def_regparentclass			4
#define cpps_def_regenum				5


#define CPPS_TO_REAL_VALUE(left) if (left.tt == CPPS_TREF){ left = *left.value.value;}

#ifndef CPPS_DECLARE_DEPRECATED
# define CPPS_DECLARE_DEPRECATED(f)   f;
# ifdef __GNUC__
#  if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0)
#   undef CPPS_DECLARE_DEPRECATED
#   define CPPS_DECLARE_DEPRECATED(f)    f __attribute__ ((deprecated));
#  endif
# elif defined(__SUNPRO_C)
#  if (__SUNPRO_C >= 0x5130)
#   undef CPPS_DECLARE_DEPRECATED
#   define CPPS_DECLARE_DEPRECATED(f)    f __attribute__ ((deprecated));
#  endif
# endif
#endif

#ifndef _WIN32
typedef long long __int64;
#endif

#define cpps_time_leap(year) (((year%4==0)&&(year%100!=0))||((year%4==0)&&(year%400==0))) 


#define cpps_try try {

#define _CPPS_TRY try {

#define cpps_catch } catch (cpps_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(), e.file().c_str(), e.line()); \
	\
}\
catch(cpps_trycatch_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(), e.file().c_str(), e.line()); \
}\
	catch (const char* s)\
{\
	printf("error: %s \n", s); \
}
#define _CPPS_CATCH } catch (cpps_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.error(), e.what().c_str(), e.file().c_str(), e.line()); \
	std::vector<cpps_stack*> *stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist->rbegin(); it != stacklist->rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f << "	The " << stack->l << " line	function:" << stack->func << std::endl; \
}\
	c->resume();\
}\
catch(cpps_trycatch_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.error(), e.what().c_str(), e.file().c_str(), e.line()); \
	printf("%s\n",e.callstack().c_str());\
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

#define  CPPS_SUBTRY cpps_stack* __takestack = c->getcallstack()->empty() ? NULL : c->getcallstack()->back();\
try{

#define CPPS_SUBCATCH2 }\
catch (...) {\
cpps_pop_stack_to_here(c, __takestack);\
throw;\
}\

#define CPPS_SUBCATCH }\
catch(...){\
cpps_pop_stack_to_here(c,__takestack);\
if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }\
throw;\
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
#pragma warning(disable: 26495)
#pragma warning(disable: 26812)
#pragma warning(disable: 28182)
#pragma warning(disable: 26444)
#pragma warning(disable: 26451)
#pragma warning(disable: 6011) 
#pragma warning(disable: 4819) 
#pragma warning(disable: 5003) 
#pragma warning(disable: 4100)  
#pragma warning(disable: 4702) 
#pragma warning(disable: 4706) 
#pragma warning(disable: 4458) 
#pragma warning(disable: 6285) 
#pragma warning(disable: 6255) 
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define WIN32_LEAN_AND_MEAN	
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <Process.h>
#include <corecrt_io.h>
#ifdef _WIN32
#define S_ISDIR(m) (m & S_IFDIR)
#define S_ISREG(m) (m & S_IFREG)
#define S_ISBLK(m) (false)
#define S_ISCHR(m) (m & S_IFCHR)
#define S_ISFIFO(m) (m & _S_IFIFO)
#define S_ISLNK(m)  (false)
#define S_ISSOCK(m)  (false)
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
//#include <filesystem> C++17 ��ʱ������.
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