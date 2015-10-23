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

typedef struct linked_list {
    int nb;
    struct linked_list *next;
} linked_list;

typedef struct lectred {
    sem_t ecriture;
    sem_t lecture;
    int nombre_lecteurs;
} lectred;

typedef struct linked_list_head {
    lectred sync;
    linked_list *head;
} linked_list_head;

linked_list_head llh;

void init(lectred *l){
	sem_init(&(l->ecriture), 0, 1);
    sem_init(&(l->lecture), 0, 1);
    l->nombre_lecteurs = 0;
}

void begin_read(lectred *l) {
	//Debut de lecture.
    P(&(l->lecture));
    l->nombre_lecteurs++;
    if (l->nombre_lecteurs == 1){
    	P(&(l->ecriture)); //Bloque les ecrivains.
    }
    V(&(l->lecture));
    //Acces a la variable protégé.
}

void end_read(lectred *l) {
	//Apres lecture de la variable en section critique.
	P(&(l->lecture));
	l->nombre_lecteurs--;
	if (l->nombre_lecteurs == 0){
		V(&(l->ecriture)); //Debloque les ecrivains.
	}
	V(&(l->lecture));
	//Fin de la lecture.
}

void begin_write(lectred *l) {
    P(&(l->ecriture));
}

void end_write(lectred *l) {
    V(&(l->ecriture));
}

void list_init(linked_list_head *list) {
    list->head = NULL;
    init(&list->sync);
}

int exists(linked_list_head *list, int val) {
    linked_list *p;
    begin_read(&list->sync);
    /*p = list->head;
    while(p) {
    	printf("if (%d == %d)\n", p->nb, val);
        if (p->nb == val) {
            end_read(&list->sync);
            return 1;
        }
        p = p->next;
    }
    printf("c\n");*/
    printf("thread %u lit\n", (unsigned int) pthread_self());
    sleep(1);
    end_read(&list->sync);
    return 0;
}

linked_list* remove_elem(linked_list_head* list, int val) {
    linked_list **p, *ret=NULL;
    begin_write(&list->sync);
    p = &list->head;
    while(*p) {
        if ((*p)->nb == val) {
            ret = *p;
            *p = (*p)->next;
            break;
        }
        p=&(*p)->next;
    }
    end_write(&list->sync);
    return ret;
}

linked_list* add_elem(linked_list_head* list, int val) {
    linked_list **p;
    begin_write(&list->sync);
    /*p = &list->head;
    while(*p) {
        p=&(*p)->next;
    }
    linked_list newL;
    newL.nb = val;
    newL.next = NULL;
    *p=&newL;*/
    printf("thread %u écrit\n", (unsigned int) pthread_self());
    sleep(1);
    end_write(&list->sync);
    return *p;
}

void* lecteurs() {
	int r = 5; //rand() % (10-1) + 1;
	printf("thread %u recherche la valeur (%d)\n", (unsigned int) pthread_self(), r);
	if (exists(&llh, r)) {
		printf("thread %u a trouvé la valeur (%d) et termine\n", (unsigned int) pthread_self(), r);
	} else {
		printf("thread %u n'a pas trouvé la valeur (%d) et termine\n", (unsigned int) pthread_self(), r);
	}
    return (void *) pthread_self();
}

void* redacteurs() {
	int r = rand() % (10-1) + 1;
	printf("thread %u veut ajouter la valeur (%d)\n", (unsigned int) pthread_self(), r);
	add_elem(&llh, r);
	printf("thread %u a ajouté la valeur (%d) et termine\n", (unsigned int) pthread_self(), r);
    return (void *) pthread_self() ;
}

int main (int argc, char* argv[]){
	if (argc != 3){
		printf("Usage : <nb_lecteurs> <nb_redacteurs>\n");
	}
	int nb_lecteurs = atoi(argv[1]);
	int nb_redacteurs = atoi(argv[2]);
	int i=0;
    //initialise les sempaphores.
    list_init(&llh);
    printf("liste initialisée\n");

    pthread_t tids[nb_redacteurs + nb_lecteurs];
    for (; i < nb_redacteurs; i++){
        pthread_create(&tids[i], NULL, &redacteurs, NULL);
    }
    printf("rédacteurs créés\n");
    for (; i < nb_lecteurs+nb_redacteurs; i++){
        pthread_create(&tids[i], NULL, &lecteurs, NULL);
    }
    printf("lecteurs créés\n");

    /* Wait until every thread ened */ 
    for (i = 0; i < nb_lecteurs+nb_redacteurs; i++){
        pthread_join(tids[i], NULL);
    }
}

//Question 1 : Il peut y avoir un seul lecteur en même temps.