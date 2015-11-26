#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int partie2 (char* fichier1, char* fichier2){
	FICHIER *f1;
	FICHIER *f2;
	char c;

	f1 = ouvrir (fichier1, 'L');
	if (f1 == NULL)
		exit (-1);
	
	f2 = ouvrir (fichier2, 'E');
	if (f2 == NULL)
		exit (-1);

	while (lire (&c, 1, 1, f1) == 1) {
		ecrire (&c, 1, 1, f2);
	}

	fermer (f1);
	fermer (f2);

	return 0;
}

int partie3 (char* fichier){
	FICHIER *f1;

	f1 = ouvrir(fichier, 'E');
	if(f1 == NULL) exit(-1);

	time_t date = time(NULL); 

	fecriref(f1, "Ceci est une ligne sans parametre\n");
	fecriref (f1, "Nous sommes le : %s",asctime(localtime(&date)));
	fecriref(f1,"Il s'est ecoule %d secondes depuis le 1er %s %d.\n", (int) time(NULL),"janvier",1970);
	fecriref (f1, "Un exemple avec un chiffre negatif : %d \n", -1257);

	fermer(f1);
	return 0;
}

int main(int argc, char *argv[]){

	if (argc == 2){
		partie3(argv[1]);
	}else if (argc == 3){
		partie2(argv[1], argv[2]);
	}else{
		printf("Usage : ./test-supp <fichier1> <fichier2 : optionnel>\n");
		printf("Usage : Avec un seul argument : lancement du test de la partie 3\n");
		printf("Usage : Avec deux arguments : lancment du test de la partie 2\n");
		exit(-1);
	}
	return 0;
}