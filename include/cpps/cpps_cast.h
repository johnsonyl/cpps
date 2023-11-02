#ifndef CPPS_CAST_CPPS_HEAD_
#define CPPS_CAST_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/24 (yy/mm/dd)
//@Module		:	CPPS_CAST
//@Description	:	Cpps ×ª»»µ½ C++
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	template<class T>
	static T object_cast(object o)
	{
		if (o.value.tt == CPPS_TREF) {
			typedef cpps_converter<T> converter;
			if (!converter::match((*o.value.value.value)))
				throw(cpps_error(__FILE__, __LINE__, 0, "cppsvalue can't convert to %s, cppsvalue type is %s , conversion failed", typeid(T).name(), type_s(o).c_str()));
			return converter::apply((*o.value.value.value));
		}
		else {
			typedef cpps_converter<T> converter;
			if (!converter::match((o.value)))
				throw(cpps_error(__FILE__, __LINE__, 0, "cppsvalue can't convert to %s, cppsvalue type is %s , conversion failed", typeid(T).name(), type_s(o).c_str()));
			return converter::apply((o.value));
		}
		
	}
}
#endif // CPPS_CAST_CPPS_HEAD_