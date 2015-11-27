# Flags de compilation
CC=gcc
CFLAGS= -Wall -g
LDFLAGS=

# Exécutables
EXEC=test partie-3 test-supp clean
SRC= $(wildcard *.c)

.PHONY: clean mrproper

all: $(EXEC)

test: test.o stdes.o
	$(CC) -o $@ $^ $(LDFLAGS)

partie-3: partie-3.o stdes.o
	$(CC) -o $@ $^ $(LDFLAGS)

test-supp: test-supp.o stdes.o
	$(CC) -o $@ $^ $(LDFLAGS)

stdes.c: stdes.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) 

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
