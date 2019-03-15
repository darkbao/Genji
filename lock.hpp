/*
* @Author:             darkbao
* @Date:               2019-03-14 09:31:40
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_LOCK_HPP__
#define __MJ_LOCK_HPP__
#include <exception>
#include <stdint.h>
#include <sched.h>

namespace mj
{
	class spinLock
	{
	public:
		spinLock() : m_lock(0) { }
		~spinLock() { }
		void lock()
		{
			while (!__sync_bool_compare_and_swap(&m_lock, 0, 1)) {
        		sched_yield();
    		}
		}
		void unlock()
		{
			m_lock = 0;
		}
	private:
		volatile uint32_t m_lock;	
	};


	class mutexLock
	{
	public:
		mutexLock()
		{
		    if (pthread_mutex_init(&m_mutex, NULL) != 0) {
		        throw std::exception();
		    }
		}
		~mutexLock()
		{
		    pthread_mutex_destroy(&m_mutex);
		}
		bool lock()
		{
		    return (pthread_mutex_lock(&m_mutex) == 0);
		}
		bool unlock()
		{
		    return (pthread_mutex_unlock(&m_mutex) == 0);
		}
	private:
		pthread_mutex_t m_mutex;
	};
}

#endif // __MJ_LOCK_HPP__