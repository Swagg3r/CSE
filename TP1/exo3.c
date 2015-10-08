//Monnet-Paquet & Thebaud
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int rang = 0;
int status=0;

typedef struct Transmission {
    int valeur;
    int node;
    pid_t pid;
} Transmission;
 
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
 
int tirage_aleatoire(){
    srand(getpid());
    int val = rand();
    printf("processus pid %d node %d val = %d\n",getpid(), rang, val);
    return val;
}
 
void attendre_fin_processus(int o){
    int i =0;
    for(i = 0; i<o; i++){
        wait(&status);
    }
}
 
void creer_tube(int tab2[][2], int nb){
    int i,verif;
    for(i = 0; i<nb; i++){
        verif = pipe(tab2[i]);
        if(verif != 0){
            perror("pipe");
        }
    }
}
 
void selection_tube(){
 
}
 
void fermeture_tube_inutiles(int n, int tab4[][2]){
    int i;
    for (i = 0; i < n; ++i){
		if (i == rang) {
			close(tab4[i][0]);
		} else if (i == rang-1 || i == n-1) {
			close(tab4[i][1]);
		} else {
			close(tab4[i][0]);
			close(tab4[i][1]);
		}
    }
}
 
Transmission transmission_valeur(int valeur, int tab3[][2], int n, Transmission t2){
     
    if(rang == 0){
        t2.valeur = valeur;
        t2.node = rang;
        t2.pid = getpid();
        if(write(tab3[0][1], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur write\n");}
        if(read(tab3[n-1][0], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur read\n");}
        //2e tour
        if(write(tab3[0][1], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur write\n");}
    
    } else if(rang == n-1){
        if(read(tab3[n-2][0], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur read\n");}
        if(valeur > t2.valeur){
            t2.valeur = valeur;
            t2.node = rang;
            t2.pid = getpid();
        }
        if(write(tab3[n-1][1], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur write\n");}
        //2e tour
        if(read(tab3[n-2][0], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur read\n");}
    
    } else {
        if(read(tab3[rang-1][0], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur read\n");}
        if(valeur > t2.valeur){
            t2.valeur = valeur;
            t2.node = rang;
            t2.pid = getpid();
        }
        if(write(tab3[rang][1], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur write\n");}
        //2e tour
        if(read(tab3[rang-1][0], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur read\n");}
        if(write(tab3[rang][1], &t2, sizeof(t2)) == -1){fprintf(stderr, "Erreur write\n");}
    }
    return t2;
}
 
void print_valeur(Transmission t3){
    printf("Node %d The winner is %d pid %d node %d\n", rang, t3.valeur,t3.pid,t3.node);
}
 
int main (int argc, char* argv[]){
    if (argc < 2){
        printf("Usage : exo3 <nb processus>\n");
        return -1;
    }
    Transmission t1;
    t1.valeur = 0;
    t1.node = 0;
    t1.pid = 0;
    int n = atoi(argv[1]),valeur;
    int tab[n][2];
    creer_tube(tab, n);
    creer_processus(n);
    //selection_tube();
    fermeture_tube_inutiles(n,tab);
    valeur = tirage_aleatoire();
    t1 = transmission_valeur(valeur,tab, n,t1);
    print_valeur(t1);
    if(rang == 0){
        attendre_fin_processus(n);
    }
    return 0;
}
