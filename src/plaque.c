/**
 * Representation de plaque.
 * 
 * author : Shafiq Daniel & Nicolas Meurgues
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/resource.h>
#include "plaque.h"

#define TEMP_CHAUD 255
#define TEMP_FROID 0

#define H 6

float *cellPrev = NULL;
float *cellCurr = NULL;

int s = 0;
int nbLigne = 0;
int nbCell = 0;
int nbIteration = 0;
int mFlag;
int MFlag;

void copyPlaque(float *plOrigin, float *plDest, int nbCell) {
	int i;
	for(i=0; i<nbCell; i++){
		plDest[i] = plOrigin[i];
	}
}

/**
 * Afficher les cellules. Seulement le quart superieur gauche du tableau
 * doit etre affiche, et pour les indices modulo 2**s
 */
void display(float *cell, int nbLigne) {
	int i;
	
	for(i=0; i<nbLigne*nbLigne/2; i++){
		
		if(i%(1<<s) == 0 && (i/nbLigne)%(1<<s) == 0 && i%nbLigne < nbLigne/2 && i/nbLigne < nbLigne/2) {
			if(i%nbLigne == 0)
				printf("\n");
			printf("|%5.1f| ", cell[i]);
		}
	}
	printf("\n");
}

void diffusionHorizontale() {
	int i;

	for(i=0; i<nbCell; i++) {
		//printf("Checking cell %d \n", i);
		float prevLeftCell = 0.0;
		float prevRightCell = 0.0;

		if(!isZoneInterne(i,s)) {
			prevLeftCell = (i%nbLigne == 0) ? TEMP_FROID : cellPrev[i-1];
			prevRightCell = (i%nbLigne == nbLigne-1) ? TEMP_FROID : cellPrev[i+1];

			cellCurr[i] = (prevRightCell + (H-2)*(cellPrev[i]) + prevLeftCell)/H;
		}
	}
}

void diffusionVerticale() {
	int i;

	for(i=0; i<nbCell; i++) {

		float prevTopCell = 0.0;
		float prevBottomCell = 0.0;

		//printf("Checking cell %d \n", i);
		if(!isZoneInterne(i,s)) {
			prevTopCell = (i/nbLigne == 0) ? TEMP_FROID : cellPrev[i-nbLigne];
			prevBottomCell = (i/nbLigne == nbLigne-1) ? TEMP_FROID : cellPrev[i+nbLigne];

			cellCurr[i] = (prevTopCell + (H-2)*(cellPrev[i]) + prevBottomCell)/H;
		}
	}
}

void updatePlaque() {
	copyPlaque(cellCurr, cellPrev, nbCell);

	diffusionHorizontale();
	copyPlaque(cellCurr, cellPrev, nbCell);
	diffusionVerticale();
}

/**
 * Verifier si une cellule est dans la zone interne.
 */
bool isZoneInterne(int i, int s) {
	nbLigne = 1 << ((s+4));

	// Calcul de la zone interne, dans laquelle la temperature est constante
	int highBound = 1 << (s+3);
	int lowBound = 1 << s;
	
	int zoneInterneMin = highBound - lowBound;
	int zoneInterneMax = highBound + lowBound;

	return (i%nbLigne >= zoneInterneMin && i%nbLigne < zoneInterneMax
			&& i/nbLigne >= zoneInterneMin && i/nbLigne < zoneInterneMax);

}

/**
 * La taille de plaque est defini par une valeur s donne en argument de program.
 * La plaque aura 2**(s+4) cases sur une ligne, et 2**(s+4) colonne. 
 */ 
void plaqueInit(int argS, int nbIter, int aflag, int mflag, int Mflag) {

	s = argS;
 	nbLigne = 1 << ((s+4));
	nbCell = nbLigne*nbLigne;	
	nbIteration = nbIter;
	mFlag = mflag;
	MFlag = Mflag;

	printf("Plaque taille %d*%d \n", nbLigne, nbLigne);
	cellCurr = (float*)malloc(nbCell*sizeof(float));
	cellPrev = (float*)malloc(nbCell*sizeof(float));


	int i=0;
	
	for(i=0; i<nbCell; i++){
		if(isZoneInterne(i,s))
			cellCurr[i] = TEMP_CHAUD;
		else
			cellCurr[i] = TEMP_FROID;
	}
	
	if(aflag) {
		printf("\n========= BEFORE ITERATION ===========\n");
		display(cellCurr, nbLigne);
	}

	// Pour l'instant, c'est que scenario 0 10 fois
	executeScenario(0, 10);

	if(aflag) {
		printf("\n========= AFTER %d ITERATION ===========\n", nbIter);
		display(cellCurr, nbLigne);
	}

}

void executeIteration() {
	int i;
	
	for(i=0;i<nbIteration;i++){
		updatePlaque();
	}
}

double calculMoyenne(double array[], int size) {
	double total = 0;
	double max = array[0];
	double min = array[0];
	double moyenne = 0;
	
	for(int i=0;i<size;i++) {
		if(array[i] > max)
			max = array[i];
		if (array[i] < min)
			min = array[i];
		total += array[i];
	}
	
	total = total - max - min;
	moyenne = total/(size-2);
	
	return moyenne;
}

void executeScenario(int numScenario, int nbRepetition) {
	int i;
	
	// Stocker les temps d'execution CPU et d'utilisateur
	// pour ensuite faire le calcul de moyenne
	double cpuTime[nbRepetition];
	double userTime[nbRepetition];
	double moyenCpuTime, moyenUserTime;
	
	clock_t start, end;
	time_t begin, fin;
	double diffCpu;
	double diffUser;
	struct rusage usage;
	
	for(i=0;i<nbRepetition;i++){
		time(&begin);
		start = clock();
		executeIteration();
		end = clock();
		time(&fin);	
		diffCpu = ((double) (end - start)) / CLOCKS_PER_SEC;	
		diffUser = difftime(fin,begin);
		printf("Rep %d : Time CPU - %f  : Time user - %f \n", i, diffCpu, diffUser);
		cpuTime[i] = diffCpu;
		userTime[i] = diffUser;
	}
	getrusage(RUSAGE_SELF, &usage);
	
	moyenCpuTime = calculMoyenne(cpuTime, nbRepetition);
	moyenUserTime = calculMoyenne(userTime, nbRepetition);

	if(mFlag)
		printf("Moyenne cpuTime : %f \n", moyenCpuTime);
	if(MFlag)
		printf("Moyenne userTime : %f \n", moyenUserTime);
	
	printf("Empreinte max : %ld \n", usage.ru_maxrss);


}

