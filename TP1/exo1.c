//Monnet-Paquet & Thebaud

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int rang = 0;
int status=0;

void creer_processus (int m){
	int i;
	for(i=1; i<m; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("fork");
			exit(-1);
		}
		if(pid == 0){
			rang = i;
			return;
		}
	}
}

void tirage_aleatoire(){
	srand(getpid());
	int val = rand();
	printf("processus pid %d node %d val = %d\n",getpid(), rang, val);
}

void attendre_fin_processus(int o){
	int i =0;
	for(i = 0; i<o; i++){
		wait(&status);
	}
}

int main (int argc, char* argv[]){
	if (argc < 2){
		printf("Usage : exo1 <nb processus>\n");
		return -1;
	}
	int n = atoi(argv[1]);
	creer_processus(n);
	tirage_aleatoire();
	if(rang == 0){
		attendre_fin_processus(n);
	}
	return 0;
}
