/*
* @Author:             darkbao
* @Date:               2019-03-14 15:14:10
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <exception>
#include "LFQueue.h"
#include "mutexQueue.h"

struct data
{
	uint64_t index;
	//uint64_t remain;
};

static const uint64_t MAX_DATA_NUM = 16ull*(1024*1024*1024)/sizeof(data);
static const int32_t PRODUCER_CPU_ID = 0;
static const int32_t CONSUMER_CPU_ID = 1;

int setAffinity(pthread_t thID, int cpuID)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
	CPU_SET(cpuID, &cpuset);
    return pthread_setaffinity_np(thID, sizeof(cpu_set_t), &cpuset);
}

void* producerLFQ(void* arg)
{
	int ret = setAffinity(pthread_self(), PRODUCER_CPU_ID);
	if (ret != 0) {
		printf("error: setAffinity failed in producerLFQ, ret[%d]\n", ret);
		exit(-1);
	}
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
	for (uint64_t i = 0; i < MAX_DATA_NUM; ) {
		data newData = {i};
		uint64_t len = pQueue->putData((char*)&newData, sizeof(data));
		if (len > 0) {
			++i;
		}
	}
	gettimeofday(&end, NULL);
	unsigned long diff = 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
    printf("LFQueue: dataSize[%lu], put %lu data total in [%ld]ms\n",
    		sizeof(data), MAX_DATA_NUM, diff);
	return NULL;
}

void* consumerLFQ(void* arg)
{
	int ret = setAffinity(pthread_self(), CONSUMER_CPU_ID);
	if (ret != 0) {
		printf("error: setAffinity failed in consumerLFQ, ret[%d]\n", ret);
		exit(-1);
	}
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	for (uint32_t i = 0; i < MAX_DATA_NUM; ) {
		data newData;
		uint64_t len = pQueue->getData((char*)&newData, sizeof(data));
		if (len > 0) {
			++i;
		}
	}
	return NULL;
}

void* producerMtxQ(void* arg)
{
	int ret = setAffinity(pthread_self(), PRODUCER_CPU_ID);
	if (ret != 0) {
		printf("error: setAffinity failed in producerMtxQ, ret[%d]\n", ret);
		exit(-1);
	}
	mj::mutexQueue* pMtxQueue = (mj::mutexQueue*)arg;
	struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
	for (uint64_t i = 0; i < MAX_DATA_NUM; ) {
		data newData = {i};
		uint64_t len = pMtxQueue->putData((char*)&newData, sizeof(data));
		if (len > 0) {
			++i;
		}
	}
	gettimeofday(&end, NULL);
	unsigned long diff = 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
    printf("mutexQueue: dataSize[%lu], put %lu data total in [%ld]ms\n",
    		sizeof(data), MAX_DATA_NUM, diff);
	return NULL;
}

void* consumerMtxQ(void* arg)
{
	int ret = setAffinity(pthread_self(), CONSUMER_CPU_ID);
	if (ret != 0) {
		printf("error: setAffinity failed in consumerMtxQ, ret[%d]\n", ret);
		exit(-1);
	}
	mj::mutexQueue* pMtxQueue = (mj::mutexQueue*)arg;
	for (uint64_t i = 0; i < MAX_DATA_NUM; ) {
		data newData;
		uint64_t len = pMtxQueue->getData((char*)&newData, sizeof(data));
		if (len > 0) {
			++i;
		}
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	uint64_t qsize = 1024*1024;
	mj::LFQueue* pQueue = NULL;
	mj::mutexQueue* pMtxQueue = NULL;
	try {
		pQueue = new mj::LFQueue(qsize);
		pMtxQueue = new mj::mutexQueue(qsize);
	}
	catch(const std::exception& e){
		printf("%s\n", e.what());
		return -1;
	}
	printf("*************test start******************\n");

	printf("-->testing LFQueue...\n");
	pthread_t thid1, thid2;
	pthread_create(&thid1, NULL, producerLFQ, (void*)pQueue);
    pthread_create(&thid2, NULL, consumerLFQ, (void*)pQueue);
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

	printf("-->testing mutexQueue...\n");
    pthread_create(&thid1, NULL, producerMtxQ, (void*)pMtxQueue);
    pthread_create(&thid2, NULL, consumerMtxQ, (void*)pMtxQueue);
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

    printf("*************test finish*****************\n");
	return 0;
}