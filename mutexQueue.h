/*
* @Author:             darkbao
* @Date:               2019-03-15 12:26:10
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_MUTEXQUEUE_H__
#define __MJ_MUTEXQUEUE_H__
#include <stdint.h>
#include "lock.hpp"

namespace mj
{
	class mutexQueue
	{
	public:
		mutexQueue(uint64_t size);
		~mutexQueue();
		uint64_t putData(const char* buffer, uint64_t length);
		uint64_t getData(char* buffer, uint64_t length);
		inline uint64_t size() const { return (m_in - m_out); }
	protected:
		inline bool is_power_of_2(uint64_t n) { return (n != 0 && ((n & (n - 1)) == 0)); }
	private:
		uint8_t*		m_pBuffer;
		uint64_t		m_size;
		uint64_t		m_in;
		uint64_t		m_out;
		mutexLock     	m_lock;
	};
}

#endif //__MJ_MUTEXQUEUE_H__