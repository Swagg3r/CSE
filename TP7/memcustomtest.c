//Antoine Thebaud & Aurelien Monnet-Paquet
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>

#define DEBUG 1
#define TAILLE_BUFFER 128
#define TAILLE_MEMOIRE 4096
static char memoire[TAILLE_MEMOIRE];

#define MAX_ALLOC 1024
static int nb_free, nb_busy;
static void *allocs[MAX_ALLOC];
static size_t sizes[MAX_ALLOC];
static size_t free_size[MAX_ALLOC];
static size_t busy_size[MAX_ALLOC];
static int position;
static intptr_t mem_align=0;

void decompte_zone(void *adresse, size_t taille, int free) {
    if (free)
        free_size[nb_free++] = taille;
    else
        busy_size[nb_busy++] = taille;
}

void afficher_zone(void *adresse, size_t taille, int free) {
    decompte_zone(adresse, taille, free);
    printf("Zone %s, Adresse : %lu, Taille : %lu\n", free?"libre":"occupée",
            (unsigned long) adresse, (unsigned long) taille);
}

void my_mem_init(void *mem, size_t s) {
    position = 0;
    mem_init(mem, s);
}

void *my_mem_alloc(size_t s) {
    void *result = mem_alloc(s);
    sizes[position] = s;
    allocs[position++] = result;
    printf("Allocation de %lu octets en %lu\n", (unsigned long) s,
           (unsigned long) result);
    mem_align |= (intptr_t)result;
    return result;
}

void my_mem_free(int num) {
    printf("Liberation de la zone en %lu\n", (unsigned long) allocs[num]);
    mem_free(allocs[num]);
}

void my_mem_show(void (*print)(void *, size_t, int free)) {
    nb_free = 0;
    nb_busy = 0;
    mem_show(print);
}


int main()
{
	my_mem_init(memoire, TAILLE_MEMOIRE);
	printf("Test de l'etat initial de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 1);
	printf("TEST INIT OK\n\n");
	
	printf("\n--------------------------\n\n");
	
	printf("Test de free :\n");
	mem_fit(&mem_fit_best);
	my_mem_alloc(500);
	my_mem_alloc(400);
	my_mem_alloc(300);
	my_mem_alloc(200);
	//my_mem_free(0);
	my_mem_free(1);
	//my_mem_free(2);
	my_mem_free(3);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	printf("nb_free = %d ; nb_busy = %d\n", nb_free, nb_busy);
	assert(nb_free == 2 && nb_busy == 2);
	printf("TEST FREE OK\n\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	
	printf("\n--------------------------\n\n");
	
	printf("Test de best fit :\n");
	my_mem_alloc(500);
	my_mem_alloc(400);
	my_mem_alloc(300);
	my_mem_alloc(200);
	my_mem_alloc(100);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(400);
	my_mem_alloc(500);
	printf("Etat de la memoire :\n");
	my_mem_free(0);
	my_mem_free(2);
	my_mem_free(4);
	my_mem_free(7);
	my_mem_alloc(200);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	printf("nb_free = %d ; nb_busy = %d\n", nb_free, nb_busy);
	assert(nb_free == 5 && nb_busy == 6);
	printf("TEST BEST OK\n\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	
	printf("\n--------------------------\n\n");

	printf("Test de worst fit :\n");
	mem_fit(&mem_fit_worst);
	my_mem_alloc(500);
	my_mem_alloc(400);
	my_mem_alloc(300);
	my_mem_alloc(200);
	my_mem_alloc(100);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(400);
	my_mem_alloc(500);
	my_mem_alloc(1096);
	my_mem_free(0);//Libere 500
	my_mem_free(2);//Libere 300
	my_mem_free(4);//Libere 100
	my_mem_alloc(200);
	
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	printf("nb_free = %d ; nb_busy = %d\n", nb_free, nb_busy);
	assert(nb_free == 3 && nb_busy == 8);
	printf("TEST WORST OK\n\n");

	printf("\n--------------------------\n\n");

	printf("1.Best Fit est meilleur que First Fit :\n");
	printf(" 1.1 Test avec Best fit :\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	mem_fit(&mem_fit_best);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(100);
	my_mem_alloc(3456);
	my_mem_free(0);//Libere 200
	my_mem_free(2);//Libere 100
	my_mem_alloc(90);
	my_mem_alloc(190);
	my_mem_show(afficher_zone);

	printf("\n 1.2 Test avec First fit :\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(100);
	my_mem_alloc(3456);
	my_mem_free(0);//Libere 200
	my_mem_free(2);//Libere 100
	my_mem_alloc(90);
	my_mem_alloc(190);//Allocaionimpossible avec first fit
	my_mem_show(afficher_zone);

	printf("\n2.First Fit est meilleur que Best Fit :\n");
	printf(" 2.1 Test avec First fit :\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(100);
	my_mem_alloc(3456);
	my_mem_free(0);//Libere 200
	my_mem_free(2);//Libere 100
	my_mem_alloc(72);
	my_mem_alloc(112);
	my_mem_alloc(92);
	my_mem_show(afficher_zone);
	printf(" 2.2 Test avec Best fit :\n");
	my_mem_init(memoire, TAILLE_MEMOIRE);
	mem_fit(&mem_fit_best);
	my_mem_alloc(200);
	my_mem_alloc(300);
	my_mem_alloc(100);
	my_mem_alloc(3456);
	my_mem_free(0);//Libere 200
	my_mem_free(2);//Libere 100
	my_mem_alloc(72);
	my_mem_alloc(112);//Allocation impossible avec best_fit
	my_mem_alloc(92);
	my_mem_show(afficher_zone);
}
