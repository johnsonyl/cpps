#ifndef CPPS_OBJECT_POOL_HEAD_
#define CPPS_OBJECT_POOL_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/11/26 (yy/mm/dd)
//@Module		:	CPPS_OBJECT_POOL
//@Description	:	非管理模式内存池.
//@website		:	http://cppscript.org
//==================================

#include <vector>

namespace cpps
{
	template <class T>
	class cpps_object_pool
	{
	public:
		cpps_object_pool()
		{

		}
		virtual ~cpps_object_pool()
		{
			freeall();
		}
		T* alloc()
		{
			if (_object_list.empty())
			{
				return new T();
			}
			T* obj = _object_list[_object_list.size() - 1];
			_object_list.pop_back();
			return obj;
		}
		void	free(T* v)
		{
			_object_list.insert(_object_list.end(), v);
		}
		void	freeall()
		{
			for (size_t i = 0; i < _object_list.size(); i++)
			{
				T* obj = _object_list[i];
				delete obj;
			}
			_object_list.clear();
		}
	public:
		std::vector<T*>		_object_list;
	};
}

#endif //CPPS_OBJECT_POOL_HEAD_