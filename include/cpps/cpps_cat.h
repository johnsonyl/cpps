#ifndef CPPS_CAT_CPPS_HEAD_
#define CPPS_CAT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_CAT
//@Description	:	��ƴ��
//@website		:	http://cpps.wiki
//==================================


#define CPPS_PP_CAT(a,b)  CPPS_PP_CAT_I(a,b)
#define CPPS_PP_CAT_I(a, b) CPPS_PP_CAT_II(a ## b)
#define CPPS_PP_CAT_II(res) res


#endif // CPPS_CAT_CPPS_HEAD_
