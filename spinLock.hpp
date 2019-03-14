/*
* @Author:             darkbao
* @Date:               2019-03-14 09:31:40
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_SPINLOCK_HPP__
#define __MJ_SPINLOCK_HPP__
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
}

#endif // __MJ_SPINLOCK_HPP__