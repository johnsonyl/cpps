#ifndef CPPS_MULITRETURN_CPPS_HEAD_
#define CPPS_MULITRETURN_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2021/01/07 (yy/mm/dd)
//@Module		:	CPPS_MULITRETURN
//@Description	:	mulitreturn∂‡∑µªÿ÷µ
//@website		:	http://cppscript.org
//==================================

namespace cpps {
	struct tuple
	{
		tuple(){}
		template<typename T, class... _Types>
		tuple(const T&& v, _Types&&... _Args) {
			tuple_list.reserve(sizeof... (_Types));
			push_back(v, _Args...);
		}
		template<typename T>
		void push_back(const T& v) {
			tuple_list.emplace_back(v);
		}
		template<typename T, class... _Types>
		void push_back(const T& v, _Types&&... _Args) {
			tuple_list.emplace_back(v);
			push_back(_Args...);
		}
		std::vector<object> real() { return tuple_list; }
		std::vector<object> tuple_list;
	};
}
#endif