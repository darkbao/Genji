/*
* @Author:             darkbao
* @Date:               2019-03-14 09:31:40
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_LOCK_HPP__
#define __MJ_LOCK_HPP__
#include <sched.h>
#include <sys/sysinfo.h>
#include <exception>
#include <stdint.h>
#include "define.h"

namespace mj
{
    class spinLock
    {
    public:
        spinLock() : m_lock(0), m_cpuNum(0)
        {
            m_cpuNum = get_nprocs();
        }
        ~spinLock()
        {
        }
        inline void lock(uint32_t spin = 256)
        {
            for ( ; ; ) {
                if (m_lock == 0 && __sync_bool_compare_and_swap(&m_lock, 0, 1)) {
                    return;
                }
                if (m_cpuNum > 1) {
                    for (uint32_t n = 1; n < spin; n <<= 1) {
                        for (uint32_t i = 0; i < n; ++i) {
                            cpu_pause();
                        }
                        if (m_lock == 0 && __sync_bool_compare_and_swap(&m_lock, 0, 1)) {
                            return;
                        }
                    }
                }
                sched_yield();
            }
        }
        inline void unlock()
        {
            m_lock = 0;
        }
    private:
        volatile uint32_t m_lock;
        uint32_t m_cpuNum;
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