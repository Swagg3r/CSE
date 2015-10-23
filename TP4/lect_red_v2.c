
//v2 = priorité aux rédacteurs


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define mutex_t pthread_mutex_t
#define cond_t pthread_cond_t
#define mutex_init pthread_mutex_init
#define cond_init pthread_cond_init
#define cond_wait pthread_cond_wait
#define cond_signal pthread_cond_signal
#define cond_broadcast pthread_cond_broadcast

// Les structures définies
typedef struct lectred {
    mutex_t m;
    cond_t cl, ce;
    int ecriture;
    int nb_lecteurs, nb_lect_att, nb_ecri_att;
} lectred;
 
typedef struct linked_list {
    int nb;
    struct linked_list *next;
} linked_list;
 
typedef struct linked_list_head {
    struct lectred sync;
    struct linked_list *head;
} linked_list_head;
 
// La liste partagée
linked_list_head llh;
 
void init(lectred* l) {
    mutex_init(&(l->m), NULL);
    cond_init(&(l->cl), NULL);
    cond_init(&(l->ce), NULL);
    l->ecriture = 0;
    l->nb_lect_att = 0;
    l->nb_ecri_att = 0;
    l->nb_lecteurs = 0;
}
 
void begin_read(lectred* l) {
    lock(&(l->m));
    while(l->ecriture == 1 || l->nb_ecri_att > 0){
            l->nb_lect_att++;
            cond_wait(&(l->cl), &(l->m));
            l->nb_lect_att--;
    }
    l->nb_lecteurs++;
    unlock(&(l->m));
}
 
void end_read(lectred* l) {
    lock(&(l->m));
    l->nb_lecteurs--;
    if(l->nb_lect_att == 0 && l->nb_ecri_att > 0){
        cond_signal(&(l->cl));
    }
    unlock(&(l->m));
}
 
void begin_write(lectred* l) {
    lock(&(l->m));
    while(l->ecriture == 1 || l->nb_lecteurs > 0){
        l->nb_ecri_att++;
        cond_wait(&(l->ce), &(l->m));
        l->nb_ecri_att--;
    }
    l->ecriture = 1;
    unlock(&(l->m));
}
 
void end_write(lectred* l) {
    lock(&(l->m));
    l->ecriture = 0;
    if(l->nb_ecri_att > 0){
        cond_signal(&(l->ce));
    }
    else if(l->nb_lect_att > 0){
        cond_broadcast(&(l->cl));
    }
    unlock(&(l->m));
}
 
void list_init(linked_list_head *list) {
    list->head = NULL;
    init(&list->sync);
}
 
int exists(struct linked_list_head *list, int val) {
    begin_read(&list->sync);
    /*
    struct linked_list *p;
    p = list->head;
    while(p) {
            if (p->nb == val) {
                    end_read(&list->sync);
                    return 1;
            }
            p = p->next;
    }*/
    printf("thread %u lit\n", (unsigned int) pthread_self());
	sleep(rand() % 5);
    end_read(&list->sync);
    return 0;
}
 
linked_list* add_elem(linked_list_head* list, int val) {
    begin_write(&list->sync);
    /*
	linked_list **p;
    p = &list->head;
    while(*p) {
        p=&(*p)->next;
    }
    linked_list newL;
    newL.nb = val;
    newL.next = NULL;
    *p=&newL;*/
    printf("thread %u écrit\n", (unsigned int) pthread_self());
    sleep(rand() % 5);
    end_write(&list->sync);
    return list->head;
    //return *p;
}
 
linked_list* remove_element(struct linked_list_head *list, int val) {
    begin_write(&list->sync);
   	/* 
	linked_list **p, *ret = NULL;
   	p = &list->head;
    while(*p) {
        if ((*p)->nb == val) {
            ret = *p;
            *p = (*p)->next;
            break;
        }
        p = &(*p)->next;
    }*/
    end_write(&list->sync);
    return list->head;
}
 
void* lecteurs() {
	int r = rand() % (10-1) + 1;
	if (exists(&llh, r)) {}
    return (void *) pthread_self();
}

void* redacteurs() {
	int r = rand() % (10-1) + 1;
	add_elem(&llh, r);
    return (void *) pthread_self() ;
}
 
int main (int argc, char** argv) {
    if (argc != 3) {
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