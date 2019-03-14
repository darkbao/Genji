/*
* @Author:             darkbao
* @Date:               2019-03-14 15:14:10
* @Last Modified by:   darkbao
* @Email:              freecare_j@163.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <exception>
#include "LFQueue.h"

struct data
{
	uint32_t index;
};

void* producer(void* arg)
{
	printf("producer thread begin...\n");
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	for (uint32_t i = 0; ; ++i) {
		data newData = {i};
		uint64_t len = pQueue->putData(&newData, sizeof(data));
		// if (len == sizeof(data)) {
		// 	printf("-->put newData[%u]\n", newData.index);
		// }
	}
	return NULL;
}

void* consumer(void* arg)
{
	printf("consumer thread begin...\n");
	mj::LFQueue* pQueue = (mj::LFQueue*)arg;
	for ( ; ; ) {
		data newData;
		uint64_t len = pQueue->getData(&newData, sizeof(data));
		// if (len == sizeof(data)) {
		// 	printf("<--get newData[%u]\n", newData.index);
		// }
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