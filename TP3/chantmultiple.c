#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct chant {
  char* phrase ;
  int nbfois ;
} chant;

void *supporter (void *arg){
  chant*    ch = (chant*) arg ;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  srand ((int) tid) ;

  for (i = 0; i < ch->nbfois; i++){
    printf ("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, ch->phrase) ;
    usleep (rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){

  int team1;
  int team2;
  int nbfois1;
  int nbfois2;
  int i ;
  int nb_threads = 0 ;
  pthread_t *tids ;

  if (argc != 5){
    fprintf(stderr, "usage : %s team1 team2 nbfois1 nbfois2\n", argv[0]) ;
    exit (-1) ;
  }

  team1 = atoi(argv[1]) ;
  team2  = atoi(argv[2]);
  nbfois1 = atoi(argv[3]) ;
  nbfois2  = atoi(argv[4]);
  nb_threads = team1 + team2;
  tids = malloc (nb_threads*sizeof(pthread_t)) ;

  chant chant1;
  chant1.phrase = "Allons enfants de la patrie";
  chant1.nbfois = nbfois1;

  chant chant2;
  chant2.phrase = "Swing low, sweet chariot";
  chant2.nbfois = nbfois2;

  /* Create the threads for team1 */
  for (i = 0 ; i < team1; i++){
    pthread_create (&tids[i], NULL, &supporter, &chant1) ;
  }
  /* Create the other threads (ie. team2) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, &supporter, &chant2) ;
  }  

  /* Wait until every thread ened */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  return EXIT_SUCCESS;
}