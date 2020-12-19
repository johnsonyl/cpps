#ifndef CPPS_MAKE_FUNCTION_CPPS_HEAD_
#define CPPS_MAKE_FUNCTION_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_MAKE_FUNCTION
//@Description	:	��������������
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	template< typename p1 >
	dofunc< p1 > make_function(p1 param1)
	{
		return dofunc< p1 >(param1);
	}
}


#endif // CPPS_MAKE_FUNCTION_CPPS_HEAD_