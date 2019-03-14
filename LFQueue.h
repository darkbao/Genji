/*
* @Author:             darkbao
* @Date:               2019-03-14 09:32:10
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#ifndef __MJ_LFQUEUE_H__
#define __MJ_LFQUEUE_H__
#include <stdint.h>
#include "spinLock.hpp"

namespace mj
{
	class LFQueue
	{
	public:
		LFQueue(uint64_t size);
		~LFQueue();
		uint64_t putData(const void* buffer, uint64_t length);
		uint64_t getData(void* buffer, uint64_t length);
		inline uint64_t size() const { return (m_in - m_out); }
	protected:
		inline bool is_power_of_2(uint64_t n) { return (n != 0 && ((n & (n - 1)) == 0)); }
		uint64_t putData_locked(const void* buffer, uint64_t length);
		uint64_t getData_locked(void* buffer, uint64_t length);
	private:
		uint8_t*		m_pBuffer;  /* the buffer holding the data */
		uint64_t		m_size;    	/* the size of the allocated buffer */
		uint64_t		m_in;    	/* data is added at offset (in % size) */
		uint64_t		m_out;    	/* data is extracted from off. (out % size) */
		spinLock     	m_lock;     /* protects concurrent modifications */
	};
}

#endif //__MJ_LFQUEUE_H__