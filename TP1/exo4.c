//Monnet-Paquet & Thebaud
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
 
typedef struct Transmission Transmission;
struct Transmission{
    int valeur;
    int node;
    pid_t pid;
};
 
int rang = 0;
int status=0;
key_t cleSMP=0;
int id_mp=0;
Transmission *tab=NULL;
#define FICHIER_CLE "cle.serv"
 
void creer_segment_memoire_partagee(){
    int i=0;
    /*Creation de la  cle pour le SMP*/
    cleSMP = ftok(FICHIER_CLE,i);
    if (cleSMP == -1){
        printf("Pb creation cle SMP\n");
        exit(1);
    }
 
    /* On cree les SMP et on teste s'ils existent deja    */
    id_mp = shmget(cleSMP,4096,IPC_CREAT | IPC_EXCL | 0660);
    if (id_mp == -1){
        printf("Pb creation SMP ou il existe deja\n");
        exit(2);
    }
    //Attachement
    tab = shmat(id_mp,NULL,0);
    if (tab==(Transmission *) -1){
        printf("Pb attachement SMP\n");
        exit(3);
    }
}
 
void suppression_segment_memoire_partagee(){
    shmdt(tab);
    if (shmctl(id_mp,IPC_RMID,NULL) == -1){
        fprintf(stderr,"Erreur suppression segment de memoire partagée...\n");
    }
}
 
void creer_processus (int m){
    int i;
    pid_t pid;
    pid = getpid();
    for(i=1; i<m; i++){
        pid = fork();//sauvegarde du pid des fils
        if(pid == -1){
            perror("fork");
            exit(-1);
        }
        if(pid == 0){//Code du fils
            rang = i;
            return;//Sort de la boucle
        }
    }
}
 
void tirage_aleatoire(Transmission t2){
    srand(getpid());
    int val = rand();
    printf("processus pid %d node %d val = %d\n",getpid(), rang, val);
    t2.valeur = val;
    t2.node = rang;
    t2.pid = getpid();
    tab[rang] = t2;
}
 
void election(int n){
    int i,verif=0;
    //Les processus attendent que tout le monde est marqué le resultat de son tirage aleatoire.
    while(verif != n){
        verif = 0;
        for (i = 0; i < n; i++){
            if (tab[i].valeur != 0){
                verif++;
            }
        }
    }
    //Une fois que c'est fait on elit le plus grand nombre.
    int plus_grand = 0;
    int node_plus_grand = 0;
    int pid_plus_grand = 0;
    for (i = 0; i < n; i++){
        if (tab[i].valeur > plus_grand){
            plus_grand = tab[i].valeur;
            node_plus_grand = tab[i].node;
            pid_plus_grand = tab[i].pid;
        }
    }
    //Affichage du plus grand par tous le monde.
    printf("[node %d] The winner is %d  pid %d node %d\n",rang,plus_grand,pid_plus_grand,node_plus_grand);
}
 
void attendre_fin_processus(int o){
    int i =0;
    for(i = 0; i<o; i++){
        wait(&status);
    }
}
 
int main (int argc, char* argv[]){
    if (argc < 2){
        printf("Usage : exo4 <nb processus>\n");
        return -1;
    }
    Transmission t1;
    t1.pid = 0;
    t1.node = 0;
    t1.valeur = 0;
    int n = atoi(argv[1]);
    creer_segment_memoire_partagee();
    creer_processus(n);
    tirage_aleatoire(t1);
    election(n);
    if(rang == 0){
        attendre_fin_processus(n);
        suppression_segment_memoire_partagee();
    }
    return 5;
}
