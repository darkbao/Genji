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
#include "mutexQueue.h"

namespace mj
{
	mutexQueue::mutexQueue(uint64_t size) : m_pBuffer(NULL), m_size(0), m_in(0), m_out(0)    
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

	mutexQueue::~mutexQueue()
	{
		if (m_pBuffer) {
			delete m_pBuffer;
		}
	}

	uint64_t mutexQueue::putData(const char* buffer, uint64_t len)
	{
		if (!m_lock.lock()) {
			return 0;
		}
    	len = std::min(len, m_size - m_in + m_out);
		uint64_t l = std::min(len, m_size - (m_in & (m_size - 1)));
		memcpy(m_pBuffer + (m_in & (m_size - 1)), buffer, l);
		memcpy(m_pBuffer, buffer + l, len - l);
		m_in += len;
		if (!m_lock.unlock()) {
			exit(-1);
		}
		return len;
	}

	uint64_t mutexQueue::getData(char* buffer, uint64_t len)
	{
		if (!m_lock.lock()) {
			return 0;
		}
		len = std::min(len, m_in - m_out);
 		uint64_t l = std::min(len, m_size - (m_out & (m_size - 1)));
 		memcpy(buffer, m_pBuffer + (m_out & (m_size - 1)), l);
 		memcpy(buffer + l, m_pBuffer, len - l);
 		m_out += len;
		if (!m_lock.unlock()) {
			exit(-1);
		}
		return len;
	}
}