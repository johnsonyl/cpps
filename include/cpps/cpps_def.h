#ifndef CPPS_DEF_CPPS_HEAD_
#define CPPS_DEF_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_DEF
//@Description	:	宏定义
//@website		:	http://cppscript.org
//==================================



#if defined _WIN32
#ifndef _WIN64
#define _CPPS_X86 1
#endif 
#ifdef _CPPS_X86
#define CPPS_CURRENT_PLANTFORM "win32"
#else
#define CPPS_CURRENT_PLANTFORM "win64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "windows"
#elif defined LINUX
#ifndef __x86_64__
#define _CPPS_X86 1
#endif
#ifdef _CPPS_X86
#define CPPS_CURRENT_PLANTFORM "linux32"
#else
#define CPPS_CURRENT_PLANTFORM "linux64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "linux"
#elif defined __APPLE__
#ifndef __x86_64__
#define _CPPS_X86 1
#endif
#ifdef _CPPS_X86
#define CPPS_CURRENT_PLANTFORM "macos32"
#else
#define CPPS_CURRENT_PLANTFORM "macos64"
#endif
#define CPPS_CURRENT_EASYPLANTFORM "macos"
#else
#define CPPS_CURRENT_PLANTFORM "unknow OS system."
#endif

#ifdef _CPPS_X86
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

#define CPPS_VER		"1.1.0 Build 20241004 [" CPPS_BUILDER_VERSION "]"
#define CPPS_VERN		11000
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
#define CPPS_TDOMAIN			7	//暂时内部用（域）
#define CPPS_TREF			8	//变量引用（指针)（域）
#define CPPS_TCLASS				9	//定义类
#define CPPS_TCLASSVAR			10  //类的对象
#define CPPS_TLAMBDAFUNCTION	11
#define CPPS_TTUPLE				12
#define CPPS_TSTRINGV			13
#define CPPS_TELLIPSIS			14
#define CPPS_TUINTEGER			15	
#define CPPS_TQUOTECLASSVAR		16

#define CPPS_SINGLERET		0
#define CPPS_MUNITRET		1

//step
#define CPPS_OEXPRESSION		100 //表达式
#define CPPS_ODEFVAR			101 //定义变量
#define CPPS_ODEFCONSTVAR		102 //变量名字const
#define CPPS_ODEFASYNCVAR		103 //定义协程函数
#define CPPS_OASSEMBLE			104 //执行集
#define CPPS_ORETURN			105	//执行if else
#define CPPS_OTHROW				106	//执行if else
#define CPPS_OECHO				107 //ECHO.
#define	CPPS_OBREAK				108	//break
#define	CPPS_OCONTINUE			109	//continue
#define CPPS_OYIELD				110 //字符串参数
#define CPPS_OIF				111	//执行if else
#define CPPS_OFOR				112	//for循环
#define CPPS_OFOREACH			113	//foreach循环
#define	CPPS_OWHILE				114	//while循环
#define	CPPS_OCLASS				115	//class类
#define	CPPS_ONAMESPACE			116	//名空间
#define CPPS_OMODULE			117 //模块
#define CPPS_OENUM				118 //ENUM 枚举.
#define CPPS_OTRYCATCH			119 //trycatch
#define	CPPS_ODOFILE			120	//dofile
#define	CPPS_OINCLUDE			121	//include
#define	CPPS_OIMPORT			122	//import导入库
#define	CPPS_ODOSTRING			123	//dostring
#define	CPPS_ODONODE			124	//parse
#define	CPPS_OASSERT			125	//assert

//expression
#define	CPPS_OOFFSET			200	//offset相对定位免查找更快速执行
#define CPPS_FUNCNAME			201 //变量名字
#define	CPPS_QUOTEOFFSET		202	//offset引用
#define CPPS_VARNAME			203 //变量名字
#define CPPS_ODOFUNCTION		204 //执行函数
#define CPPS_OSTR				205 //字符串
#define CPPS_ONUMBER			206 //数字(浮点数）
#define CPPS_OINTEGER			207 //数字（整数）
#define CPPS_OINTEGER16			208 //数字（整数）
#define CPPS_ONEWVAR			209	//创建对象
#define CPPS_OBRACKET			210	//括号
#define CPPS_OARRAY				211	//数组{}
#define CPPS_OOBJECT			212	//数组{}
#define CPPS_OBOOL				213 //字符串参数
#define CPPS_OTHIS				214 //this关键字的重写
#define CPPS_OAWAIT				215 //await 等待异步函数
#define CPPS_VARNAME_LAMBDA		216	//lambda变量名
#define CPPS_OELLIPSIS			217 //...
#define CPPS_OGETCHIILD			218	//执行 [ ] 函数
#define CPPS_OSLICE				219 //切片.
#define CPPS_OGETOBJECT			220	//获取子对象
#define CPPS_QUOTEVARNAME		221 //引用变量名字
#define CPPS_QUOTEGETCHIILD		222  //引用变量名字
#define CPPS_QUOTEGETOBJECT		223  //引用变量名字
#define CPPS_MULITPARAMS		224 //多参数
#define CPPS_OUINTEGER			225 //数字（正整数）
#define CPPS_OGLOBAL			226 //_G

