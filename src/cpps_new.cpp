#include "cpps/cpps.h"

#ifdef USE_NEDMALLOC
#include "nedmalloc.h"
#endif

cpps::memory_allocal::memory_allocal()
{
	handler = NULL;
	_real = false;
	mainthreadid = 0;
}

cpps::memory_allocal::~memory_allocal()
{
	if (_real) {
#ifdef _DEBUG
		handler->dump();
#endif
		delete handler;
		handler = NULL;
	}
}

void cpps::memory_allocal::init()
{
	if (handler == NULL) {
		_real = true;
		mainthreadid = cpps_this_thread_get_id();
		handler = new memory_allocal_handler();
	}
}

bool cpps::memory_allocal::global()
{
	return !(_real && mainthreadid == cpps_this_thread_get_id());
}

cpps::memory_allocal_handler* cpps::memory_allocal::gethandler()
{
	return handler;
}

void cpps::memory_allocal::sethandler(memory_allocal_handler* h)
{
	handler = h;
}

cpps::memory_allocal& cpps::memory_allocal::instance()
{
	static memory_allocal ins;
	return ins;
}

void* cpps::memory_allocal_handler::mmalloc(size_t __size, const char* file, unsigned int _line)
{
#ifdef USE_NEDMALLOC
	void* ret = nedalloc::nedmalloc(__size);
#else
#ifdef _WIN32
	//__size = (size_t)ceil(__size / 1024.0) * 1024;
	//void* ret =  (PBYTE)VirtualAlloc(NULL, __size, MEM_COMMIT, PAGE_READWRITE);
	__size++;
	char* ret = memory_allocal::instance().global() ? (char*)VirtualAlloc(NULL, __size, MEM_COMMIT, PAGE_READWRITE) : (char*)malloc( __size) ;
	ret[0] = memory_allocal::instance().global() ? 1 : 0;
	ret = ret + 1;
	//if(memory_allocal::instance().global())
		//printf("alloc %s memory...\n", memory_allocal::instance().global() ? "global" : "local");
#else
	void* ret = malloc(__size);
#endif
#endif
#ifdef _DEBUG
#ifdef _WIN32
	if (ret == NULL) {
		system("pause");
	}
#endif

	_lock.lock();
	_size += __size;
	memorylist.insert(memory_info_list::value_type(ret, new memory_info(__size, file, _line)));
	_lock.unlock();
#endif
	return ret;
}

void cpps::memory_allocal_handler::mfree(void* m)
{
#ifdef _DEBUG
	_lock.lock();
	auto it = memorylist.find(m);
	if (it != memorylist.end()) {
		auto info = it->second;
		_size -= info->size;
		delete info;
		memorylist.erase(it);
	}
	_lock.unlock();
	if (m == NULL) {
		system("pause");
	}
#endif
#ifdef USE_NEDMALLOC
	nedalloc::nedfree(m);
#else
#ifdef _WIN32
	if (m) {
		//printf("Thread ID:%d release Address: %I64d \r\n", GetCurrentThreadId(), (size_t)m);
		char* p = (char*)m;
		p = p - 1;
		p[0] == 1 ? VirtualFree(p, 0, MEM_RELEASE) : free(p);
	}
#else
	if(m) free(m);
#endif
#endif
}



#ifdef _DEBUG
size_t cpps::memory_allocal_handler::size()
{
	return _size;
}
void cpps::memory_allocal_handler::dump()
{
	_lock.lock();
	if (!memorylist.empty()) {
		char outFileName[256];

		//获取系统时间

		sprintf(outFileName, "%s_%d.log", "dump_no_delete_memory.txt", rand());
		FILE* pFile = fopen(outFileName, "wb+");
		std::string out;
		for (auto it : memorylist) {
			auto info = it.second;
			char buffer[10240];
			sprintf(buffer, "file :%s line:%d \r\n", info->file, info->line);
			fwrite(buffer, strlen(buffer), 1, pFile);
		}

		fclose(pFile);
	}

	

	_lock.unlock();
}
#endif