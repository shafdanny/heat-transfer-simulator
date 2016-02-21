#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TEMP_CHAUD 255
#define TEMP_FROID 0

#define H 6

bool isZoneInterne(int i, int s);


float *cellPrev = NULL;
float *cellCurr = NULL;



void copyPlaque(float *plOrigin, float *plDest, int nbCell) {
	int i;
	for(i=0; i<nbCell; i++){
		plDest[i] = plOrigin[i];
	}
}

/**
 * Afficher les cellules. Utilisation pour debogage, 
 * seulement pour des petits tailles.
 */
void display(float *cell, int nbLigne) {
	int i,j;
	
	for(i=0; i<nbLigne*nbLigne; i++){
		if(i%nbLigne == 0)
			printf("\n");
		printf("|%4.0f| ", cell[i]);
	}
	printf("\n");
}

void diffusionHorizontale() {
	int i;

	for(i=0; i<16*16; i++) {
		//printf("Checking cell %d \n", i);
		if(!isZoneInterne(i,0)) {
			cellCurr[i] = (cellPrev[i+1] + (H-2)*(cellPrev[i]) + cellPrev[i-1])/H;
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

void updatePlaque() {
	copyPlaque(cellCurr, cellPrev, 16*16);
	
	printf("before diffusion \n");

	display(cellPrev, 16);
	display(cellCurr, 16);

	diffusionHorizontale();
	diffusionVerticale();

	printf("after diffusion \n");

	display(cellPrev, 16);
	display(cellCurr, 16);
}

/**
 * Verifier si une cellule est dans la zone interne.
 */
bool isZoneInterne(int i, int s) {
	int nbLigne = 1 << ((s+4));

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
void plaqueInit(int s) {

	int nbLigne = 1 << ((s+4));
	int nbCell = nbLigne*nbLigne;	
	
	printf("Will create %d row and line, so %d cell \n", nbLigne, nbCell);
	cellCurr = (float*)malloc(nbCell*sizeof(float));
	cellPrev = (float*)malloc(nbCell*sizeof(float));


	int i=0;
	int j=0;
	
	for(i=0; i<nbCell; i++){
		if(isZoneInterne(i,s))
			cellCurr[i] = TEMP_CHAUD;
		else
			cellCurr[i] = TEMP_FROID;
	}
	
	printf("cellCurr \n");
	display(cellCurr, nbLigne);
	printf("cellPrev \n");
	display(cellPrev, nbLigne);
	updatePlaque();
}

void printIntro(){
	printf("Creation de plaque ...\n");
}