//optimization
#define	CPPS_OVARAUTOINC		300	//value++
#define	CPPS_OVARAUTODEC		301	//value--
#define	CPPS_OVARPTR			302	//value--




#define CPPS_ONIL				0 //空 约等于NULL
#define CPPS_ONUMPARAM			2 //数字参数
#define CPPS_OSTRPARAM			3 //字符串参数
#define CPPS_ROOT				5 //根节点
#define CPPS_ORETLIST			6 //返回值列表
#define CPPS_OPARAMLIST			7 //参数列表
#define CPPS_ODEFVAR_VAR		10	//变量确实为变量啊？
#define CPPS_ODEFVAR_FUNCTION	11	//变量为函数
#define CPPS_OVARPARAM			12 //变量参数
#define CPPS_ODEFVAR_FUNC		16 //定义函数
#define CPPS_ODEFVAR_NIL		17 // NIL
#define CPPS_OBOOLPARAM			18 //字符串参数
#define CPPS_CHILDVARNAME		28	//子对象名字
#define CPPS_ONAMESPANCE_CHILD	47	//namespace::class
#define CPPS_OCLASS_CONSTRUCTOR	48	//new class(XX)
#define CPPS_ONEW_SETV			50	//new Class(){ this }
#define CPPS_OCATCH				55 //catch
#define CPPS_ODEFVAR_LAMBDA_FUNC 56 //定义函数
#define CPPS_VARNAMES			60 //模块
#define CPPS_MULTIVAR			61 //模块
#define CPPS_OTHISPARAM			63 //字符串参数
#define CPPS_ODEFVAR_HOTUPDATE  65 //子函数.
#define CPPS_OVECTORSIZE		66 //设置vector大小

#define CPPS_NOERROR			0	//函数返回没有错误


#define CPPS_NOT_DEFASSEMBLE	1 //不可以定义集
#define CPPS_NOT_DEFFUNCTION	2 //不可以定义函数
#define CPPS_NOT_USEBUILTIN		4	//不可以使用关键字
#define CPPS_NOT_DEFVAR			8 //不可以给变量设置值
#define CPPS_NOT_DONTDELETEEND	16 //不需要移除;
#define CPPS_NOT_DEFCLASS		32 //不要定义类
#define CPPS_NOT_DEFSYSTEM		64 //不允许使用#
#define CPPS_RUN_COMPILE		128 //COMPILE - 不执行import offset禁用 用于运行时优化

//regvatlist
#define CPPS_OFFSET_TYPE_GLOBAL 0		//_G 
#define CPPS_OFFSET_TYPE_LEFTDOMAIN 1	//leftdomain + N
#define	CPPS_OFFSET_TYPE_SELF 2			// SELF + N;
#define	CPPS_OFFSET_TYPE_LEFTCLASS 3	// LEFT CLASS + N;

enum
{
	cpps_domain_type_root = 1, //根
	cpps_domain_type_ass,		//集
	cpps_domain_type_func,     //函数
	cpps_domain_type_if,		//if
	cpps_domain_type_for,		//for
	cpps_domain_type_while,		//while
	cpps_domain_type_switch,	//switch
	cpps_domain_type_exec,		//执行域（临时域）
	cpps_domain_type_class,		//类的域
	cpps_domain_type_classvar,		//类的域
	cpps_domain_type_namespace,		//名空间
	cpps_domain_type_module,		//模块
	cpps_domain_type_enum,		//枚举
	cpps_domain_type_trycatch,		//trycatch
	cpps_domain_type_foreach,		//foreach
	//cpps_domain_type_

};
#define cpps_def_regfunction			1
#define cpps_def_regclass				2
#define cpps_def_regvar					3
#define cpps_def_regparentclass			4
#define cpps_def_regenum				5
#define cpps_def_regclassvar			6

#define cpps_step_check_none			0
#define cpps_step_check_break			1
#define cpps_step_check_continue		2

#define MAXSYMBOLPRIO 10 // 不得超过16

#define NOLIMIT		((usint8)0x00)
#define LEFTMUSTVAR ((usint8)0x80)
#define RIGHTMUSTVAR ((usint8)0x40)


#define CPPS_TO_REAL_VALUE(left) if (left.tt == CPPS_TREF){ left = *left.value.value;}
#define CPPS_REF_REAL_VALUE(left) ((left.tt == CPPS_TREF) ? *left.value.value : left)
#ifdef _WIN32
#define CPPS_WAITFOR_HEADER template<typename Function, class... _ArgTypes> static inline std::_Invoke_result_t<std::decay_t<Function>, std::decay_t<_ArgTypes>...>
#else
#define CPPS_WAITFOR_HEADER template<typename Function, class... _ArgTypes> static inline typename std::result_of<Function(_ArgTypes...)>::type
#endif
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

