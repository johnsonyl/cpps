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
			_lock.lock();
			if (_object_list.empty())
			{
				_lock.unlock();
				return CPPSNEW(T)();
			}
			T* obj = _object_list[_object_list.size() - 1];
			_object_list.pop_back();
			_lock.unlock();
			return obj;
		}
		void	free(T* v)
		{
			_lock.lock();
			_object_list.emplace_back(v);
			_lock.unlock();
		}
		void	freeall()
		{
			_lock.lock();
			for (size_t i = 0; i < _object_list.size(); i++)
			{
				T* obj = _object_list[i];
				CPPSDELETE( obj);
			}
			_object_list.clear();
			_lock.unlock();
		}
	public:
		std::vector<T*>		_object_list;
		cpps::cpps_lock		_lock;
	};
}

#endif //CPPS_OBJECT_POOL_HEAD_