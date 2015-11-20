#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// int main(int argc, char *argv[]) {
// 	FICHIER *f1;
// 	FICHIER *f2;
// 	char c;

// 	if (argc != 3)
// 		exit(-1);

// 	f1 = ouvrir (argv[1], 'L');
// 	if (f1 == NULL)
// 		exit (-1);

// 	f2 = ouvrir (argv[2], 'E');
// 	if (f2 == NULL)
// 		exit (-1);

// 	while (lire (&c, 1, 1, f1) == 1) {
// 		ecrire (&c, 1, 1, f2);
// 	}

// 	fermer (f1);
// 	fermer (f2);

// 	return 0;
// }

int main (int argc, char **argv) { 
	FICHIER *f1;
	if(argc != 2) exit(-1);

	f1 = ouvrir(argv[2], 'E');
	if(f1 == NULL) exit(-1);

	fecriref(f1, "je %c %s 12\n", 'a', "bonjour");
	fecriref(f1, "tu %d \n", -1257);

	fermer(f1);

	return 0;
}