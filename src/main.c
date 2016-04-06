#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "plaque.h"
#include "barriere.h"

#define toDigit(c) (c-'0')


int main(int argc, char **argv){
  int i=0;
  int j=0;

  // define all flags
  int mflag = 0;
  int Mflag = 0;
  int aflag = 0;
  
  // define all values from an option argument
  char* svalue = 0;
  char* ivalue = 0;
  char* evalue = 0;
  char* tvalue = 0;
 
  opterr = 0;
  int c;
  
  /** 
   * Recuperer les parametres d'execution de programme.
   * -m : mesure et affichage du temps d'éxécution (consommation du CPU)
   * -M : mesure et affichage du temps d'éxécution (temps de réponse utilisateur)
   * -a : affichage de la température initiale et de la température finale 
   * 		pour les indices correspondant au quart supérieur gauche du tableau, 
   * 		pour les indices modulo 2^s
   * -i number : number = nombre d'itération à exécuter (par defaut 10000)
   * -s [0123456789] : taille du problème à exécuter, 2^(s+4) cases sur une ligne, 0 <= s <= 9
   * -e [012345] : étape du programme à exécuter
   * -t [012345] : nombre de thread à créer, 4^t thread à créer, 
   * 				t varie entre 0 (itératif) et 5 (1024 thread)
   */ 
  while ((c = getopt (argc, argv, "mMas:i:e:t:")) != -1)
    switch (c)
      {
		case 'm':
		  mflag = 1;
		  break;
		case 'M':
		  Mflag = 1;
		  break;
		case 'a':
		  aflag = 1;
		  break;
		case 's':
		  svalue = optarg;
		  break;
		case 'i':
		  ivalue = optarg;
		  break;
		case 'e':
		  evalue = optarg;
		  break;
		case 't':
		  tvalue = optarg;
		  break;
		case '?':
		  if (strchr("siet", optopt) != NULL)
			fprintf (stderr, "Option -%c requires an argument.\n", optopt);
		  else if (isprint (optopt))
			fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		  else
			fprintf (stderr,
					"Unknown option character `\\x%x'.\n",
					 optopt);
			return 1;
		  default:
			abort ();
    }

	
	// Executer chaque scenario dans le parametre -e
	for(i=0;i<strlen(evalue);i++) {
		printf("\n########### Running SCENARIO %c ###########\n", evalue[i]);
		int scenario = toDigit(evalue[i]);
		
		// Executer la scenario avec les tailles dans le parametre -s
		for(j=0;j<strlen(svalue);j++) {
			int tailleS = toDigit(svalue[j]);
			if(scenario == 0) {
				plaqueInit(scenario, tailleS, atoi(ivalue), aflag, mflag, Mflag, 0);
			} else {
				int k;
				
				// Executer la scenario avec les nombres de threads dans le parametre -t
				for(k=0;k<strlen(tvalue);k++) {
					printf("\n === Option -t %d === \n", toDigit(tvalue[k]));
					plaqueInit(scenario, tailleS, atoi(ivalue), aflag, mflag, Mflag, toDigit(tvalue[k]));
				}
			}
		}
	}
	
	/** FOR DEBUGGING PURPOSES **/
	/*
    // Affiche les valeurs des options
	printf ("svalue = %s, tvalue = %s, ivalue = %s, evalue = %s, mflag = %d\n",
			  svalue, tvalue, ivalue, evalue, mflag);

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	*/
	//char *token = strtok(evalue, "");
	//testBarriere();
	//plaqueInit(0, 0, atoi(ivalue), aflag, mflag, Mflag);
	//plaqueInit(1, 0, atoi(ivalue), aflag, mflag, Mflag);
	//plaqueInit(1, 1, atoi(ivalue), aflag, mflag, Mflag);

	
	//plaqueInit(atoi(svalue));
	/** END FOR DEBUGGING **/
	
	return 0;

}
