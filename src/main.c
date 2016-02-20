#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>


int main(int argc, char **argv){
	
  int mflag = 0;
  int Mflag = 0;
  int tvalue = 0;
  int svalue = 0;
  int index;

  opterr = 0;
  int c;

  while ((c = getopt (argc, argv, "s:mMai:e:t:")) != -1)
    switch (c)
      {
		case 's':
		  svalue = atoi(optarg);
		case 'm':
		  mflag = 1;
		  break;
		case 'M':
		  Mflag = 1;
		  break;
		case 't':
		  tvalue = atoi(optarg);
		  break;
		case '?':
		  if (optopt == 'c')
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

  printf ("svalue = %d, tvalue = %d, mflag = %d\n",
          svalue, tvalue, mflag);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;

}
