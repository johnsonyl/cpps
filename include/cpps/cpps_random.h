#ifndef CPPS_RANDOM_CPPS_HEAD_
#define CPPS_RANDOM_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2023/12/18 (yy/mm/dd)
//@Module		:	CPPS_RANDOM
//@Description	:	RadnomËæ»úÊýÀà
//@website		:	http://cppscript.org
//==================================

namespace cpps {
	struct cpps_random
	{
		cpps_random();
		~cpps_random();
		cpps_integer	rand(object min,object max);
		cpps_number		randf(object min,object max);
		void			seed(object _seed);
		void			generate(object _v);
		std::mt19937_64 generator;
	};

	void	cpps_regrandom(C* c);

}
#endif