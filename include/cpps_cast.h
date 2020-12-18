#ifndef CPPS_CAST_CPPS_HEAD_
#define CPPS_CAST_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_CAST
//@Description	:	Cpps ×ª»»µ½ C++
//@website		:	http://cpps.wiki
//==================================

namespace cpps
{
	template<class T>
	static T object_cast(object o)
	{
		typedef cpps_converter<T> converter;
		if (!converter::match((o.value)))
			throw(cpps_error("0", 0, 0, "%s is not defined to script, conversion failed.", typeid(T).name()));
		return converter::apply((o.value));
	}
}
#endif // CPPS_CAST_CPPS_HEAD_