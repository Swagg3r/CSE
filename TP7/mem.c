//Antoine Thebaud & Aurelien Monnet-Paquet
#define _GNU_SOURCE

#include <mem.h>
#include <pthread.h>

#define ALIGNEMENT sizeof(void*)
#define METASIZE sizeof(size_t)
#define align(val) (((val) + ALIGNEMENT-1) &~ (ALIGNEMENT-1))
#define DEBUG 0
// #define DEBORDEMENT 0xD34DB33F

typedef struct fb { /* fb pour free block */
  size_t size ;
  struct fb *next ;
} fb;

//pointeur première zone libre
fb* ptr_init;
//pointeur première case mémoire
void* ptr_first; 
//pointeur dernière case mémoire
void* ptr_last; 
//stratégie
mem_fit_function_t* active_fit_fonction;

//mutex
pthread_mutex_t mutex=PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

//variables pour calculation()
int fragmentation; // taille jusqu'au dernier bloc occupé
int occupation;    // somme de la taille des blocs occupés
int accumulation;  // axe horizontal : somme totale des tailles demandées 


void mem_init(char* mem, size_t taille) {
    mem_fit(&mem_fit_first);

    ptr_first = mem;
    ptr_last = mem + taille;

    ptr_init = (fb*) mem;
    ptr_init->size = taille;
    ptr_init->next = NULL;

    accumulation = 0;
}

