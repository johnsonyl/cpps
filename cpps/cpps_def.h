#ifndef CPPS_DEF_CPPS_HEAD_
#define CPPS_DEF_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_DEF
//@Description	:	宏定义
//@website		:	http://cpps.wiki
//==================================

#define CPPS_VER		"1.0 Beta"
#define CPPS_NAME		"CPPS"

#define CPPS_GEN1_CHECKSIZE			1024 * 5  // 512 M
#define CPPS_GEN0_CHECKSIZE			CPPS_GEN1_CHECKSIZE / 8 * 3
#define CPPS_GEN0_CHECKCOUNT		10
#define CPPS_BARRIER_CHECKCOUNT		10

#define CPPS_TNIL		0
#define CPPS_TINTEGER	1	
#define CPPS_TNUMBER	2
#define CPPS_TBOOLEAN	3
#define CPPS_TSTRING	4
#define	CPPS_TUSERDATA	5
#define CPPS_TFUNCTION	6
#define CPPS_TDOMAIN	7	//暂时内部用（域）
#define CPPS_TREGVAR	8	//变量引用（指针)（域）
#define CPPS_TCLASS		9	//定义类
#define CPPS_TCLASSVAR  10  //类的对象

#define CPPS_SINGLERET		0
#define CPPS_MUNITRET		1



#define CPPS_ONIL				0 //空 约等于NULL
#define CPPS_ODOFUNCTION		1 //执行函数
#define CPPS_ONUMPARAM			2 //数字参数
#define CPPS_OSTRPARAM			3 //字符串参数
#define CPPS_OSTR				4 //字符串
#define CPPS_ROOT				5 //根节点
#define CPPS_ORETLIST			6 //返回值列表
#define CPPS_OPARAMLIST			7 //参数列表
#define CPPS_ODEFVAR			8 //定义变量
#define CPPS_VARNAME			9 //变量名字
#define CPPS_ODEFVAR_VAR		10	//变量确实为变量啊？
#define CPPS_ODEFVAR_FUNCTION	11	//变量为函数
#define CPPS_OVARPARAM			12 //变量参数
#define CPPS_FUNCNAME			13 //变量名字
#define CPPS_ONUMBER			14 //数字(浮点数）
#define CPPS_OINTEGER			15 //数字（整数）
#define CPPS_ODEFVAR_FUNC		16 //定义函数
#define CPPS_ODEFVAR_NIL		17 // NIL
#define CPPS_OBOOLPARAM			18 //字符串参数
#define CPPS_OBOOL				19 //字符串参数
#define CPPS_OASSEMBLE			20 //执行集
#define CPPS_OIF				21	//执行if else
#define CPPS_ORETURN			22	//执行if else
#define CPPS_OBRACKET			23	//括号
#define CPPS_OFOR				24	//for循环
#define CPPS_QUOTEVARNAME		25 //引用变量名字
#define CPPS_OEXPRESSION		26 //表达式
#define CPPS_OGETOBJECT			27	//获取子对象
#define CPPS_CHILDVARNAME		28	//子对象名字
#define	CPPS_OWHILE				29	//while循环
#define	CPPS_OCLASS				30	//class类
#define CPPS_ONEWVAR			31	//创建对象
#define CPPS_QUOTEGETOBJECT		32  //引用变量名字
#define	CPPS_ODOFILE			33	//dofile
#define	CPPS_ODOSTRING			34	//dostring
#define	CPPS_OBREAK				35	//break
#define	CPPS_OCONTINUE			36	//continue
#define CPPS_OGETCHIILD			37	//执行 [ ] 函数
#define CPPS_QUOTEGETCHIILD		38  //引用变量名字
#define	CPPS_OINCLUDE			39	//include
#define CPPS_VARNAME_LAMBDA		40	//lambda变量名
#define CPPS_OARRAY				41	//括号

#define CPPS_NOERROR			0	//函数返回没有错误


#define CPPS_NOT_DEFASSEMBLE	1 //不可以定义集
#define CPPS_NOT_DEFFUNCTION	2 //不可以定义函数
#define CPPS_NOT_USEBUILTIN		4	//不可以使用关键字
#define CPPS_NOT_DEFVAR			8 //不可以定义变量
#define CPPS_NOT_DONTDELETEEND	16 //不需要移除;
#define CPPS_NOT_DEFCLASS		32 //不要定义类

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
	printf("错误: %d : %s file:%s line:%d \n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
	\
}\
	catch (const char* s)\
{\
	printf("错误: %s \n", s); \
}
#define _CPPS_CATCH } catch (cpps_error e)\
{\
	printf("错误: %d : %s file:%s line:%d \n错误堆栈信息：\n", e.erron, e.s.c_str(), e.file.c_str(), e.line); \
	std::vector<cpps_stack*> &stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist.rbegin(); it != stacklist.rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f.c_str() << "	第" << stack->l << "行	函数：" << stack->func.c_str() << std::endl; \
}\
	\
}\
	catch (const char* s)\
{\
	printf("错误: %s \n错误堆栈信息：\n", s); \
	std::vector<cpps_stack*> &stacklist = c->getcallstack(); \
for (std::vector<cpps_stack*>::reverse_iterator it = stacklist.rbegin(); it != stacklist.rend(); ++it)\
{\
	cpps::cpps_stack *stack = *it; \
	std::cout << " " << stack->f.c_str() << "	第" << stack->l << "行	函数：" << stack->func.c_str() << std::endl; \
}\
	\
}


typedef double cpps_number;
typedef __int64 cpps_integer;
typedef unsigned char byte;

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

//一些linux下需要的头文件
#ifndef _WIN32
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>

#ifndef FALSE  
	#define FALSE   0
#endif
#ifndef TRUE  
	#define TRUE    1
#endif
#ifndef NULL  
	#define NULL    0
#endif
#else

#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <time.h>

#endif


#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <direct.h>

//////////////////////////////////////////////////////////////////////////


//让脚本在vs里更好看
typedef  void var;
void println(std::string asd);
void printfln(std::string asd);
void sleep(int time);
typedef void math;
void random(int min, int max);
void gettime();
void gettickcount();

#endif // CPPS_DEF_CPPS_HEAD_