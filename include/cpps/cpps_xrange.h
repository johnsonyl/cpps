#ifndef CPPS_XRANGE_CPPS_HEAD_
#define CPPS_XRANGE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/24 (yy/mm/dd)
//@Module		:	CPPS_ARRAY
//@Description	:	vector×¢²áÀà
//@website		:	http://cppscript.org
//==================================


namespace cpps
{

	struct cpps_range
	{
		virtual ~cpps_range() {}
	public:
		cpps_integer  begin;
		cpps_integer  end;
		cpps_integer  inc;

		cpps_integer start_func();
		cpps_integer end_func();
		cpps_integer step_func();
	};
	void	cpps_regrange(C* c);


}

#endif // CPPS_ARRAY_CPPS_HEAD_