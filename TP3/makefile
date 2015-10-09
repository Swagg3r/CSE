## MAKEFILE

CC = gcc -Wall -O3
LDFLAGS = -lm -lpthread

all : match demi-finales

match : match.c
	$(CC) match.c -o match $(LDFLAGS)

demi-finales : demi-finales.c
	$(CC) demi-finales.c -o demi-finales $(LDFLAGS)

	
clean: rm -f match demi-finales
