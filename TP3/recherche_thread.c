#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#define SIZE 2000000
#define NOMBRE 862180

struct timeval t0;
struct timeval t1;
volatile int arret = 0;
//cat /proc/cpuinfo | grep processor | wc -l
//Recupere le nombre de processeurs des machines sur lequel la commande est lancer.

int tab[SIZE];
int val = NOMBRE;

int* recherche_debut(){
	int i;
	for (i = 0; i < SIZE/2 && arret == 0; i++){
		if (tab[i] == val){
			arret = 1;
			return (int*) i;
		}
	}
	return -1;
}

int* recherche_fin(){
	int i;
	for (i = SIZE-1; i >= SIZE/2 && arret == 0; i--){
		if (tab[i] == val){
			arret = 1;
			return (int*) i;
		}
	}
	return -1;
}

int main (int argc, char **argv){
	pthread_t tids[2];
	int i, retour=-1;
	int status = 0;
	for (i = 0; i < SIZE; i++){
		tab[i] = (float) rand() / RAND_MAX * SIZE;
	}
	gettimeofday(&t0, 0);
	pthread_create (&tids[0], NULL, recherche_debut, NULL);
	pthread_create (&tids[1], NULL, recherche_fin, NULL);
	for (i = 0; i < 2; i++){
	  pthread_join (tids[i], &status);
	  if (status != 0){
	  	retour = status;
	  }
	}
	gettimeofday(&t1, 0);
	if (retour != -1){
		printf("Trouvé : %d est en %d\n",NOMBRE, retour);
	} else {
		printf("%d n'est pas dans le tableau !!!!!!!!!!!!\n",NOMBRE);
	}
	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("Temps d'executuion de la fonction : [%ld] µs\n",elapsed);
	return 0;
}