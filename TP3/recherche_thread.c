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
//Recupere le nombre de processeurs des machines sur lequel la commande est lancée.

int tab[SIZE];
int val = NOMBRE;

void* recherche_debut(void *arg){
	int i;
	for (i = 0; i < SIZE/2 && arret == 0; i++){
		//printf("Thread debut : arret = %d\n",arret);
		if (tab[i] == val){
			arret = 1;
			pthread_exit((void*)i);
		}
	}
	printf("Debut : i = %d et arret = %d\n",i,arret);
	pthread_exit((void*)-1);
}

void* recherche_fin(void *arg){
	int i;
	for (i = SIZE-1; i >= SIZE/2 && arret == 0; i--){
		//printf(" threaf fin, arret = %d\n",arret);
		if (tab[i] == val){
			arret = 1;
			printf("Fin : i = %d et arret = %d\n",i,arret);
			pthread_exit((void*)i);
		}
	}
	printf("Fin : i = %d et arret = %d\n",i,arret);
	pthread_exit((void*)-1);
}

int main (int argc, char **argv){
	pthread_t tids[2];
	int i, retour=-1;
	void* status;
	for (i = 0; i < SIZE; i++){
		tab[i] = (float) rand() / RAND_MAX * SIZE;
	}
	gettimeofday(&t0, 0);
	pthread_create (&tids[0], NULL, recherche_debut, NULL);
	pthread_create (&tids[1], NULL, recherche_fin, NULL);
	for (i = 0; i < 2; i++){
	  pthread_join (tids[i], (void *) &status);
	  if (status != (int) -1){
	  	retour = (int) status;
	  }
	  printf("Valeur de retour de la thread %lu : [%d]\n",(unsigned long)tids[i],(int)status);
	}
	gettimeofday(&t1, 0);
	if (retour != -1){
		printf("Trouvé : %d est en %d\n",NOMBRE, (int)retour);
	} else {
		printf("%d n'est pas dans le tableau !!!!!!!!!!!!\n",NOMBRE);
	}
	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("Temps d'executuion de la fonction : [%ld] µs\n",elapsed);
	return 0;
}