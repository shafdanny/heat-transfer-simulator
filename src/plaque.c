#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TEMP_CHAUD 255
#define TEMP_FROID 0

#define H 6

bool isZoneInterne(int i, int s);


float *cellPrev = NULL;
float *cellCurr = NULL;

int s = 0;
int nbLigne = 0;
int nbCell = 0;

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

		/**
		if(!isZoneInterne(i+1,0)) {
			//printf("Updating right cell %d with value %f \n", i+1, cellPrev[i]/6);
			cellCurr[i+1] += cellPrev[i+1] + cellPrev[i]/6;
		}

		if(!isZoneInterne(i-1,0)) {
			//printf("Updating left  cell %d with value %f \n", i-1, cellPrev[i]/6);
			cellCurr[i-1] += cellPrev[i-1] + cellPrev[i]/6;
		}**/
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
	
	//printf("before diffusion \n");

	//display(cellPrev, 16);
	//display(cellCurr, 16);

	diffusionHorizontale();
	copyPlaque(cellCurr, cellPrev, nbCell);
	diffusionVerticale();

	//printf("after diffusion \n");

	//display(cellPrev, 16);
	//display(cellCurr, 16);
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
void plaqueInit(int argS) {

	s = argS;
 	nbLigne = 1 << ((s+4));
	nbCell = nbLigne*nbLigne;	
	
	printf("Will create %d row and line, so %d cell \n", nbLigne, nbCell);
	cellCurr = (float*)malloc(nbCell*sizeof(float));
	cellPrev = (float*)malloc(nbCell*sizeof(float));


	int i=0;
	
	for(i=0; i<nbCell; i++){
		if(isZoneInterne(i,s))
			cellCurr[i] = TEMP_CHAUD;
		else
			cellCurr[i] = TEMP_FROID;
	}
	
	printf("\n========= BEFORE ITERATION ===========\n");
	display(cellCurr, nbLigne);

	for(i=0;i<10000;i++){
		updatePlaque();
	}

	printf("\n========= AFTER ITERATION ===========\n");
	display(cellCurr, nbLigne);

	//updatePlaque();
	//updatePlaque();
}

