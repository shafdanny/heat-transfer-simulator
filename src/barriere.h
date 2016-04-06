#ifndef _BARRIERE_H_
#define _BARRIERE_H_

#include <stdbool.h>

typedef struct _thread_data_t {
	float *cell1;
	float *cell2;
	int tid;
	int limit;
	int leftOffset;
	int topOffset;
	
} thread_data_t;



void executeBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS, int nbThread);
void executeMaBarriere(float *oldCell, float *newCell, int nbLine, int nbCellule, int argS, int argT);
void *updatePlaqueThread(void *arg);

#endif

