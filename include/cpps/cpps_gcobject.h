#ifndef CPPS_GCOBJECT_CPPS_HEAD_
#define CPPS_GCOBJECT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_GCOBJECT
//@Description	:	垃圾回收
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	struct cpps_gcobject
	{
		cpps_gcobject();
		virtual ~cpps_gcobject();
		//virtual void							setgcLevel(char l);
		//virtual char							getgcLevel();
		void									incruse();
		void									decruse();
		int32									count();
		//char									gclevel;//0 新手，1 元老 2.永恒（不知道有没有用）
		int32									gc_usecount; //usecount;
		//std::atomic<int32>						gc_usecount; //usecount;
	};
}
#endif // CPPS_GCOBJECT_CPPS_HEAD_