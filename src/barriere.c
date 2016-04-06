
/**
 * Thread synchronisation using POSIX barrier.
 * 
 * author : Shafiq Daniel
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

#include "barriere.h"
#include "plaque.h"

pthread_barrier_t barrier;


int H = 2;

int TEMP_FROID = 0;
int nbCell = 0;
//int nbLigne = 0;
int s = 0;


void diffusionHorizontaleT(float* cell1, float* cell2, int nbLigne, int numCell) {
	
		//printf("Checking cell %d \n", numCell);
		float prevLeftCell = 0.0;
		float prevRightCell = 0.0;

		if(!isZoneInterne(numCell,s)) {
			//printf("%d \n", nbLigne);
			//printf("%d\n" , numCell%nbLigne);
			prevLeftCell = (numCell%nbLigne == 0) ? TEMP_FROID : cell1[numCell-1];
			prevRightCell = (numCell%nbLigne == nbLigne-1) ? TEMP_FROID : cell1[numCell+1];

			cell2[numCell] = (prevRightCell + (H-2)*(cell1[numCell]) + prevLeftCell)/H;
		}
	
}

void diffusionVerticaleT(float* cell1, float* cell2, int nbLigne, int numCell) {

		float prevTopCell = 0.0;
		float prevBottomCell = 0.0;

		//printf("Checking cell %d \n", numCell);
		if(!isZoneInterne(numCell,s)) {
			prevTopCell = (numCell/nbLigne == 0) ? TEMP_FROID : cell1[numCell-nbLigne];
			prevBottomCell = (numCell/nbLigne == nbLigne-1) ? TEMP_FROID : cell1[numCell+nbLigne];

			cell2[numCell] = (prevTopCell + (H-2)*(cell1[numCell]) + prevBottomCell)/H;
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
	int id = data->tid;
	float *cell1 = data->cell1;
	float *cell2 = data->cell2;
	
	int nbLigne = data->nbLigne;
		
	int i, iter;
	
	//~ printf("This is thread %d , prev %p, curr %p , \n limit %d, leftOffset %d, topOffset %d \n\n", id, prevCell, currCell,
				//~ limit, leftOffset, topOffset);
	
	int nbCellSection = limit*limit;
	
	for(iter=0;iter<10000;iter++) {
		//printf("Iteration %d", iter);
		
		//copyPlaque(currCell, prevCell, nbLigne*nbLigne);
		
		for(i=0;i<nbCellSection;i++) {
			//calculer le numero de cellule à traiter
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
			//printf("Treating cell %d \n", numCell);
			//if(numCell < 0 || numCell > nbCellSection) printf("OHOHOHOHOHO");
			diffusionHorizontaleT(cell1, cell2, nbLigne, numCell);
		}
		
		int bar = pthread_barrier_wait(&barrier);
		if(bar==0) {
			//printf("Looks like all thread is finished \n");
		}
		else if(bar==PTHREAD_BARRIER_SERIAL_THREAD) {
			//printf("Looks like all thread is finished \n");
				////float *temp = prevCell;
				//prevCell = currCell;
				////currCell = temp;
			//copyPlaque(currCell, prevCell, nbLigne*nbLigne);
			//swapCell(&currCell, &prevCell);

		}
		
		//sleep(1);
		
			
		for(i=0;i<nbCellSection;i++) {
		
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
	
			diffusionVerticaleT(cell2, cell1, nbLigne, numCell);
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
				//copyPlaque(currCell, prevCell, nbLigne*nbLigne);
				//swapCell(&currCell, &prevCell);
				//display(currCell, nbLigne);
			}
	}
}

void executeBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS, int argT) {
	//printf("Calling test barriere %p %p %d", oldCell, newCell, nbLine);
	int division = 1<<argT; // Si on divise les cellules par 4, donc 2 colonne et 2 lignes, donc cette valeur vaut 2.
	int nbThread = division*division;
	
	int ret;
	thread_data_t thr_data[nbThread];
	//printf("Testing barrier with %d thread \n", division*division);
	pthread_t threads[nbThread];
	pthread_attr_t attr;
	pthread_barrier_init(&barrier,NULL,nbThread);	
	
	int limit = nbLine/division;
	int i;
	
	for(i=0;i<nbThread;i++) {
		thr_data[i].tid = i;
		thr_data[i].limit = limit;
		thr_data[i].leftOffset = i%division*limit;
		thr_data[i].topOffset = i/division*limit;
		
		thr_data[i].cell1 = oldCell;
		thr_data[i].cell2 = newCell;
		
		thr_data[i].nbLigne = nbLine;
		
		//printf("Creating thread %d %d \n", thr_data[i].leftOffset, thr_data[i].topOffset);

		ret = pthread_create(&threads[i], NULL, &updatePlaqueThread, &thr_data[i]);
			
		if(ret!=0) {
			printf("Unable to create thread%d", i);
		}
	}
	
	for(i=0; i<nbThread; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	pthread_barrier_destroy(&barrier);
	
	//printf("Finished testing barrier\n");		
}
