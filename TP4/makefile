## MAKEFILE

CC = gcc -Wall -std=c99 -O3
LDFLAGS = -lm -lpthread

all: glacier lect_red
	./lect_red
	make clean

glacier : glacier.c
	$(CC) glacier.c -o glacier $(LDFLAGS)

lect_red : lect_red.c
	$(CC) lect_red.c -o lect_red $(LDFLAGS)

clean:
	rm -rf *.o glacier lect_red

