/**
 * Thread synchronisation using my implementation of POSIX barrier.
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

pthread_mutex_t mutex;
pthread_cond_t cond;
int totalThread = 0;
int remainingThread = 0;

/**
 * Thread to be used to update the temperature of plaque.
 * Each thread correspond to a certain area of the plaque.
 * The thread must wait for all thread to finish updating the plaque before updating it again.
 * 
 */ 
void *updatePlaqueThreadMaBarriere(void *arg) {
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
	
	//~ printf("This is thread %d , prev %p, curr %p , \n limit %d, leftOffset %d, topOffset %d \n\n", id, cell1, cell2,
				//~ limit, leftOffset, topOffset);
	
	int nbCellSection = limit*limit;
	
	for(iter=0;iter<10000;iter++) {
		//printf("Iteration %d", iter);
		
		//copyPlaque(currCell, prevCell, nbLigne*nbLigne);
		
		for(i=0;i<nbCellSection;i++) {
			//calculer le numero de cellule à traiter
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
			//printf("Treating cell %d \n", numCell);
			diffusionHorizontaleT(cell1, cell2, nbLigne, numCell);
		}
		
		pthread_mutex_lock(&mutex);
		remainingThread--; 
		if (remainingThread == 0) { 
			remainingThread = totalThread;
			pthread_cond_broadcast(&cond); 
		}
		else {
		    pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);

			
		for(i=0;i<nbCellSection;i++) {
		
			int numCell = ((i/limit)*nbLigne + topOffset*nbLigne ) + ((i%limit) +  leftOffset);
			diffusionVerticaleT(cell2, cell1, nbLigne, numCell);
		}
		
		
		pthread_mutex_lock(&mutex);
		remainingThread--; 
		if (remainingThread == 0) { 
			remainingThread = totalThread;
			pthread_cond_broadcast(&cond); 
		}
		else {
		    pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		
	}
}

void executeMaBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS, int argT) {
	
	if (pthread_mutex_init(&mutex, NULL) != 0)
    {
		perror("\n Error init mutex \n");
    }
    
    if (pthread_cond_init(&cond, NULL) != 0)
    {
		perror("\n Error init cond \n");
    }    
    
	
	//printf("Calling test barriere %p %p %d \n", oldCell, newCell, nbLine);
	int division = 1<<argT; // Si on divise les cellules par 4, donc 2 colonne et 2 lignes, donc cette valeur vaut 2.
	int nbThread = division*division;
	remainingThread = nbThread;
	totalThread = nbThread;
	int ret;
	thread_data_t thr_data[nbThread];
	//printf("Testing barrier with %d thread \n", division*division);
	pthread_t threads[nbThread];

	
	int limit = nbLine/division;
	int i;
	
	for(i=0;i<nbThread;i++) {
		//~ printf("Calculating offset using division: %d limit: %d, i: %d\n", division, limit, i);
		thr_data[i].tid = i;
		thr_data[i].limit = limit;
		thr_data[i].leftOffset = i%division*limit;
		thr_data[i].topOffset = i/division*limit;
		
		thr_data[i].cell1 = oldCell;
		thr_data[i].cell2 = newCell;
		
		thr_data[i].nbLigne = nbLine;
		
		
		//~ printf("Creating thread  id: %d leftOffset: %d topOffset: %d %p\n\n", thr_data[i].tid, thr_data[i].leftOffset, thr_data[i].topOffset, &threads[i]);
		
		ret = pthread_create(&threads[i], NULL, &updatePlaqueThreadMaBarriere, &thr_data[i]);
		
		if(ret!=0) {
			printf("Unable to create thread%d", i);
		}
	}
	//~ 
	for(i=0; i<nbThread; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	//printf("Finished testing barrier\n");
			
}
