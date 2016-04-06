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

pthread_barrier_t barrier;
float *prevCell;
float *currCell;

extern int nbCell;

extern int nbLigne;
extern int s;

void executeMaBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS, int argT) {
	//printf("Calling test barriere %p %p %d \n", oldCell, newCell, nbLine);
	int division = 1<<argT; // Si on divise les cellules par 4, donc 2 colonne et 2 lignes, donc cette valeur vaut 2.
	int nbThread = division*division;
	
	/** FOR DEBUG PURPOSE **/
	int *share;
	int init = 1;
	share = &init;
	/** FIN DEBUG **/
	
	int ret;
	thread_data_t thr_data[nbThread];
	//printf("Testing barrier with %d thread \n", division*division);
	pthread_t threads[nbThread];
	pthread_attr_t attr;
	int retInitBarrier = pthread_barrier_init(&barrier,NULL,nbThread);	
	
	if(retInitBarrier == 0) {
		//printf("Barrier init successful\n"); 
	} else {
		err("Barrier init fail\n");
	}
	
	prevCell = oldCell;
	currCell = newCell;
	nbCell = nbCellule;
	nbLigne = nbLine;
	s = argS;
	
	int limit = nbLigne/division;
	int i;
	
	for(i=0;i<nbThread;i++) {
		//~ printf("Calculating offset using division: %d limit: %d, i: %d\n", division, limit, i);
		thr_data[i].tid = i;
		thr_data[i].limit = limit;
		thr_data[i].leftOffset = i%division*limit;
		thr_data[i].topOffset = i/division*limit;
		
		//~ printf("Creating thread  id: %d leftOffset: %d topOffset: %d %p\n\n", thr_data[i].tid, thr_data[i].leftOffset, thr_data[i].topOffset, &threads[i]);
		
		ret = pthread_create(&threads[i], NULL, &updatePlaqueThread, &thr_data[i]);
		
		if(ret!=0) {
			//~ printf("Unable to create thread%d", i);
		}
	}
	
	for(i=0; i<nbThread; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	//printf("Finished testing barrier\n");
			
}