void* mem_alloc(size_t size_alloc) {

    pthread_mutex_lock(&mutex);

    if (size_alloc == 0) {
        printf("opération invalide (size=0)\n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    fb* ptr_alloc = ptr_init;

    //calcul de l'espace mémoire réel à réserver

    //1) métadonnées
    size_alloc += METASIZE;
    //2) alignement
    size_alloc = align(size_alloc);
    // //3) zone de débordement
    // size_alloc += 8;

    //recherche d'une zone libre selon la stratégie d'allocation choisie
    ptr_alloc = (*active_fit_fonction)(ptr_alloc, size_alloc);
    if (ptr_alloc == NULL) {
        printf("Allocation impossible\n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    //3) padding
    if (size_alloc + align(1) + METASIZE >= ptr_alloc->size) {
        size_alloc = ptr_alloc->size;
    }

    //récupère le pointeur de zone libre précédant la zone libre choisie
    fb* ptr_prec = ptr_init;
    if (ptr_prec->next != NULL) {
        while (ptr_prec->next < ptr_alloc) {
            ptr_prec = ptr_prec->next;
        }
    }

    //met à jour le chainage des zones libres restantes
    if (ptr_alloc->size == size_alloc) {
        //1er cas : la zone libre choisie est totalement remplie
        if (ptr_alloc == ptr_init) {
            // la zone allouée est la première du chainage
            // on remplace le chainage (ptr_alloc---->ptr_alloc.next---->...) par (ptr_alloc.next---->...)
            ptr_init = ptr_init->next;
        } else {
            // on remplace le chainage (ptr_prec---->ptr_alloc---->ptr_alloc.next) par (ptr_prec---->ptr_alloc.next)
            ptr_prec->next = (fb*) ptr_alloc->next;
        }
    } else {
        //2e cas : la zone libre choisie est partiellement remplie
        //=> on décale le pointeur de zone libre
        if ((fb*) ptr_alloc == ptr_init) {
            //décale les métadonnées de la zone libre :
            //1) buffer
            fb* ptr_tmp = ptr_init;
            //2) décale le pointeur
            ptr_init = (fb*) ((void*) ptr_init + size_alloc);
            //3) met à jour la taille
            ptr_tmp->size = ptr_tmp->size - size_alloc;
            //4) écrit les métadonnées
            ptr_init->size = ptr_tmp->size;
            ptr_init->next = ptr_tmp->next;
        } else {
            //décale les métadonnées de la zone libre :
            //1) buffer
            fb* ptr_tmp = ptr_prec->next;
            //2) décale le pointeur
            ptr_prec->next = (fb*) ((void*) ptr_prec->next + size_alloc);
            //3) met à jour la taille
            ptr_tmp->size = ptr_tmp->size - size_alloc;
            //4) écrit les métadonnées
            ptr_prec->next->size = ptr_tmp->size;
            ptr_prec->next->next = ptr_tmp->next;
        }
    }

    //écrit la taille de l'espace mémoire réservé avant celui-ci
    *((size_t*) ptr_alloc) = size_alloc;

    // //écrit DEBORDEMENT à la fin de la zone réservée
    // int* ptr_debordement = (int*) ((void*) ptr_alloc + size_alloc - sizeof(int));
    // *ptr_debordement = DEBORDEMENT; 

    //décale ptr_alloc sur la premiere case de l'espace réservé (=valeur de retour)
    ptr_alloc = (fb*) ((void*) ptr_alloc + METASIZE);

    accumulation += size_alloc;
    mem_show(calculation);

    pthread_mutex_unlock(&mutex);

    return ptr_alloc;
}

void mem_free(void* zone) {

    pthread_mutex_lock(&mutex);

    //décale pointeur vers l'arrière pour récupérer les métadonnées
    fb* ptr_free = (fb*) (zone - METASIZE);
    ptr_free->size = *((size_t*) ptr_free);

    // //vérifie si il n'y a pas eu de débordement
    // int* ptr_debordement = (int*) ((void*) ptr_alloc + ptr_free->size - sizeof(int));


    if(ptr_free < ptr_init) {
	    //cas 1 : ptr_free < ptr_init = ptr_free devient le nouveau ptr_init
        if((fb*) ((void*) ptr_free + ptr_free->size) == ptr_init) {
            //cas 1.1 : adjacent à la premiere zone libre = fusion (amont)
            fb* ptr_tmp = ptr_init;
            ptr_init = (fb*) ((void*) ptr_init - ptr_free->size);
            ptr_init->size = ptr_free->size + ptr_tmp->size;
            ptr_init->next = ptr_tmp->next;
        } else {
            //cas 1.2 : entouré de zones occupées
            //on remplace le chainage (ptr_init) par (ptr_free(nouveau init)---->ptr_init(ancien init))
            fb* ptr_tmp = ptr_init;
            ptr_init = ptr_free;
            ptr_init->next = ptr_tmp;
            ptr_init->size = ptr_free->size;
        }
    } else if (ptr_free > ptr_init) {
      	//cas 2 : ptr_free > ptr_init = mise à jour du chainage à partir de ptr_init
        if (ptr_init == NULL) {
          	//cas 2.1 : ptr_init == NULL donc la mémoire est pleine : la zone libérée devient le nouveau ptr_init
            ptr_init = ptr_free;
            ptr_init->size = ptr_free->size;
            ptr_init->next = NULL;
        } else {
            //cas 2.2 : tous les autres cas
            //récupére les pointeurs des zones libres "encadrant" la zone à libérer
            fb* ptr_prec = (fb*) ptr_init;
            while (ptr_prec->next != NULL && ptr_prec->next < ptr_free) {
                ptr_prec = ptr_prec->next;
            }
            fb* ptr_suivant = ptr_prec->next;
            
            if ((fb*) ((void*) ptr_prec + ptr_prec->size) != ptr_free && (fb*) ((void*) ptr_suivant - ptr_free->size) != ptr_free) {
          		//cas 2.2.1 : entouré de zones occupées : on remplace le chainage (ptr_prec---->ptr_suivant) par (ptr_prec---->ptr_free---->ptr_suivant)
                ptr_prec->next = ptr_free;
                ptr_free->next = ptr_suivant;
            } else {
      		    if ((fb*) ((void*) ptr_prec + ptr_prec->size) == ptr_free) {
      			   //cas 2.2.2 : zone libre juste avant la zone à libérer(fusion aval)
          			fb* ptr_tmp = ptr_free;
          			ptr_free = ptr_prec;
          			ptr_free->size += ptr_tmp->size;
          		}
      		    if ((fb*) ((void*) ptr_free + ptr_free->size) == ptr_suivant) {
          			//cas 2.2.3 : zone libre juste après la zone à libérer(fusion amont)
          			fb* ptr_tmp = ptr_suivant;
          			ptr_suivant = (fb*) ((void*) ptr_suivant - ptr_free->size);
          			ptr_suivant->size = ptr_tmp->size + ptr_free->size;
          			ptr_suivant->next = ptr_tmp->next;
          			if (ptr_prec == ptr_suivant) {
          				//cas particulier pour ptr_prec = ptr_init
          				ptr_prec = ptr_suivant;
          			} else {
          				ptr_prec->next = ptr_suivant;
          			}
      		    }
            }
        }
    }
    mem_show(calculation);

    pthread_mutex_unlock(&mutex);
}

size_t mem_get_size(void* zone) {
    return 0;
}

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void* zone, size_t size, int free)) {

     //initialisation
    fragmentation = 0;
    occupation = 0;

    //parcouration de la mémoire
    fb* ptr = ptr_first;
    fb* ptr_zl = ptr_init;

    while ((void*) ptr < ptr_last) {
        if ((void*) ptr == ptr_zl) {
            print((void*) ptr, ptr->size, 1);//zone libre
            ptr_zl = ptr_zl->next;
            ptr = (fb*) ((void*) ptr + ptr->size);
        } else {
            print((void*) ptr, *((size_t*)ptr), 0);//zone occupée 
            ptr = (fb*) ((void*) ptr + *((size_t*)ptr));
        }
    }

    static int in_mem_show = 0;
    if (in_mem_show == 0) {
        in_mem_show = 1;
        //ecritation dans le fichier : WORKS
        FILE* f = fopen("stats.txt","a");
        print_int(f, accumulation);
        fwrite("\t", 1, 1, f);
        print_int(f, occupation);
        fwrite("\t", 1, 1, f);
        print_int(f, fragmentation);
        fwrite("\n", 1, 1, f);
        fclose(f);
        in_mem_show = 0;
    }
 }

/* Choix de la strategie */
void mem_fit(mem_fit_function_t* fit_function) {
	active_fit_fonction = fit_function;
}

/* First fit */
fb* mem_fit_first(fb* ptr, size_t size) {
    //Tant que la zone libre testée n'est pas assez grande on en cherche une autre.
    while (ptr != NULL && ptr->size < size) {
        ptr = ptr->next;
    }
    return ptr;
}

/* Best fit */
fb* mem_fit_best(fb* ptr, size_t size) {
    fb* ptr_bestChoice = NULL;
    while (ptr != NULL) {
        //si (espace mémoire suffisant && encore plus proche que la précédente zone retenue)
        if(ptr->size >= size && (ptr_bestChoice == NULL || ptr->size < ptr_bestChoice->size)) {
        ptr_bestChoice = ptr;
        }
        ptr = ptr->next;
    }
    return ptr_bestChoice;
}

/* Worst fit */
fb* mem_fit_worst(fb* ptr, size_t size) {
    fb* ptr_worstChoice = NULL;
    while (ptr != NULL) {
        //si (espace mémoire suffisant && encore plus grand que la précédente zone retenue)
        if(ptr->size >= size && (ptr_worstChoice == NULL || ptr->size > ptr_worstChoice->size)) {
            ptr_worstChoice = ptr;
        }
        ptr = ptr->next;
    }
    return ptr_worstChoice;
}

void calculation(void* zone, size_t size, int free) {

    //fragmentation : on comptabilise toutes les zones SAUF la derniere si elle est libre
    if (zone + size != (void*) ptr_last || !free) {
        fragmentation += size;
    }
    if (!free) {
        occupation += size;
    }
}

void print_int(FILE* f, int nb) {
    if (nb < 0){
        fwrite("-", 1, 1, f);
        nb *= -1;
    }
    if (nb > 9){
        print_int(f, nb / 10);
        print_int(f, nb % 10);
    }
    else{
        nb += '0';
        fwrite(&nb, 1, 1, f);
    }
}