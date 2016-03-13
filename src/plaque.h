#ifndef _PLAQUE_H_
#define _PLAQUE_H_

#include <stdbool.h>


bool isZoneInterne(int i, int s);
void copyPlaque(float *plOrigin, float *plDest, int nbCell);
void display(float *cell, int nbLigne);
void diffusionHorizontale();
void diffusionVerticale();
void updatePlaque();
bool isZoneInterne(int i, int s);
void plaqueInit(int scenario, int argS, int nbIter, int aflag, int mflag, int Mflag, int nbThread);
void executeScenario(int numScenario, int nbRepetition);
void executeIteration();
double calculMoyenne(double array[], int size);

#endif
