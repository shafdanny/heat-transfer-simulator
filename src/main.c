#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "plaque.c"


int main(int argc, char **argv){

  // define all flags
  int mflag = 0;
  int Mflag = 0;
  int aflag = 0;
  
  // define all values from an option argument
  char* svalue = 0;
  char* ivalue = 0;
  char* evalue = 0;
  char* tvalue = 0;
  
  int index;

  opterr = 0;
  int c;

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

	printf ("svalue = %s, tvalue = %s, ivalue = %s, evalue = %s, mflag = %d\n",
			  svalue, tvalue, ivalue, evalue, mflag);

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	
	plaqueInit(atoi(svalue));
	return 0;

}
