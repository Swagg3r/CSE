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

int recherche(int val, int tab[SIZE]){
	int i;
	for (i = 0; i < SIZE; i++){
		if (tab[i] == val){
			return i;
		}
	}
	return -1;
}

int main (int argc, char **argv){
	int tab[SIZE];
	int i, retour=-1;
	for (i = 0; i < SIZE; i++){
		tab[i] = (float) rand() / RAND_MAX * SIZE;
	}
	gettimeofday(&t0, 0);
	retour = recherche(NOMBRE, tab);
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