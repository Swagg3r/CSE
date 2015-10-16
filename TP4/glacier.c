//Monnet-Paquet & Thebaud

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define P sem_wait
#define V sem_post
#define M 4
#define NB_THREAD 5

int nb_glaces;
sem_t dring;
sem_t vente;

void* acheter_glace() {
    P(&vente);
    nb_glaces--;
    printf("thread %u a acheté une glace. (nombre restant = %d)\n", (unsigned int) pthread_self(), nb_glaces);
    sleep(1);
    V(&dring);

    return (void *) pthread_self() ;
}

void* recharger_glace() {
    while(1) {
        P(&dring);
        if (nb_glaces == 0) {
            nb_glaces += M;
            printf("le glacier a rechargé\n");
            sleep(1);
        }
        V(&vente);
    }
    
    printf("thread %u termine (glacier)\n", (unsigned int) pthread_self());
    return (void *) pthread_self();
}

int main (int argc, char* argv[]){

    sem_init(&dring, 0, 0);
    sem_init(&vente, 0, 1);
    nb_glaces = M;

    pthread_t tids[NB_THREAD];
    for (int i = 0; i < NB_THREAD; i++){
        pthread_create(&tids[i], NULL, &acheter_glace, NULL);
    }
    pthread_create(&tids[NB_THREAD], NULL, &recharger_glace, NULL);

    /* Wait until every thread ened */ 
    for (int i = 0; i < NB_THREAD; i++){
        pthread_join(tids[i], NULL);
    }
}