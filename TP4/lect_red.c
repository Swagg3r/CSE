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
#define LECTEURS 5

sem_t ecriture;
sem_t lecture;
int nombre_lecteurs;

typedef struct linked_list {
    int nb;
    struct linked_list *next;
} linked_list;

typedef struct lectred {
    //ToDo
} lectred;

typedef struct linked_list_head {
    lectred sync;
    linked_list *head;
} linked_list_head;

void init(){
	sem_init(&ecriture, 0, 1);
    sem_init(&lecture, 0, LECTEURS);
}

void begin_read(sem_t s) {
	//Debut de lecture.
    P(&lecture);
    nombre_lecteurs++;
    if (nombre_lecteurs == 1){
    	P(&ecriture); //Bloque les ecrivains.
    }
    V(&lecture);
    //Acces a la variable protégé.
}

void end_read(sem_t s) {
	//Apres lecture de la variable en section critique.
	P(&lecture);
	nombre_lecteurs--;
	if (nombre_lecteurs == 0){
		V(&ecriture); //Debloque les ecrivains.
	}
	V(&lecture);
	//Fin de la lecture.
}

void begin_write(sem_t s) {
    P(&ecriture);
}

void end_write(sem_t s) {
    V(&ecriture);
}

void list_init(linked_list_head *list) {
    list->head = NULL;
    init(&list->sync);
}

int exists(linked_list_head *list, int val) {
    linked_list *p;
    begin_read(&list->sync);
    p = list->head;
    while(p) {
        if (p->nb == val) {
            end_read(&list->sync);
            return 1;
        }
        p = p->next;
    }
    end_read(&list->sync);
    return 0;
}

linked_list* remove(linked_list_head* list, int val) {
    linked_list **p, *ret=NULL;
    begin_write(&list->sync);
    p = list->head;
    while(*p) {
        if ((*p)->nb == val) {
            ret = *p;
            *p = (*p)->next;
            break;
        }
        p=&(*p)->next
    }
    end_write(&list->sync);
    return ret;
}

int main (int argc, char* argv[]){
    nombre_lecteurs = 0;
    //initialise les sempaphores.
    init();
}