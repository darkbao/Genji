/*
* @Author:             darkbao
* @Date:               2019-03-13 18:33:40
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#include <algorithm>
#include <stdexcept>
#include <new>
#include <string.h>
#include "define.h"
#include "LFQueue.h"

namespace mj
{
	LFQueue::LFQueue(uint64_t size) : m_pBuffer(NULL), m_size(0), m_in(0), m_out(0)    
	{
		if (!is_power_of_2(size)) {
			throw std::invalid_argument("queue size must be pow of 2");
		}
		m_pBuffer = new uint8_t[size];
		if (NULL == m_pBuffer) {
			throw std::bad_alloc();
		}
		m_size = size;
	}

	LFQueue::~LFQueue()
	{
		if (m_pBuffer) {
			delete m_pBuffer;
		}
	}

	uint64_t LFQueue::putData(const void* buffer, uint64_t length)
	{
		uint64_t len = 0;
		//m_lock.lock();
		len = putData_locked(buffer, length);
		//m_lock.unlock();
		return len;
	}

	uint64_t LFQueue::getData(void* buffer, uint64_t length)
	{
		uint64_t len = 0;
		//m_lock.lock();
		len = getData_locked(buffer, length);
		//m_lock.unlock();
		return len;
	}

	uint64_t LFQueue::putData_locked(const void* buffer, uint64_t len)
	{
		uint64_t l = 0;
    	len = std::min(len, m_size - m_in + m_out);
		
		smp_mb();
		l = std::min(len, m_size - (m_in & (m_size - 1)));
		memcpy(m_pBuffer + (m_in & (m_size - 1)), buffer, l);
		memcpy(m_pBuffer, buffer + l, len - l);
		
		smp_wmb();
		m_in += len;
		return len;
	}

	uint64_t LFQueue::getData_locked(void* buffer, uint64_t len)
	{
 		uint64_t l = 0;
 		len = std::min(len, m_in - m_out);
 		
 		smp_rmb();
 		l = std::min(len, m_size - (m_out & (m_size - 1)));
 		memcpy(buffer, m_pBuffer + (m_out & (m_size - 1)), l);
 		memcpy(buffer + l, m_pBuffer, len - l);
 		
 		smp_mb();
 		m_out += len;
 		return len;
	}

}