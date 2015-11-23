#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) { 
	FICHIER *f1;//*f2 : unused variable
	if(argc != 2) exit(-1);

	//On a du modifier cette ligne car argv[2] est faux.
	//ligne originale : f1 = ouvrir (argv[2], 'E');
	//Sachant que argc == 2 prendre argv[2] ne sert pas à grand chose .....
	f1 = ouvrir(argv[1], 'E');
	if(f1 == NULL) exit(-1);

	fecriref (f1, " %c %s 12\n", 'a', "bonjour");
	fecriref (f1, " %d \n", -1257);

	fermer(f1);
	return 0;
}