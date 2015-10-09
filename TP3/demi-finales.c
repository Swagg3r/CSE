#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define NB_SONG 3

void *supporter (void *arg){
  char      *str = (char *) arg ;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid() ;
  tid = pthread_self();
  srand ((int) tid);

  for (i = 0; i < NB_SONG; i++){
    printf ("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, str) ;
    usleep(rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

void demi_finales(int team1, int team2, char* chant1, char* chant2){
	int i;
	pthread_t *tids;
	int nb_threads = team1 + team2; 
	tids = malloc (nb_threads*sizeof(pthread_t)) ;

	/* Create the threads for team1 */
	for (i = 0 ; i < team1; i++) {
	  pthread_create(&tids[i], NULL, supporter, chant1) ;
	}
	/* Create the other threads (ie. team2) */
	for ( ; i < nb_threads; i++){
	  pthread_create(&tids[i], NULL, supporter, chant2) ;
	}  

	/* Wait until every thread ened */ 
	for (i = 0; i < nb_threads; i++){
	  pthread_join(tids[i], NULL) ;
	}
	
	free (tids) ;
}

int main (int argc, char **argv){

  int team1,team2, team3, team4;

  if (argc != 5){
    fprintf(stderr, "usage : %s team1 team2 team3 team4\n", argv[0]) ;
    exit (-1) ;
  }

  team1 = atoi(argv[1]);
  team2  = atoi(argv[2]);
  team3 = atoi(argv[3]);
  team4 = atoi(argv[4]);

  pid_t pid = fork();
  if (pid == -1){
  	perror("erreur fork\n");
  }
  if (pid == 0){
  	//fils
  	demi_finales(team1, team2, "1", "2");
  }
  else {
  	//pere
  	demi_finales(team3, team4, "3", "4");
  }
  return EXIT_SUCCESS;
}
