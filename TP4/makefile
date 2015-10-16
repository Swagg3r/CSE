## MAKEFILE

CC = gcc -Wall -std=c99 -O3
LDFLAGS = -lm -lpthread

all: glacier

glacier : glacier.c
	$(CC) glacier.c -o glacier $(LDFLAGS)

clean:
	rm -rf *.o glacier

