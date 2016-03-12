/**
 * Thread synchronisation using POSIX barrier.
 * 
 * author : Shafiq Daniel
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "barriere.h"
#include "plaque.h"

pthread_barrier_t barrier;
float *prevCell;
float *currCell;

typedef struct _thread_data_t {
	int tid;
	int *sharedNumber;
	
} thread_data_t;

/**
 * Thread to be used to update the temperature of plaque.
 * Each thread correspond to a certain area of the plaque.
 * The thread must wait for all thread to finish updating the plaque before updating it again.
 * 
 */ 
void *updatePlaqueThread(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;
	
	int i;
	while(true) {
		int s;
		
		int *pointer = data->sharedNumber;

		printf("This is thread %d , prev %p, curr %p \n", data->tid, prevCell, currCell);
		*pointer = *pointer + 1;
	
		s = pthread_barrier_wait(&barrier);
		if(s==0) {
			printf("Looks like all thread is finished \n");
		}
		else if(s==PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Looks like all thread is finished \n");
			float *temp = prevCell;
			prevCell = currCell;
			currCell = temp;
		}
		
		sleep(1);
	}
}

void testBarriere(float *oldCell, float *newCell) {
	int number = 3;
	int *share;
	int init = 1;
	share = &init;
	int ret;
	thread_data_t thr_data[number];
	printf("Testing barrier %p\n", share);
	pthread_t threads[number];
	pthread_attr_t attr;
	pthread_barrier_init(&barrier,NULL,number);	
	prevCell = oldCell;
	currCell = newCell;
	
	int i;
	
	for(i=0;i<number;i++) {
		thr_data[i].tid = i;
		thr_data[i].sharedNumber = share;


		ret = pthread_create(&threads[i], NULL, &updatePlaqueThread, &thr_data[i]);
			
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
