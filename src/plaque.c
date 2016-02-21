#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEMP_CHAUD 255
#define TEMP_FROID 0

float cell[16][16];
float *cellPrev = NULL;
float *cellCurr = NULL;

/**
 * Afficher les cellules. Utilisation pour debogage, 
 * seulement pour des petits tailles.
 */
void display(float *cell, int nbLigne){
	int i,j;
	
	for(i=0; i<nbLigne*nbLigne; i++){
		if(i%nbLigne == 0)
			printf("\n");
		printf("|%4.0f| ", cell[i]);
	}
	printf("\n");
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
	
	int i=0;
	int j=0;
	
	for(i=0; i<nbCell; i++){
	
		if(isZoneInterne(i,s))
			cellCurr[i] = TEMP_CHAUD;
		else
			cellCurr[i] = TEMP_FROID;
	}
	
	display(cellCurr, nbLigne);
}

void printIntro(){
	printf("Creation de plaque ...\n");
}

