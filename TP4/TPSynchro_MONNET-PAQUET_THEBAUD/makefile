## MAKEFILE

CC = gcc -Wall -std=c99 -g
LDFLAGS = -lm -lpthread

all: lect_red lect_red_v2 lect_red_v3

lect_red : lect_red.c
	$(CC) lect_red.c -o lect_red $(LDFLAGS)

lect_red_v2 : lect_red_v2.c
	$(CC) lect_red_v2.c -o lect_red_v2 $(LDFLAGS)

lect_red_v3 : lect_red_v3.c
	$(CC) lect_red_v3.c -o lect_red_v3 $(LDFLAGS)

clean:
	rm -rf *.o lect_red lect_red_v2 lect_red_v3

