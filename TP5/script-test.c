#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
	Idée : Lancé 20 fois consecutivement des tests pour la prise de mesures de performances sur les algos de tris.
	Le programme initial (celui la) lancera a l'aide de fork plusieurs fois les differents tests.
	Le programme initial recupere les prises de mesure du temps effectué par les programmes de tests.
	Un segment de memoire partagé sera créer entre les processus pour transmettre les mesures.
*/

int main(int argc, char *argv[]) {
	//Creation du vecteur dans un fichier .txt
	printf("Creation du vecteur a 1M d'elements.\n");
    pid_t pid;
    pid = fork();
	if (pid==0){/*Code du fils*/
    	//sprintf(taille,"%d",i);
		char taille[11] = "--help";
		char sortie[14] = ">vecteur.txt";
		execl("tri-1.0/creer_vecteur","creer_vecteur",taille,sortie,NULL);
		exit(-1);//Jamais atteint.
	}



	//Premier test : 
	//		Taille du vecteur : 1 Million de nombres.
	//		Séquentiel.
	printf("Lancement du test :\n");
	printf("Taille vecteur : 1M\n");
	printf("Mode sequentiel\n");









	return 0;
}