## MAKEFILE

CC = gcc -Wall -g
LDFLAGS = -lm -lpthread

all: script-test

script-test : script-test.c
	$(CC) script-test.c -o script-test $(LDFLAGS)

clean:
	rm -rf *.o script-test

