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
void plaqueInit(int argS, int nbIter, int aflag);
void executeScenario(int numScenario, int nbRepetition);

#endif