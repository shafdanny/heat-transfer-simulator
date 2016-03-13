
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

int H = 2;

int TEMP_FROID = 0;
static int nbCell = 0;
static int nbLigne = 0;
static int s = 0;

typedef struct _thread_data_t {
	int tid;
	int *sharedNumber;
	int limit;
	int leftOffset;
	int topOffset;
	
} thread_data_t;

void diffusionHorizontaleT(int numCell) {
	
		//printf("Checking cell %d \n", numCell);
		float prevLeftCell = 0.0;
		float prevRightCell = 0.0;

		if(!isZoneInterne(numCell,s)) {
			prevLeftCell = (numCell%nbLigne == 0) ? TEMP_FROID : prevCell[numCell-1];
			prevRightCell = (numCell%nbLigne == nbLigne-1) ? TEMP_FROID : prevCell[numCell+1];

			currCell[numCell] = (prevRightCell + (H-2)*(prevCell[numCell]) + prevLeftCell)/H;
		}
	
}

void diffusionVerticaleT(int numCell) {

		float prevTopCell = 0.0;
		float prevBottomCell = 0.0;

		//printf("Checking cell %d \n", numCell);
		if(!isZoneInterne(numCell,s)) {
			prevTopCell = (numCell/nbLigne == 0) ? TEMP_FROID : prevCell[numCell-nbLigne];
			prevBottomCell = (numCell/nbLigne == nbLigne-1) ? TEMP_FROID : prevCell[numCell+nbLigne];

			currCell[numCell] = (prevTopCell + (H-2)*(prevCell[numCell]) + prevBottomCell)/H;
		}
}


/**
 * Thread to be used to update the temperature of plaque.
 * Each thread correspond to a certain area of the plaque.
 * The thread must wait for all thread to finish updating the plaque before updating it again.
 * 
 */ 
void *updatePlaqueThread(void *arg) {
	int nbIteration = 10000;
	
	thread_data_t *data = (thread_data_t *)arg;
	
	int limit = data->limit;
	int leftOffset = data->leftOffset;
	int topOffset = data->topOffset;
		
	int i, iter;
	
	//printf("This is thread %d , prev %p, curr %p , \n limit %d, leftOffset %d, topOffset %d \n", data->tid, prevCell, currCell,
	//			limit, leftOffset, topOffset);
	
	int nbCellSection = limit*limit;
	
	for(iter=0;iter<10000;iter++) {
		//printf("Iteration %d", iter);
		
		//copyPlaque(currCell, prevCell, nbLigne*nbLigne);
		
		for(i=0;i<nbCellSection;i++) {
			//calculer le numero de cellule à traiter
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
			//printf("Treating cell %d \n", numCell);

			diffusionHorizontaleT(numCell);
		}
		
		int bar = pthread_barrier_wait(&barrier);
		if(bar==0) {
			//printf("Looks like all thread is finished \n");
		}
		else if(bar==PTHREAD_BARRIER_SERIAL_THREAD) {
			//printf("Looks like all thread is finished \n");
				////float *temp = prevCell;
				////prevCell = currCell;
				////currCell = temp;
			copyPlaque(currCell, prevCell, nbLigne*nbLigne);

		}
		
		//sleep(1);
		
			
		for(i=0;i<nbCellSection;i++) {
		
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
	
			diffusionVerticaleT(numCell);
		}
		
		bar = pthread_barrier_wait(&barrier);
			if(bar==0) {
				//printf("Looks like all thread is finished \n");
			}
			else if(bar==PTHREAD_BARRIER_SERIAL_THREAD) {
				//printf("Looks like all thread is finished \n");
				//float *temp = prevCell;
				//prevCell = currCell;
				//currCell = temp;
				copyPlaque(currCell, prevCell, nbLigne*nbLigne);

			}
	}
}

void testBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS) {
	//printf("Calling test barriere %p %p %d", oldCell, newCell, nbLine);
	int number = 4;
	int *share;
	int init = 1;
	share = &init;
	int ret;
	thread_data_t thr_data[number];
	//printf("Testing barrier %p\n", share);
	pthread_t threads[number];
	pthread_attr_t attr;
	pthread_barrier_init(&barrier,NULL,number);	
	
	prevCell = oldCell;
	currCell = newCell;
	nbCell = nbCellule;
	nbLigne = nbLine;
	s = argS;
	int division = 2; // Si on divise les cellules par 4, donc 2 colonne et 2 lignes, donc cette valeur vaut 2.
	int limit = nbLigne/2;
	int i;
	
	for(i=0;i<number;i++) {
		thr_data[i].tid = i;
		thr_data[i].sharedNumber = share;
		thr_data[i].limit = limit;
		thr_data[i].leftOffset = i%division*limit;
		thr_data[i].topOffset = i/division*limit;
		
		//printf("Creating thread %d %d \n", thr_data[i].leftOffset, thr_data[i].topOffset);

		ret = pthread_create(&threads[i], NULL, &updatePlaqueThread, &thr_data[i]);
			
		if(ret!=0) {
			printf("Unable to create thread%d", i);
		}
	}
	
	for(i=0; i<number; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	//printf("Finished testing barrier\n");
			
}
