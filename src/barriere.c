#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "barriere.h"

pthread_barrier_t barrier;

typedef struct _thread_data_t {
	int tid;
} thread_data_t;

void *thread(void *arg) {
	int i;
	while(true) {
		int s;
		thread_data_t *data = (thread_data_t *)arg;
		printf("This is thread %d \n", data->tid);
		s = pthread_barrier_wait(&barrier);
		if(s==0) {
			printf("Looks like all thread is finished \n");
		}
		else if(s==PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Looks like all thread is finished \n");
		}
		
		sleep(1);
	}
}

void testBarriere() {
	int number = 3;
	int ret;
	thread_data_t thr_data[number];
	printf("Testing barrier\n");
	pthread_t threads[number];
	pthread_attr_t attr;
	pthread_barrier_init(&barrier,NULL,3);	
	
	
	int i;
	
	for(i=0;i<number;i++) {
		thr_data[i].tid = i;
		ret = pthread_create(&threads[i], NULL, &thread, &thr_data[i]);
			
		if(ret!=0) {
			printf("Unable to create thread%d", i);
		}
	}
		
	for(i=0; i<number; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	printf("Finished testing barrier\n");
			
}
