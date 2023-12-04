#ifndef CPPS_NEW_HEAD_
#define CPPS_NEW_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2021/1/17 (yy/mm/dd)
//@Module		:	LIBCPPS
//@Description	:	重写new delete 来监测内存泄露问题.
//@website		:	http://cppscript.org
//==================================

#include <new>


namespace cpps
{
	cpps_integer cpps_this_thread_get_id();
#ifdef _DEBUG
	struct memory_info
	{
		memory_info(size_t _size, const char* _file, unsigned int _line)
		{
			strcpy(file, _file);
			line = _line;
			size = _size;
		}
		char file[MAX_PATH];
		unsigned int line;
		size_t size;
	};

	typedef phmap::flat_hash_map<void*, memory_info*> memory_info_list;
#endif
	struct  memory_allocal_handler
	{
		void*						mmalloc(size_t __size,const char *file, unsigned int _line);
		void						mfree(void* m);
		cpps_lock					_lock;
#ifdef _DEBUG
		size_t						size();
		void						dump();
		memory_info_list			memorylist;
	private:
		size_t	_size;
#endif
	};
	struct memory_allocal
	{
		memory_allocal();
		~memory_allocal();
		void					init();
		bool					global();
		memory_allocal_handler* gethandler();
		void					sethandler(memory_allocal_handler*);
		static memory_allocal&	instance();
		memory_allocal_handler* handler;
		bool					_real;
		cpps_integer			mainthreadid;
	};
}

#define CPPSNEW(_Ty) new (cpps::memory_allocal::instance().gethandler()->mmalloc(sizeof(_Ty),__FILE__,__LINE__)) _Ty
#define CPPSNEW_S(_Size) cpps::memory_allocal::instance().gethandler()->mmalloc(_Size,__FILE__,__LINE__)
#define CPPSMALLOC(_Size) cpps::memory_allocal::instance().gethandler()->mmalloc(_Size,__FILE__,__LINE__)
#define CPPSFREE(_Ptr) cpps::memory_allocal::instance().gethandler()->mfree(_Ptr)
#define CPPSMEMORYINIT() cpps::memory_allocal::instance().init()
#define CPPSMEMORYHANDLER() cpps::memory_allocal::instance().gethandler()
#define CPPSMEMORYSETHANDLER(_handler) cpps::memory_allocal::instance().sethandler(_handler)
#ifdef _DEBUG
#define CPPSALLOCATESIZE cpps::memory::memory_allocal::instance().gethandler()->size()
#endif
template<typename _Ty>
static void CPPSDELETE(_Ty* ptr)
{
	if (ptr)
	{
		ptr->~_Ty();
		cpps::memory_allocal::instance().gethandler()->mfree(ptr);
	}
}


#endif //CPPS_NEW_HEAD_