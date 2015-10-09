## MAKEFILE

CC = gcc -Wall -O3
LDFLAGS = -lm -lpthread

all: match demi-finales chantmultiple
	./chantmultiple 1 1 5 2
	$(MAKE) clean

match : match.c
	$(CC) match.c -o match $(LDFLAGS)

demi-finales : demi-finales.c
	$(CC) demi-finales.c -o demi-finales $(LDFLAGS)


chantmultiple : chantmultiple.c
	$(CC) chantmultiple.c -o chantmultiple $(LDFLAGS)


clean:
	rm -rf *.o match demi-finales chantmultiple

