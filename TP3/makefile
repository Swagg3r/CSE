## MAKEFILE

CC = gcc -Wall -O3
LDFLAGS = -lm -lpthread

all: match demi-finales chantmultiple recherche recherche_thread
	./recherche
	./recherche_thread
	$(MAKE) clean

match : match.c
	$(CC) match.c -o match $(LDFLAGS)

demi-finales : demi-finales.c
	$(CC) demi-finales.c -o demi-finales $(LDFLAGS)


chantmultiple : chantmultiple.c
	$(CC) chantmultiple.c -o chantmultiple $(LDFLAGS)

recherche : recherche.c
	$(CC) recherche.c -o recherche $(LDFLAGS)

recherche_thread : recherche_thread.c
	$(CC) recherche_thread.c -o recherche_thread $(LDFLAGS)

clean:
	rm -rf *.o match demi-finales chantmultiple recherche recherche_thread

