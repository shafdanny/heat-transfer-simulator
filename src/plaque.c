#include <stdio.h>
#include <stdlib.h>

#define TEMP_CHAUD 50

float cell[16][16];
float *cellule = NULL;

/**
 * La taille de plaque est defini par une valeur s donne en argument de program.
 * La plaque aura 2**(s+4) cases sur une ligne, et 2**(s+4) colonne. 
 */ 
void plaqueInit(int s) {

	int nbLigne = 1 << ((s+4));
	int nbCell = nbLigne*nbLigne;	
	
	printf("Will create %d row and line, so %d cell \n", nbLigne, nbCell);
	cellule = (float*)malloc(nbCell*sizeof(float));
	printf("%p \n", cellule);
	
	int i=0;
	int j=0;
	
	// Calcul de la zone interne, dans laquelle la temperature est constante
	int highBound = 1 << (s+3);
	int lowBound = 1 << s;
	
	int zoneInterneMin = highBound - lowBound;
	int zoneInterneMax = highBound + lowBound;
	
	for(i=0; i<nbCell; i++){
		if(i%16 == 0)
			j++;
	
		if(i%16 >= zoneInterneMin && i%16 < zoneInterneMax
			&& j >= zoneInterneMin && j < zoneInterneMax)
			cellule[i] = TEMP_CHAUD;
	}
	
	printf("2**n-1 : %d \n", 1 << (s+3));
	printf("2**n-4 : %d \n", 1 << (s));
	
	display(nbLigne);
}

/**
 * Afficher les cellules. Utilisation pour debogage, 
 * seulement pour des petits tailles.
 */
void display(int nbLigne){
	int i,j;
	
	for(i=0; i<nbLigne*nbLigne; i++){
		if(i%nbLigne == 0)
			printf("\n");
		printf("|%4.0f| ", cellule[i]);
	}
	printf("\n");
}

void printIntro(){
	printf("Creation de plaque ...\n");
}

