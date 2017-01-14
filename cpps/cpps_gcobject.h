#ifndef CPPS_GCOBJECT_CPPS_HEAD_
#define CPPS_GCOBJECT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_GCOBJECT
//@Description	:	垃圾回收
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	struct cpps_gcobject
	{
		cpps_gcobject()
		{
			gclevel = 0;
		}
		virtual void							setGCLevel(char l)
		{
			gclevel = l;
		}
		virtual char							getGCLevel()
		{
			return gclevel;
		}
		char gclevel;//0 新手，1 元老 2.永恒（不知道有没有用）
	};
}
#endif // CPPS_GCOBJECT_CPPS_HEAD_