#define cpps_newclass(c,type1,name1,type2,name2) type1* name1;type2 name2; cpps::newclass(c,&name1,&name2)


#define cpps_try try {

#define _CPPS_TRY try {

#define cpps_catch } catch (cpps_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(),e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) :e.line()); \
	\
}\
catch(cpps_trycatch_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \n", e.error(), e.what().c_str(),e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) :e.line()); \
}\
	catch (const char* s)\
{\
	printf("error: %s \n", s); \
}
#define _CPPS_CATCH } catch (cpps_error& e)\
{\
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.error(), e.what().c_str(),e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) :e.line()); \
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
	printf("error: %d : %s file:%s line:%d \nError stack information:\n", e.error(), e.what().c_str(), e.file().empty() ? (c->curnode ? c->curnode->filename.c_str() : "") : e.file().c_str(), e.line() == 0 ? (c->curnode ? c->curnode->line : 0) :e.line()); \
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
#ifdef _WIN32
typedef unsigned long long cpps_uinteger;
#else
typedef long long unsigned int cpps_uinteger;
#endif
namespace cpps
{
	typedef signed char			int8;
	typedef unsigned char		usint8;
	typedef short				int16;
	typedef unsigned short		usint16;
	typedef	int					int32;
	typedef	unsigned int		usint32;
	typedef long				long32;
	typedef unsigned long		uslong32;
	typedef	long long			int64;
#ifdef _WIN32
	typedef	unsigned long long	usint64;
#else
	typedef long long unsigned int usint64;
#endif
}




#if defined LINUX
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <unistd.h>
#ifdef __aarch64__
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif
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
#pragma warning(disable: 4146) 
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
#include <sys/timeb.h>
#ifdef _WIN32
#define S_ISDIR(m) (m & S_IFDIR)
#define S_ISREG(m) (m & S_IFREG)
#define S_ISBLK(m) (false)
#define S_ISCHR(m) (m & S_IFCHR)
#define S_ISFIFO(m) (m & _S_IFIFO)
#define S_ISLNK(m)  (false)
#define S_ISSOCK(m)  (false)
#endif
#define cpps_export_type(type) extern "C" _declspec(dllexport) type 
#define cpps_export_void cpps_export_type(void)
#define cpps_export_finish 

#endif //_WIN32

//一些非windows下定义
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

#define  cpps_export_type(type) extern "C" __attribute__((visibility ("default"))) type 
#define  cpps_export_void cpps_export_type(void)
/*#define cpps_export_finish extern "C" const CPPS_ST_API  LIBAPI = {\
cpps_attach,\
cpps_detach,\
};*/
#define cpps_export_finish

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
#if _HAS_CXX17
#include <filesystem> //C++17 暂时不用了
#endif
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
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include <random>
//lib
namespace cpps
{
	struct C;

	struct cpps_value;
	typedef std::vector<cpps_value> cpps_std_vector;
	struct cpps_domain;
	struct cpps_stack;
	struct node;
}


#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED) && !defined(SSIZE_MAX)
typedef intptr_t ssize_t;
# define SSIZE_MAX INTPTR_MAX
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

#ifdef _WIN32 
#define __CPPSCDECL __cdecl
#else
#define __CPPSCDECL 
#endif


typedef void(__CPPSCDECL*cpps_attach_func)(cpps::C *c);
typedef void(__CPPSCDECL*cpps_detach_func)(cpps::C *c);
typedef cpps::cpps_value* (*cpps_func)( cpps::C* c, cpps::cpps_value ps,...);
typedef cpps::cpps_value* (*cpps_class_func)(cpps::cpps_value _cls, cpps::C* c, cpps::cpps_value ps);
typedef phmap::flat_hash_map<std::string, cpps_class_func> FUNCMAP;

typedef void* (*cpps_alloc_f)(size_t);
typedef void (*cpps_free_f)(void*);
typedef void* (*cpps_class_alloc)();
typedef void (*cpps_class_free)(void*);
typedef std::string(*cpps_cxo_func)(cpps::C*c,std::string&);
typedef std::string(*cpps_chartrans_func)(std::string&);
struct cpps_jit_context;
typedef cpps_jit_context*(__CPPSCDECL*cpps_jit_compile_func)(cpps::C*, cpps::cpps_domain*, cpps::node*, cpps::node*);
typedef void(__CPPSCDECL*cpps_jit_run_func)(cpps::C*, cpps::cpps_domain*, cpps::cpps_stack*, cpps_jit_context*);

//////////////////////////////////////////////////////////////////////////

#endif // CPPS_DEF_CPPS_HEAD_