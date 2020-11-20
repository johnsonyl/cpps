#ifndef CPPS_IS_VOID_CPPS_HEAD_
#define CPPS_IS_VOID_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_IS_VOID
//@Description	:	检测是否为void参数
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	template<class T>
	class cpps_is_void
	{
	public:
		cpps_is_void()
		{
			b = false;
		}
	public:
		bool b;
	};
	template<>
	class cpps_is_void<void>
	{
	public:
		cpps_is_void()
		{
			b = true;
		}

	public:
		bool b;
	};

	

}


#endif // CPPS_IS_VOID_CPPS_HEAD_