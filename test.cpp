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

static const uint32_t MAX_DATA_NUM = 1024*1024;

struct data
{
	uint32_t index;
};

void* producer(void* arg)
{
	printf("producer thread running...\n");
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	struct  timeval start;
    struct  timeval end;
    gettimeofday(&start, NULL);
	for (uint32_t i = 0; i < MAX_DATA_NUM; ) {
		data newData = {i};
		uint64_t len = pQueue->putData((char*)&newData, sizeof(data));
		if (len == sizeof(data)) {
			++i;
		}
	}
	gettimeofday(&end, NULL);
	unsigned long diff = 1000000 * (end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
    printf("put %u data total in [%ld]us\n", MAX_DATA_NUM, diff);
	return NULL;
}

void* consumer(void* arg)
{
	printf("consumer thread running...\n");
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	for (uint32_t i = 0; i < MAX_DATA_NUM; ) {
		data newData;
		uint64_t len = pQueue->getData((char*)&newData, sizeof(data));
		if (len == sizeof(data)) {
			++i;
		}
	}
	return NULL;
}


int main(int argc, char const *argv[])
{
	uint64_t qsize = 1024;
	if (argc == 2) {
		qsize = strtoul(argv[1], NULL, 0);
	}
	mj::LFQueue* pQueue = NULL;
	try {
		pQueue = new mj::LFQueue(qsize);
	}
	catch(const std::exception& e){
		printf("%s\n", e.what());
		return -1;
	}
	printf("test start...\n");
	pthread_t thid1, thid2;
	pthread_create(&thid1, NULL, producer, (void*)pQueue);
    pthread_create(&thid2, NULL, consumer, (void*)pQueue);
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);
    printf("test finish\n");
	return 0;
}