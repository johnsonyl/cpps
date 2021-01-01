#ifndef CPPS_ASYNC_OBJECT_CPPS_HEAD_
#define CPPS_ASYNC_OBJECT_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/5 (yy/mm/dd)
//@Module		:	CPPS_ASYNC_OBJECT
//@Description	:	协程对象
//@website		:	http://cpps.wiki
//==================================
namespace cpps
{
	struct cpps_async_object
	{
	public:
		cpps_async_object() {
			leftdomain = NULL;
			f = NULL;
			line = 0;
		}

		cpps_value call(C* c);

		cpps_std_vector params;
		cpps_domain* leftdomain;
		cpps_function* f;
		std::string filename;
		std::string funcname;
		int32 line;
	};

}
#endif //CPPS_ASYNC_OBJECT_CPPS_HEAD_