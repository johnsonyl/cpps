#ifndef CPPS_LOCK_CPPS_HEAD_
#define CPPS_LOCK_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2018/09/26 (yy/mm/dd)
//@Module		:	CPPS_LOCK
//@Description	:	�̻߳�������
//@website		:	http://cppscript.org
//==================================

#ifndef _WIN32
#include <netinet/in.h>
#include <pthread.h>
#endif

#if _HAS_CXX17
#include <shared_mutex>
#endif

namespace cpps
{

#if !_HAS_CXX17
	class  cpps_lock
	{
#ifdef _WIN32
		//��������
	private:
		CRITICAL_SECTION					thread_lock;					//�ٽ����
																			//��������
	public:
		//���캯��
		inline cpps_lock() { ::InitializeCriticalSection(&thread_lock); }
		//��������
		virtual ~cpps_lock() { ::DeleteCriticalSection(&thread_lock); }

		//���ܺ���
	public:
		//��������
		virtual inline void lock() { ::EnterCriticalSection(&thread_lock); }
		//�������� 
		virtual inline void unlock() { ::LeaveCriticalSection(&thread_lock); }
		//��������
		virtual inline void lock_shared() { lock(); }
		//�������� 
		virtual inline void unlock_shared() { unlock(); }
#else
		//��������
	private:
		pthread_mutex_t						thread_lock;					//�ٽ����

																			//��������
	public:
		//���캯��
		inline cpps_lock() { pthread_mutex_init(&thread_lock, NULL); }
		//��������
		virtual ~cpps_lock() { ::pthread_mutex_destroy(&thread_lock); }

		//���ܺ���
	public:
		//��������
		virtual inline void lock() { ::pthread_mutex_lock(&thread_lock); }
		//�������� 
		virtual inline void unlock() { ::pthread_mutex_unlock(&thread_lock); }
		//��������
		virtual inline void lock_shared() { lock(); }
		//�������� 
		virtual inline void unlock_shared() { unlock(); }
#endif
	};
#else
	class cpps_lock
	{
	private:
		std::shared_mutex					thread_lock;

	public:
		//���캯��
		inline cpps_lock() {  }
		//��������
		virtual ~cpps_lock() {  }

		//���ܺ���
	public:
		//��������
		virtual inline void lock() { thread_lock.lock(); }
		//�������� 
		virtual inline void unlock() { thread_lock.unlock(); }
	public:
		//��������
		virtual inline void lock_shared() { thread_lock.lock_shared(); }
		//�������� 
		virtual inline void unlock_shared() { thread_lock.unlock_shared(); }

	};
#endif
	struct C;
	void	cpps_reglock(C *c);

}

#endif // CPPS_LOCK_CPPS_HEAD_